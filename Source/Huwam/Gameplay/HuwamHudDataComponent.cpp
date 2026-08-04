#include "Gameplay/HuwamHudDataComponent.h"

#include "Engine/GameInstance.h"
#include "GameFramework/Actor.h"
#include "Gameplay/HuwamCombatComponent.h"
#include "Gameplay/HuwamLiveContentComponent.h"
#include "Gameplay/HuwamRewardComponent.h"
#include "Gameplay/HuwamSurvivalServiceActor.h"

UHuwamHudDataComponent::UHuwamHudDataComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

FHuwamHudSnapshot UHuwamHudDataComponent::RefreshHudSnapshot()
{
    if (UHuwamQuestComponent* Quests = ResolveQuestComponent())
    {
        if (UHuwamRewardComponent* Rewards = ResolveRewardComponent())
        {
            Quests->RefreshAllQuestProgressFromRewards(Rewards);
        }
    }

    LastHudSnapshot = BuildHudSnapshot();
    OnHudSnapshotChanged.Broadcast(LastHudSnapshot);
    return LastHudSnapshot;
}

FHuwamHudSnapshot UHuwamHudDataComponent::GetLastHudSnapshot() const
{
    return LastHudSnapshot;
}

FHuwamHudSnapshot UHuwamHudDataComponent::BuildHudSnapshot(
    UHuwamCombatComponent* Combat,
    UHuwamEquipmentComponent* Equipment,
    UHuwamInventoryComponent* Inventory,
    UHuwamSurvivalVitalsComponent* Survival,
    UHuwamMapAwarenessComponent* MapAwareness,
    UHuwamRewardComponent* Rewards,
    UHuwamQuestComponent* Quests,
    UHuwamLiveContentComponent* LiveContent,
    UHuwamTutorialLabComponent* TutorialLab,
    UHuwamWorldInteractionComponent* WorldInteraction
) const
{
    FHuwamHudSnapshot Snapshot;

    UHuwamCombatComponent* ResolvedCombat = Combat ? Combat : ResolveCombatComponent();
    UHuwamEquipmentComponent* ResolvedEquipment = Equipment ? Equipment : ResolveEquipmentComponent();
    UHuwamInventoryComponent* ResolvedInventory = Inventory ? Inventory : ResolveInventoryComponent();
    UHuwamSurvivalVitalsComponent* ResolvedSurvival = Survival ? Survival : ResolveSurvivalVitalsComponent();
    UHuwamMapAwarenessComponent* ResolvedMapAwareness = MapAwareness ? MapAwareness : ResolveMapAwarenessComponent();
    UHuwamRewardComponent* ResolvedRewards = Rewards ? Rewards : ResolveRewardComponent();
    UHuwamQuestComponent* ResolvedQuests = Quests ? Quests : ResolveQuestComponent();
    UHuwamLiveContentComponent* ResolvedLiveContent = LiveContent ? LiveContent : ResolveLiveContentComponent();
    UHuwamTutorialLabComponent* ResolvedTutorialLab = TutorialLab ? TutorialLab : ResolveTutorialLabComponent();
    UHuwamWorldInteractionComponent* ResolvedWorldInteraction = WorldInteraction ? WorldInteraction : ResolveWorldInteractionComponent();
    UHuwamEconomyLedgerSubsystem* EconomyLedger = GetWorld() && GetWorld()->GetGameInstance()
        ? GetWorld()->GetGameInstance()->GetSubsystem<UHuwamEconomyLedgerSubsystem>()
        : nullptr;

    Snapshot.bHasCombat = ResolvedCombat != nullptr;
    Snapshot.bHasEquipment = ResolvedEquipment != nullptr;
    Snapshot.bHasInventory = ResolvedInventory != nullptr;
    Snapshot.bHasSurvival = ResolvedSurvival != nullptr;
    Snapshot.bHasMapAwareness = ResolvedMapAwareness != nullptr;
    Snapshot.bHasRewards = ResolvedRewards != nullptr;
    Snapshot.bHasQuests = ResolvedQuests != nullptr;
    Snapshot.bHasLiveContent = ResolvedLiveContent != nullptr;
    Snapshot.bHasTutorialLab = ResolvedTutorialLab != nullptr;
    Snapshot.bHasWorldInteraction = ResolvedWorldInteraction != nullptr;
    Snapshot.bHasEconomyLedger = EconomyLedger != nullptr;

    if (ResolvedCombat)
    {
        Snapshot.Vitals.CurrentHealth = ResolvedCombat->GetCurrentHealth();
        Snapshot.Vitals.MaxHealth = ResolvedCombat->GetMaximumHealth();
        Snapshot.Vitals.HealthPercent = Snapshot.Vitals.MaxHealth > 0
            ? FMath::Clamp(static_cast<float>(Snapshot.Vitals.CurrentHealth) / static_cast<float>(Snapshot.Vitals.MaxHealth), 0.0f, 1.0f)
            : 0.0f;
        Snapshot.Vitals.CurrentMana = ResolvedCombat->GetCurrentMana();
        Snapshot.Vitals.MaxMana = ResolvedCombat->GetMaximumMana();
        Snapshot.Vitals.ManaPercent = Snapshot.Vitals.MaxMana > 0
            ? FMath::Clamp(static_cast<float>(Snapshot.Vitals.CurrentMana) / static_cast<float>(Snapshot.Vitals.MaxMana), 0.0f, 1.0f)
            : 0.0f;
        Snapshot.Vitals.bDefeated = ResolvedCombat->IsDefeated();
    }

    if (ResolvedInventory)
    {
        Snapshot.Inventory.UsedSlots = ResolvedInventory->GetUsedSlots();
        Snapshot.Inventory.MaxSlots = ResolvedInventory->GetMaxSlots();
        Snapshot.Inventory.RemainingSlots = ResolvedInventory->GetRemainingSlots();
        Snapshot.Inventory.Stacks = ResolvedInventory->GetInventoryStacks();
        Snapshot.Inventory.StorageRoutes = ResolvedInventory->GetStorageRoutes();
    }

    if (ResolvedSurvival)
    {
        Snapshot.Survival = ResolvedSurvival->RefreshSurvivalVitals();
    }

    if (ResolvedEquipment)
    {
        Snapshot.Equipment.Slots = ResolvedEquipment->GetEquipmentSlots();
        Snapshot.Equipment.StatSummary = ResolvedEquipment->GetEquipmentStatSummary();
    }

    if (ResolvedMapAwareness)
    {
        Snapshot.MapAwareness = ResolvedMapAwareness->GetMapAwarenessSnapshot();
    }

    if (ResolvedRewards)
    {
        Snapshot.Wallet.CurrencyBalanceCopper = ResolvedRewards->GetCurrencyBalanceCopper();
        Snapshot.Wallet.Currency = ResolvedRewards->GetCurrencyBreakdown();
        Snapshot.Wallet.TotalExperience = ResolvedRewards->GetTotalExperience();
    }

    if (ResolvedQuests)
    {
        Snapshot.ActiveQuestStates = ResolvedQuests->GetActiveQuestStates();
        Snapshot.TrackedQuestId = ResolvedQuests->GetTrackedQuestId();
        for (const FHuwamQuestRuntimeState& QuestState : Snapshot.ActiveQuestStates)
        {
            Snapshot.ActiveQuestSummaries.Add(MakeQuestSnapshot(QuestState));
        }
    }

    if (ResolvedLiveContent && bIncludeContentPackDebug)
    {
        for (const FHuwamContentPackRow& ContentPackRow : ResolvedLiveContent->GetKnownContentPacks())
        {
            const FHuwamContentAccessResult AccessResult = ResolvedLiveContent->CanUseContentPack(ContentPackRow.ContentPackId);

            FHuwamHudContentPackSnapshot ContentPackSnapshot;
            ContentPackSnapshot.ContentPackId = ContentPackRow.ContentPackId;
            ContentPackSnapshot.DisplayName = ContentPackRow.DisplayName;
            ContentPackSnapshot.PackType = ContentPackRow.PackType;
            ContentPackSnapshot.PackState = ContentPackRow.PackState;
            ContentPackSnapshot.bAccessible = AccessResult.bAllowed;
            ContentPackSnapshot.AccessReason = AccessResult.bAllowed
                ? NSLOCTEXT("HuwamHUD", "ContentPackAccessible", "Accessible")
                : AccessResult.FailureReason;
            Snapshot.ContentPacks.Add(ContentPackSnapshot);
        }
    }

    if (ResolvedTutorialLab)
    {
        Snapshot.TutorialLab = ResolvedTutorialLab->GetTutorialLabSnapshot();
    }

    if (ResolvedWorldInteraction)
    {
        Snapshot.WorldInteraction = ResolvedWorldInteraction->GetLastInteractionSnapshot();
        if (const AHuwamSurvivalServiceActor* FocusedService = Cast<AHuwamSurvivalServiceActor>(Snapshot.WorldInteraction.TargetActor))
        {
            Snapshot.Economy.bHasFocusedServicePressure = true;
            Snapshot.Economy.FocusedServicePressure = FocusedService->GetServicePressureNeedState();
            Snapshot.Economy.FocusedServiceOwnerNpcId = FocusedService->GetOwnerNpcId();
            Snapshot.Economy.FocusedServiceOwningFactionId = FocusedService->GetOwningFactionId();
            Snapshot.Economy.FocusedServiceSettlementId = FocusedService->GetSettlementId();
            Snapshot.Economy.FocusedServiceCostCopper = FocusedService->GetCostCopper();
            Snapshot.Economy.FocusedServiceUpkeepCostCopper = FocusedService->GetUpkeepCostCopper();
            Snapshot.Economy.bFocusedServiceUnderfunded = FocusedService->IsUnderfunded();
        }
    }

    if (EconomyLedger)
    {
        const FHuwamEconomyLedgerSnapshot LedgerSnapshot = EconomyLedger->GetLedgerSnapshot();
        Snapshot.Economy.TransactionCount = LedgerSnapshot.TransactionCount;
        Snapshot.Economy.TotalRevenueCopper = LedgerSnapshot.TotalRevenueCopper;
        Snapshot.Economy.AccountBalances = LedgerSnapshot.AccountBalances;
    }

    return Snapshot;
}

UHuwamCombatComponent* UHuwamHudDataComponent::ResolveCombatComponent() const
{
    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamCombatComponent>();
    }

    return nullptr;
}

UHuwamEquipmentComponent* UHuwamHudDataComponent::ResolveEquipmentComponent() const
{
    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamEquipmentComponent>();
    }

    return nullptr;
}

UHuwamInventoryComponent* UHuwamHudDataComponent::ResolveInventoryComponent() const
{
    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamInventoryComponent>();
    }

    return nullptr;
}

UHuwamSurvivalVitalsComponent* UHuwamHudDataComponent::ResolveSurvivalVitalsComponent() const
{
    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamSurvivalVitalsComponent>();
    }

    return nullptr;
}

UHuwamMapAwarenessComponent* UHuwamHudDataComponent::ResolveMapAwarenessComponent() const
{
    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamMapAwarenessComponent>();
    }

    return nullptr;
}

UHuwamRewardComponent* UHuwamHudDataComponent::ResolveRewardComponent() const
{
    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamRewardComponent>();
    }

    return nullptr;
}

UHuwamQuestComponent* UHuwamHudDataComponent::ResolveQuestComponent() const
{
    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamQuestComponent>();
    }

    return nullptr;
}

UHuwamLiveContentComponent* UHuwamHudDataComponent::ResolveLiveContentComponent() const
{
    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamLiveContentComponent>();
    }

    return nullptr;
}

UHuwamTutorialLabComponent* UHuwamHudDataComponent::ResolveTutorialLabComponent() const
{
    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamTutorialLabComponent>();
    }

    return nullptr;
}

UHuwamWorldInteractionComponent* UHuwamHudDataComponent::ResolveWorldInteractionComponent() const
{
    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamWorldInteractionComponent>();
    }

    return nullptr;
}

FHuwamHudQuestSnapshot UHuwamHudDataComponent::MakeQuestSnapshot(const FHuwamQuestRuntimeState& QuestState) const
{
    FHuwamHudQuestSnapshot QuestSnapshot;
    QuestSnapshot.QuestId = QuestState.QuestId;
    QuestSnapshot.DisplayName = QuestState.DisplayName;
    QuestSnapshot.ObjectiveId = QuestState.PrimaryObjective.ObjectiveId;
    QuestSnapshot.ObjectiveText = QuestState.PrimaryObjective.Description;
    QuestSnapshot.RequestNeedId = QuestState.NeedRequest.NeedId;
    QuestSnapshot.RequestNeedDisplayName = QuestState.NeedRequest.NeedDisplayName;
    QuestSnapshot.RequestNeedReason = QuestState.NeedRequest.NeedReason;
    QuestSnapshot.RequestNeededQuantity = QuestState.NeedRequest.NeededQuantityAtRequest;
    QuestSnapshot.CurrentValue = QuestState.PrimaryObjective.CurrentValue;
    QuestSnapshot.TargetValue = QuestState.PrimaryObjective.TargetValue;
    QuestSnapshot.ProgressPercent = QuestSnapshot.TargetValue > 0
        ? FMath::Clamp(static_cast<float>(QuestSnapshot.CurrentValue) / static_cast<float>(QuestSnapshot.TargetValue), 0.0f, 1.0f)
        : 0.0f;
    QuestSnapshot.bReadyToTurnIn = QuestState.bReadyToTurnIn;
    return QuestSnapshot;
}
