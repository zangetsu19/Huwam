#include "Gameplay/HuwamRewardComponent.h"

#include "Gameplay/HuwamInventoryComponent.h"
#include "Gameplay/HuwamMonsterEncounterActor.h"

UHuwamRewardComponent::UHuwamRewardComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

int32 UHuwamRewardComponent::AddGold(int32 Amount)
{
    if (Amount <= 0)
    {
        return 0;
    }

    GoldBalance += Amount;
    OnRewardsChanged.Broadcast();
    return Amount;
}

bool UHuwamRewardComponent::SpendGold(int32 Amount)
{
    if (Amount <= 0 || GoldBalance < Amount)
    {
        return false;
    }

    GoldBalance -= Amount;
    OnRewardsChanged.Broadcast();
    return true;
}

int32 UHuwamRewardComponent::GetGoldBalance() const
{
    return GoldBalance;
}

int32 UHuwamRewardComponent::AddExperience(int32 Amount)
{
    if (Amount <= 0)
    {
        return 0;
    }

    TotalExperience += Amount;
    OnRewardsChanged.Broadcast();
    return Amount;
}

int32 UHuwamRewardComponent::GetTotalExperience() const
{
    return TotalExperience;
}

bool UHuwamRewardComponent::BeginQuestObjective(const FString& QuestId, const FString& ObjectiveId, int32 TargetValue)
{
    if (!IsValidQuestProgressKey(QuestId, ObjectiveId))
    {
        return false;
    }

    const int32 ExistingIndex = FindQuestProgressIndex(QuestId, ObjectiveId);
    if (ExistingIndex != INDEX_NONE)
    {
        FHuwamQuestProgressRecord& ExistingRecord = QuestProgressRecords[ExistingIndex];
        ExistingRecord.TargetValue = FMath::Max(ExistingRecord.TargetValue, TargetValue);
        ExistingRecord.bCompleted = ExistingRecord.TargetValue > 0 && ExistingRecord.CurrentValue >= ExistingRecord.TargetValue;
        OnRewardsChanged.Broadcast();
        return true;
    }

    FHuwamQuestProgressRecord NewRecord;
    NewRecord.QuestId = QuestId;
    NewRecord.ObjectiveId = ObjectiveId;
    NewRecord.CurrentValue = 0;
    NewRecord.TargetValue = FMath::Max(0, TargetValue);
    NewRecord.bCompleted = false;
    QuestProgressRecords.Add(NewRecord);

    OnRewardsChanged.Broadcast();
    return true;
}

bool UHuwamRewardComponent::AddQuestProgress(const FString& QuestId, const FString& ObjectiveId, int32 Delta, int32 TargetValue, int32& OutCurrentValue, bool& bOutCompleted)
{
    OutCurrentValue = 0;
    bOutCompleted = false;

    if (!IsValidQuestProgressKey(QuestId, ObjectiveId) || Delta == 0)
    {
        return false;
    }

    if (FindQuestProgressIndex(QuestId, ObjectiveId) == INDEX_NONE)
    {
        BeginQuestObjective(QuestId, ObjectiveId, TargetValue);
    }

    const int32 ProgressIndex = FindQuestProgressIndex(QuestId, ObjectiveId);
    if (ProgressIndex == INDEX_NONE)
    {
        return false;
    }

    FHuwamQuestProgressRecord& ProgressRecord = QuestProgressRecords[ProgressIndex];
    ProgressRecord.TargetValue = FMath::Max(ProgressRecord.TargetValue, TargetValue);
    ProgressRecord.CurrentValue = FMath::Max(0, ProgressRecord.CurrentValue + Delta);

    if (ProgressRecord.TargetValue > 0)
    {
        ProgressRecord.CurrentValue = FMath::Min(ProgressRecord.CurrentValue, ProgressRecord.TargetValue);
        ProgressRecord.bCompleted = ProgressRecord.CurrentValue >= ProgressRecord.TargetValue;
    }
    else
    {
        ProgressRecord.bCompleted = false;
    }

    OutCurrentValue = ProgressRecord.CurrentValue;
    bOutCompleted = ProgressRecord.bCompleted;

    OnRewardsChanged.Broadcast();
    return true;
}

bool UHuwamRewardComponent::GetQuestProgress(const FString& QuestId, const FString& ObjectiveId, FHuwamQuestProgressRecord& OutProgress) const
{
    const int32 ProgressIndex = FindQuestProgressIndex(QuestId, ObjectiveId);
    if (ProgressIndex == INDEX_NONE)
    {
        OutProgress = FHuwamQuestProgressRecord();
        return false;
    }

    OutProgress = QuestProgressRecords[ProgressIndex];
    return true;
}

TArray<FHuwamQuestProgressRecord> UHuwamRewardComponent::GetQuestProgressRecords() const
{
    return QuestProgressRecords;
}

bool UHuwamRewardComponent::ClaimMonsterDefeatRewards(AHuwamMonsterEncounterActor* Encounter, UHuwamInventoryComponent* RecipientInventory, const FString& QuestId, const FString& ObjectiveId, int32 QuestProgressDelta, int32 QuestTargetValue, FHuwamRewardGrantResult& OutResult)
{
    OutResult = FHuwamRewardGrantResult();

    if (!Encounter)
    {
        return false;
    }

    TArray<FHuwamIdQuantity> RewardItems;
    int32 RewardGold = 0;
    int32 RewardExperience = 0;
    if (!Encounter->GrantDefeatRewards(RecipientInventory, RewardItems, RewardGold, RewardExperience))
    {
        return false;
    }

    OutResult.bSuccess = true;
    OutResult.MonsterId = Encounter->GetMonsterId();
    OutResult.ItemsGranted = RewardItems;
    OutResult.GoldGranted = AddGold(RewardGold);
    OutResult.ExperienceGranted = AddExperience(RewardExperience);

    if (IsValidQuestProgressKey(QuestId, ObjectiveId) && QuestProgressDelta != 0)
    {
        OutResult.QuestId = QuestId;
        OutResult.ObjectiveId = ObjectiveId;
        AddQuestProgress(
            QuestId,
            ObjectiveId,
            QuestProgressDelta,
            QuestTargetValue,
            OutResult.QuestProgressAfter,
            OutResult.bQuestObjectiveCompleted
        );
    }

    OnRewardsChanged.Broadcast();
    return true;
}

int32 UHuwamRewardComponent::FindQuestProgressIndex(const FString& QuestId, const FString& ObjectiveId) const
{
    for (int32 Index = 0; Index < QuestProgressRecords.Num(); ++Index)
    {
        const FHuwamQuestProgressRecord& ProgressRecord = QuestProgressRecords[Index];
        if (ProgressRecord.QuestId == QuestId && ProgressRecord.ObjectiveId == ObjectiveId)
        {
            return Index;
        }
    }

    return INDEX_NONE;
}

bool UHuwamRewardComponent::IsValidQuestProgressKey(const FString& QuestId, const FString& ObjectiveId) const
{
    return !QuestId.IsEmpty() && !ObjectiveId.IsEmpty();
}
