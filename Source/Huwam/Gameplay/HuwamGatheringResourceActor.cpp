#include "Gameplay/HuwamGatheringResourceActor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Data/HuwamDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Gameplay/HuwamInventoryComponent.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"
#include "Gameplay/HuwamQuestComponent.h"
#include "Gameplay/HuwamRewardComponent.h"
#include "Gameplay/HuwamTimeSubsystem.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
    FName GetRowNameFromId(const FString& Id)
    {
        return FName(*Id.Replace(TEXT("."), TEXT("_")));
    }
}

AHuwamGatheringResourceActor::AHuwamGatheringResourceActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ResourceMesh"));
    ResourceMesh->SetupAttachment(SceneRoot);
    ResourceMesh->SetRelativeScale3D(FVector(0.28f, 0.28f, 0.72f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMesh(TEXT("/Engine/BasicShapes/Cone.Cone"));
    if (ConeMesh.Succeeded())
    {
        ResourceMesh->SetStaticMesh(ConeMesh.Object);
    }

    RequiredToolItemIds.Add(TEXT("item.tool.gathering_knife"));
    QuestProgressSourceIds.Add(TEXT("material.field_supply"));
}

void AHuwamGatheringResourceActor::BeginPlay()
{
    Super::BeginPlay();

    if (bAutoInitializeFromData)
    {
        InitializeFromMaterialData();
    }

    RefreshAvailabilityVisualState();
}

bool AHuwamGatheringResourceActor::InitializeFromMaterialData()
{
    FHuwamMaterialRow MaterialRow;
    if (!LoadMaterialRow(MaterialRow))
    {
        return false;
    }

    MaterialId = MaterialRow.MaterialId;
    GatheredItemId = MaterialRow.ItemId.IsEmpty() ? MaterialRow.MaterialId : MaterialRow.ItemId;
    ResourceDisplayName = MaterialRow.DisplayName;
    Rank = MaterialRow.Rank;
    RespawnInGameDays = MaterialRow.RespawnInGameDays;

    if (!MaterialRow.HarvestToolIds.IsEmpty())
    {
        RequiredToolItemIds = MaterialRow.HarvestToolIds;
    }

    RefreshAvailabilityVisualState();
    return true;
}

bool AHuwamGatheringResourceActor::CanGatherWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    if (!PrototypePlayer || !bAvailable || GatheredItemId.IsEmpty() || GatheredQuantity <= 0)
    {
        return false;
    }

    return PrototypePlayer->GetInventoryComponent() && HasRequiredTool(PrototypePlayer);
}

bool AHuwamGatheringResourceActor::GatherWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer)
{
    if (!CanGatherWithPrototypePlayer(PrototypePlayer))
    {
        return false;
    }

    UHuwamInventoryComponent* Inventory = PrototypePlayer->GetInventoryComponent();
    if (!Inventory)
    {
        return false;
    }

    int32 Remainder = 0;
    if (!Inventory->AddItemById(GatheredItemId, GatheredQuantity, Remainder) || Remainder > 0)
    {
        return false;
    }

    AdvanceMatchingQuestProgress(PrototypePlayer);
    DepleteResource();
    OnResourceGathered.Broadcast(PrototypePlayer, GatheredItemId, GatheredQuantity);
    PrototypePlayer->RefreshPrototypeHud();
    return true;
}

void AHuwamGatheringResourceActor::RespawnResource()
{
    bAvailable = true;
    RefreshAvailabilityVisualState();

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(RespawnTimerHandle);
    }

    OnResourceRespawned.Broadcast();
}

FString AHuwamGatheringResourceActor::GetMaterialId() const
{
    return MaterialId;
}

FString AHuwamGatheringResourceActor::GetGatheredItemId() const
{
    return GatheredItemId;
}

FText AHuwamGatheringResourceActor::GetResourceDisplayName() const
{
    return ResourceDisplayName;
}

FText AHuwamGatheringResourceActor::GetGatherPrompt() const
{
    return ResourceDisplayName.IsEmpty()
        ? GatherPrompt
        : FText::Format(NSLOCTEXT("HuwamGathering", "GatherNamedResourcePrompt", "Gather {0}"), ResourceDisplayName);
}

bool AHuwamGatheringResourceActor::IsAvailable() const
{
    return bAvailable;
}

float AHuwamGatheringResourceActor::GetRespawnDelaySeconds() const
{
    const float ResolvedSecondsPerDay = ResolveSecondsPerInGameDay();
    if (!bRespawns || RespawnInGameDays <= 0 || ResolvedSecondsPerDay <= 0.0f)
    {
        return 0.0f;
    }

    return static_cast<float>(RespawnInGameDays) * ResolvedSecondsPerDay;
}

bool AHuwamGatheringResourceActor::LoadMaterialRow(FHuwamMaterialRow& OutMaterialRow) const
{
    if (MaterialId.IsEmpty())
    {
        return false;
    }

    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (const UHuwamDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UHuwamDataSubsystem>())
            {
                return DataSubsystem->GetMaterialRow(GetRowNameFromId(MaterialId), OutMaterialRow);
            }
        }
    }

    return false;
}

float AHuwamGatheringResourceActor::ResolveSecondsPerInGameDay() const
{
    if (bUseSharedHuwamTime)
    {
        if (const UWorld* World = GetWorld())
        {
            if (UGameInstance* GameInstance = World->GetGameInstance())
            {
                if (const UHuwamTimeSubsystem* TimeSubsystem = GameInstance->GetSubsystem<UHuwamTimeSubsystem>())
                {
                    return TimeSubsystem->GetSecondsPerInGameDay();
                }
            }
        }
    }

    return SecondsPerInGameDay;
}

bool AHuwamGatheringResourceActor::HasRequiredTool(const AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    if (RequiredToolItemIds.IsEmpty())
    {
        return true;
    }

    const UHuwamInventoryComponent* Inventory = PrototypePlayer ? PrototypePlayer->GetInventoryComponent() : nullptr;
    if (!Inventory)
    {
        return false;
    }

    for (const FString& ToolItemId : RequiredToolItemIds)
    {
        if (!ToolItemId.IsEmpty() && Inventory->HasItemQuantity(ToolItemId, 1))
        {
            return true;
        }
    }

    return false;
}

void AHuwamGatheringResourceActor::AdvanceMatchingQuestProgress(AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    UHuwamQuestComponent* Quests = PrototypePlayer ? PrototypePlayer->GetQuestComponent() : nullptr;
    UHuwamRewardComponent* Rewards = PrototypePlayer ? PrototypePlayer->GetRewardComponent() : nullptr;
    if (!Quests || !Rewards || QuestProgressPerGather <= 0)
    {
        return;
    }

    for (const FHuwamQuestRuntimeState& QuestState : Quests->GetActiveQuestStates())
    {
        const FHuwamQuestObjectiveState& Objective = QuestState.PrimaryObjective;
        if (Objective.ObjectiveId.IsEmpty()
            || Objective.ProgressSourceId.IsEmpty()
            || (!QuestProgressSourceIds.Contains(Objective.ProgressSourceId)
                && Objective.ProgressSourceId != MaterialId
                && Objective.ProgressSourceId != GatheredItemId))
        {
            continue;
        }

        int32 ProgressAfter = 0;
        bool bObjectiveCompleted = false;
        if (Rewards->AddQuestProgress(
            QuestState.QuestId,
            Objective.ObjectiveId,
            QuestProgressPerGather,
            Objective.TargetValue,
            ProgressAfter,
            bObjectiveCompleted))
        {
            Quests->RefreshQuestProgressFromRewards(QuestState.QuestId, Rewards);
        }
    }
}

void AHuwamGatheringResourceActor::DepleteResource()
{
    bAvailable = false;
    RefreshAvailabilityVisualState();

    const float RespawnDelaySeconds = GetRespawnDelaySeconds();
    if (RespawnDelaySeconds <= 0.0f)
    {
        return;
    }

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            RespawnTimerHandle,
            this,
            &AHuwamGatheringResourceActor::RespawnResource,
            RespawnDelaySeconds,
            false
        );
    }
}

void AHuwamGatheringResourceActor::RefreshAvailabilityVisualState()
{
    if (ResourceMesh)
    {
        ResourceMesh->SetVisibility(bAvailable, true);
        ResourceMesh->SetCollisionEnabled(bAvailable ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
    }

    SetActorEnableCollision(bAvailable);
}
