#include "Gameplay/HuwamPrototypeSliceBlockoutActor.h"

#include "Components/PointLightComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Gameplay/HuwamEldoriaStarterEntryActor.h"
#include "Gameplay/HuwamGatheringResourceActor.h"
#include "Gameplay/HuwamGuildContractBoardActor.h"
#include "Gameplay/HuwamMonsterEncounterActor.h"
#include "Gameplay/HuwamNpcSupplyQuestOfferActor.h"
#include "Gameplay/HuwamSurvivalServiceActor.h"
#include "Gameplay/HuwamTutorialLabCombatTargetActor.h"
#include "Gameplay/HuwamTutorialLabEntryActor.h"
#include "Gameplay/HuwamTutorialLabGatheringActor.h"
#include "Gameplay/HuwamTutorialLabInteractableActor.h"
#include "UObject/ConstructorHelpers.h"

AHuwamPrototypeSliceBlockoutActor::AHuwamPrototypeSliceBlockoutActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    LabFloor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LabFloor"));
    LabNorthWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LabNorthWall"));
    LabSouthWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LabSouthWall"));
    LabEastWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LabEastWall"));
    LabWestWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LabWestWall"));
    EldoriaGround = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EldoriaGround"));
    EldoriaRoad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EldoriaRoad"));
    EldoriaGuildPad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EldoriaGuildPad"));
    EldoriaSupplyPad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EldoriaSupplyPad"));
    EldoriaGuildRoadSpur = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EldoriaGuildRoadSpur"));
    EldoriaSupplyRoadSpur = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EldoriaSupplyRoadSpur"));
    EldoriaNorthFence = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EldoriaNorthFence"));
    EldoriaSouthFence = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EldoriaSouthFence"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    UStaticMesh* Cube = CubeMesh.Succeeded() ? CubeMesh.Object : nullptr;
    ConfigureBlockMesh(LabFloor, Cube, FVector::ZeroVector, FVector(30.0f, 22.0f, 0.25f), true);
    ConfigureBlockMesh(LabNorthWall, Cube, FVector(0.0f, 1100.0f, 260.0f), FVector(30.0f, 0.25f, 5.0f), true);
    ConfigureBlockMesh(LabSouthWall, Cube, FVector(0.0f, -1100.0f, 260.0f), FVector(30.0f, 0.25f, 5.0f), true);
    ConfigureBlockMesh(LabEastWall, Cube, FVector(1500.0f, 0.0f, 260.0f), FVector(0.25f, 22.0f, 5.0f), true);
    ConfigureBlockMesh(LabWestWall, Cube, FVector(-1500.0f, 0.0f, 260.0f), FVector(0.25f, 22.0f, 5.0f), true);
    ConfigureBlockMesh(EldoriaGround, Cube, EldoriaOffset, FVector(36.0f, 36.0f, 0.25f), true);
    ConfigureBlockMesh(EldoriaRoad, Cube, EldoriaOffset + FVector(0.0f, 0.0f, 18.0f), FVector(28.0f, 3.0f, 0.10f), false);
    ConfigureBlockMesh(EldoriaGuildPad, Cube, EldoriaOffset + FVector(-420.0f, 620.0f, 18.0f), FVector(4.2f, 3.8f, 0.12f), false);
    ConfigureBlockMesh(EldoriaSupplyPad, Cube, EldoriaOffset + FVector(-420.0f, -620.0f, 18.0f), FVector(4.2f, 3.8f, 0.12f), false);
    ConfigureBlockMesh(EldoriaGuildRoadSpur, Cube, EldoriaOffset + FVector(-420.0f, 360.0f, 19.0f), FVector(3.5f, 3.2f, 0.08f), false);
    ConfigureBlockMesh(EldoriaSupplyRoadSpur, Cube, EldoriaOffset + FVector(-420.0f, -360.0f, 19.0f), FVector(3.5f, 3.2f, 0.08f), false);
    ConfigureBlockMesh(EldoriaNorthFence, Cube, EldoriaOffset + FVector(0.0f, 1680.0f, 72.0f), FVector(31.0f, 0.18f, 1.25f), true);
    ConfigureBlockMesh(EldoriaSouthFence, Cube, EldoriaOffset + FVector(0.0f, -1680.0f, 72.0f), FVector(31.0f, 0.18f, 1.25f), true);

    LabLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LabLight"));
    LabLight->SetupAttachment(SceneRoot);
    LabLight->SetRelativeLocation(FVector(0.0f, 0.0f, 720.0f));
    LabLight->SetIntensity(24000.0f);
    LabLight->SetAttenuationRadius(3600.0f);
}

void AHuwamPrototypeSliceBlockoutActor::BeginPlay()
{
    Super::BeginPlay();

    if (bSpawnLessonActorsOnBeginPlay)
    {
        SpawnPrototypeSliceActors();
    }
}

bool AHuwamPrototypeSliceBlockoutActor::SpawnPrototypeSliceActors()
{
    if (!GetWorld())
    {
        return false;
    }

    bool bSpawnedAny = false;
    bSpawnedAny |= GetWorld()->SpawnActor<AHuwamTutorialLabEntryActor>(AHuwamTutorialLabEntryActor::StaticClass(), MakeOffsetTransform(FVector(-760.0f, 0.0f, 116.0f))) != nullptr;
    bSpawnedAny |= GetWorld()->SpawnActor<AHuwamTutorialLabInteractableActor>(AHuwamTutorialLabInteractableActor::StaticClass(), MakeOffsetTransform(FVector(-220.0f, 0.0f, 72.0f))) != nullptr;
    bSpawnedAny |= GetWorld()->SpawnActor<AHuwamTutorialLabCombatTargetActor>(AHuwamTutorialLabCombatTargetActor::StaticClass(), MakeOffsetTransform(FVector(500.0f, 0.0f, 116.0f))) != nullptr;
    bSpawnedAny |= GetWorld()->SpawnActor<AHuwamTutorialLabGatheringActor>(AHuwamTutorialLabGatheringActor::StaticClass(), MakeOffsetTransform(FVector(980.0f, 180.0f, 62.0f))) != nullptr;
    bSpawnedAny |= GetWorld()->SpawnActor<AHuwamEldoriaStarterEntryActor>(AHuwamEldoriaStarterEntryActor::StaticClass(), MakeOffsetTransform(EldoriaOffset + FVector(-800.0f, 0.0f, 116.0f))) != nullptr;
    bSpawnedAny |= GetWorld()->SpawnActor<AHuwamGuildContractBoardActor>(AHuwamGuildContractBoardActor::StaticClass(), MakeOffsetTransform(EldoriaOffset + FVector(-420.0f, 620.0f, 0.0f))) != nullptr;
    bSpawnedAny |= GetWorld()->SpawnActor<AHuwamNpcSupplyQuestOfferActor>(AHuwamNpcSupplyQuestOfferActor::StaticClass(), MakeOffsetTransform(EldoriaOffset + FVector(-420.0f, -620.0f, 96.0f))) != nullptr;

    const auto SpawnSurvivalService = [this](EHuwamSurvivalServiceType ServiceType, const FVector& Offset, const FString& ServiceInstanceId)
    {
        AHuwamSurvivalServiceActor* ServiceActor = GetWorld()->SpawnActor<AHuwamSurvivalServiceActor>(AHuwamSurvivalServiceActor::StaticClass(), MakeOffsetTransform(Offset));
        if (ServiceActor)
        {
            ServiceActor->SetServiceType(ServiceType);
            ServiceActor->SetServiceInstanceId(ServiceInstanceId);
        }
        return ServiceActor != nullptr;
    };

    bSpawnedAny |= SpawnSurvivalService(EHuwamSurvivalServiceType::TavernMeal, EldoriaOffset + FVector(240.0f, 760.0f, 72.0f), TEXT("service.eldoria.tavern_meal.01"));
    bSpawnedAny |= SpawnSurvivalService(EHuwamSurvivalServiceType::WellWater, EldoriaOffset + FVector(420.0f, -40.0f, 72.0f), TEXT("service.eldoria.town_well.01"));
    bSpawnedAny |= SpawnSurvivalService(EHuwamSurvivalServiceType::CampfireChat, EldoriaOffset + FVector(760.0f, -520.0f, 72.0f), TEXT("service.eldoria.campfire_chat.01"));
    bSpawnedAny |= SpawnSurvivalService(EHuwamSurvivalServiceType::InnRoom, EldoriaOffset + FVector(700.0f, 680.0f, 72.0f), TEXT("service.eldoria.inn_room.01"));

    const FVector SlimeOffsets[] =
    {
        EldoriaOffset + FVector(-120.0f, 360.0f, 72.0f),
        EldoriaOffset + FVector(280.0f, 430.0f, 72.0f),
        EldoriaOffset + FVector(620.0f, 260.0f, 72.0f)
    };
    for (const FVector& Offset : SlimeOffsets)
    {
        bSpawnedAny |= GetWorld()->SpawnActor<AHuwamMonsterEncounterActor>(AHuwamMonsterEncounterActor::StaticClass(), MakeOffsetTransform(Offset)) != nullptr;
    }

    const FVector HerbOffsets[] =
    {
        EldoriaOffset + FVector(-220.0f, -360.0f, 54.0f),
        EldoriaOffset + FVector(160.0f, -480.0f, 54.0f),
        EldoriaOffset + FVector(520.0f, -320.0f, 54.0f)
    };
    for (const FVector& Offset : HerbOffsets)
    {
        bSpawnedAny |= GetWorld()->SpawnActor<AHuwamGatheringResourceActor>(AHuwamGatheringResourceActor::StaticClass(), MakeOffsetTransform(Offset)) != nullptr;
    }

    return bSpawnedAny;
}

void AHuwamPrototypeSliceBlockoutActor::ConfigureBlockMesh(UStaticMeshComponent* Mesh, UStaticMesh* MeshAsset, const FVector& Location, const FVector& Scale, bool bCollision) const
{
    if (!Mesh)
    {
        return;
    }

    Mesh->SetupAttachment(SceneRoot);
    Mesh->SetRelativeLocation(Location);
    Mesh->SetRelativeScale3D(Scale);
    Mesh->SetCollisionEnabled(bCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
    if (MeshAsset)
    {
        Mesh->SetStaticMesh(MeshAsset);
    }
}

FTransform AHuwamPrototypeSliceBlockoutActor::MakeOffsetTransform(const FVector& Offset) const
{
    return FTransform(GetActorRotation(), GetActorTransform().TransformPosition(Offset), FVector::OneVector);
}
