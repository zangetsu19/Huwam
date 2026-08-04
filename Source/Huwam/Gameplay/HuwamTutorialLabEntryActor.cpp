#include "Gameplay/HuwamTutorialLabEntryActor.h"

#include "Components/SceneComponent.h"

AHuwamTutorialLabEntryActor::AHuwamTutorialLabEntryActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);
}

FTransform AHuwamTutorialLabEntryActor::GetLabEntryTransform() const
{
    return GetActorTransform();
}

bool AHuwamTutorialLabEntryActor::IsPrimaryLabEntry() const
{
    return bPrimaryLabEntry;
}
