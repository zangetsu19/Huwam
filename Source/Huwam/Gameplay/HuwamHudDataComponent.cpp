#include "Gameplay/HuwamHudDataComponent.h"

#include "GameFramework/Actor.h"
#include "Gameplay/HuwamCombatComponent.h"
#include "Gameplay/HuwamLiveContentComponent.h"
#include "Gameplay/HuwamRewardComponent.h"

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
    UHuwamInventoryComponent* Inventory,
    UHuwamRewardComponent* Rewards,
    UHuwamQuestComponent* Quests,
    UHuwamLiveContentComponent* LiveContent
) const
{
    FHuwamHudSnapshot Snapshot;

    UHuwamCombatComponent* ResolvedCombat = Combat ? Combat : ResolveCombatComponent();
    UHuwamInventoryComponent* ResolvedInventory = Inventory ? Inventory : ResolveInventoryComponent();
    UHuwamRewardComponent* ResolvedRewards = Rewards ? Rewards : ResolveRewardComponent();
    UHuwamQuestComponent* ResolvedQuests = Quests ? Quests : ResolveQuestComponent();
    UHuwamLiveContentComponent* ResolvedLiveContent = LiveContent ? LiveContent : ResolveLiveContentComponent();

    Snapshot.bHasCombat = ResolvedCombat != nullptr;
    Snapshot.bHasInventory = ResolvedInventory != nullptr;
    Snapshot.bHasRewards = ResolvedRewards != nullptr;
    Snapshot.bHasQuests = ResolvedQuests != nullptr;
    Snapshot.bHasLiveContent = ResolvedLiveContent != nullptr;

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
    }

    if (ResolvedRewards)
    {
        Snapshot.Wallet.GoldBalance = ResolvedRewards->GetGoldBalance();
        Snapshot.Wallet.TotalExperience = ResolvedRewards->GetTotalExperience();
    }

    if (ResolvedQuests)
    {
        Snapshot.ActiveQuestStates = ResolvedQuests->GetActiveQuestStates();
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

UHuwamInventoryComponent* UHuwamHudDataComponent::ResolveInventoryComponent() const
{
    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamInventoryComponent>();
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

FHuwamHudQuestSnapshot UHuwamHudDataComponent::MakeQuestSnapshot(const FHuwamQuestRuntimeState& QuestState) const
{
    FHuwamHudQuestSnapshot QuestSnapshot;
    QuestSnapshot.QuestId = QuestState.QuestId;
    QuestSnapshot.DisplayName = QuestState.DisplayName;
    QuestSnapshot.ObjectiveId = QuestState.PrimaryObjective.ObjectiveId;
    QuestSnapshot.ObjectiveText = QuestState.PrimaryObjective.Description;
    QuestSnapshot.CurrentValue = QuestState.PrimaryObjective.CurrentValue;
    QuestSnapshot.TargetValue = QuestState.PrimaryObjective.TargetValue;
    QuestSnapshot.ProgressPercent = QuestSnapshot.TargetValue > 0
        ? FMath::Clamp(static_cast<float>(QuestSnapshot.CurrentValue) / static_cast<float>(QuestSnapshot.TargetValue), 0.0f, 1.0f)
        : 0.0f;
    QuestSnapshot.bReadyToTurnIn = QuestState.bReadyToTurnIn;
    return QuestSnapshot;
}
