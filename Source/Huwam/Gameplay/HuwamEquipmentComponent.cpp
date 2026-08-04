#include "Gameplay/HuwamEquipmentComponent.h"

#include "Data/HuwamDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Gameplay/HuwamInventoryComponent.h"

UHuwamEquipmentComponent::UHuwamEquipmentComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHuwamEquipmentComponent::BeginPlay()
{
    Super::BeginPlay();

    if (EquipmentSlots.IsEmpty())
    {
        ResetToDefaultSlots();
    }
}

bool UHuwamEquipmentComponent::EquipItemFromInventory(const FString& ItemId, const FString& PreferredSlotId, UHuwamInventoryComponent* Inventory, FString& OutUnequippedItemId)
{
    OutUnequippedItemId.Empty();

    if (!Inventory || ItemId.IsEmpty() || !Inventory->HasItemQuantity(ItemId, 1))
    {
        return false;
    }

    const FString TargetSlotId = ResolveTargetSlotForItem(ItemId, PreferredSlotId);
    const int32 SlotIndex = FindSlotIndex(TargetSlotId);
    if (SlotIndex == INDEX_NONE || !EquipmentSlots[SlotIndex].bUnlocked || !CanEquipItemToSlot(ItemId, TargetSlotId))
    {
        return false;
    }

    if (!Inventory->RemoveItemById(ItemId, 1))
    {
        return false;
    }

    FHuwamEquipmentSlotState& Slot = EquipmentSlots[SlotIndex];
    const FString PreviousItemId = Slot.EquippedItemId;
    Slot.EquippedItemId = ItemId;

    if (!PreviousItemId.IsEmpty())
    {
        int32 Remainder = 0;
        if (!Inventory->AddItemById(PreviousItemId, 1, Remainder) || Remainder > 0)
        {
            Slot.EquippedItemId = PreviousItemId;
            int32 RestoreRemainder = 0;
            Inventory->AddItemById(ItemId, 1, RestoreRemainder);
            return false;
        }

        OutUnequippedItemId = PreviousItemId;
    }

    OnEquipmentChanged.Broadcast();
    return true;
}

bool UHuwamEquipmentComponent::UnequipSlotToInventory(const FString& SlotId, UHuwamInventoryComponent* Inventory, FString& OutUnequippedItemId)
{
    OutUnequippedItemId.Empty();

    if (!Inventory)
    {
        return false;
    }

    const int32 SlotIndex = FindSlotIndex(SlotId);
    if (SlotIndex == INDEX_NONE || EquipmentSlots[SlotIndex].EquippedItemId.IsEmpty())
    {
        return false;
    }

    FHuwamEquipmentSlotState& Slot = EquipmentSlots[SlotIndex];
    const FString ItemToUnequip = Slot.EquippedItemId;

    int32 Remainder = 0;
    if (!Inventory->AddItemById(ItemToUnequip, 1, Remainder) || Remainder > 0)
    {
        return false;
    }

    Slot.EquippedItemId.Empty();
    OutUnequippedItemId = ItemToUnequip;
    OnEquipmentChanged.Broadcast();
    return true;
}

bool UHuwamEquipmentComponent::UnlockSlot(const FString& SlotId)
{
    const int32 SlotIndex = FindSlotIndex(SlotId);
    if (SlotIndex == INDEX_NONE || EquipmentSlots[SlotIndex].bUnlocked)
    {
        return false;
    }

    EquipmentSlots[SlotIndex].bUnlocked = true;
    OnEquipmentChanged.Broadcast();
    return true;
}

bool UHuwamEquipmentComponent::CanEquipItemToSlot(const FString& ItemId, const FString& SlotId) const
{
    if (ItemId.IsEmpty() || SlotId.IsEmpty() || !IsSlotUnlocked(SlotId))
    {
        return false;
    }

    FHuwamItemRow ItemRow;
    if (GetItemRowForEquippedItem(ItemId, ItemRow))
    {
        if (ItemRow.EquipSlotIds.Contains(SlotId) || ItemRow.PrimaryEquipSlotId == SlotId)
        {
            return true;
        }

        if (ItemRow.ItemType == EHuwamItemType::Pouch && IsPouchSlot(SlotId))
        {
            return true;
        }

        if (ItemRow.ItemType == EHuwamItemType::Weapon && IsWeaponSlot(SlotId) && ItemRow.PrimaryEquipSlotId != TEXT("slot.weapon.ranged"))
        {
            return true;
        }

        return false;
    }

    const FString PrimarySlot = GetPrimaryEquipSlotForItem(ItemId);
    return PrimarySlot == SlotId || (PrimarySlot == TEXT("slot.pouch.1") && IsPouchSlot(SlotId));
}

bool UHuwamEquipmentComponent::IsSlotUnlocked(const FString& SlotId) const
{
    const int32 SlotIndex = FindSlotIndex(SlotId);
    return SlotIndex != INDEX_NONE && EquipmentSlots[SlotIndex].bUnlocked;
}

bool UHuwamEquipmentComponent::IsSlotOccupied(const FString& SlotId) const
{
    const int32 SlotIndex = FindSlotIndex(SlotId);
    return SlotIndex != INDEX_NONE && EquipmentSlots[SlotIndex].IsOccupied();
}

FString UHuwamEquipmentComponent::GetEquippedItemInSlot(const FString& SlotId) const
{
    const int32 SlotIndex = FindSlotIndex(SlotId);
    return SlotIndex != INDEX_NONE ? EquipmentSlots[SlotIndex].EquippedItemId : FString();
}

TArray<FHuwamEquipmentSlotState> UHuwamEquipmentComponent::GetEquipmentSlots() const
{
    return EquipmentSlots;
}

FHuwamEquipmentStatSummary UHuwamEquipmentComponent::GetEquipmentStatSummary() const
{
    FHuwamEquipmentStatSummary Summary;

    for (const FHuwamEquipmentSlotState& Slot : EquipmentSlots)
    {
        if (Slot.EquippedItemId.IsEmpty())
        {
            continue;
        }

        FHuwamItemRow ItemRow;
        if (!GetItemRowForEquippedItem(Slot.EquippedItemId, ItemRow))
        {
            continue;
        }

        Summary.AttackPower += ItemRow.AttackPower;
        Summary.DefensePower += ItemRow.DefensePower;
        Summary.ManaPower += ItemRow.ManaPower;
        Summary.UtilityPower += ItemRow.UtilityPower;
    }

    return Summary;
}

int32 UHuwamEquipmentComponent::GetEquipmentAttackPower() const
{
    return GetEquipmentStatSummary().AttackPower;
}

int32 UHuwamEquipmentComponent::GetEquipmentDefensePower() const
{
    return GetEquipmentStatSummary().DefensePower;
}

int32 UHuwamEquipmentComponent::GetEquipmentManaPower() const
{
    return GetEquipmentStatSummary().ManaPower;
}

int32 UHuwamEquipmentComponent::GetEquipmentUtilityPower() const
{
    return GetEquipmentStatSummary().UtilityPower;
}

void UHuwamEquipmentComponent::ResetToDefaultSlots()
{
    EquipmentSlots.Reset();

    AddDefaultSlot(TEXT("slot.head"), NSLOCTEXT("HuwamEquipment", "HeadSlot", "Head"), true);
    AddDefaultSlot(TEXT("slot.face"), NSLOCTEXT("HuwamEquipment", "FaceSlot", "Face"), true);
    AddDefaultSlot(TEXT("slot.neck"), NSLOCTEXT("HuwamEquipment", "NeckSlot", "Neck"), true);
    AddDefaultSlot(TEXT("slot.ear.left"), NSLOCTEXT("HuwamEquipment", "LeftEarSlot", "Left Ear"), true);
    AddDefaultSlot(TEXT("slot.ear.right"), NSLOCTEXT("HuwamEquipment", "RightEarSlot", "Right Ear"), true);
    AddDefaultSlot(TEXT("slot.chest"), NSLOCTEXT("HuwamEquipment", "ChestSlot", "Chest"), true);
    AddDefaultSlot(TEXT("slot.arms"), NSLOCTEXT("HuwamEquipment", "ArmsSlot", "Arms"), true);
    AddDefaultSlot(TEXT("slot.hands"), NSLOCTEXT("HuwamEquipment", "HandsSlot", "Hands"), true);
    AddDefaultSlot(TEXT("slot.legs"), NSLOCTEXT("HuwamEquipment", "LegsSlot", "Legs"), true);
    AddDefaultSlot(TEXT("slot.feet"), NSLOCTEXT("HuwamEquipment", "FeetSlot", "Feet"), true);
    AddDefaultSlot(TEXT("slot.underwear"), NSLOCTEXT("HuwamEquipment", "UnderwearSlot", "Underwear"), true);
    AddDefaultSlot(TEXT("slot.weapon.main_hand"), NSLOCTEXT("HuwamEquipment", "MainHandSlot", "Main Hand"), true);
    AddDefaultSlot(TEXT("slot.weapon.off_hand"), NSLOCTEXT("HuwamEquipment", "OffHandSlot", "Off Hand"), true);
    AddDefaultSlot(TEXT("slot.weapon.ranged"), NSLOCTEXT("HuwamEquipment", "RangedSlot", "Ranged"), true);
    AddDefaultSlot(TEXT("slot.ammo"), NSLOCTEXT("HuwamEquipment", "AmmoSlot", "Ammo"), true);
    AddDefaultSlot(TEXT("slot.quiver"), NSLOCTEXT("HuwamEquipment", "QuiverSlot", "Quiver"), true);
    AddDefaultSlot(TEXT("slot.tool"), NSLOCTEXT("HuwamEquipment", "ToolSlot", "Tool"), true);
    AddDefaultSlot(TEXT("slot.backpack"), NSLOCTEXT("HuwamEquipment", "BackpackSlot", "Backpack"), true);
    AddDefaultSlot(TEXT("slot.pouch.1"), NSLOCTEXT("HuwamEquipment", "PouchOneSlot", "Pouch 1"), true);
    AddDefaultSlot(TEXT("slot.pouch.2"), NSLOCTEXT("HuwamEquipment", "PouchTwoSlot", "Pouch 2"), true);
    AddDefaultSlot(TEXT("slot.pouch.3"), NSLOCTEXT("HuwamEquipment", "PouchThreeSlot", "Pouch 3"), true);
    AddDefaultSlot(TEXT("slot.pouch.4"), NSLOCTEXT("HuwamEquipment", "PouchFourSlot", "Pouch 4"), true);
    AddDefaultSlot(TEXT("slot.pouch.5"), NSLOCTEXT("HuwamEquipment", "PouchFiveSlot", "Pouch 5"), true);
    AddDefaultSlot(TEXT("slot.magical_storage"), NSLOCTEXT("HuwamEquipment", "MagicalStorageSlot", "Magical Storage"), false);
    AddDefaultSlot(TEXT("slot.wizard.book"), NSLOCTEXT("HuwamEquipment", "WizardBookSlot", "Wizard Book"), false);
    AddDefaultSlot(TEXT("slot.mage.component"), NSLOCTEXT("HuwamEquipment", "MageComponentSlot", "Mage Component"), false);

    OnEquipmentChanged.Broadcast();
}

bool UHuwamEquipmentComponent::RestoreEquipmentSlotsForPrototypeSave(const TArray<FHuwamEquipmentSlotState>& SavedSlots)
{
    if (SavedSlots.IsEmpty())
    {
        return false;
    }

    EquipmentSlots = SavedSlots;
    OnEquipmentChanged.Broadcast();
    return true;
}

int32 UHuwamEquipmentComponent::FindSlotIndex(const FString& SlotId) const
{
    for (int32 Index = 0; Index < EquipmentSlots.Num(); ++Index)
    {
        if (EquipmentSlots[Index].SlotId == SlotId)
        {
            return Index;
        }
    }

    return INDEX_NONE;
}

FString UHuwamEquipmentComponent::ResolveTargetSlotForItem(const FString& ItemId, const FString& PreferredSlotId) const
{
    if (!PreferredSlotId.IsEmpty())
    {
        return PreferredSlotId;
    }

    return GetPrimaryEquipSlotForItem(ItemId);
}

bool UHuwamEquipmentComponent::GetItemRowForEquippedItem(const FString& ItemId, FHuwamItemRow& OutItemRow) const
{
    if (ItemId.IsEmpty())
    {
        return false;
    }

    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (const UHuwamDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UHuwamDataSubsystem>())
            {
                const FName RowName(*ItemId.Replace(TEXT("."), TEXT("_")));
                return DataSubsystem->GetItemRow(RowName, OutItemRow);
            }
        }
    }

    return false;
}

FString UHuwamEquipmentComponent::GetPrimaryEquipSlotForItem(const FString& ItemId) const
{
    FHuwamItemRow ItemRow;
    if (GetItemRowForEquippedItem(ItemId, ItemRow) && !ItemRow.PrimaryEquipSlotId.IsEmpty())
    {
        return ItemRow.PrimaryEquipSlotId;
    }

    if (ItemId.Contains(TEXT(".bow")))
    {
        return TEXT("slot.weapon.ranged");
    }

    if (ItemId.Contains(TEXT(".quiver")))
    {
        return TEXT("slot.quiver");
    }

    if (ItemId.Contains(TEXT(".pouch")))
    {
        return TEXT("slot.pouch.1");
    }

    if (ItemId.Contains(TEXT(".backpack")))
    {
        return TEXT("slot.backpack");
    }

    if (ItemId.Contains(TEXT(".tool.")))
    {
        return TEXT("slot.tool");
    }

    if (ItemId.Contains(TEXT(".weapon.")))
    {
        return TEXT("slot.weapon.main_hand");
    }

    if (ItemId.Contains(TEXT(".gear.")))
    {
        return TEXT("slot.chest");
    }

    return FString();
}

bool UHuwamEquipmentComponent::IsPouchSlot(const FString& SlotId) const
{
    return SlotId.StartsWith(TEXT("slot.pouch."));
}

bool UHuwamEquipmentComponent::IsWeaponSlot(const FString& SlotId) const
{
    return SlotId == TEXT("slot.weapon.main_hand") || SlotId == TEXT("slot.weapon.off_hand");
}

void UHuwamEquipmentComponent::AddDefaultSlot(const FString& SlotId, const FText& DisplayName, bool bUnlocked)
{
    FHuwamEquipmentSlotState Slot;
    Slot.SlotId = SlotId;
    Slot.DisplayName = DisplayName;
    Slot.bUnlocked = bUnlocked;
    EquipmentSlots.Add(Slot);
}
