#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HuwamPrototypeGameMode.generated.h"

class AHuwamPrototypePlayerActor;
class AHuwamPrototypeSliceBlockoutActor;
class AHuwamPrototypeThirdPersonCharacter;

UCLASS(Blueprintable)
class HUWAM_API AHuwamPrototypeGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AHuwamPrototypeGameMode();

    virtual void StartPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Bootstrap")
    bool BootstrapFirstPlayablePrototype();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Bootstrap")
    AHuwamPrototypePlayerActor* GetPrototypePlayerStateActor() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Bootstrap")
    AHuwamPrototypeThirdPersonCharacter* GetPrototypeCharacter() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Bootstrap")
    AHuwamPrototypeSliceBlockoutActor* GetPrototypeSliceBlockout() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Bootstrap")
    TSubclassOf<AHuwamPrototypePlayerActor> PrototypePlayerStateActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Bootstrap")
    TSubclassOf<AHuwamPrototypeSliceBlockoutActor> PrototypeSliceBlockoutClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Bootstrap")
    bool bSpawnPrototypePlayerStateActorIfMissing = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Bootstrap")
    bool bSpawnPrototypeSliceBlockoutIfMissing = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Bootstrap")
    bool bConfigureSpawnedStateForCharacterCreation = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Bootstrap")
    bool bLinkExistingThirdPersonCharacter = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Bootstrap")
    TObjectPtr<AHuwamPrototypePlayerActor> PrototypePlayerStateActor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Bootstrap")
    TObjectPtr<AHuwamPrototypeThirdPersonCharacter> PrototypeCharacter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Bootstrap")
    TObjectPtr<AHuwamPrototypeSliceBlockoutActor> PrototypeSliceBlockout;

private:
    AHuwamPrototypePlayerActor* FindPrototypePlayerStateActor() const;
    AHuwamPrototypeThirdPersonCharacter* FindPrototypeCharacter() const;
    AHuwamPrototypeSliceBlockoutActor* FindPrototypeSliceBlockout() const;
    AHuwamPrototypePlayerActor* SpawnPrototypePlayerStateActor(AHuwamPrototypeThirdPersonCharacter* SpawnAnchor);
    AHuwamPrototypeSliceBlockoutActor* SpawnPrototypeSliceBlockout();
    void BeginEditorValidationRun();
    void AdvanceEditorValidationRun();
    bool ValidateRuntimeTimeBootstrap() const;
    bool ValidateRuntimeDataBootstrap() const;
    bool ValidateRuntimeInventorySpoilageBootstrap();
    bool ValidateRuntimeSurvivalVitalsBootstrap();

    FTimerHandle EditorValidationTimerHandle;
    int32 EditorValidationStep = 0;
};
