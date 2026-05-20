#include "Gameplay/HuwamInventoryComponent.h"

#include "Data/HuwamDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

UHuwamInventoryComponent::UHuwamInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UHuwamInventoryComponent::AddItemById(const FString& ItemId, int32 Quantity, int32& OutRemainder)
{
    OutRemainder = Quantity;

    if (ItemId.IsEmpty() || Quantity <= 0)
    {
        return false;
    }

    const int32 StackLimit = FMath::Max(1, GetStackLimitForItem(ItemId));

    while (OutRemainder > 0)
    {
        const int32 PartialStackIndex = FindFirstPartialStackIndex(ItemId, StackLimit);
        if (PartialStackIndex != INDEX_NONE)
        {
            FHuwamInventoryStack& Stack = InventoryStacks[PartialStackIndex];
            const int32 SpaceInStack = StackLimit - Stack.Quantity;
            const int32 AmountToAdd = FMath::Min(SpaceInStack, OutRemainder);

            Stack.Quantity += AmountToAdd;
            OutRemainder -= AmountToAdd;
            continue;
        }

        if (InventoryStacks.Num() >= MaxSlots)
        {
            break;
        }

        FHuwamInventoryStack NewStack;
        NewStack.ItemId = ItemId;
        NewStack.Quantity = FMath::Min(StackLimit, OutRemainder);
        InventoryStacks.Add(NewStack);
        OutRemainder -= NewStack.Quantity;
    }

    const bool bAddedAny = OutRemainder < Quantity;
    if (bAddedAny)
    {
        OnInventoryChanged.Broadcast();
    }

    return bAddedAny;
}

bool UHuwamInventoryComponent::RemoveItemById(const FString& ItemId, int32 Quantity)
{
    if (ItemId.IsEmpty() || Quantity <= 0 || !HasItemQuantity(ItemId, Quantity))
    {
        return false;
    }

    int32 RemainingToRemove = Quantity;

    for (FHuwamInventoryStack& Stack : InventoryStacks)
    {
        if (RemainingToRemove <= 0)
        {
            break;
        }

        if (Stack.ItemId != ItemId)
        {
            continue;
        }

        const int32 AmountToRemove = FMath::Min(Stack.Quantity, RemainingToRemove);
        Stack.Quantity -= AmountToRemove;
        RemainingToRemove -= AmountToRemove;
    }

    RemoveEmptyStacks();
    OnInventoryChanged.Broadcast();
    return true;
}

bool UHuwamInventoryComponent::HasItemQuantity(const FString& ItemId, int32 Quantity) const
{
    if (Quantity <= 0)
    {
        return true;
    }

    return GetItemQuantity(ItemId) >= Quantity;
}

int32 UHuwamInventoryComponent::GetItemQuantity(const FString& ItemId) const
{
    if (ItemId.IsEmpty())
    {
        return 0;
    }

    int32 TotalQuantity = 0;
    for (const FHuwamInventoryStack& Stack : InventoryStacks)
    {
        if (Stack.ItemId == ItemId)
        {
            TotalQuantity += Stack.Quantity;
        }
    }

    return TotalQuantity;
}

TArray<FHuwamInventoryStack> UHuwamInventoryComponent::GetInventoryStacks() const
{
    return InventoryStacks;
}

int32 UHuwamInventoryComponent::GetUsedSlots() const
{
    return InventoryStacks.Num();
}

int32 UHuwamInventoryComponent::GetMaxSlots() const
{
    return MaxSlots;
}

int32 UHuwamInventoryComponent::GetRemainingSlots() const
{
    return FMath::Max(0, MaxSlots - InventoryStacks.Num());
}

void UHuwamInventoryComponent::ClearInventory()
{
    if (InventoryStacks.IsEmpty())
    {
        return;
    }

    InventoryStacks.Reset();
    OnInventoryChanged.Broadcast();
}

int32 UHuwamInventoryComponent::GetStackLimitForItem(const FString& ItemId) const
{
    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (const UHuwamDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UHuwamDataSubsystem>())
            {
                FHuwamItemRow ItemRow;
                const FName RowName(*ItemId.Replace(TEXT("."), TEXT("_")));
                if (DataSubsystem->GetItemRow(RowName, ItemRow))
                {
                    return FMath::Max(1, ItemRow.StackLimit);
                }
            }
        }
    }

    return FallbackStackLimit;
}

int32 UHuwamInventoryComponent::FindFirstPartialStackIndex(const FString& ItemId, int32 StackLimit) const
{
    for (int32 Index = 0; Index < InventoryStacks.Num(); ++Index)
    {
        const FHuwamInventoryStack& Stack = InventoryStacks[Index];
        if (Stack.ItemId == ItemId && Stack.Quantity < StackLimit)
        {
            return Index;
        }
    }

    return INDEX_NONE;
}

void UHuwamInventoryComponent::RemoveEmptyStacks()
{
    InventoryStacks.RemoveAll([](const FHuwamInventoryStack& Stack)
    {
        return !Stack.IsValidStack();
    });
}
