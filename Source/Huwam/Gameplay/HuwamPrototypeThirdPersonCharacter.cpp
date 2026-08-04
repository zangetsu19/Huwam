#include "Gameplay/HuwamPrototypeThirdPersonCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/HuwamGatheringResourceActor.h"
#include "Gameplay/HuwamGuildContractBoardActor.h"
#include "Gameplay/HuwamMonsterEncounterActor.h"
#include "Gameplay/HuwamNpcSupplyQuestOfferActor.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"
#include "Gameplay/HuwamTutorialLabComponent.h"
#include "Gameplay/HuwamTutorialLabCombatTargetActor.h"
#include "Gameplay/HuwamTutorialLabGatheringActor.h"
#include "Gameplay/HuwamTutorialLabInteractableActor.h"
#include "UI/HuwamPrototypeMenuWidget.h"
#include "InputCoreTypes.h"

namespace
{
    template <typename TargetType>
    void ConsiderCameraFallbackTarget(
        UWorld* World,
        const AActor* Character,
        const FVector& ViewOrigin,
        const FVector& ViewDirection,
        float MaximumDistance,
        float MinimumDot,
        AActor*& InOutBestTarget,
        float& InOutBestScore
    )
    {
        if (!World || !Character)
        {
            return;
        }

        for (TActorIterator<TargetType> It(World); It; ++It)
        {
            AActor* Candidate = *It;
            if (!Candidate || Candidate == Character)
            {
                continue;
            }

            const FVector ToCandidate = Candidate->GetActorLocation() - ViewOrigin;
            const float CandidateDistance = ToCandidate.Length();
            if (CandidateDistance <= KINDA_SMALL_NUMBER || CandidateDistance > MaximumDistance)
            {
                continue;
            }

            const float CandidateDot = FVector::DotProduct(ViewDirection, ToCandidate / CandidateDistance);
            if (CandidateDot < MinimumDot)
            {
                continue;
            }

            const float CandidateScore = CandidateDot * 2.0f - CandidateDistance / MaximumDistance;
            if (CandidateScore > InOutBestScore)
            {
                InOutBestTarget = Candidate;
                InOutBestScore = CandidateScore;
            }
        }
    }
}

AHuwamPrototypeThirdPersonCharacter::AHuwamPrototypeThirdPersonCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessPlayer = EAutoReceiveInput::Player0;

    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        Movement->bOrientRotationToMovement = true;
        Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
        Movement->JumpZVelocity = 540.0f;
        Movement->AirControl = 0.28f;
        Movement->MaxWalkSpeed = 500.0f;
    }

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 420.0f;
    CameraBoom->SocketOffset = FVector(0.0f, 45.0f, 70.0f);
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 12.0f;
    CameraBoom->bDoCollisionTest = true;
    CameraBoom->ProbeSize = 16.0f;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    PrototypeBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypeBodyMesh"));
    PrototypeBodyMesh->SetupAttachment(RootComponent);
    PrototypeBodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PrototypeBodyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -24.0f));
    PrototypeBodyMesh->SetRelativeScale3D(FVector(0.50f, 0.50f, 1.18f));

    PrototypeHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypeHeadMesh"));
    PrototypeHeadMesh->SetupAttachment(RootComponent);
    PrototypeHeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PrototypeHeadMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 56.0f));
    PrototypeHeadMesh->SetRelativeScale3D(FVector(0.34f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (CylinderMesh.Succeeded())
    {
        PrototypeBodyMesh->SetStaticMesh(CylinderMesh.Object);
    }

    if (SphereMesh.Succeeded())
    {
        PrototypeHeadMesh->SetStaticMesh(SphereMesh.Object);
    }
}

void AHuwamPrototypeThirdPersonCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (!PrototypePlayerActor && bFindPrototypePlayerActorOnBeginPlay)
    {
        PrototypePlayerActor = FindPrototypePlayerActorInWorld();
    }

    LinkPrototypeWorldInteraction();
    RefreshCameraInteractionFocus();
}

void AHuwamPrototypeThirdPersonCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AnimatePrototypeAvatar(DeltaSeconds);
    TryAdvanceTutorialMovementLesson();

    if (!bCameraDrivesInteractionFocus || !PrototypePlayerActor)
    {
        return;
    }

    CameraFocusElapsedSeconds += DeltaSeconds;
    if (CameraFocusRefreshIntervalSeconds > 0.0f && CameraFocusElapsedSeconds < CameraFocusRefreshIntervalSeconds)
    {
        return;
    }

    CameraFocusElapsedSeconds = 0.0f;
    RefreshCameraInteractionFocus();
}

void AHuwamPrototypeThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (!PlayerInputComponent)
    {
        return;
    }

    PlayerInputComponent->BindAxis(TEXT("HuwamMoveForward"), this, &AHuwamPrototypeThirdPersonCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("HuwamMoveRight"), this, &AHuwamPrototypeThirdPersonCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("HuwamTurn"), this, &AHuwamPrototypeThirdPersonCharacter::TurnCamera);
    PlayerInputComponent->BindAxis(TEXT("HuwamLook"), this, &AHuwamPrototypeThirdPersonCharacter::LookCamera);
    PlayerInputComponent->BindAction(TEXT("HuwamJump"), IE_Pressed, this, &AHuwamPrototypeThirdPersonCharacter::BeginJump);
    PlayerInputComponent->BindAction(TEXT("HuwamJump"), IE_Released, this, &AHuwamPrototypeThirdPersonCharacter::EndJump);
    PlayerInputComponent->BindAction(TEXT("HuwamInteract"), IE_Pressed, this, &AHuwamPrototypeThirdPersonCharacter::HandleInteractInput);
    PlayerInputComponent->BindAction(TEXT("HuwamAttack"), IE_Pressed, this, &AHuwamPrototypeThirdPersonCharacter::HandleAttackInput);
    PlayerInputComponent->BindAction(TEXT("HuwamTutorialHud"), IE_Pressed, this, &AHuwamPrototypeThirdPersonCharacter::HandleTutorialHudInput);
    PlayerInputComponent->BindAction(TEXT("HuwamInventory"), IE_Pressed, this, &AHuwamPrototypeThirdPersonCharacter::HandleInventoryMenuInput);
    PlayerInputComponent->BindAction(TEXT("HuwamMap"), IE_Pressed, this, &AHuwamPrototypeThirdPersonCharacter::HandleMapMenuInput);
    PlayerInputComponent->BindAction(TEXT("HuwamQuests"), IE_Pressed, this, &AHuwamPrototypeThirdPersonCharacter::HandleQuestMenuInput);
    PlayerInputComponent->BindAction(TEXT("HuwamQuickSave"), IE_Pressed, this, &AHuwamPrototypeThirdPersonCharacter::HandleSaveInput);
    PlayerInputComponent->BindAction(TEXT("HuwamQuickLoad"), IE_Pressed, this, &AHuwamPrototypeThirdPersonCharacter::HandleLoadInput);
}

void AHuwamPrototypeThirdPersonCharacter::SetPrototypePlayerActor(AHuwamPrototypePlayerActor* InPrototypePlayerActor)
{
    PrototypePlayerActor = InPrototypePlayerActor;
    LinkPrototypeWorldInteraction();
    RefreshCameraInteractionFocus();
}

bool AHuwamPrototypeThirdPersonCharacter::RefreshCameraInteractionFocus()
{
    if (!PrototypePlayerActor || !PrototypePlayerActor->GetWorldInteractionComponent())
    {
        LastCameraFocusActor = nullptr;
        return false;
    }

    AActor* CameraTarget = FindCameraInteractionTarget();
    LastCameraFocusActor = CameraTarget;
    if (CameraTarget)
    {
        PrototypePlayerActor->GetWorldInteractionComponent()->SetInteractionFocusActor(CameraTarget);
    }
    else
    {
        PrototypePlayerActor->GetWorldInteractionComponent()->ClearInteractionFocusActor();
    }

    return CameraTarget != nullptr;
}

bool AHuwamPrototypeThirdPersonCharacter::InteractWithCameraFocus()
{
    if (!PrototypePlayerActor)
    {
        return false;
    }

    RefreshCameraInteractionFocus();
    FHuwamWorldInteractionResult InteractionResult;
    return PrototypePlayerActor->InteractWithFocusedWorldTarget(InteractionResult, Cast<APlayerController>(GetController()));
}

bool AHuwamPrototypeThirdPersonCharacter::AttackTutorialTargetInCameraFocus()
{
    if (!PrototypePlayerActor)
    {
        return false;
    }

    FHuwamCombatResult CombatResult;
    if (PrototypePlayerActor->AttackTutorialLabCombatTarget(FindCameraTutorialCombatTarget(), CombatResult))
    {
        return true;
    }

    FHuwamRewardGrantResult RewardResult;
    return PrototypePlayerActor->AttackMonsterAndClaimIfDefeated(FindCameraMonsterTarget(), CombatResult, RewardResult);
}

AHuwamPrototypePlayerActor* AHuwamPrototypeThirdPersonCharacter::GetPrototypePlayerActor() const
{
    return PrototypePlayerActor;
}

bool AHuwamPrototypeThirdPersonCharacter::IsLinkedToPrototypePlayerActor() const
{
    return PrototypePlayerActor != nullptr;
}

AActor* AHuwamPrototypeThirdPersonCharacter::GetLastCameraFocusActor() const
{
    return LastCameraFocusActor;
}

UCameraComponent* AHuwamPrototypeThirdPersonCharacter::GetFollowCamera() const
{
    return FollowCamera;
}

USpringArmComponent* AHuwamPrototypeThirdPersonCharacter::GetCameraBoom() const
{
    return CameraBoom;
}

AHuwamPrototypePlayerActor* AHuwamPrototypeThirdPersonCharacter::FindPrototypePlayerActorInWorld() const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    for (TActorIterator<AHuwamPrototypePlayerActor> It(GetWorld()); It; ++It)
    {
        return *It;
    }

    return nullptr;
}

AActor* AHuwamPrototypeThirdPersonCharacter::FindCameraInteractionTarget() const
{
    FVector ViewOrigin;
    FVector ViewDirection;
    if (!GetCameraView(ViewOrigin, ViewDirection) || !GetWorld())
    {
        return nullptr;
    }

    const FVector TraceEnd = ViewOrigin + ViewDirection * CameraInteractionTraceDistance;
    FHitResult Hit;
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(HuwamThirdPersonInteractionTrace), true, this);
    if (PrototypePlayerActor)
    {
        QueryParams.AddIgnoredActor(PrototypePlayerActor);
    }

    if (GetWorld()->LineTraceSingleByChannel(Hit, ViewOrigin, TraceEnd, ECC_Visibility, QueryParams))
    {
        return IsSupportedInteractionTarget(Hit.GetActor()) ? Hit.GetActor() : nullptr;
    }

    return FindFallbackCameraInteractionTarget(ViewOrigin, ViewDirection);
}

AActor* AHuwamPrototypeThirdPersonCharacter::FindFallbackCameraInteractionTarget(const FVector& ViewOrigin, const FVector& ViewDirection) const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    AActor* BestTarget = nullptr;
    float BestScore = -TNumericLimits<float>::Max();
    ConsiderCameraFallbackTarget<AHuwamGuildContractBoardActor>(
        GetWorld(),
        this,
        ViewOrigin,
        ViewDirection,
        CameraInteractionTraceDistance,
        FallbackCameraFocusMinimumDot,
        BestTarget,
        BestScore
    );
    ConsiderCameraFallbackTarget<AHuwamNpcSupplyQuestOfferActor>(
        GetWorld(),
        this,
        ViewOrigin,
        ViewDirection,
        CameraInteractionTraceDistance,
        FallbackCameraFocusMinimumDot,
        BestTarget,
        BestScore
    );
    ConsiderCameraFallbackTarget<AHuwamTutorialLabInteractableActor>(
        GetWorld(),
        this,
        ViewOrigin,
        ViewDirection,
        CameraInteractionTraceDistance,
        FallbackCameraFocusMinimumDot,
        BestTarget,
        BestScore
    );
    ConsiderCameraFallbackTarget<AHuwamTutorialLabGatheringActor>(
        GetWorld(),
        this,
        ViewOrigin,
        ViewDirection,
        CameraInteractionTraceDistance,
        FallbackCameraFocusMinimumDot,
        BestTarget,
        BestScore
    );
    ConsiderCameraFallbackTarget<AHuwamGatheringResourceActor>(
        GetWorld(),
        this,
        ViewOrigin,
        ViewDirection,
        CameraInteractionTraceDistance,
        FallbackCameraFocusMinimumDot,
        BestTarget,
        BestScore
    );
    return BestTarget;
}

AHuwamTutorialLabCombatTargetActor* AHuwamPrototypeThirdPersonCharacter::FindCameraTutorialCombatTarget() const
{
    FVector ViewOrigin;
    FVector ViewDirection;
    if (!GetCameraView(ViewOrigin, ViewDirection) || !GetWorld())
    {
        return nullptr;
    }

    const FVector TraceEnd = ViewOrigin + ViewDirection * CameraInteractionTraceDistance;
    FHitResult Hit;
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(HuwamThirdPersonTutorialCombatTrace), true, this);
    if (PrototypePlayerActor)
    {
        QueryParams.AddIgnoredActor(PrototypePlayerActor);
    }

    if (GetWorld()->LineTraceSingleByChannel(Hit, ViewOrigin, TraceEnd, ECC_Visibility, QueryParams))
    {
        return Cast<AHuwamTutorialLabCombatTargetActor>(Hit.GetActor());
    }

    return FindFallbackCameraTutorialCombatTarget(ViewOrigin, ViewDirection);
}

AHuwamTutorialLabCombatTargetActor* AHuwamPrototypeThirdPersonCharacter::FindFallbackCameraTutorialCombatTarget(const FVector& ViewOrigin, const FVector& ViewDirection) const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    AActor* BestTarget = nullptr;
    float BestScore = -TNumericLimits<float>::Max();
    ConsiderCameraFallbackTarget<AHuwamTutorialLabCombatTargetActor>(
        GetWorld(),
        this,
        ViewOrigin,
        ViewDirection,
        CameraInteractionTraceDistance,
        FallbackCameraFocusMinimumDot,
        BestTarget,
        BestScore
    );
    return Cast<AHuwamTutorialLabCombatTargetActor>(BestTarget);
}

AHuwamMonsterEncounterActor* AHuwamPrototypeThirdPersonCharacter::FindCameraMonsterTarget() const
{
    FVector ViewOrigin;
    FVector ViewDirection;
    if (!GetCameraView(ViewOrigin, ViewDirection) || !GetWorld())
    {
        return nullptr;
    }

    const FVector TraceEnd = ViewOrigin + ViewDirection * CameraInteractionTraceDistance;
    FHitResult Hit;
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(HuwamThirdPersonMonsterTrace), true, this);
    if (PrototypePlayerActor)
    {
        QueryParams.AddIgnoredActor(PrototypePlayerActor);
    }

    if (GetWorld()->LineTraceSingleByChannel(Hit, ViewOrigin, TraceEnd, ECC_Visibility, QueryParams))
    {
        return Cast<AHuwamMonsterEncounterActor>(Hit.GetActor());
    }

    return FindFallbackCameraMonsterTarget(ViewOrigin, ViewDirection);
}

AHuwamMonsterEncounterActor* AHuwamPrototypeThirdPersonCharacter::FindFallbackCameraMonsterTarget(const FVector& ViewOrigin, const FVector& ViewDirection) const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    AActor* BestTarget = nullptr;
    float BestScore = -TNumericLimits<float>::Max();
    ConsiderCameraFallbackTarget<AHuwamMonsterEncounterActor>(
        GetWorld(),
        this,
        ViewOrigin,
        ViewDirection,
        CameraInteractionTraceDistance,
        FallbackCameraFocusMinimumDot,
        BestTarget,
        BestScore
    );
    return Cast<AHuwamMonsterEncounterActor>(BestTarget);
}

bool AHuwamPrototypeThirdPersonCharacter::IsSupportedInteractionTarget(const AActor* TargetActor) const
{
    return TargetActor
        && (TargetActor->IsA<AHuwamGuildContractBoardActor>()
            || TargetActor->IsA<AHuwamNpcSupplyQuestOfferActor>()
            || TargetActor->IsA<AHuwamTutorialLabInteractableActor>()
            || TargetActor->IsA<AHuwamTutorialLabGatheringActor>()
            || TargetActor->IsA<AHuwamGatheringResourceActor>());
}

bool AHuwamPrototypeThirdPersonCharacter::GetCameraView(FVector& OutViewOrigin, FVector& OutViewDirection) const
{
    if (FollowCamera)
    {
        OutViewOrigin = FollowCamera->GetComponentLocation();
        OutViewDirection = FollowCamera->GetForwardVector().GetSafeNormal();
        return !OutViewDirection.IsNearlyZero();
    }

    OutViewOrigin = GetPawnViewLocation();
    OutViewDirection = GetViewRotation().Vector().GetSafeNormal();
    return !OutViewDirection.IsNearlyZero();
}

void AHuwamPrototypeThirdPersonCharacter::LinkPrototypeWorldInteraction()
{
    if (!PrototypePlayerActor || !PrototypePlayerActor->GetWorldInteractionComponent())
    {
        return;
    }

    PrototypePlayerActor->SetPrototypeAvatarActor(this);
    PrototypePlayerActor->GetWorldInteractionComponent()->SetAutoRefreshFocus(!bCameraDrivesInteractionFocus);
    if (bDisableLogicActorInteractInputWhenLinked)
    {
        PrototypePlayerActor->DisablePrototypeWorldInteractionInput(Cast<APlayerController>(GetController()));
    }

    if (bShowPrototypePromptWhenLinked)
    {
        PrototypePlayerActor->ShowWorldInteractionPromptScreen(Cast<APlayerController>(GetController()));
    }

    if (bUseLinkedStateActorAsViewTarget)
    {
        if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
        {
            PlayerController->SetViewTarget(this);
        }
    }
}

bool AHuwamPrototypeThirdPersonCharacter::IsMovementTutorialBeatActive() const
{
    const UHuwamTutorialLabComponent* TutorialLab = PrototypePlayerActor
        ? PrototypePlayerActor->GetTutorialLabComponent()
        : nullptr;
    if (!TutorialLab)
    {
        return false;
    }

    const FHuwamTutorialLabSnapshot TutorialSnapshot = TutorialLab->GetTutorialLabSnapshot();
    return TutorialSnapshot.bActive
        && !TutorialSnapshot.bMovementLessonComplete
        && TutorialSnapshot.CurrentBeat == EHuwamTutorialLabBeat::Movement;
}

void AHuwamPrototypeThirdPersonCharacter::RecordTutorialLookInput(float Value)
{
    if (FMath::IsNearlyZero(Value) || !IsMovementTutorialBeatActive())
    {
        return;
    }

    TutorialLookInputAmount += FMath::Abs(Value);
    TryAdvanceTutorialMovementLesson();
}

void AHuwamPrototypeThirdPersonCharacter::TryAdvanceTutorialMovementLesson()
{
    if (!IsMovementTutorialBeatActive())
    {
        ResetTutorialMovementLessonTracking();
        return;
    }

    if (!bTutorialMovementStartRecorded)
    {
        TutorialMovementStartLocation = GetActorLocation();
        bTutorialMovementStartRecorded = true;
    }

    const float MovementDistance = FVector::Dist2D(TutorialMovementStartLocation, GetActorLocation());
    if (MovementDistance < TutorialMovementRequiredDistance || TutorialLookInputAmount < TutorialLookInputRequiredAmount)
    {
        return;
    }

    UHuwamTutorialLabComponent* TutorialLab = PrototypePlayerActor
        ? PrototypePlayerActor->GetTutorialLabComponent()
        : nullptr;
    if (TutorialLab && TutorialLab->MarkMovementLessonComplete())
    {
        PrototypePlayerActor->RefreshPrototypeHud();
    }

    ResetTutorialMovementLessonTracking();
}

void AHuwamPrototypeThirdPersonCharacter::ResetTutorialMovementLessonTracking()
{
    TutorialLookInputAmount = 0.0f;
    bTutorialMovementStartRecorded = false;
    TutorialMovementStartLocation = FVector::ZeroVector;
}

void AHuwamPrototypeThirdPersonCharacter::AnimatePrototypeAvatar(float DeltaSeconds)
{
    if (!PrototypeBodyMesh || !PrototypeHeadMesh)
    {
        return;
    }

    const float HorizontalSpeed = GetVelocity().Size2D();
    const float WalkAlpha = FMath::Clamp(HorizontalSpeed / 500.0f, 0.0f, 1.0f);
    const float WalkPhase = GetWorld() ? GetWorld()->GetTimeSeconds() * 9.0f : DeltaSeconds;
    const float BobHeight = FMath::Sin(WalkPhase) * 3.0f * WalkAlpha;
    const float LeanPitch = -FMath::Clamp(GetVelocity().Size2D() / 45.0f, 0.0f, 8.0f);

    PrototypeBodyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -24.0f + BobHeight));
    PrototypeBodyMesh->SetRelativeRotation(FRotator(LeanPitch, 0.0f, 0.0f));
    PrototypeHeadMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 56.0f + BobHeight * 0.5f));
}

void AHuwamPrototypeThirdPersonCharacter::MoveForward(float Value)
{
    if (FMath::IsNearlyZero(Value))
    {
        return;
    }

    const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
}

void AHuwamPrototypeThirdPersonCharacter::MoveRight(float Value)
{
    if (FMath::IsNearlyZero(Value))
    {
        return;
    }

    const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
}

void AHuwamPrototypeThirdPersonCharacter::TurnCamera(float Value)
{
    RecordTutorialLookInput(Value);
    AddControllerYawInput(Value);
}

void AHuwamPrototypeThirdPersonCharacter::LookCamera(float Value)
{
    RecordTutorialLookInput(Value);
    AddControllerPitchInput(Value);
}

void AHuwamPrototypeThirdPersonCharacter::BeginJump()
{
    Jump();
}

void AHuwamPrototypeThirdPersonCharacter::EndJump()
{
    StopJumping();
}

void AHuwamPrototypeThirdPersonCharacter::HandleInteractInput()
{
    InteractWithCameraFocus();
}

void AHuwamPrototypeThirdPersonCharacter::HandleAttackInput()
{
    AttackTutorialTargetInCameraFocus();
}

void AHuwamPrototypeThirdPersonCharacter::HandleTutorialHudInput()
{
    if (PrototypePlayerActor)
    {
        PrototypePlayerActor->AcknowledgeTutorialHudLesson();
    }
}

void AHuwamPrototypeThirdPersonCharacter::HandleInventoryMenuInput()
{
    if (PrototypePlayerActor)
    {
        PrototypePlayerActor->ShowPrototypeMenuScreen(EHuwamPrototypeMenuPage::InventoryEquipment, Cast<APlayerController>(GetController()));
    }
}

void AHuwamPrototypeThirdPersonCharacter::HandleMapMenuInput()
{
    if (PrototypePlayerActor)
    {
        PrototypePlayerActor->ShowPrototypeMenuScreen(EHuwamPrototypeMenuPage::Map, Cast<APlayerController>(GetController()));
    }
}

void AHuwamPrototypeThirdPersonCharacter::HandleQuestMenuInput()
{
    if (PrototypePlayerActor)
    {
        PrototypePlayerActor->ShowPrototypeMenuScreen(EHuwamPrototypeMenuPage::Quests, Cast<APlayerController>(GetController()));
    }
}

void AHuwamPrototypeThirdPersonCharacter::HandleSaveInput()
{
    if (PrototypePlayerActor)
    {
        PrototypePlayerActor->SavePrototypeProgress();
    }
}

void AHuwamPrototypeThirdPersonCharacter::HandleLoadInput()
{
    if (PrototypePlayerActor)
    {
        PrototypePlayerActor->LoadPrototypeProgress();
    }
}
