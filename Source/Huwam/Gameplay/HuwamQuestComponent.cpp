#include "Gameplay/HuwamQuestComponent.h"

#include "Data/HuwamDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Gameplay/HuwamInventoryComponent.h"
#include "Gameplay/HuwamRewardComponent.h"

namespace
{
    const TCHAR* TutorialQuestId = TEXT("quest.tutorial.progression_zero");
    const TCHAR* TutorialObjectiveId = TEXT("objective.complete_tutorial");
    const TCHAR* TutorialProgressSourceId = TEXT("progress.tutorial_complete");
    const TCHAR* SlimesQuestId = TEXT("quest.eldoria.slimes_in_tall_grass");
    const TCHAR* SlimesObjectiveId = TEXT("objective.defeat_basic_slime");
    const TCHAR* SlimesProgressSourceId = TEXT("monster.slime.basic");
    const TCHAR* GatherFieldSuppliesQuestId = TEXT("quest.eldoria.gather_field_supplies");
    const TCHAR* GatherFieldSuppliesObjectiveId = TEXT("objective.gather_field_supplies");
    const TCHAR* GatherFieldSuppliesProgressSourceId = TEXT("material.field_supply");
    const TCHAR* ServiceRestorationQuestPrefix = TEXT("quest.eldoria.service_restoration.");
    const TCHAR* ServiceRestorationObjectiveId = TEXT("objective.restore_service");

    FName GetRowNameFromId(const FString& Id)
    {
        return FName(*Id.Replace(TEXT("."), TEXT("_")));
    }

    FString BuildServiceRestorationQuestId(const FString& NeedId)
    {
        return FString(ServiceRestorationQuestPrefix) + NeedId;
    }

    EHuwamQuestUrgency MapServiceNeedUrgency(EHuwamNpcSupplyNeedUrgency NeedUrgency)
    {
        switch (NeedUrgency)
        {
        case EHuwamNpcSupplyNeedUrgency::Critical:
            return EHuwamQuestUrgency::Emergency;
        case EHuwamNpcSupplyNeedUrgency::Urgent:
            return EHuwamQuestUrgency::Urgent;
        case EHuwamNpcSupplyNeedUrgency::Low:
            return EHuwamQuestUrgency::Timed;
        case EHuwamNpcSupplyNeedUrgency::Stable:
        default:
            return EHuwamQuestUrgency::Normal;
        }
    }
}

UHuwamQuestComponent::UHuwamQuestComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UHuwamQuestComponent::StartQuest(const FString& QuestId, UHuwamRewardComponent* Rewards)
{
    if (QuestId.IsEmpty())
    {
        return false;
    }

    FHuwamQuestRuntimeState QuestState;
    if (!MakeQuestPreviewState(QuestId, QuestState))
    {
        return false;
    }

    const bool bQuestWasTurnedIn = TurnedInQuestIds.Contains(QuestId);
    if (bQuestWasTurnedIn && !QuestState.bRepeatable)
    {
        return false;
    }

    const int32 ExistingIndex = FindQuestIndex(QuestId);
    if (ExistingIndex != INDEX_NONE)
    {
        FHuwamQuestRuntimeState& ExistingQuestState = QuestStates[ExistingIndex];
        if (ExistingQuestState.bTurnedIn && QuestState.bRepeatable)
        {
            QuestState.bActive = true;
            QuestStates[ExistingIndex] = QuestState;
            TurnedInQuestIds.Remove(QuestId);

            if (Rewards && !QuestState.PrimaryObjective.ObjectiveId.IsEmpty())
            {
                Rewards->ResetQuestObjectiveProgress(
                    QuestState.QuestId,
                    QuestState.PrimaryObjective.ObjectiveId,
                    QuestState.PrimaryObjective.TargetValue
                );
                RefreshQuestProgressFromRewards(QuestState.QuestId, Rewards);
            }

            OnQuestsChanged.Broadcast();
            return true;
        }

        if (Rewards)
        {
            RefreshQuestProgressFromRewards(QuestId, Rewards);
        }

        return ExistingQuestState.bActive;
    }

    QuestState.bActive = true;
    QuestStates.Add(QuestState);
    if (bQuestWasTurnedIn && QuestState.bRepeatable)
    {
        TurnedInQuestIds.Remove(QuestId);
    }

    if (Rewards && !QuestState.PrimaryObjective.ObjectiveId.IsEmpty())
    {
        if (bQuestWasTurnedIn && QuestState.bRepeatable)
        {
            Rewards->ResetQuestObjectiveProgress(
                QuestState.QuestId,
                QuestState.PrimaryObjective.ObjectiveId,
                QuestState.PrimaryObjective.TargetValue
            );
        }
        else
        {
            Rewards->BeginQuestObjective(
                QuestState.QuestId,
                QuestState.PrimaryObjective.ObjectiveId,
                QuestState.PrimaryObjective.TargetValue
            );
        }
        RefreshQuestProgressFromRewards(QuestState.QuestId, Rewards);
    }

    OnQuestsChanged.Broadcast();
    return true;
}

bool UHuwamQuestComponent::StartSlimesInTallGrassQuest(UHuwamRewardComponent* Rewards)
{
    return StartQuest(SlimesQuestId, Rewards);
}

bool UHuwamQuestComponent::StartGatherFieldSuppliesQuest(UHuwamRewardComponent* Rewards)
{
    return StartQuest(GatherFieldSuppliesQuestId, Rewards);
}

bool UHuwamQuestComponent::StartProgressionZeroTutorialQuest(UHuwamRewardComponent* Rewards)
{
    return StartQuest(TutorialQuestId, Rewards);
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

bool UHuwamQuestComponent::TrackQuest(const FString& QuestId)
{
    const int32 QuestIndex = FindQuestIndex(QuestId);
    if (QuestIndex == INDEX_NONE || !QuestStates[QuestIndex].bActive || QuestStates[QuestIndex].bTurnedIn)
    {
        return false;
    }

    if (TrackedQuestId == QuestId)
    {
        return true;
    }

    TrackedQuestId = QuestId;
    OnQuestsChanged.Broadcast();
    return true;
}

bool UHuwamQuestComponent::SetQuestNeedRequestContext(const FString& QuestId, const FHuwamQuestNeedRequestContext& NeedRequest)
{
    const int32 QuestIndex = FindQuestIndex(QuestId);
    if (QuestIndex == INDEX_NONE || !QuestStates[QuestIndex].bActive || QuestStates[QuestIndex].bTurnedIn)
    {
        return false;
    }

    QuestStates[QuestIndex].NeedRequest = NeedRequest;
    OnQuestsChanged.Broadcast();
    return true;
}

bool UHuwamQuestComponent::GetQuestOffer(const FString& QuestId, FHuwamQuestOffer& OutQuestOffer) const
{
    OutQuestOffer = FHuwamQuestOffer();

    if (QuestId.IsEmpty())
    {
        return false;
    }

    FHuwamQuestRuntimeState QuestState;
    if (!MakeQuestPreviewState(QuestId, QuestState))
    {
        return false;
    }

    if (TurnedInQuestIds.Contains(QuestId) && !QuestState.bRepeatable)
    {
        return false;
    }

    const int32 ExistingIndex = FindQuestIndex(QuestId);
    if (ExistingIndex != INDEX_NONE && QuestStates[ExistingIndex].bActive && !QuestStates[ExistingIndex].bTurnedIn)
    {
        return false;
    }

    OutQuestOffer = MakeQuestOfferFromState(QuestState);
    return !OutQuestOffer.QuestId.IsEmpty();
}

bool UHuwamQuestComponent::GetServiceRestorationQuestOfferFromPressure(const FHuwamNpcSupplyNeedState& ServicePressure, FHuwamQuestOffer& OutQuestOffer) const
{
    OutQuestOffer = FHuwamQuestOffer();
    if (ServicePressure.NeedId.IsEmpty() || !ServicePressure.bShouldRequestSupplyHelp)
    {
        return false;
    }

    const FString QuestId = BuildServiceRestorationQuestId(ServicePressure.NeedId);
    const int32 ExistingIndex = FindQuestIndex(QuestId);
    if (ExistingIndex != INDEX_NONE && QuestStates[ExistingIndex].bActive && !QuestStates[ExistingIndex].bTurnedIn)
    {
        return false;
    }

    OutQuestOffer = MakeQuestOfferFromState(MakeServiceRestorationFallbackState(ServicePressure));
    return !OutQuestOffer.QuestId.IsEmpty();
}

bool UHuwamQuestComponent::StartServiceRestorationQuestFromPressure(const FHuwamNpcSupplyNeedState& ServicePressure, UHuwamRewardComponent* Rewards)
{
    FHuwamQuestOffer QuestOffer;
    if (!GetServiceRestorationQuestOfferFromPressure(ServicePressure, QuestOffer))
    {
        return false;
    }

    FHuwamQuestRuntimeState QuestState = MakeServiceRestorationFallbackState(ServicePressure);
    QuestState.bActive = true;

    const int32 ExistingIndex = FindQuestIndex(QuestState.QuestId);
    if (ExistingIndex != INDEX_NONE)
    {
        QuestStates[ExistingIndex] = QuestState;
    }
    else
    {
        QuestStates.Add(QuestState);
    }

    TurnedInQuestIds.Remove(QuestState.QuestId);
    TrackedQuestId = QuestState.QuestId;

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

bool UHuwamQuestComponent::CanTurnInQuest(const FString& QuestId) const
{
    const int32 QuestIndex = FindQuestIndex(QuestId);
    return QuestIndex != INDEX_NONE
        && QuestStates[QuestIndex].bActive
        && QuestStates[QuestIndex].bReadyToTurnIn
        && !QuestStates[QuestIndex].bTurnedIn;
}

bool UHuwamQuestComponent::IsQuestTracked(const FString& QuestId) const
{
    return !QuestId.IsEmpty() && TrackedQuestId == QuestId;
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
    if (TrackedQuestId == QuestState.QuestId)
    {
        TrackedQuestId.Empty();
    }
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

FString UHuwamQuestComponent::GetTrackedQuestId() const
{
    return TrackedQuestId;
}

void UHuwamQuestComponent::RestoreQuestStateForPrototypeSave(const TArray<FHuwamQuestRuntimeState>& SavedQuestStates, const TArray<FString>& SavedTurnedInQuestIds, const FString& SavedTrackedQuestId)
{
    QuestStates = SavedQuestStates;
    TurnedInQuestIds = SavedTurnedInQuestIds;
    TrackedQuestId = SavedTrackedQuestId;
    OnQuestsChanged.Broadcast();
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

bool UHuwamQuestComponent::MakeQuestPreviewState(const FString& QuestId, FHuwamQuestRuntimeState& OutQuestState) const
{
    FHuwamQuestRow QuestRow;
    OutQuestState = LoadQuestRow(QuestId, QuestRow)
        ? MakeQuestStateFromRow(QuestRow)
        : (QuestId == TutorialQuestId
            ? MakeProgressionZeroTutorialFallbackState()
            : (QuestId == SlimesQuestId
                ? MakeSlimesInTallGrassFallbackState()
                : (QuestId == GatherFieldSuppliesQuestId ? MakeGatherFieldSuppliesFallbackState() : FHuwamQuestRuntimeState())));

    return !OutQuestState.QuestId.IsEmpty();
}

FHuwamQuestOffer UHuwamQuestComponent::MakeQuestOfferFromState(const FHuwamQuestRuntimeState& QuestState) const
{
    FHuwamQuestOffer QuestOffer;
    QuestOffer.QuestId = QuestState.QuestId;
    QuestOffer.DisplayName = QuestState.DisplayName;
    QuestOffer.Description = QuestState.Description;
    QuestOffer.QuestType = QuestState.QuestType;
    QuestOffer.Urgency = QuestState.Urgency;
    QuestOffer.bRepeatable = QuestState.bRepeatable;
    QuestOffer.QuestGiverId = QuestState.QuestGiverId;
    QuestOffer.PrimaryObjective = QuestState.PrimaryObjective;
    QuestOffer.NeedRequest = QuestState.NeedRequest;
    QuestOffer.RewardItems = QuestState.RewardItems;
    QuestOffer.RewardCopper = QuestState.RewardCopper;
    QuestOffer.RewardGold = QuestState.RewardGold;
    QuestOffer.RewardExperience = QuestState.RewardExperience;
    QuestOffer.WorldStateEffect = QuestState.WorldStateEffect;
    return QuestOffer;
}

FHuwamQuestRuntimeState UHuwamQuestComponent::MakeQuestStateFromRow(const FHuwamQuestRow& QuestRow) const
{
    FHuwamQuestRuntimeState QuestState;
    QuestState.QuestId = QuestRow.QuestId;
    QuestState.DisplayName = QuestRow.DisplayName;
    QuestState.Description = QuestRow.Description;
    QuestState.QuestType = QuestRow.QuestType;
    QuestState.Urgency = QuestRow.Urgency;
    QuestState.bRepeatable = QuestRow.bRepeatable;
    QuestState.QuestGiverId = QuestRow.QuestGiverId;
    QuestState.RewardItems = QuestRow.RewardItems;
    QuestState.RewardCopper = QuestRow.RewardCopper;
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
    QuestState.bRepeatable = true;
    QuestState.QuestGiverId = TEXT("guild.adventurers");
    QuestState.RewardCopper = 15;
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

FHuwamQuestRuntimeState UHuwamQuestComponent::MakeProgressionZeroTutorialFallbackState() const
{
    FHuwamQuestRuntimeState QuestState;
    QuestState.QuestId = TutorialQuestId;
    QuestState.DisplayName = NSLOCTEXT("HuwamQuest", "ProgressionZeroTutorialName", "Welcome to Progression Zero");
    QuestState.Description = NSLOCTEXT("HuwamQuest", "ProgressionZeroTutorialDescription", "Tutorial in Laucian's white lab.");
    QuestState.QuestType = EHuwamQuestType::Tutorial;
    QuestState.Urgency = EHuwamQuestUrgency::Normal;
    QuestState.QuestGiverId = TEXT("npc.laucian");
    QuestState.WorldStateEffect = NSLOCTEXT("HuwamQuest", "ProgressionZeroTutorialWorldStateEffect", "Player exits tutorial and may enter Eldoria.");
    QuestState.PrimaryObjective.ObjectiveId = TutorialObjectiveId;
    QuestState.PrimaryObjective.Description = NSLOCTEXT("HuwamQuest", "ProgressionZeroTutorialObjective", "Complete Laucian's first tutorial.");
    QuestState.PrimaryObjective.ProgressSourceId = TutorialProgressSourceId;
    QuestState.PrimaryObjective.TargetValue = 1;
    QuestState.PrimaryObjective.CurrentValue = 0;
    QuestState.PrimaryObjective.bCompleted = false;
    QuestState.bReadyToTurnIn = false;
    return QuestState;
}

FHuwamQuestRuntimeState UHuwamQuestComponent::MakeGatherFieldSuppliesFallbackState() const
{
    FHuwamQuestRuntimeState QuestState;
    QuestState.QuestId = GatherFieldSuppliesQuestId;
    QuestState.DisplayName = NSLOCTEXT("HuwamQuest", "GatherFieldSuppliesName", "Gather Field Supplies");
    QuestState.Description = NSLOCTEXT("HuwamQuest", "GatherFieldSuppliesDescription", "Gather basic field supplies for a resident in need.");
    QuestState.QuestType = EHuwamQuestType::Gather;
    QuestState.Urgency = EHuwamQuestUrgency::Timed;
    QuestState.bRepeatable = true;
    QuestState.QuestGiverId = TEXT("npc.eldoria.poor_resident_01");
    QuestState.RewardCopper = 12;
    QuestState.RewardExperience = 6;
    QuestState.WorldStateEffect = NSLOCTEXT("HuwamQuest", "GatherFieldSuppliesWorldStateEffect", "NPC inventory updates and poor district trust rises.");
    QuestState.PrimaryObjective.ObjectiveId = GatherFieldSuppliesObjectiveId;
    QuestState.PrimaryObjective.Description = NSLOCTEXT("HuwamQuest", "GatherFieldSuppliesObjective", "Gather 10 field supplies.");
    QuestState.PrimaryObjective.ProgressSourceId = GatherFieldSuppliesProgressSourceId;
    QuestState.PrimaryObjective.TargetValue = 10;
    QuestState.PrimaryObjective.CurrentValue = 0;
    QuestState.PrimaryObjective.bCompleted = false;
    QuestState.bReadyToTurnIn = false;
    return QuestState;
}

FHuwamQuestRuntimeState UHuwamQuestComponent::MakeServiceRestorationFallbackState(const FHuwamNpcSupplyNeedState& ServicePressure) const
{
    FHuwamQuestRuntimeState QuestState;
    QuestState.QuestId = BuildServiceRestorationQuestId(ServicePressure.NeedId);
    QuestState.DisplayName = NSLOCTEXT("HuwamQuest", "ServiceRestorationName", "Restore Service");
    QuestState.Description = FText::Format(
        NSLOCTEXT("HuwamQuest", "ServiceRestorationDescription", "Restore a local service affected by {0}."),
        ServicePressure.DisplayName.IsEmpty() ? FText::FromString(ServicePressure.NeedId) : ServicePressure.DisplayName
    );
    QuestState.QuestType = EHuwamQuestType::Contract;
    QuestState.Urgency = MapServiceNeedUrgency(ServicePressure.Urgency);
    QuestState.bRepeatable = true;
    QuestState.QuestGiverId = ServicePressure.NpcId;
    QuestState.RewardCopper = FMath::Max<int64>(10, static_cast<int64>(ServicePressure.NeededQuantity) / 4);
    QuestState.RewardExperience = FMath::Max(8, ServicePressure.NeededQuantity / 8);
    QuestState.WorldStateEffect = NSLOCTEXT("HuwamQuest", "ServiceRestorationWorldStateEffect", "Service funding, repairs, and settlement stability can recover.");
    QuestState.NeedRequest.NeedId = ServicePressure.NeedId;
    QuestState.NeedRequest.NeedDisplayName = ServicePressure.DisplayName;
    QuestState.NeedRequest.NeedReason = ServicePressure.Reason;
    QuestState.NeedRequest.NeededQuantityAtRequest = ServicePressure.NeededQuantity;
    QuestState.PrimaryObjective.ObjectiveId = ServiceRestorationObjectiveId;
    QuestState.PrimaryObjective.Description = NSLOCTEXT("HuwamQuest", "ServiceRestorationObjective", "Contribute funds, repair materials, or approved labor to restore the service.");
    QuestState.PrimaryObjective.ProgressSourceId = ServicePressure.NeedId;
    QuestState.PrimaryObjective.TargetValue = FMath::Max(1, ServicePressure.NeededQuantity);
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

    if (QuestState.RewardCopper > 0)
    {
        Rewards->AddCopper(QuestState.RewardCopper);
    }
    else
    {
        Rewards->AddGold(QuestState.RewardGold);
    }
    Rewards->AddExperience(QuestState.RewardExperience);
    return true;
}
