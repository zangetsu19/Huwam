#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/HuwamDataTypes.h"
#include "HuwamQuestComponent.generated.h"

class UHuwamInventoryComponent;
class UHuwamRewardComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHuwamQuestsChangedSignature);

USTRUCT(BlueprintType)
struct FHuwamQuestObjectiveState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FString ObjectiveId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FString ProgressSourceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    int32 CurrentValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    int32 TargetValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    bool bCompleted = false;
};

USTRUCT(BlueprintType)
struct FHuwamQuestRuntimeState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FString QuestId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    EHuwamQuestType QuestType = EHuwamQuestType::Gather;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    EHuwamQuestUrgency Urgency = EHuwamQuestUrgency::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FString QuestGiverId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FHuwamQuestObjectiveState PrimaryObjective;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    TArray<FHuwamIdQuantity> RewardItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    int32 RewardGold = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    int32 RewardExperience = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FText WorldStateEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    bool bActive = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    bool bReadyToTurnIn = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    bool bTurnedIn = false;
};

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamQuestComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamQuestComponent();

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Quest")
    FHuwamQuestsChangedSignature OnQuestsChanged;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Quest", meta=(AdvancedDisplay="Rewards"))
    bool StartQuest(const FString& QuestId, UHuwamRewardComponent* Rewards = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Quest", meta=(AdvancedDisplay="Rewards"))
    bool StartSlimesInTallGrassQuest(UHuwamRewardComponent* Rewards = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Quest")
    bool RefreshQuestProgressFromRewards(const FString& QuestId, UHuwamRewardComponent* Rewards);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Quest")
    void RefreshAllQuestProgressFromRewards(UHuwamRewardComponent* Rewards);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Quest")
    bool CanTurnInQuest(const FString& QuestId) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Quest", meta=(AdvancedDisplay="Inventory"))
    bool TurnInQuest(const FString& QuestId, UHuwamRewardComponent* Rewards, UHuwamInventoryComponent* Inventory, FHuwamQuestRuntimeState& OutTurnedInQuest);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Quest")
    bool GetQuestState(const FString& QuestId, FHuwamQuestRuntimeState& OutQuestState) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Quest")
    TArray<FHuwamQuestRuntimeState> GetQuestStates() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Quest")
    TArray<FHuwamQuestRuntimeState> GetActiveQuestStates() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Quest")
    TArray<FString> GetTurnedInQuestIds() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Quest")
    TArray<FHuwamQuestRuntimeState> QuestStates;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Quest")
    TArray<FString> TurnedInQuestIds;

private:
    int32 FindQuestIndex(const FString& QuestId) const;
    bool LoadQuestRow(const FString& QuestId, FHuwamQuestRow& OutQuestRow) const;
    FHuwamQuestRuntimeState MakeQuestStateFromRow(const FHuwamQuestRow& QuestRow) const;
    FHuwamQuestRuntimeState MakeSlimesInTallGrassFallbackState() const;
    bool GrantQuestRewards(const FHuwamQuestRuntimeState& QuestState, UHuwamRewardComponent* Rewards, UHuwamInventoryComponent* Inventory) const;
};
