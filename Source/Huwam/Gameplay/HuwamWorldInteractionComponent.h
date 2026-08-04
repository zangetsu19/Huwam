#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HuwamWorldInteractionComponent.generated.h"

class AActor;
class APlayerController;
class AHuwamPrototypePlayerActor;

UENUM(BlueprintType)
enum class EHuwamWorldInteractionKind : uint8
{
    None UMETA(DisplayName = "None"),
    GuildContractBoard UMETA(DisplayName = "Guild Contract Board"),
    NpcSupplyQuestRequester UMETA(DisplayName = "NPC Supply Quest Requester"),
    TutorialLabObject UMETA(DisplayName = "Tutorial Lab Object"),
    TutorialLabGatherable UMETA(DisplayName = "Tutorial Lab Gatherable"),
    GatheringResource UMETA(DisplayName = "Gathering Resource"),
    SurvivalService UMETA(DisplayName = "Survival Service")
};

UENUM(BlueprintType)
enum class EHuwamWorldInteractionOutcome : uint8
{
    None UMETA(DisplayName = "None"),
    GuildContractUpdated UMETA(DisplayName = "Guild Contract Updated"),
    NpcSupplyQuestScreenOpened UMETA(DisplayName = "NPC Supply Quest Screen Opened"),
    TutorialInteractionCompleted UMETA(DisplayName = "Tutorial Interaction Completed"),
    TutorialResourceGathered UMETA(DisplayName = "Tutorial Resource Gathered"),
    ResourceGathered UMETA(DisplayName = "Resource Gathered"),
    SurvivalServiceUsed UMETA(DisplayName = "Survival Service Used"),
    Unavailable UMETA(DisplayName = "Unavailable")
};

USTRUCT(BlueprintType)
struct FHuwamWorldInteractionSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|World Interaction")
    bool bHasTarget = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|World Interaction")
    bool bCanInteract = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|World Interaction")
    EHuwamWorldInteractionKind Kind = EHuwamWorldInteractionKind::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|World Interaction")
    TObjectPtr<AActor> TargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|World Interaction")
    FText PromptText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|World Interaction")
    float DistanceToTarget = 0.0f;
};

USTRUCT(BlueprintType)
struct FHuwamWorldInteractionResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|World Interaction")
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|World Interaction")
    EHuwamWorldInteractionOutcome Outcome = EHuwamWorldInteractionOutcome::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|World Interaction")
    FHuwamWorldInteractionSnapshot SnapshotBefore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|World Interaction")
    FHuwamWorldInteractionSnapshot SnapshotAfter;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHuwamWorldInteractionSnapshotSignature, FHuwamWorldInteractionSnapshot, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHuwamWorldInteractionResultSignature, FHuwamWorldInteractionResult, Result);

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamWorldInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamWorldInteractionComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(BlueprintAssignable, Category = "Huwam|World Interaction")
    FHuwamWorldInteractionSnapshotSignature OnInteractionFocusChanged;

    UPROPERTY(BlueprintAssignable, Category = "Huwam|World Interaction")
    FHuwamWorldInteractionResultSignature OnInteractionRouted;

    UFUNCTION(BlueprintCallable, Category = "Huwam|World Interaction")
    FHuwamWorldInteractionSnapshot RefreshInteractionFocus();

    UFUNCTION(BlueprintCallable, Category = "Huwam|World Interaction")
    FHuwamWorldInteractionSnapshot SetInteractionFocusActor(AActor* InTargetActor);

    UFUNCTION(BlueprintCallable, Category = "Huwam|World Interaction")
    FHuwamWorldInteractionSnapshot ClearInteractionFocusActor();

    UFUNCTION(BlueprintCallable, Category = "Huwam|World Interaction")
    void SetInteractionOriginActor(AActor* InOriginActor);

    UFUNCTION(BlueprintCallable, Category = "Huwam|World Interaction")
    void SetAutoRefreshFocus(bool bInAutoRefreshFocus);

    UFUNCTION(BlueprintCallable, Category = "Huwam|World Interaction", meta=(AdvancedDisplay="OwningPlayer"))
    bool InteractWithFocusedTarget(FHuwamWorldInteractionResult& OutResult, APlayerController* OwningPlayer = nullptr);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|World Interaction")
    FHuwamWorldInteractionSnapshot GetLastInteractionSnapshot() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|World Interaction")
    FHuwamWorldInteractionResult GetLastInteractionResult() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|World Interaction")
    AActor* GetFocusedTargetActor() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|World Interaction")
    AActor* GetInteractionOriginActor() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|World Interaction")
    bool IsAutoRefreshFocusEnabled() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|World Interaction", meta=(ClampMin="0.0"))
    float FocusRadius = 360.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|World Interaction")
    bool bAutoRefreshFocus = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|World Interaction", meta=(ClampMin="0.0"))
    float FocusRefreshIntervalSeconds = 0.2f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|World Interaction")
    TObjectPtr<AActor> FocusedTargetActor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|World Interaction")
    TObjectPtr<AActor> InteractionOriginActor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|World Interaction")
    FHuwamWorldInteractionSnapshot LastInteractionSnapshot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|World Interaction")
    FHuwamWorldInteractionResult LastInteractionResult;

private:
    float FocusRefreshElapsedSeconds = 0.0f;

    AHuwamPrototypePlayerActor* ResolvePrototypePlayer() const;
    const AActor* ResolveInteractionOriginActor() const;
    AActor* FindNearestSupportedTarget(AHuwamPrototypePlayerActor* PrototypePlayer) const;
    FHuwamWorldInteractionSnapshot BuildSnapshotForTarget(AHuwamPrototypePlayerActor* PrototypePlayer, AActor* TargetActor) const;
    bool IsTargetInRange(const AActor* TargetActor) const;
};
