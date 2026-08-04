#include "Gameplay/HuwamSurvivalVitalsComponent.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Gameplay/HuwamTimeSubsystem.h"

UHuwamSurvivalVitalsComponent::UHuwamSurvivalVitalsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

FHuwamSurvivalVitalsSnapshot UHuwamSurvivalVitalsComponent::ResetSurvivalVitals()
{
    Snapshot = FHuwamSurvivalVitalsSnapshot();
    Snapshot.Hunger = MaxHunger;
    Snapshot.MaxHunger = MaxHunger;
    Snapshot.Thirst = MaxThirst;
    Snapshot.MaxThirst = MaxThirst;
    Snapshot.SocialEnergy = MaxSocialEnergy;
    Snapshot.MaxSocialEnergy = MaxSocialEnergy;
    Snapshot.LastUpdatedInGameDay = GetCurrentInGameDay();
    Snapshot.bLastFoodWasUnsafe = false;
    Snapshot.LastSurvivalMessage = NSLOCTEXT("HuwamSurvival", "SurvivalReset", "Survival vitals are stable.");
    ClampAndRefreshSnapshot();
    BroadcastSnapshot();
    return Snapshot;
}

FHuwamSurvivalVitalsSnapshot UHuwamSurvivalVitalsComponent::RefreshSurvivalVitals()
{
    const int32 CurrentInGameDay = GetCurrentInGameDay();
    const int32 DaysElapsed = FMath::Max(0, CurrentInGameDay - Snapshot.LastUpdatedInGameDay);
    if (DaysElapsed > 0)
    {
        ApplyInGameDaysElapsed(DaysElapsed);
        Snapshot.LastUpdatedInGameDay = CurrentInGameDay;
    }
    else
    {
        ClampAndRefreshSnapshot();
    }

    return Snapshot;
}

FHuwamSurvivalVitalsSnapshot UHuwamSurvivalVitalsComponent::ApplyInGameDaysElapsed(int32 InGameDays)
{
    const int32 SafeDays = FMath::Max(0, InGameDays);
    if (SafeDays <= 0)
    {
        ClampAndRefreshSnapshot();
        return Snapshot;
    }

    Snapshot.Hunger -= HungerDrainPerInGameDay * SafeDays;
    Snapshot.Thirst -= ThirstDrainPerInGameDay * SafeDays;
    Snapshot.SocialEnergy -= SocialDrainPerInGameDay * SafeDays;
    Snapshot.LastSurvivalMessage = FText::Format(
        NSLOCTEXT("HuwamSurvival", "VitalsDrained", "{0} in-game day(s) passed. Hunger, thirst, and social energy shifted."),
        FText::AsNumber(SafeDays)
    );

    ClampAndRefreshSnapshot();
    BroadcastSnapshot();
    return Snapshot;
}

bool UHuwamSurvivalVitalsComponent::ConsumeFoodFromInventory(UHuwamInventoryComponent* Inventory, const FString& ItemId, FHuwamSurvivalVitalsSnapshot& OutSnapshot)
{
    if (!Inventory || ItemId.IsEmpty() || !Inventory->HasItemQuantity(ItemId, 1))
    {
        OutSnapshot = RefreshSurvivalVitals();
        return false;
    }

    RefreshSurvivalVitals();

    const EHuwamInventoryFreshness Freshness = GetBestFreshnessForItem(Inventory, ItemId);
    int32 HungerRestored = 0;
    bool bUnsafe = false;

    switch (Freshness)
    {
    case EHuwamInventoryFreshness::Fresh:
        HungerRestored = FreshFoodHungerRestored;
        Snapshot.LastSurvivalMessage = NSLOCTEXT("HuwamSurvival", "AteFreshFood", "Ate fresh food. Hunger improved.");
        break;
    case EHuwamInventoryFreshness::Stale:
        HungerRestored = StaleFoodHungerRestored;
        Snapshot.LastSurvivalMessage = NSLOCTEXT("HuwamSurvival", "AteStaleFood", "Ate stale food. It helped, but not much.");
        break;
    case EHuwamInventoryFreshness::Spoiled:
        HungerRestored = 0;
        bUnsafe = true;
        Snapshot.LastSurvivalMessage = NSLOCTEXT("HuwamSurvival", "AteSpoiledFood", "Ate spoiled food. That was unsafe.");
        break;
    case EHuwamInventoryFreshness::Stable:
    default:
        HungerRestored = StableFoodHungerRestored;
        Snapshot.LastSurvivalMessage = NSLOCTEXT("HuwamSurvival", "AteStableFood", "Ate preserved food. Hunger improved.");
        break;
    }

    if (!Inventory->RemoveItemById(ItemId, 1))
    {
        OutSnapshot = Snapshot;
        return false;
    }

    Snapshot.Hunger += HungerRestored;
    Snapshot.bLastFoodWasUnsafe = bUnsafe;
    ClampAndRefreshSnapshot();
    BroadcastSnapshot();
    OutSnapshot = Snapshot;
    return true;
}

FHuwamSurvivalVitalsSnapshot UHuwamSurvivalVitalsComponent::DrinkWater(int32 ThirstRestored)
{
    RefreshSurvivalVitals();
    Snapshot.Thirst += FMath::Max(0, ThirstRestored);
    Snapshot.LastSurvivalMessage = NSLOCTEXT("HuwamSurvival", "DrankWater", "Drank water. Thirst improved.");
    ClampAndRefreshSnapshot();
    BroadcastSnapshot();
    return Snapshot;
}

FHuwamSurvivalVitalsSnapshot UHuwamSurvivalVitalsComponent::RestoreSocialEnergy(int32 SocialRestored)
{
    RefreshSurvivalVitals();
    Snapshot.SocialEnergy += FMath::Max(0, SocialRestored);
    Snapshot.LastSurvivalMessage = NSLOCTEXT("HuwamSurvival", "SocialRestored", "Social energy improved.");
    ClampAndRefreshSnapshot();
    BroadcastSnapshot();
    return Snapshot;
}

FHuwamSurvivalVitalsSnapshot UHuwamSurvivalVitalsComponent::UseSurvivalService(EHuwamSurvivalServiceType ServiceType)
{
    RefreshSurvivalVitals();

    switch (ServiceType)
    {
    case EHuwamSurvivalServiceType::TavernMeal:
        Snapshot.Hunger += 40;
        Snapshot.SocialEnergy += 5;
        Snapshot.bLastFoodWasUnsafe = false;
        Snapshot.LastSurvivalMessage = NSLOCTEXT("HuwamSurvival", "TavernMealService", "A tavern meal restored hunger without moving server time forward.");
        break;
    case EHuwamSurvivalServiceType::WellWater:
        Snapshot.Thirst += 40;
        Snapshot.LastSurvivalMessage = NSLOCTEXT("HuwamSurvival", "WellWaterService", "Well water restored thirst without moving server time forward.");
        break;
    case EHuwamSurvivalServiceType::CampfireChat:
        Snapshot.SocialEnergy += 35;
        Snapshot.LastSurvivalMessage = NSLOCTEXT("HuwamSurvival", "CampfireChatService", "A calm conversation restored social energy without moving server time forward.");
        break;
    case EHuwamSurvivalServiceType::InnRoom:
        Snapshot.Hunger += 10;
        Snapshot.Thirst += 10;
        Snapshot.SocialEnergy += 20;
        Snapshot.LastSurvivalMessage = NSLOCTEXT("HuwamSurvival", "InnRoomService", "An inn room restored comfort without skipping the living server clock.");
        break;
    default:
        break;
    }

    ClampAndRefreshSnapshot();
    BroadcastSnapshot();
    return Snapshot;
}

bool UHuwamSurvivalVitalsComponent::ApplySurvivalConsequences(UHuwamCombatComponent* Combat, FHuwamCombatResult& OutDamageResult)
{
    OutDamageResult = FHuwamCombatResult();
    RefreshSurvivalVitals();

    if (!Combat)
    {
        Snapshot.LastConsequenceDamage = 0;
        return false;
    }

    int32 DamageToApply = 0;
    if (Snapshot.bStarving)
    {
        DamageToApply += StarvationDamagePerConsequence;
    }
    if (Snapshot.bDehydrated)
    {
        DamageToApply += DehydrationDamagePerConsequence;
    }
    if (Snapshot.bLastFoodWasUnsafe)
    {
        DamageToApply += UnsafeFoodDamagePerConsequence;
    }

    Snapshot.LastConsequenceDamage = DamageToApply;
    if (DamageToApply <= 0)
    {
        Snapshot.LastSurvivalMessage = NSLOCTEXT("HuwamSurvival", "NoSurvivalConsequence", "No survival consequence damage applied.");
        ClampAndRefreshSnapshot();
        BroadcastSnapshot();
        return false;
    }

    FHuwamCombatResult PendingDamage;
    PendingDamage.bSuccess = true;
    PendingDamage.AttackType = EHuwamCombatAttackType::TrueDamage;
    PendingDamage.RawDamage = DamageToApply;
    PendingDamage.FinalDamage = DamageToApply;
    PendingDamage.SourceTag = TEXT("survival.consequence");

    const bool bApplied = Combat->ApplyResolvedDamage(PendingDamage, OutDamageResult);
    Snapshot.LastSurvivalMessage = FText::Format(
        NSLOCTEXT("HuwamSurvival", "SurvivalConsequenceDamage", "Survival conditions caused {0} true damage."),
        FText::AsNumber(DamageToApply)
    );
    ClampAndRefreshSnapshot();
    BroadcastSnapshot();
    return bApplied;
}

FHuwamSurvivalVitalsSnapshot UHuwamSurvivalVitalsComponent::GetSurvivalVitalsSnapshot() const
{
    return Snapshot;
}

void UHuwamSurvivalVitalsComponent::RestoreSurvivalVitalsSnapshot(const FHuwamSurvivalVitalsSnapshot& SavedSnapshot)
{
    Snapshot = SavedSnapshot;
    ClampAndRefreshSnapshot();
    BroadcastSnapshot();
}

int32 UHuwamSurvivalVitalsComponent::GetCurrentInGameDay() const
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

EHuwamSurvivalNeedState UHuwamSurvivalVitalsComponent::GetNeedState(int32 CurrentValue, int32 MaxValue) const
{
    if (MaxValue <= 0 || CurrentValue <= 0)
    {
        return EHuwamSurvivalNeedState::Empty;
    }

    const float Percent = static_cast<float>(CurrentValue) / static_cast<float>(MaxValue);
    if (Percent >= 0.85f)
    {
        return EHuwamSurvivalNeedState::Full;
    }
    if (Percent >= 0.45f)
    {
        return EHuwamSurvivalNeedState::Comfortable;
    }
    if (Percent >= 0.20f)
    {
        return EHuwamSurvivalNeedState::Low;
    }

    return EHuwamSurvivalNeedState::Critical;
}

float UHuwamSurvivalVitalsComponent::GetNeedPercent(int32 CurrentValue, int32 MaxValue) const
{
    return MaxValue > 0 ? FMath::Clamp(static_cast<float>(CurrentValue) / static_cast<float>(MaxValue), 0.0f, 1.0f) : 0.0f;
}

void UHuwamSurvivalVitalsComponent::ClampAndRefreshSnapshot()
{
    Snapshot.MaxHunger = MaxHunger;
    Snapshot.MaxThirst = MaxThirst;
    Snapshot.MaxSocialEnergy = MaxSocialEnergy;
    Snapshot.Hunger = FMath::Clamp(Snapshot.Hunger, 0, Snapshot.MaxHunger);
    Snapshot.Thirst = FMath::Clamp(Snapshot.Thirst, 0, Snapshot.MaxThirst);
    Snapshot.SocialEnergy = FMath::Clamp(Snapshot.SocialEnergy, 0, Snapshot.MaxSocialEnergy);
    Snapshot.HungerPercent = GetNeedPercent(Snapshot.Hunger, Snapshot.MaxHunger);
    Snapshot.ThirstPercent = GetNeedPercent(Snapshot.Thirst, Snapshot.MaxThirst);
    Snapshot.SocialEnergyPercent = GetNeedPercent(Snapshot.SocialEnergy, Snapshot.MaxSocialEnergy);
    Snapshot.HungerState = GetNeedState(Snapshot.Hunger, Snapshot.MaxHunger);
    Snapshot.ThirstState = GetNeedState(Snapshot.Thirst, Snapshot.MaxThirst);
    Snapshot.SocialEnergyState = GetNeedState(Snapshot.SocialEnergy, Snapshot.MaxSocialEnergy);
    Snapshot.bStarving = Snapshot.HungerState == EHuwamSurvivalNeedState::Empty;
    Snapshot.bDehydrated = Snapshot.ThirstState == EHuwamSurvivalNeedState::Empty;
    Snapshot.bIsolated = Snapshot.SocialEnergyState == EHuwamSurvivalNeedState::Empty;
    Snapshot.ActiveEffectIds.Reset();
    if (Snapshot.bStarving)
    {
        Snapshot.ActiveEffectIds.Add(TEXT("effect.survival.starving"));
    }
    if (Snapshot.bDehydrated)
    {
        Snapshot.ActiveEffectIds.Add(TEXT("effect.survival.dehydrated"));
    }
    if (Snapshot.bIsolated)
    {
        Snapshot.ActiveEffectIds.Add(TEXT("effect.survival.isolated"));
    }
    if (Snapshot.bLastFoodWasUnsafe)
    {
        Snapshot.ActiveEffectIds.Add(TEXT("effect.survival.unsafe_food"));
    }
}

void UHuwamSurvivalVitalsComponent::BroadcastSnapshot()
{
    OnSurvivalVitalsChanged.Broadcast(Snapshot);
}

EHuwamInventoryFreshness UHuwamSurvivalVitalsComponent::GetBestFreshnessForItem(UHuwamInventoryComponent* Inventory, const FString& ItemId) const
{
    if (!Inventory)
    {
        return EHuwamInventoryFreshness::Stable;
    }

    EHuwamInventoryFreshness BestFreshness = EHuwamInventoryFreshness::Spoiled;
    bool bFoundTrackedStack = false;

    for (const FHuwamInventoryStack& Stack : Inventory->GetInventoryStacks())
    {
        if (!Stack.IsValidStack() || Stack.ItemId != ItemId)
        {
            continue;
        }

        if (!Stack.bTracksFreshness)
        {
            return EHuwamInventoryFreshness::Stable;
        }

        bFoundTrackedStack = true;
        if (Stack.Freshness == EHuwamInventoryFreshness::Fresh)
        {
            return EHuwamInventoryFreshness::Fresh;
        }
        if (Stack.Freshness == EHuwamInventoryFreshness::Stale)
        {
            BestFreshness = EHuwamInventoryFreshness::Stale;
        }
    }

    return bFoundTrackedStack ? BestFreshness : EHuwamInventoryFreshness::Stable;
}
