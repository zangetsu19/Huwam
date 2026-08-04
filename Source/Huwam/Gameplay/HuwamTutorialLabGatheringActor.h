#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HuwamTutorialLabGatheringActor.generated.h"

class AHuwamPrototypePlayerActor;
class USceneComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHuwamTutorialLabGatheringSignature, AHuwamPrototypePlayerActor*, PrototypePlayer, FString, GatheredItemId, int32, GatheredQuantity);

UCLASS(Blueprintable)
class HUWAM_API AHuwamTutorialLabGatheringActor : public AActor
{
    GENERATED_BODY()

public:
    AHuwamTutorialLabGatheringActor();

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Tutorial Lab Gathering")
    FHuwamTutorialLabGatheringSignature OnTutorialLabGathered;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab Gathering")
    bool CanGatherWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab Gathering")
    bool GatherWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab Gathering")
    FText GetGatherPrompt() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab Gathering")
    bool HasBeenGathered() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Gathering")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Gathering")
    TObjectPtr<UStaticMeshComponent> GatheringMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Gathering")
    FString GatheringSourceId = TEXT("gatherable.laucian.tutorial_lavender");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Gathering")
    FString GatheredItemId = TEXT("material.herb.lavender");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Gathering", meta = (ClampMin = "1"))
    int32 GatheredQuantity = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Gathering")
    FString RequiredToolItemId = TEXT("item.tool.gathering_knife");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Gathering")
    FText GatherPrompt = NSLOCTEXT("HuwamTutorialLab", "TutorialLavenderGatherPrompt", "Gather Laucian's tutorial lavender");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Gathering")
    bool bOneShot = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Gathering")
    bool bHasBeenGathered = false;
};
