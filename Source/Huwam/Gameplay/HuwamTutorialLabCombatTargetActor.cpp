#include "Gameplay/HuwamTutorialLabCombatTargetActor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"
#include "Gameplay/HuwamTutorialLabComponent.h"
#include "UObject/ConstructorHelpers.h"

AHuwamTutorialLabCombatTargetActor::AHuwamTutorialLabCombatTargetActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
    TargetMesh->SetupAttachment(SceneRoot);
    TargetMesh->SetRelativeScale3D(FVector(0.80f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (SphereMesh.Succeeded())
    {
        TargetMesh->SetStaticMesh(SphereMesh.Object);
    }

    Combat = CreateDefaultSubobject<UHuwamCombatComponent>(TEXT("Combat"));
}

bool AHuwamTutorialLabCombatTargetActor::CanReceiveAttackFromPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    if (!PrototypePlayer || !Combat || Combat->IsDefeated() || (bOneShotLessonTarget && bHasCompletedCombatLesson))
    {
        return false;
    }

    const UHuwamTutorialLabComponent* TutorialLab = PrototypePlayer->GetTutorialLabComponent();
    if (!TutorialLab)
    {
        return false;
    }

    const FHuwamTutorialLabSnapshot Snapshot = TutorialLab->GetTutorialLabSnapshot();
    return Snapshot.bActive
        && Snapshot.CurrentBeat == EHuwamTutorialLabBeat::CombatBasics
        && !Snapshot.bCombatLessonComplete
        && (Snapshot.CombatLessonTargetId.IsEmpty() || Snapshot.CombatLessonTargetId == TargetId);
}

bool AHuwamTutorialLabCombatTargetActor::ReceiveAttackFromPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, const FHuwamCombatRollConfig& RollConfig, FHuwamCombatResult& OutCombatResult)
{
    OutCombatResult = FHuwamCombatResult();
    if (!CanReceiveAttackFromPrototypePlayer(PrototypePlayer))
    {
        return false;
    }

    UHuwamCombatComponent* AttackerCombat = PrototypePlayer->GetCombatComponent();
    UHuwamTutorialLabComponent* TutorialLab = PrototypePlayer->GetTutorialLabComponent();
    if (!AttackerCombat || !TutorialLab || !AttackerCombat->AttackTarget(Combat, EHuwamCombatAttackType::Melee, RollConfig, OutCombatResult))
    {
        return false;
    }

    if (!TutorialLab->MarkCombatLessonComplete(TargetId))
    {
        return false;
    }

    bHasCompletedCombatLesson = true;
    OnTutorialLabCombatTargetHit.Broadcast(PrototypePlayer, OutCombatResult);
    PrototypePlayer->RefreshPrototypeHud();
    return true;
}

FString AHuwamTutorialLabCombatTargetActor::GetTargetId() const
{
    return TargetId;
}

FText AHuwamTutorialLabCombatTargetActor::GetTargetDisplayName() const
{
    return TargetDisplayName;
}

UHuwamCombatComponent* AHuwamTutorialLabCombatTargetActor::GetCombatComponent() const
{
    return Combat;
}

bool AHuwamTutorialLabCombatTargetActor::HasCompletedCombatLesson() const
{
    return bHasCompletedCombatLesson;
}
