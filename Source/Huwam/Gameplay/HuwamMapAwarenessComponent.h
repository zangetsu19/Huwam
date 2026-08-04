#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HuwamMapAwarenessComponent.generated.h"

UENUM(BlueprintType)
enum class EHuwamMapFogState : uint8
{
    Hidden UMETA(DisplayName = "Hidden"),
    Vague UMETA(DisplayName = "Vague"),
    Explored UMETA(DisplayName = "Explored"),
    Mapped UMETA(DisplayName = "Mapped"),
    FullyKnown UMETA(DisplayName = "Fully Known")
};

USTRUCT(BlueprintType)
struct FHuwamMapAwarenessSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Map")
    FString MapId = TEXT("map.laucian.white_lab");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Map")
    FString RegionId = TEXT("location.laucian.white_lab");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Map")
    FText DisplayName = NSLOCTEXT("HuwamMap", "WhiteLabMapDisplayName", "Laucian's White Lab");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Map")
    EHuwamMapFogState FogState = EHuwamMapFogState::Explored;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Map")
    bool bMinimapAvailable = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Map")
    bool bFullMapOpen = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Map")
    bool bCardinalMarkersVisible = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Map")
    bool bBattleFogEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Map")
    bool bQuestMarkersAvailable = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Map")
    FString TrackedQuestId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Map")
    FString TrackedObjectiveId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Map")
    bool bTrackedQuestMarkerVisible = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHuwamMapAwarenessChangedSignature, FHuwamMapAwarenessSnapshot, Snapshot);

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamMapAwarenessComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamMapAwarenessComponent();

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Map")
    FHuwamMapAwarenessChangedSignature OnMapAwarenessChanged;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Map")
    bool OpenFullMap();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Map")
    bool CloseFullMap();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Map")
    bool SetCurrentMap(const FString& MapId, const FString& RegionId, const FText& DisplayName, EHuwamMapFogState FogState);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Map")
    bool TrackQuestObjective(const FString& QuestId, const FString& ObjectiveId);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Map")
    FHuwamMapAwarenessSnapshot GetMapAwarenessSnapshot() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Map")
    void RestoreMapAwarenessForPrototypeSave(const FHuwamMapAwarenessSnapshot& SavedSnapshot);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Map")
    FHuwamMapAwarenessSnapshot MapAwarenessSnapshot;

private:
    void BroadcastSnapshot();
};
