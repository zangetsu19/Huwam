#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Gameplay/HuwamEconomyLedgerSubsystem.h"
#include "Gameplay/HuwamEquipmentComponent.h"
#include "Gameplay/HuwamInventoryComponent.h"
#include "Gameplay/HuwamMapAwarenessComponent.h"
#include "Gameplay/HuwamQuestComponent.h"
#include "Gameplay/HuwamRewardComponent.h"
#include "Gameplay/HuwamSurvivalServiceActor.h"
#include "Gameplay/HuwamSurvivalVitalsComponent.h"
#include "Gameplay/HuwamTutorialLabComponent.h"
#include "HuwamPrototypeSaveGame.generated.h"

UCLASS()
class HUWAM_API UHuwamPrototypeSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    static constexpr int32 LatestSaveVersion = 4;

    UPROPERTY()
    int32 SaveVersion = LatestSaveVersion;

    UPROPERTY()
    bool bHasAvatarTransform = false;

    UPROPERTY()
    FTransform AvatarTransform = FTransform::Identity;

    UPROPERTY()
    TArray<FHuwamInventoryStack> InventoryStacks;

    UPROPERTY()
    TArray<FHuwamEquipmentSlotState> EquipmentSlots;

    UPROPERTY()
    FHuwamSurvivalVitalsSnapshot SurvivalVitals;

    UPROPERTY()
    int64 CurrencyBalanceCopper = 0;

    UPROPERTY()
    int32 TotalExperience = 0;

    UPROPERTY()
    int32 CurrentHealth = INDEX_NONE;

    UPROPERTY()
    int32 CurrentMana = INDEX_NONE;

    UPROPERTY()
    TArray<FHuwamQuestProgressRecord> QuestProgressRecords;

    UPROPERTY()
    TArray<FHuwamQuestRuntimeState> QuestStates;

    UPROPERTY()
    TArray<FString> TurnedInQuestIds;

    UPROPERTY()
    FString TrackedQuestId;

    UPROPERTY()
    FHuwamMapAwarenessSnapshot MapAwareness;

    UPROPERTY()
    FHuwamTutorialLabSnapshot TutorialLab;

    UPROPERTY()
    TArray<FHuwamEconomyLedgerEntry> EconomyLedgerEntries;

    UPROPERTY()
    TArray<FHuwamEconomyAccountBalance> EconomyAccountBalances;

    UPROPERTY()
    TArray<FHuwamSurvivalServiceSaveState> SurvivalServiceStates;

    UPROPERTY()
    bool bEnteredEldoriaStarterArea = false;
};
