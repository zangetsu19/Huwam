#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/HuwamDataTypes.h"
#include "HuwamRewardComponent.generated.h"

class AHuwamMonsterEncounterActor;
class UHuwamInventoryComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHuwamRewardsChangedSignature);

USTRUCT(BlueprintType)
struct FHuwamQuestProgressRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    FString QuestId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    FString ObjectiveId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    int32 CurrentValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    int32 TargetValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    bool bCompleted = false;
};

USTRUCT(BlueprintType)
struct FHuwamRewardGrantResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    FString MonsterId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    TArray<FHuwamIdQuantity> ItemsGranted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    int64 CurrencyCopperGranted = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    int32 GoldGranted = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    int32 ExperienceGranted = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    FString QuestId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    FString ObjectiveId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    int32 QuestProgressAfter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Rewards")
    bool bQuestObjectiveCompleted = false;
};

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamRewardComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamRewardComponent();

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Rewards")
    FHuwamRewardsChangedSignature OnRewardsChanged;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Rewards")
    int32 AddGold(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Rewards")
    bool SpendGold(int32 Amount);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Rewards")
    int32 GetGoldBalance() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Rewards")
    int64 AddCopper(int64 Amount);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Rewards")
    bool SpendCopper(int64 Amount);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Rewards")
    int64 GetCurrencyBalanceCopper() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Rewards")
    FHuwamCurrencyBreakdown GetCurrencyBreakdown() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Rewards")
    int32 AddExperience(int32 Amount);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Rewards")
    int32 GetTotalExperience() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Rewards")
    bool BeginQuestObjective(const FString& QuestId, const FString& ObjectiveId, int32 TargetValue);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Rewards")
    bool ResetQuestObjectiveProgress(const FString& QuestId, const FString& ObjectiveId, int32 TargetValue);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Rewards")
    bool AddQuestProgress(const FString& QuestId, const FString& ObjectiveId, int32 Delta, int32 TargetValue, int32& OutCurrentValue, bool& bOutCompleted);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Rewards")
    bool GetQuestProgress(const FString& QuestId, const FString& ObjectiveId, FHuwamQuestProgressRecord& OutProgress) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Rewards")
    TArray<FHuwamQuestProgressRecord> GetQuestProgressRecords() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Rewards")
    void RestoreRewardStateForPrototypeSave(int64 SavedCurrencyBalanceCopper, int32 SavedTotalExperience, const TArray<FHuwamQuestProgressRecord>& SavedQuestProgressRecords);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Rewards", meta=(AdvancedDisplay="QuestId,ObjectiveId,QuestProgressDelta,QuestTargetValue"))
    bool ClaimMonsterDefeatRewards(AHuwamMonsterEncounterActor* Encounter, UHuwamInventoryComponent* RecipientInventory, const FString& QuestId, const FString& ObjectiveId, int32 QuestProgressDelta, int32 QuestTargetValue, FHuwamRewardGrantResult& OutResult);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Rewards", meta=(ClampMin="0"))
    int64 CurrencyBalanceCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Rewards", meta=(ClampMin="0"))
    int32 TotalExperience = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Rewards")
    TArray<FHuwamQuestProgressRecord> QuestProgressRecords;

private:
    int32 FindQuestProgressIndex(const FString& QuestId, const FString& ObjectiveId) const;
    bool IsValidQuestProgressKey(const FString& QuestId, const FString& ObjectiveId) const;
};
