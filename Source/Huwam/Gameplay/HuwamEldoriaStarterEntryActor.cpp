#include "Gameplay/HuwamEldoriaStarterEntryActor.h"

#include "Components/SceneComponent.h"

AHuwamEldoriaStarterEntryActor::AHuwamEldoriaStarterEntryActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);
}

FTransform AHuwamEldoriaStarterEntryActor::GetEntryTransform() const
{
    return GetActorTransform();
}

bool AHuwamEldoriaStarterEntryActor::IsPrimaryEntry() const
{
    return bPrimaryEntry;
}
