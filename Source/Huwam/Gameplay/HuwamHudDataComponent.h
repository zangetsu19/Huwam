#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/HuwamDataTypes.h"
#include "Gameplay/HuwamInventoryComponent.h"
#include "Gameplay/HuwamQuestComponent.h"
#include "HuwamHudDataComponent.generated.h"

class UHuwamCombatComponent;
class UHuwamEquipmentComponent;
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
    int32 GoldBalance = 0;

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
    TArray<FHuwamQuestRuntimeState> ActiveQuestStates;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    TArray<FHuwamHudQuestSnapshot> ActiveQuestSummaries;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    TArray<FHuwamHudContentPackSnapshot> ContentPacks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasCombat = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasInventory = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasRewards = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasQuests = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|HUD")
    bool bHasLiveContent = false;
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

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|HUD", meta=(AdvancedDisplay="Combat,Inventory,Rewards,Quests,LiveContent"))
    FHuwamHudSnapshot BuildHudSnapshot(
        UHuwamCombatComponent* Combat = nullptr,
        UHuwamInventoryComponent* Inventory = nullptr,
        UHuwamRewardComponent* Rewards = nullptr,
        UHuwamQuestComponent* Quests = nullptr,
        UHuwamLiveContentComponent* LiveContent = nullptr
    ) const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|HUD")
    bool bIncludeContentPackDebug = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|HUD")
    FHuwamHudSnapshot LastHudSnapshot;

private:
    UHuwamCombatComponent* ResolveCombatComponent() const;
    UHuwamInventoryComponent* ResolveInventoryComponent() const;
    UHuwamRewardComponent* ResolveRewardComponent() const;
    UHuwamQuestComponent* ResolveQuestComponent() const;
    UHuwamLiveContentComponent* ResolveLiveContentComponent() const;
    FHuwamHudQuestSnapshot MakeQuestSnapshot(const FHuwamQuestRuntimeState& QuestState) const;
};
