#include "Gameplay/HuwamTutorialLabGatheringActor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gameplay/HuwamInventoryComponent.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"
#include "Gameplay/HuwamTutorialLabComponent.h"
#include "UObject/ConstructorHelpers.h"

AHuwamTutorialLabGatheringActor::AHuwamTutorialLabGatheringActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    GatheringMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GatheringMesh"));
    GatheringMesh->SetupAttachment(SceneRoot);
    GatheringMesh->SetRelativeScale3D(FVector(0.32f, 0.32f, 0.85f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMesh(TEXT("/Engine/BasicShapes/Cone.Cone"));
    if (ConeMesh.Succeeded())
    {
        GatheringMesh->SetStaticMesh(ConeMesh.Object);
    }
}

bool AHuwamTutorialLabGatheringActor::CanGatherWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    if (!PrototypePlayer
        || (bOneShot && bHasBeenGathered)
        || GatheringSourceId.IsEmpty()
        || GatheredItemId.IsEmpty()
        || GatheredQuantity <= 0)
    {
        return false;
    }

    const UHuwamTutorialLabComponent* TutorialLab = PrototypePlayer->GetTutorialLabComponent();
    const UHuwamInventoryComponent* Inventory = PrototypePlayer->GetInventoryComponent();
    if (!TutorialLab || !Inventory)
    {
        return false;
    }

    const FHuwamTutorialLabSnapshot Snapshot = TutorialLab->GetTutorialLabSnapshot();
    return Snapshot.bActive
        && Snapshot.CurrentBeat == EHuwamTutorialLabBeat::GatheringBasics
        && !Snapshot.bGatheringLessonComplete
        && (RequiredToolItemId.IsEmpty() || Inventory->HasItemQuantity(RequiredToolItemId, 1));
}

bool AHuwamTutorialLabGatheringActor::GatherWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer)
{
    if (!CanGatherWithPrototypePlayer(PrototypePlayer))
    {
        return false;
    }

    UHuwamInventoryComponent* Inventory = PrototypePlayer->GetInventoryComponent();
    UHuwamTutorialLabComponent* TutorialLab = PrototypePlayer->GetTutorialLabComponent();
    if (!Inventory || !TutorialLab)
    {
        return false;
    }

    int32 Remainder = 0;
    if (!Inventory->AddItemById(GatheredItemId, GatheredQuantity, Remainder) || Remainder > 0)
    {
        return false;
    }

    if (!TutorialLab->MarkGatheringLessonComplete(GatheringSourceId, GatheredItemId, GatheredQuantity))
    {
        Inventory->RemoveItemById(GatheredItemId, GatheredQuantity);
        return false;
    }

    bHasBeenGathered = true;
    OnTutorialLabGathered.Broadcast(PrototypePlayer, GatheredItemId, GatheredQuantity);
    PrototypePlayer->RefreshPrototypeHud();
    return true;
}

FText AHuwamTutorialLabGatheringActor::GetGatherPrompt() const
{
    return GatherPrompt;
}

bool AHuwamTutorialLabGatheringActor::HasBeenGathered() const
{
    return bHasBeenGathered;
}
