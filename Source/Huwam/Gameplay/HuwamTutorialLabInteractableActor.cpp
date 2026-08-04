#include "Gameplay/HuwamTutorialLabInteractableActor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"
#include "Gameplay/HuwamTutorialLabComponent.h"
#include "UObject/ConstructorHelpers.h"

AHuwamTutorialLabInteractableActor::AHuwamTutorialLabInteractableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    PedestalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PedestalMesh"));
    PedestalMesh->SetupAttachment(SceneRoot);
    PedestalMesh->SetRelativeScale3D(FVector(0.75f, 0.75f, 1.10f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (CylinderMesh.Succeeded())
    {
        PedestalMesh->SetStaticMesh(CylinderMesh.Object);
    }
}

bool AHuwamTutorialLabInteractableActor::CanInteractWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    if (!PrototypePlayer || (bOneShot && bHasBeenInteractedWith))
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
        && Snapshot.CurrentBeat == EHuwamTutorialLabBeat::InteractionReady
        && !Snapshot.bInteractionLessonComplete;
}

bool AHuwamTutorialLabInteractableActor::InteractWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer)
{
    if (!CanInteractWithPrototypePlayer(PrototypePlayer))
    {
        return false;
    }

    UHuwamTutorialLabComponent* TutorialLab = PrototypePlayer->GetTutorialLabComponent();
    if (!TutorialLab || !TutorialLab->MarkInteractionLessonComplete(InteractionId))
    {
        return false;
    }

    bHasBeenInteractedWith = true;
    OnTutorialLabInteraction.Broadcast(PrototypePlayer);
    PrototypePlayer->RefreshPrototypeHud();
    return true;
}

FText AHuwamTutorialLabInteractableActor::GetInteractionPrompt() const
{
    return InteractionPrompt;
}

bool AHuwamTutorialLabInteractableActor::HasBeenInteractedWith() const
{
    return bHasBeenInteractedWith;
}
