#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/HuwamDataTypes.h"
#include "Gameplay/HuwamEconomyLedgerSubsystem.h"
#include "Gameplay/HuwamEquipmentComponent.h"
#include "Gameplay/HuwamInventoryComponent.h"
#include "Gameplay/HuwamMapAwarenessComponent.h"
#include "Gameplay/HuwamNpcSupplyComponent.h"
#include "Gameplay/HuwamQuestComponent.h"
#include "Gameplay/HuwamSurvivalVitalsComponent.h"
#include "Gameplay/HuwamTutorialLabComponent.h"
#include "Gameplay/HuwamWorldInteractionComponent.h"
#include "HuwamHudDataComponent.generated.h"

class UHuwamCombatComponent;
class UHuwamLiveContentComponent;
class UHuwamRewardComponent;

USTRUCT(BlueprintType)
struct FHuwamHudVitalsSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int32 CurrentHealth = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int32 MaxHealth = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    float HealthPercent = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int32 CurrentMana = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int32 MaxMana = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    float ManaPercent = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bDefeated = false;
};

USTRUCT(BlueprintType)
struct FHuwamHudWalletSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int64 CurrencyBalanceCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FHuwamCurrencyBreakdown Currency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int32 TotalExperience = 0;
};

USTRUCT(BlueprintType)
struct FHuwamHudInventorySnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int32 UsedSlots = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int32 MaxSlots = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int32 RemainingSlots = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    TArray<FHuwamInventoryStack> Stacks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    TArray<FHuwamInventoryStorageRoute> StorageRoutes;
};

USTRUCT(BlueprintType)
struct FHuwamHudEquipmentSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    TArray<FHuwamEquipmentSlotState> Slots;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FHuwamEquipmentStatSummary StatSummary;
};

USTRUCT(BlueprintType)
struct FHuwamHudQuestSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FString QuestId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FString ObjectiveId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FText ObjectiveText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FString RequestNeedId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FText RequestNeedDisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FText RequestNeedReason;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int32 RequestNeededQuantity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int32 CurrentValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int32 TargetValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    float ProgressPercent = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bReadyToTurnIn = false;
};

USTRUCT(BlueprintType)
struct FHuwamHudContentPackSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FString ContentPackId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    EHuwamContentPackType PackType = EHuwamContentPackType::Cosmetic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    EHuwamContentPackState PackState = EHuwamContentPackState::Draft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bAccessible = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FText AccessReason;
};

USTRUCT(BlueprintType)
struct FHuwamHudEconomySnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int32 TransactionCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int64 TotalRevenueCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    TArray<FHuwamEconomyAccountBalance> AccountBalances;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasFocusedServicePressure = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FHuwamNpcSupplyNeedState FocusedServicePressure;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FString FocusedServiceOwnerNpcId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FString FocusedServiceOwningFactionId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FString FocusedServiceSettlementId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int64 FocusedServiceCostCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    int64 FocusedServiceUpkeepCostCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bFocusedServiceUnderfunded = false;
};

USTRUCT(BlueprintType)
struct FHuwamHudSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FHuwamHudVitalsSnapshot Vitals;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FHuwamHudWalletSnapshot Wallet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FHuwamHudInventorySnapshot Inventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FHuwamSurvivalVitalsSnapshot Survival;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FHuwamHudEquipmentSnapshot Equipment;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FHuwamMapAwarenessSnapshot MapAwareness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    TArray<FHuwamQuestRuntimeState> ActiveQuestStates;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    TArray<FHuwamHudQuestSnapshot> ActiveQuestSummaries;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FString TrackedQuestId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    TArray<FHuwamHudContentPackSnapshot> ContentPacks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FHuwamTutorialLabSnapshot TutorialLab;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FHuwamWorldInteractionSnapshot WorldInteraction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    FHuwamHudEconomySnapshot Economy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasCombat = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasInventory = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasSurvival = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasEquipment = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasMapAwareness = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasRewards = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasQuests = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasLiveContent = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasTutorialLab = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasWorldInteraction = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasEconomyLedger = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHuwamHudSnapshotChangedSignature, FHuwamHudSnapshot, Snapshot);

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamHudDataComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamHudDataComponent();

    UPROPERTY(BlueprintAssignable, Category = "Huwam|HUD")
    FHuwamHudSnapshotChangedSignature OnHudSnapshotChanged;

    UFUNCTION(BlueprintCallable, Category = "Huwam|HUD")
    FHuwamHudSnapshot RefreshHudSnapshot();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|HUD")
    FHuwamHudSnapshot GetLastHudSnapshot() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|HUD", meta=(AdvancedDisplay="Combat,Equipment,Inventory,Survival,MapAwareness,Rewards,Quests,LiveContent,TutorialLab,WorldInteraction"))
    FHuwamHudSnapshot BuildHudSnapshot(
        UHuwamCombatComponent* Combat = nullptr,
        UHuwamEquipmentComponent* Equipment = nullptr,
        UHuwamInventoryComponent* Inventory = nullptr,
        UHuwamSurvivalVitalsComponent* Survival = nullptr,
        UHuwamMapAwarenessComponent* MapAwareness = nullptr,
        UHuwamRewardComponent* Rewards = nullptr,
        UHuwamQuestComponent* Quests = nullptr,
        UHuwamLiveContentComponent* LiveContent = nullptr,
        UHuwamTutorialLabComponent* TutorialLab = nullptr,
        UHuwamWorldInteractionComponent* WorldInteraction = nullptr
    ) const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|HUD")
    bool bIncludeContentPackDebug = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|HUD")
    FHuwamHudSnapshot LastHudSnapshot;

private:
    UHuwamCombatComponent* ResolveCombatComponent() const;
    UHuwamEquipmentComponent* ResolveEquipmentComponent() const;
    UHuwamInventoryComponent* ResolveInventoryComponent() const;
    UHuwamSurvivalVitalsComponent* ResolveSurvivalVitalsComponent() const;
    UHuwamMapAwarenessComponent* ResolveMapAwarenessComponent() const;
    UHuwamRewardComponent* ResolveRewardComponent() const;
    UHuwamQuestComponent* ResolveQuestComponent() const;
    UHuwamLiveContentComponent* ResolveLiveContentComponent() const;
    UHuwamTutorialLabComponent* ResolveTutorialLabComponent() const;
    UHuwamWorldInteractionComponent* ResolveWorldInteractionComponent() const;
    FHuwamHudQuestSnapshot MakeQuestSnapshot(const FHuwamQuestRuntimeState& QuestState) const;
};
