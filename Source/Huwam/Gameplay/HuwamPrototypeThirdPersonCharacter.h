#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HuwamPrototypeThirdPersonCharacter.generated.h"

class AActor;
class AHuwamPrototypePlayerActor;
class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class HUWAM_API AHuwamPrototypeThirdPersonCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AHuwamPrototypeThirdPersonCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Third Person Prototype")
    void SetPrototypePlayerActor(AHuwamPrototypePlayerActor* InPrototypePlayerActor);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Third Person Prototype")
    bool RefreshCameraInteractionFocus();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Third Person Prototype")
    bool InteractWithCameraFocus();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Third Person Prototype")
    bool AttackTutorialTargetInCameraFocus();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Third Person Prototype")
    AHuwamPrototypePlayerActor* GetPrototypePlayerActor() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Third Person Prototype")
    bool IsLinkedToPrototypePlayerActor() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Third Person Prototype")
    AActor* GetLastCameraFocusActor() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Third Person Prototype")
    UCameraComponent* GetFollowCamera() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Third Person Prototype")
    USpringArmComponent* GetCameraBoom() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype")
    TObjectPtr<UStaticMeshComponent> PrototypeBodyMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype")
    TObjectPtr<UStaticMeshComponent> PrototypeHeadMesh;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Huwam|Third Person Prototype")
    TObjectPtr<AHuwamPrototypePlayerActor> PrototypePlayerActor;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype")
    bool bFindPrototypePlayerActorOnBeginPlay = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype")
    bool bShowPrototypePromptWhenLinked = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype")
    bool bDisableLogicActorInteractInputWhenLinked = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype")
    bool bUseLinkedStateActorAsViewTarget = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype")
    bool bCameraDrivesInteractionFocus = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype", meta=(ClampMin="50.0"))
    float CameraInteractionTraceDistance = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype", meta=(ClampMin="0.0"))
    float CameraFocusRefreshIntervalSeconds = 0.05f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype", meta=(ClampMin="-1.0", ClampMax="1.0"))
    float FallbackCameraFocusMinimumDot = 0.72f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype|Tutorial", meta=(ClampMin="1.0"))
    float TutorialMovementRequiredDistance = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype|Tutorial", meta=(ClampMin="1.0"))
    float TutorialLookInputRequiredAmount = 8.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Third Person Prototype")
    TObjectPtr<AActor> LastCameraFocusActor;

private:
    float CameraFocusElapsedSeconds = 0.0f;
    float TutorialLookInputAmount = 0.0f;
    bool bTutorialMovementStartRecorded = false;
    FVector TutorialMovementStartLocation = FVector::ZeroVector;

    AHuwamPrototypePlayerActor* FindPrototypePlayerActorInWorld() const;
    AActor* FindCameraInteractionTarget() const;
    AActor* FindFallbackCameraInteractionTarget(const FVector& ViewOrigin, const FVector& ViewDirection) const;
    class AHuwamTutorialLabCombatTargetActor* FindCameraTutorialCombatTarget() const;
    class AHuwamTutorialLabCombatTargetActor* FindFallbackCameraTutorialCombatTarget(const FVector& ViewOrigin, const FVector& ViewDirection) const;
    class AHuwamMonsterEncounterActor* FindCameraMonsterTarget() const;
    class AHuwamMonsterEncounterActor* FindFallbackCameraMonsterTarget(const FVector& ViewOrigin, const FVector& ViewDirection) const;
    bool IsSupportedInteractionTarget(const AActor* TargetActor) const;
    bool GetCameraView(FVector& OutViewOrigin, FVector& OutViewDirection) const;
    void LinkPrototypeWorldInteraction();
    bool IsMovementTutorialBeatActive() const;
    void RecordTutorialLookInput(float Value);
    void TryAdvanceTutorialMovementLesson();
    void ResetTutorialMovementLessonTracking();
    void AnimatePrototypeAvatar(float DeltaSeconds);
    void MoveForward(float Value);
    void MoveRight(float Value);
    void TurnCamera(float Value);
    void LookCamera(float Value);
    void BeginJump();
    void EndJump();
    void HandleInteractInput();
    void HandleAttackInput();
    void HandleTutorialHudInput();
    void HandleInventoryMenuInput();
    void HandleMapMenuInput();
    void HandleQuestMenuInput();
    void HandleSaveInput();
    void HandleLoadInput();
};
