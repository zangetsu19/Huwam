#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HuwamTutorialLabComponent.generated.h"

class AHuwamTutorialLabEntryActor;
class UHuwamQuestComponent;
class UHuwamRewardComponent;

UENUM(BlueprintType)
enum class EHuwamTutorialLabBeat : uint8
{
    Inactive UMETA(DisplayName = "Inactive"),
    Movement UMETA(DisplayName = "Movement"),
    HudBasics UMETA(DisplayName = "HUD Basics"),
    InteractionReady UMETA(DisplayName = "Interaction Ready"),
    InventoryBasics UMETA(DisplayName = "Inventory Basics"),
    EquipmentBasics UMETA(DisplayName = "Equipment Basics"),
    CombatBasics UMETA(DisplayName = "Combat Basics"),
    MapBasics UMETA(DisplayName = "Map Basics"),
    QuestBasics UMETA(DisplayName = "Quest Basics"),
    GatheringBasics UMETA(DisplayName = "Gathering Basics"),
    Complete UMETA(DisplayName = "Complete")
};

USTRUCT(BlueprintType)
struct FHuwamTutorialLabSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    bool bActive = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    bool bMovedToLabEntry = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    bool bMovementLessonComplete = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    bool bHudLessonComplete = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    bool bInteractionLessonComplete = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    bool bInventoryLessonComplete = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    bool bEquipmentLessonComplete = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    bool bCombatLessonComplete = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    bool bMapLessonComplete = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    bool bQuestLessonComplete = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    bool bGatheringLessonComplete = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    EHuwamTutorialLabBeat CurrentBeat = EHuwamTutorialLabBeat::Inactive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString LocationId = TEXT("location.laucian.white_lab");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString QuestId = TEXT("quest.tutorial.progression_zero");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString ObjectiveId = TEXT("objective.complete_tutorial");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString LastInteractionId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString InventoryLessonItemId = TEXT("item.tool.gathering_knife");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString LastInspectedInventoryItemId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString EquipmentLessonSlotId = TEXT("slot.weapon.main_hand");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString EquipmentLessonItemId = TEXT("item.weapon.basic_sword");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString LastInspectedEquipmentSlotId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString LastInspectedEquipmentItemId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString CombatLessonTargetId = TEXT("target.laucian.training_focus");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString LastCombatTargetId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString MapLessonMapId = TEXT("map.laucian.white_lab");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString LastOpenedMapId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString QuestLessonQuestId = TEXT("quest.tutorial.progression_zero");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString QuestLessonObjectiveId = TEXT("objective.complete_tutorial");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString LastTrackedQuestId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString LastTrackedObjectiveId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString GatheringLessonSourceId = TEXT("gatherable.laucian.tutorial_lavender");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString GatheringLessonItemId = TEXT("material.herb.lavender");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab", meta = (ClampMin = "1"))
    int32 GatheringLessonQuantity = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString LastGatheringSourceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FString LastGatheredItemId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab", meta = (ClampMin = "0"))
    int32 LastGatheredItemQuantity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FText BeatTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FText InstructionText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Tutorial Lab")
    FText LaucianLine;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHuwamTutorialLabChangedSignature, FHuwamTutorialLabSnapshot, Snapshot);

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamTutorialLabComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamTutorialLabComponent();

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Tutorial Lab")
    FHuwamTutorialLabChangedSignature OnTutorialLabChanged;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab", meta=(AdvancedDisplay="Quests,Rewards,LabEntry,ArrivalActor"))
    bool BeginLaucianTutorialLab(
        UHuwamQuestComponent* Quests = nullptr,
        UHuwamRewardComponent* Rewards = nullptr,
        AHuwamTutorialLabEntryActor* LabEntry = nullptr,
        AActor* ArrivalActor = nullptr
    );

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab")
    bool MarkMovementLessonComplete();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab")
    bool MarkHudLessonComplete();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab")
    bool MarkInteractionLessonComplete(const FString& InteractionId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab")
    bool MarkInventoryLessonComplete(const FString& InspectedItemId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab")
    bool MarkEquipmentLessonComplete(const FString& InspectedSlotId, const FString& InspectedItemId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab")
    bool MarkCombatLessonComplete(const FString& TargetId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab")
    bool MarkMapLessonComplete(const FString& MapId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab")
    bool MarkQuestLessonComplete(const FString& QuestId, const FString& ObjectiveId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab")
    bool MarkGatheringLessonComplete(const FString& SourceId, const FString& GatheredItemId, int32 GatheredQuantity);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab", meta=(AdvancedDisplay="Quests,Rewards"))
    bool CompleteLaucianTutorialLab(UHuwamQuestComponent* Quests = nullptr, UHuwamRewardComponent* Rewards = nullptr);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab")
    FHuwamTutorialLabSnapshot GetTutorialLabSnapshot() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab")
    bool IsTutorialLabActive() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab")
    void RestoreTutorialLabForPrototypeSave(const FHuwamTutorialLabSnapshot& SavedSnapshot);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab")
    FHuwamTutorialLabSnapshot TutorialLabSnapshot;

private:
    AHuwamTutorialLabEntryActor* FindPrimaryLabEntry() const;
    UHuwamQuestComponent* ResolveQuestComponent(UHuwamQuestComponent* Quests) const;
    UHuwamRewardComponent* ResolveRewardComponent(UHuwamRewardComponent* Rewards) const;
    void SetBeat(EHuwamTutorialLabBeat NewBeat);
    void BroadcastSnapshot();
};
