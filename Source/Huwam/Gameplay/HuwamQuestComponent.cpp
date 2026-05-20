#include "Gameplay/HuwamQuestComponent.h"

#include "Data/HuwamDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Gameplay/HuwamInventoryComponent.h"
#include "Gameplay/HuwamRewardComponent.h"

namespace
{
    const TCHAR* SlimesQuestId = TEXT("quest.eldoria.slimes_in_tall_grass");
    const TCHAR* SlimesObjectiveId = TEXT("objective.defeat_basic_slime");
    const TCHAR* SlimesProgressSourceId = TEXT("monster.slime.basic");

    FName GetRowNameFromId(const FString& Id)
    {
        return FName(*Id.Replace(TEXT("."), TEXT("_")));
    }
}

UHuwamQuestComponent::UHuwamQuestComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UHuwamQuestComponent::StartQuest(const FString& QuestId, UHuwamRewardComponent* Rewards)
{
    if (QuestId.IsEmpty() || TurnedInQuestIds.Contains(QuestId))
    {
        return false;
    }

    const int32 ExistingIndex = FindQuestIndex(QuestId);
    if (ExistingIndex != INDEX_NONE)
    {
        if (Rewards)
        {
            RefreshQuestProgressFromRewards(QuestId, Rewards);
        }

        return QuestStates[ExistingIndex].bActive;
    }

    FHuwamQuestRow QuestRow;
    FHuwamQuestRuntimeState QuestState = LoadQuestRow(QuestId, QuestRow)
        ? MakeQuestStateFromRow(QuestRow)
        : (QuestId == SlimesQuestId ? MakeSlimesInTallGrassFallbackState() : FHuwamQuestRuntimeState());

    if (QuestState.QuestId.IsEmpty())
    {
        return false;
    }

    QuestState.bActive = true;
    QuestStates.Add(QuestState);

    if (Rewards && !QuestState.PrimaryObjective.ObjectiveId.IsEmpty())
    {
        Rewards->BeginQuestObjective(
            QuestState.QuestId,
            QuestState.PrimaryObjective.ObjectiveId,
            QuestState.PrimaryObjective.TargetValue
        );
        RefreshQuestProgressFromRewards(QuestState.QuestId, Rewards);
    }

    OnQuestsChanged.Broadcast();
    return true;
}

bool UHuwamQuestComponent::StartSlimesInTallGrassQuest(UHuwamRewardComponent* Rewards)
{
    return StartQuest(SlimesQuestId, Rewards);
}

bool UHuwamQuestComponent::RefreshQuestProgressFromRewards(const FString& QuestId, UHuwamRewardComponent* Rewards)
{
    if (!Rewards)
    {
        return false;
    }

    const int32 QuestIndex = FindQuestIndex(QuestId);
    if (QuestIndex == INDEX_NONE)
    {
        return false;
    }

    FHuwamQuestRuntimeState& QuestState = QuestStates[QuestIndex];
    if (!QuestState.bActive || QuestState.bTurnedIn || QuestState.PrimaryObjective.ObjectiveId.IsEmpty())
    {
        return false;
    }

    FHuwamQuestProgressRecord ProgressRecord;
    if (!Rewards->GetQuestProgress(QuestState.QuestId, QuestState.PrimaryObjective.ObjectiveId, ProgressRecord))
    {
        Rewards->BeginQuestObjective(
            QuestState.QuestId,
            QuestState.PrimaryObjective.ObjectiveId,
            QuestState.PrimaryObjective.TargetValue
        );

        if (!Rewards->GetQuestProgress(QuestState.QuestId, QuestState.PrimaryObjective.ObjectiveId, ProgressRecord))
        {
            return false;
        }
    }

    QuestState.PrimaryObjective.CurrentValue = ProgressRecord.CurrentValue;
    QuestState.PrimaryObjective.TargetValue = FMath::Max(QuestState.PrimaryObjective.TargetValue, ProgressRecord.TargetValue);
    QuestState.PrimaryObjective.bCompleted = ProgressRecord.bCompleted;
    QuestState.bReadyToTurnIn = QuestState.PrimaryObjective.bCompleted;

    OnQuestsChanged.Broadcast();
    return true;
}

void UHuwamQuestComponent::RefreshAllQuestProgressFromRewards(UHuwamRewardComponent* Rewards)
{
    if (!Rewards)
    {
        return;
    }

    for (const FHuwamQuestRuntimeState& QuestState : QuestStates)
    {
        RefreshQuestProgressFromRewards(QuestState.QuestId, Rewards);
    }
}

bool UHuwamQuestComponent::CanTurnInQuest(const FString& QuestId) const
{
    const int32 QuestIndex = FindQuestIndex(QuestId);
    return QuestIndex != INDEX_NONE
        && QuestStates[QuestIndex].bActive
        && QuestStates[QuestIndex].bReadyToTurnIn
        && !QuestStates[QuestIndex].bTurnedIn;
}

bool UHuwamQuestComponent::TurnInQuest(const FString& QuestId, UHuwamRewardComponent* Rewards, UHuwamInventoryComponent* Inventory, FHuwamQuestRuntimeState& OutTurnedInQuest)
{
    OutTurnedInQuest = FHuwamQuestRuntimeState();

    const int32 QuestIndex = FindQuestIndex(QuestId);
    if (QuestIndex == INDEX_NONE || !CanTurnInQuest(QuestId) || !Rewards)
    {
        return false;
    }

    FHuwamQuestRuntimeState& QuestState = QuestStates[QuestIndex];
    if (!GrantQuestRewards(QuestState, Rewards, Inventory))
    {
        return false;
    }

    QuestState.bActive = false;
    QuestState.bReadyToTurnIn = false;
    QuestState.bTurnedIn = true;
    TurnedInQuestIds.AddUnique(QuestState.QuestId);
    OutTurnedInQuest = QuestState;

    OnQuestsChanged.Broadcast();
    return true;
}

bool UHuwamQuestComponent::GetQuestState(const FString& QuestId, FHuwamQuestRuntimeState& OutQuestState) const
{
    const int32 QuestIndex = FindQuestIndex(QuestId);
    if (QuestIndex == INDEX_NONE)
    {
        OutQuestState = FHuwamQuestRuntimeState();
        return false;
    }

    OutQuestState = QuestStates[QuestIndex];
    return true;
}

TArray<FHuwamQuestRuntimeState> UHuwamQuestComponent::GetQuestStates() const
{
    return QuestStates;
}

TArray<FHuwamQuestRuntimeState> UHuwamQuestComponent::GetActiveQuestStates() const
{
    TArray<FHuwamQuestRuntimeState> ActiveQuestStates;
    for (const FHuwamQuestRuntimeState& QuestState : QuestStates)
    {
        if (QuestState.bActive && !QuestState.bTurnedIn)
        {
            ActiveQuestStates.Add(QuestState);
        }
    }

    return ActiveQuestStates;
}

TArray<FString> UHuwamQuestComponent::GetTurnedInQuestIds() const
{
    return TurnedInQuestIds;
}

int32 UHuwamQuestComponent::FindQuestIndex(const FString& QuestId) const
{
    for (int32 Index = 0; Index < QuestStates.Num(); ++Index)
    {
        if (QuestStates[Index].QuestId == QuestId)
        {
            return Index;
        }
    }

    return INDEX_NONE;
}

bool UHuwamQuestComponent::LoadQuestRow(const FString& QuestId, FHuwamQuestRow& OutQuestRow) const
{
    if (QuestId.IsEmpty())
    {
        return false;
    }

    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (const UHuwamDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UHuwamDataSubsystem>())
            {
                return DataSubsystem->GetQuestRow(GetRowNameFromId(QuestId), OutQuestRow);
            }
        }
    }

    return false;
}

FHuwamQuestRuntimeState UHuwamQuestComponent::MakeQuestStateFromRow(const FHuwamQuestRow& QuestRow) const
{
    FHuwamQuestRuntimeState QuestState;
    QuestState.QuestId = QuestRow.QuestId;
    QuestState.DisplayName = QuestRow.DisplayName;
    QuestState.Description = QuestRow.Description;
    QuestState.QuestType = QuestRow.QuestType;
    QuestState.Urgency = QuestRow.Urgency;
    QuestState.QuestGiverId = QuestRow.QuestGiverId;
    QuestState.RewardItems = QuestRow.RewardItems;
    QuestState.RewardGold = QuestRow.RewardGold;
    QuestState.RewardExperience = QuestRow.RewardExperience;
    QuestState.WorldStateEffect = QuestRow.WorldStateEffect;

    QuestState.PrimaryObjective.ObjectiveId = QuestRow.PrimaryObjectiveId;
    QuestState.PrimaryObjective.Description = QuestRow.PrimaryObjectiveDescription;
    QuestState.PrimaryObjective.ProgressSourceId = QuestRow.PrimaryObjectiveProgressSourceId;
    QuestState.PrimaryObjective.TargetValue = QuestRow.PrimaryObjectiveTargetValue;
    QuestState.PrimaryObjective.CurrentValue = 0;
    QuestState.PrimaryObjective.bCompleted = QuestState.PrimaryObjective.TargetValue == 0 && QuestState.PrimaryObjective.ObjectiveId.IsEmpty();
    QuestState.bReadyToTurnIn = QuestState.PrimaryObjective.bCompleted;

    return QuestState;
}

FHuwamQuestRuntimeState UHuwamQuestComponent::MakeSlimesInTallGrassFallbackState() const
{
    FHuwamQuestRuntimeState QuestState;
    QuestState.QuestId = SlimesQuestId;
    QuestState.DisplayName = NSLOCTEXT("HuwamQuest", "SlimesInTallGrassName", "Slimes in the Tall Grass");
    QuestState.Description = NSLOCTEXT("HuwamQuest", "SlimesInTallGrassDescription", "Defeat or harvest basic slimes near Eldoria.");
    QuestState.QuestType = EHuwamQuestType::Combat;
    QuestState.Urgency = EHuwamQuestUrgency::Normal;
    QuestState.QuestGiverId = TEXT("guild.adventurers");
    QuestState.RewardGold = 15;
    QuestState.RewardExperience = 10;
    QuestState.WorldStateEffect = NSLOCTEXT("HuwamQuest", "SlimesInTallGrassWorldStateEffect", "Slime population and guild stock update.");
    QuestState.PrimaryObjective.ObjectiveId = SlimesObjectiveId;
    QuestState.PrimaryObjective.Description = NSLOCTEXT("HuwamQuest", "SlimesInTallGrassObjective", "Defeat or harvest 3 basic slimes.");
    QuestState.PrimaryObjective.ProgressSourceId = SlimesProgressSourceId;
    QuestState.PrimaryObjective.TargetValue = 3;
    QuestState.PrimaryObjective.CurrentValue = 0;
    QuestState.PrimaryObjective.bCompleted = false;
    QuestState.bReadyToTurnIn = false;
    return QuestState;
}

bool UHuwamQuestComponent::GrantQuestRewards(const FHuwamQuestRuntimeState& QuestState, UHuwamRewardComponent* Rewards, UHuwamInventoryComponent* Inventory) const
{
    if (!Rewards)
    {
        return false;
    }

    if (!QuestState.RewardItems.IsEmpty() && !Inventory)
    {
        return false;
    }

    if (Inventory)
    {
        for (const FHuwamIdQuantity& RewardItem : QuestState.RewardItems)
        {
            if (RewardItem.Id.IsEmpty() || RewardItem.Quantity <= 0)
            {
                continue;
            }

            int32 Remainder = 0;
            if (!Inventory->AddItemById(RewardItem.Id, RewardItem.Quantity, Remainder) || Remainder > 0)
            {
                return false;
            }
        }
    }

    Rewards->AddGold(QuestState.RewardGold);
    Rewards->AddExperience(QuestState.RewardExperience);
    return true;
}
