#include "Gameplay/HuwamInventoryComponent.h"

#include "Data/HuwamDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Gameplay/HuwamTimeSubsystem.h"

UHuwamInventoryComponent::UHuwamInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UHuwamInventoryComponent::AddItemById(const FString& ItemId, int32 Quantity, int32& OutRemainder)
{
    return AddItemByIdToStorage(ItemId, Quantity, GetPreferredStorageQualityForItem(ItemId), OutRemainder);
}

bool UHuwamInventoryComponent::AddItemByIdToStorage(const FString& ItemId, int32 Quantity, EHuwamInventoryStorageQuality StorageQuality, int32& OutRemainder)
{
    OutRemainder = Quantity;

    if (ItemId.IsEmpty() || Quantity <= 0)
    {
        return false;
    }

    const int32 StackLimit = FMath::Max(1, GetStackLimitForItem(ItemId));
    const FHuwamInventoryStack IncomingTemplate = MakeInventoryStack(ItemId, 0, StorageQuality);

    while (OutRemainder > 0)
    {
        const int32 PartialStackIndex = FindFirstPartialStackIndex(IncomingTemplate, StackLimit);
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

        if (StorageQuality != EHuwamInventoryStorageQuality::Loose && GetRemainingStorageRouteStacks(StorageQuality) <= 0)
        {
            break;
        }

        FHuwamInventoryStack NewStack = IncomingTemplate;
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

bool UHuwamInventoryComponent::AssignFirstMatchingStackToStorage(const FString& ItemId, EHuwamInventoryStorageQuality TargetStorageQuality)
{
    if (ItemId.IsEmpty())
    {
        return false;
    }

    if (TargetStorageQuality != EHuwamInventoryStorageQuality::Loose && GetMaxStorageRouteStacks(TargetStorageQuality) <= 0)
    {
        return false;
    }

    for (FHuwamInventoryStack& Stack : InventoryStacks)
    {
        if (!Stack.IsValidStack() || Stack.ItemId != ItemId)
        {
            continue;
        }

        if (Stack.StorageQuality == TargetStorageQuality && Stack.StorageRouteId == GetStorageRouteIdForQuality(TargetStorageQuality))
        {
            return true;
        }

        if (TargetStorageQuality != EHuwamInventoryStorageQuality::Loose && GetRemainingStorageRouteStacks(TargetStorageQuality) <= 0)
        {
            return false;
        }

        const EHuwamInventoryFreshness PreviousFreshness = Stack.Freshness;
        const FHuwamSpoilageProfile SpoilageProfile = ApplyStorageToSpoilageProfile(GetSpoilageProfileForItem(Stack.ItemId), TargetStorageQuality);

        Stack.StorageQuality = TargetStorageQuality;
        Stack.StorageRouteId = GetStorageRouteIdForQuality(TargetStorageQuality);
        Stack.SpoilageDurationMultiplier = GetSpoilageDurationMultiplierForStorage(TargetStorageQuality);
        Stack.bTracksFreshness = SpoilageProfile.bTracksFreshness;
        Stack.StaleAfterInGameDays = SpoilageProfile.StaleAfterInGameDays;
        Stack.SpoilsAfterInGameDays = SpoilageProfile.SpoilsAfterInGameDays;
        Stack.Freshness = Stack.bTracksFreshness
            ? GetWorseFreshness(PreviousFreshness, CalculateFreshness(SpoilageProfile, GetCurrentInGameDay() - Stack.AcquiredInGameDay))
            : EHuwamInventoryFreshness::Stable;

        OnInventoryChanged.Broadcast();
        return true;
    }

    return false;
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

int32 UHuwamInventoryComponent::RefreshSpoilageState()
{
    const int32 CurrentInGameDay = GetCurrentInGameDay();
    int32 ChangedStacks = 0;

    for (FHuwamInventoryStack& Stack : InventoryStacks)
    {
        if (!Stack.IsValidStack() || !Stack.bTracksFreshness)
        {
            continue;
        }

        FHuwamSpoilageProfile Profile;
        Profile.bTracksFreshness = true;
        Profile.StaleAfterInGameDays = Stack.StaleAfterInGameDays;
        Profile.SpoilsAfterInGameDays = Stack.SpoilsAfterInGameDays;

        const EHuwamInventoryFreshness PreviousFreshness = Stack.Freshness;
        Stack.Freshness = CalculateFreshness(Profile, CurrentInGameDay - Stack.AcquiredInGameDay);
        if (Stack.Freshness != PreviousFreshness)
        {
            ++ChangedStacks;
        }
    }

    if (ChangedStacks > 0)
    {
        OnInventoryChanged.Broadcast();
    }

    return ChangedStacks;
}

EHuwamInventoryFreshness UHuwamInventoryComponent::PreviewItemFreshnessAfterInGameDays(const FString& ItemId, int32 ElapsedInGameDays) const
{
    return CalculateFreshness(GetSpoilageProfileForItem(ItemId), ElapsedInGameDays);
}

EHuwamInventoryFreshness UHuwamInventoryComponent::PreviewItemFreshnessAfterInGameDaysWithStorage(const FString& ItemId, int32 ElapsedInGameDays, EHuwamInventoryStorageQuality StorageQuality) const
{
    const FHuwamSpoilageProfile SpoilageProfile = ApplyStorageToSpoilageProfile(GetSpoilageProfileForItem(ItemId), StorageQuality);
    return CalculateFreshness(SpoilageProfile, ElapsedInGameDays);
}

EHuwamInventoryStorageQuality UHuwamInventoryComponent::GetBestStorageQuality() const
{
    if (GetRemainingStorageRouteStacks(EHuwamInventoryStorageQuality::Magical) > 0)
    {
        return EHuwamInventoryStorageQuality::Magical;
    }

    if (GetRemainingStorageRouteStacks(EHuwamInventoryStorageQuality::Cooled) > 0)
    {
        return EHuwamInventoryStorageQuality::Cooled;
    }

    if (GetRemainingStorageRouteStacks(EHuwamInventoryStorageQuality::Chest) > 0)
    {
        return EHuwamInventoryStorageQuality::Chest;
    }

    if (GetRemainingStorageRouteStacks(EHuwamInventoryStorageQuality::Pouch) > 0)
    {
        return EHuwamInventoryStorageQuality::Pouch;
    }

    return EHuwamInventoryStorageQuality::Loose;
}

TArray<FHuwamInventoryStorageRoute> UHuwamInventoryComponent::GetStorageRoutes() const
{
    TArray<FHuwamInventoryStorageRoute> Routes;
    const EHuwamInventoryStorageQuality Qualities[] =
    {
        EHuwamInventoryStorageQuality::Loose,
        EHuwamInventoryStorageQuality::Pouch,
        EHuwamInventoryStorageQuality::Chest,
        EHuwamInventoryStorageQuality::Cooled,
        EHuwamInventoryStorageQuality::Magical
    };

    for (const EHuwamInventoryStorageQuality StorageQuality : Qualities)
    {
        FHuwamInventoryStorageRoute Route;
        Route.RouteId = GetStorageRouteIdForQuality(StorageQuality);
        Route.DisplayName = GetStorageRouteDisplayName(StorageQuality);
        Route.ContainerItemId = GetContainerItemIdForStorageQuality(StorageQuality);
        Route.StorageQuality = StorageQuality;
        Route.UsedStacks = GetUsedStorageRouteStacks(StorageQuality);
        Route.MaxStacks = GetMaxStorageRouteStacks(StorageQuality);
        Route.RemainingStacks = GetRemainingStorageRouteStacks(StorageQuality);
        Route.SpoilageDurationMultiplier = GetSpoilageDurationMultiplierForStorage(StorageQuality);
        Route.bAvailable = StorageQuality == EHuwamInventoryStorageQuality::Loose || Route.MaxStacks > 0;
        Route.bCanPreserveFreshness = StorageQuality != EHuwamInventoryStorageQuality::Loose;
        Routes.Add(Route);
    }

    return Routes;
}

int32 UHuwamInventoryComponent::GetUsedStorageRouteStacks(EHuwamInventoryStorageQuality StorageQuality) const
{
    int32 UsedStacks = 0;
    for (const FHuwamInventoryStack& Stack : InventoryStacks)
    {
        if (Stack.IsValidStack() && Stack.StorageQuality == StorageQuality)
        {
            ++UsedStacks;
        }
    }

    return UsedStacks;
}

int32 UHuwamInventoryComponent::GetMaxStorageRouteStacks(EHuwamInventoryStorageQuality StorageQuality) const
{
    switch (StorageQuality)
    {
    case EHuwamInventoryStorageQuality::Pouch:
        return (GetItemQuantity(TEXT("item.container.small_pouch")) * 4) + (GetItemQuantity(TEXT("item.container.food_pouch")) * 8);
    case EHuwamInventoryStorageQuality::Chest:
        return GetItemQuantity(TEXT("item.container.basic_chest")) * 20;
    case EHuwamInventoryStorageQuality::Cooled:
        return GetItemQuantity(TEXT("item.container.cooled_storage_box")) * 12;
    case EHuwamInventoryStorageQuality::Magical:
        return GetItemQuantity(TEXT("item.container.magical_storage_box")) * 50;
    case EHuwamInventoryStorageQuality::Loose:
    default:
        return MaxSlots;
    }
}

int32 UHuwamInventoryComponent::GetRemainingStorageRouteStacks(EHuwamInventoryStorageQuality StorageQuality) const
{
    if (StorageQuality == EHuwamInventoryStorageQuality::Loose)
    {
        return GetRemainingSlots();
    }

    return FMath::Max(0, GetMaxStorageRouteStacks(StorageQuality) - GetUsedStorageRouteStacks(StorageQuality));
}

float UHuwamInventoryComponent::GetSpoilageDurationMultiplierForStorage(EHuwamInventoryStorageQuality StorageQuality) const
{
    switch (StorageQuality)
    {
    case EHuwamInventoryStorageQuality::Pouch:
        return 1.5f;
    case EHuwamInventoryStorageQuality::Chest:
        return 2.0f;
    case EHuwamInventoryStorageQuality::Cooled:
        return 5.0f;
    case EHuwamInventoryStorageQuality::Magical:
        return 0.0f;
    case EHuwamInventoryStorageQuality::Loose:
    default:
        return 1.0f;
    }
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

bool UHuwamInventoryComponent::RestoreInventoryStacksForPrototypeSave(const TArray<FHuwamInventoryStack>& SavedStacks)
{
    InventoryStacks.Reset();
    for (const FHuwamInventoryStack& Stack : SavedStacks)
    {
        if (!Stack.IsValidStack())
        {
            continue;
        }

        FHuwamInventoryStack RestoredStack = Stack;
        if (RestoredStack.StorageRouteId.IsEmpty())
        {
            RestoredStack.StorageRouteId = GetStorageRouteIdForQuality(RestoredStack.StorageQuality);
        }

        RestoredStack.SpoilageDurationMultiplier = GetSpoilageDurationMultiplierForStorage(RestoredStack.StorageQuality);
        InventoryStacks.Add(RestoredStack);
    }

    RemoveEmptyStacks();
    RefreshSpoilageState();
    OnInventoryChanged.Broadcast();
    return true;
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

int32 UHuwamInventoryComponent::FindFirstPartialStackIndex(const FHuwamInventoryStack& IncomingStack, int32 StackLimit) const
{
    for (int32 Index = 0; Index < InventoryStacks.Num(); ++Index)
    {
        const FHuwamInventoryStack& Stack = InventoryStacks[Index];
        if (CanStacksMerge(Stack, IncomingStack) && Stack.Quantity < StackLimit)
        {
            return Index;
        }
    }

    return INDEX_NONE;
}

FHuwamInventoryStack UHuwamInventoryComponent::MakeInventoryStack(const FString& ItemId, int32 Quantity) const
{
    return MakeInventoryStack(ItemId, Quantity, GetPreferredStorageQualityForItem(ItemId));
}

FHuwamInventoryStack UHuwamInventoryComponent::MakeInventoryStack(const FString& ItemId, int32 Quantity, EHuwamInventoryStorageQuality StorageQuality) const
{
    FHuwamInventoryStack Stack;
    Stack.ItemId = ItemId;
    Stack.Quantity = Quantity;

    const FHuwamSpoilageProfile SpoilageProfile = ApplyStorageToSpoilageProfile(GetSpoilageProfileForItem(ItemId), StorageQuality);
    Stack.bTracksFreshness = SpoilageProfile.bTracksFreshness;
    Stack.StaleAfterInGameDays = SpoilageProfile.StaleAfterInGameDays;
    Stack.SpoilsAfterInGameDays = SpoilageProfile.SpoilsAfterInGameDays;
    Stack.AcquiredInGameDay = GetCurrentInGameDay();
    Stack.Freshness = CalculateFreshness(SpoilageProfile, 0);
    Stack.StorageQuality = StorageQuality;
    Stack.StorageRouteId = GetStorageRouteIdForQuality(StorageQuality);
    Stack.SpoilageDurationMultiplier = GetSpoilageDurationMultiplierForStorage(StorageQuality);
    return Stack;
}

UHuwamInventoryComponent::FHuwamSpoilageProfile UHuwamInventoryComponent::GetSpoilageProfileForItem(const FString& ItemId) const
{
    FHuwamSpoilageProfile Profile;
    if (ItemId.IsEmpty())
    {
        return Profile;
    }

    FHuwamMaterialRow MaterialRow;
    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (const UHuwamDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UHuwamDataSubsystem>())
            {
                const FName RowName(*ItemId.Replace(TEXT("."), TEXT("_")));
                DataSubsystem->GetMaterialRow(RowName, MaterialRow);
            }
        }
    }

    const FString LowerItemId = ItemId.ToLower();
    const bool bIsEdibleMaterial = MaterialRow.bEdible || LowerItemId.Contains(TEXT("fish")) || LowerItemId.Contains(TEXT("meat")) || LowerItemId.Contains(TEXT("fruit")) || LowerItemId.Contains(TEXT("vegetable")) || LowerItemId.Contains(TEXT("honey"));
    if (!bIsEdibleMaterial)
    {
        return Profile;
    }

    Profile.bTracksFreshness = true;
    if (LowerItemId.Contains(TEXT("fish")) || LowerItemId.Contains(TEXT("meat")))
    {
        Profile.StaleAfterInGameDays = 1;
        Profile.SpoilsAfterInGameDays = 1;
    }
    else if (LowerItemId.Contains(TEXT("fruit")))
    {
        Profile.StaleAfterInGameDays = 1;
        Profile.SpoilsAfterInGameDays = 2;
    }
    else if (LowerItemId.Contains(TEXT("vegetable")))
    {
        Profile.StaleAfterInGameDays = 2;
        Profile.SpoilsAfterInGameDays = 3;
    }
    else if (LowerItemId.Contains(TEXT("honey")))
    {
        Profile.StaleAfterInGameDays = 30;
        Profile.SpoilsAfterInGameDays = 0;
    }
    else
    {
        Profile.StaleAfterInGameDays = 5;
        Profile.SpoilsAfterInGameDays = 10;
    }

    return Profile;
}

UHuwamInventoryComponent::FHuwamSpoilageProfile UHuwamInventoryComponent::ApplyStorageToSpoilageProfile(const FHuwamSpoilageProfile& SpoilageProfile, EHuwamInventoryStorageQuality StorageQuality) const
{
    if (!SpoilageProfile.bTracksFreshness)
    {
        return SpoilageProfile;
    }

    FHuwamSpoilageProfile AdjustedProfile = SpoilageProfile;
    const float DurationMultiplier = GetSpoilageDurationMultiplierForStorage(StorageQuality);

    if (StorageQuality == EHuwamInventoryStorageQuality::Magical)
    {
        AdjustedProfile.StaleAfterInGameDays = 0;
        AdjustedProfile.SpoilsAfterInGameDays = 0;
        return AdjustedProfile;
    }

    const auto ScaleDays = [DurationMultiplier](int32 Days)
    {
        return Days > 0 ? FMath::Max(1, FMath::CeilToInt(static_cast<float>(Days) * DurationMultiplier)) : 0;
    };

    AdjustedProfile.StaleAfterInGameDays = ScaleDays(SpoilageProfile.StaleAfterInGameDays);
    AdjustedProfile.SpoilsAfterInGameDays = ScaleDays(SpoilageProfile.SpoilsAfterInGameDays);
    return AdjustedProfile;
}

EHuwamInventoryFreshness UHuwamInventoryComponent::CalculateFreshness(const FHuwamSpoilageProfile& SpoilageProfile, int32 ElapsedInGameDays) const
{
    if (!SpoilageProfile.bTracksFreshness)
    {
        return EHuwamInventoryFreshness::Stable;
    }

    const int32 SafeElapsedDays = FMath::Max(0, ElapsedInGameDays);
    if (SpoilageProfile.SpoilsAfterInGameDays > 0 && SafeElapsedDays >= SpoilageProfile.SpoilsAfterInGameDays)
    {
        return EHuwamInventoryFreshness::Spoiled;
    }

    if (SpoilageProfile.StaleAfterInGameDays > 0 && SafeElapsedDays >= SpoilageProfile.StaleAfterInGameDays)
    {
        return EHuwamInventoryFreshness::Stale;
    }

    return EHuwamInventoryFreshness::Fresh;
}

EHuwamInventoryFreshness UHuwamInventoryComponent::GetWorseFreshness(EHuwamInventoryFreshness FirstFreshness, EHuwamInventoryFreshness SecondFreshness) const
{
    const auto FreshnessRank = [](EHuwamInventoryFreshness Freshness)
    {
        switch (Freshness)
        {
        case EHuwamInventoryFreshness::Spoiled:
            return 3;
        case EHuwamInventoryFreshness::Stale:
            return 2;
        case EHuwamInventoryFreshness::Fresh:
            return 1;
        case EHuwamInventoryFreshness::Stable:
        default:
            return 0;
        }
    };

    return FreshnessRank(FirstFreshness) >= FreshnessRank(SecondFreshness) ? FirstFreshness : SecondFreshness;
}

EHuwamInventoryStorageQuality UHuwamInventoryComponent::GetPreferredStorageQualityForItem(const FString& ItemId) const
{
    const FHuwamSpoilageProfile SpoilageProfile = GetSpoilageProfileForItem(ItemId);
    return SpoilageProfile.bTracksFreshness ? GetBestStorageQuality() : EHuwamInventoryStorageQuality::Loose;
}

FString UHuwamInventoryComponent::GetStorageRouteIdForQuality(EHuwamInventoryStorageQuality StorageQuality) const
{
    switch (StorageQuality)
    {
    case EHuwamInventoryStorageQuality::Pouch:
        return TEXT("storage.pouch");
    case EHuwamInventoryStorageQuality::Chest:
        return TEXT("storage.chest");
    case EHuwamInventoryStorageQuality::Cooled:
        return TEXT("storage.cooled");
    case EHuwamInventoryStorageQuality::Magical:
        return TEXT("storage.magical");
    case EHuwamInventoryStorageQuality::Loose:
    default:
        return TEXT("storage.loose");
    }
}

FString UHuwamInventoryComponent::GetContainerItemIdForStorageQuality(EHuwamInventoryStorageQuality StorageQuality) const
{
    switch (StorageQuality)
    {
    case EHuwamInventoryStorageQuality::Pouch:
        return GetItemQuantity(TEXT("item.container.food_pouch")) > 0 ? TEXT("item.container.food_pouch") : TEXT("item.container.small_pouch");
    case EHuwamInventoryStorageQuality::Chest:
        return TEXT("item.container.basic_chest");
    case EHuwamInventoryStorageQuality::Cooled:
        return TEXT("item.container.cooled_storage_box");
    case EHuwamInventoryStorageQuality::Magical:
        return TEXT("item.container.magical_storage_box");
    case EHuwamInventoryStorageQuality::Loose:
    default:
        return FString();
    }
}

FText UHuwamInventoryComponent::GetStorageRouteDisplayName(EHuwamInventoryStorageQuality StorageQuality) const
{
    switch (StorageQuality)
    {
    case EHuwamInventoryStorageQuality::Pouch:
        return NSLOCTEXT("HuwamInventory", "PouchStorageRoute", "Pouches");
    case EHuwamInventoryStorageQuality::Chest:
        return NSLOCTEXT("HuwamInventory", "ChestStorageRoute", "Chests");
    case EHuwamInventoryStorageQuality::Cooled:
        return NSLOCTEXT("HuwamInventory", "CooledStorageRoute", "Cooled Storage");
    case EHuwamInventoryStorageQuality::Magical:
        return NSLOCTEXT("HuwamInventory", "MagicalStorageRoute", "Magical Storage");
    case EHuwamInventoryStorageQuality::Loose:
    default:
        return NSLOCTEXT("HuwamInventory", "LooseStorageRoute", "Loose Pack");
    }
}

int32 UHuwamInventoryComponent::GetCurrentInGameDay() const
{
    if (const UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (const UHuwamTimeSubsystem* TimeSubsystem = GameInstance->GetSubsystem<UHuwamTimeSubsystem>())
            {
                return TimeSubsystem->GetCurrentTimeSnapshot().TotalInGameDays;
            }
        }
    }

    return 0;
}

bool UHuwamInventoryComponent::CanStacksMerge(const FHuwamInventoryStack& ExistingStack, const FHuwamInventoryStack& IncomingStack) const
{
    if (ExistingStack.ItemId != IncomingStack.ItemId)
    {
        return false;
    }

    if (!ExistingStack.bTracksFreshness && !IncomingStack.bTracksFreshness)
    {
        return ExistingStack.StorageQuality == IncomingStack.StorageQuality
            && ExistingStack.StorageRouteId == IncomingStack.StorageRouteId;
    }

    return ExistingStack.bTracksFreshness == IncomingStack.bTracksFreshness
        && ExistingStack.AcquiredInGameDay == IncomingStack.AcquiredInGameDay
        && ExistingStack.StaleAfterInGameDays == IncomingStack.StaleAfterInGameDays
        && ExistingStack.SpoilsAfterInGameDays == IncomingStack.SpoilsAfterInGameDays
        && ExistingStack.Freshness == IncomingStack.Freshness
        && ExistingStack.StorageQuality == IncomingStack.StorageQuality
        && ExistingStack.StorageRouteId == IncomingStack.StorageRouteId
        && FMath::IsNearlyEqual(ExistingStack.SpoilageDurationMultiplier, IncomingStack.SpoilageDurationMultiplier);
}

void UHuwamInventoryComponent::RemoveEmptyStacks()
{
    InventoryStacks.RemoveAll([](const FHuwamInventoryStack& Stack)
    {
        return !Stack.IsValidStack();
    });
}
