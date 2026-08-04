#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HuwamTutorialLabInteractableActor.generated.h"

class AHuwamPrototypePlayerActor;
class USceneComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHuwamTutorialLabInteractionSignature, AHuwamPrototypePlayerActor*, PrototypePlayer);

UCLASS(Blueprintable)
class HUWAM_API AHuwamTutorialLabInteractableActor : public AActor
{
    GENERATED_BODY()

public:
    AHuwamTutorialLabInteractableActor();

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Tutorial Lab Interaction")
    FHuwamTutorialLabInteractionSignature OnTutorialLabInteraction;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab Interaction")
    bool CanInteractWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab Interaction")
    bool InteractWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab Interaction")
    FText GetInteractionPrompt() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab Interaction")
    bool HasBeenInteractedWith() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Interaction")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Interaction")
    TObjectPtr<UStaticMeshComponent> PedestalMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Interaction")
    FString InteractionId = TEXT("interaction.laucian.training_pedestal");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Interaction")
    FText InteractionPrompt = NSLOCTEXT("HuwamTutorialLab", "TrainingPedestalPrompt", "Interact with Laucian's training pedestal");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Interaction")
    bool bOneShot = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Interaction")
    bool bHasBeenInteractedWith = false;
};
