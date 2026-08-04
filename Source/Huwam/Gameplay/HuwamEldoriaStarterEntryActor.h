#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HuwamEldoriaStarterEntryActor.generated.h"

class USceneComponent;

UCLASS(Blueprintable)
class HUWAM_API AHuwamEldoriaStarterEntryActor : public AActor
{
    GENERATED_BODY()

public:
    AHuwamEldoriaStarterEntryActor();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Eldoria Starter")
    FTransform GetEntryTransform() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Eldoria Starter")
    bool IsPrimaryEntry() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Eldoria Starter")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Eldoria Starter")
    bool bPrimaryEntry = true;
};
