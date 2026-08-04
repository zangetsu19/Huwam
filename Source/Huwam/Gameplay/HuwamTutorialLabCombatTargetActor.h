#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/HuwamCombatComponent.h"
#include "HuwamTutorialLabCombatTargetActor.generated.h"

class AHuwamPrototypePlayerActor;
class USceneComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHuwamTutorialLabCombatTargetHitSignature, AHuwamPrototypePlayerActor*, PrototypePlayer, FHuwamCombatResult, CombatResult);

UCLASS(Blueprintable)
class HUWAM_API AHuwamTutorialLabCombatTargetActor : public AActor
{
    GENERATED_BODY()

public:
    AHuwamTutorialLabCombatTargetActor();

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Tutorial Lab Combat")
    FHuwamTutorialLabCombatTargetHitSignature OnTutorialLabCombatTargetHit;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab Combat")
    bool CanReceiveAttackFromPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab Combat")
    bool ReceiveAttackFromPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, const FHuwamCombatRollConfig& RollConfig, FHuwamCombatResult& OutCombatResult);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab Combat")
    FString GetTargetId() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab Combat")
    FText GetTargetDisplayName() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab Combat")
    UHuwamCombatComponent* GetCombatComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab Combat")
    bool HasCompletedCombatLesson() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Combat")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Combat")
    TObjectPtr<UStaticMeshComponent> TargetMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Combat")
    TObjectPtr<UHuwamCombatComponent> Combat;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Combat")
    FString TargetId = TEXT("target.laucian.training_focus");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Combat")
    FText TargetDisplayName = NSLOCTEXT("HuwamTutorialLab", "TrainingFocusDisplayName", "Laucian's Training Focus");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Combat")
    bool bOneShotLessonTarget = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Tutorial Lab Combat")
    bool bHasCompletedCombatLesson = false;
};
