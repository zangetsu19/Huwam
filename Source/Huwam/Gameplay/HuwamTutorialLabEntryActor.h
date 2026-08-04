#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HuwamTutorialLabEntryActor.generated.h"

class USceneComponent;

UCLASS(Blueprintable)
class HUWAM_API AHuwamTutorialLabEntryActor : public AActor
{
    GENERATED_BODY()

public:
    AHuwamTutorialLabEntryActor();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab")
    FTransform GetLabEntryTransform() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab")
    bool IsPrimaryLabEntry() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab")
    bool bPrimaryLabEntry = true;
};
