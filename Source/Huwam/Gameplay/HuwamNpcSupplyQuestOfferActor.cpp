#include "Gameplay/HuwamNpcSupplyQuestOfferActor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
    const TCHAR* GatherFieldSuppliesQuestId = TEXT("quest.eldoria.gather_field_supplies");
}

AHuwamNpcSupplyQuestOfferActor::AHuwamNpcSupplyQuestOfferActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    PrototypeBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypeBodyMesh"));
    PrototypeBodyMesh->SetupAttachment(SceneRoot);
    PrototypeBodyMesh->SetRelativeScale3D(FVector(0.46f, 0.46f, 1.12f));

    PrototypeHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypeHeadMesh"));
    PrototypeHeadMesh->SetupAttachment(SceneRoot);
    PrototypeHeadMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 116.0f));
    PrototypeHeadMesh->SetRelativeScale3D(FVector(0.30f));

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

    NpcSupply = CreateDefaultSubobject<UHuwamNpcSupplyComponent>(TEXT("NpcSupply"));
}

bool AHuwamNpcSupplyQuestOfferActor::CanInteractWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    return GetInteractionSnapshotForPrototypePlayer(PrototypePlayer).State
        != EHuwamNpcSupplyQuestInteractionState::Unavailable;
}

FHuwamNpcSupplyQuestInteractionSnapshot AHuwamNpcSupplyQuestOfferActor::GetInteractionSnapshotForPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    FHuwamNpcSupplyQuestInteractionSnapshot Snapshot;
    Snapshot.PromptText = UnavailablePrompt;

    if (NpcSupply)
    {
        Snapshot.SupplyNeed = NpcSupply->GetSupplyNeedState();
    }

    const UHuwamQuestComponent* Quests = PrototypePlayer ? PrototypePlayer->GetQuestComponent() : nullptr;
    if (!PrototypePlayer || !NpcSupply || !Quests)
    {
        return Snapshot;
    }

    if (Quests->GetQuestState(GatherFieldSuppliesQuestId, Snapshot.ActiveQuest)
        && Snapshot.ActiveQuest.bActive
        && !Snapshot.ActiveQuest.bTurnedIn)
    {
        Snapshot.bCanTurnInQuest = Quests->CanTurnInQuest(GatherFieldSuppliesQuestId);
        Snapshot.State = Snapshot.bCanTurnInQuest
            ? EHuwamNpcSupplyQuestInteractionState::ReadyForTurnIn
            : EHuwamNpcSupplyQuestInteractionState::WaitingForDelivery;
        Snapshot.PromptText = Snapshot.bCanTurnInQuest ? ReadyForTurnInPrompt : WaitingForDeliveryPrompt;
        return Snapshot;
    }

    if (PrototypePlayer->GetGatherFieldSuppliesQuestOfferFromSupply(NpcSupply, Snapshot.QuestOffer))
    {
        Snapshot.State = EHuwamNpcSupplyQuestInteractionState::OfferingWork;
        Snapshot.PromptText = InteractionPrompt;
        Snapshot.bCanPreviewOffer = true;
        Snapshot.bCanAcceptOffer = true;
        return Snapshot;
    }

    if (Snapshot.SupplyNeed.bNeedSatisfied || !Snapshot.SupplyNeed.bShouldRequestSupplyHelp)
    {
        Snapshot.State = EHuwamNpcSupplyQuestInteractionState::Satisfied;
        Snapshot.PromptText = SatisfiedPrompt;
    }

    return Snapshot;
}

bool AHuwamNpcSupplyQuestOfferActor::InteractByStateWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, bool bAcceptOfferWhenAvailable, FHuwamNpcSupplyQuestInteractionResult& OutInteractionResult)
{
    OutInteractionResult = FHuwamNpcSupplyQuestInteractionResult();
    OutInteractionResult.SnapshotBefore = GetInteractionSnapshotForPrototypePlayer(PrototypePlayer);

    switch (OutInteractionResult.SnapshotBefore.State)
    {
        case EHuwamNpcSupplyQuestInteractionState::OfferingWork:
        {
            OutInteractionResult.bSuccess = bAcceptOfferWhenAvailable
                ? AcceptQuestOfferWithPrototypePlayer(PrototypePlayer, OutInteractionResult.QuestOffer)
                : InteractWithPrototypePlayer(PrototypePlayer, OutInteractionResult.QuestOffer);
            OutInteractionResult.Outcome = OutInteractionResult.bSuccess
                ? (bAcceptOfferWhenAvailable
                    ? EHuwamNpcSupplyQuestInteractionOutcome::OfferAccepted
                    : EHuwamNpcSupplyQuestInteractionOutcome::OfferPreviewed)
                : EHuwamNpcSupplyQuestInteractionOutcome::Unavailable;
            break;
        }
        case EHuwamNpcSupplyQuestInteractionState::WaitingForDelivery:
            OutInteractionResult.bSuccess = true;
            OutInteractionResult.Outcome = EHuwamNpcSupplyQuestInteractionOutcome::WaitingForDelivery;
            break;
        case EHuwamNpcSupplyQuestInteractionState::ReadyForTurnIn:
            OutInteractionResult.bSuccess = TurnInQuestWithPrototypePlayer(
                PrototypePlayer,
                OutInteractionResult.TurnedInQuest,
                OutInteractionResult.DeliveryResult
            );
            OutInteractionResult.Outcome = OutInteractionResult.bSuccess
                ? EHuwamNpcSupplyQuestInteractionOutcome::QuestTurnedIn
                : EHuwamNpcSupplyQuestInteractionOutcome::Unavailable;
            break;
        case EHuwamNpcSupplyQuestInteractionState::Satisfied:
            OutInteractionResult.bSuccess = true;
            OutInteractionResult.Outcome = EHuwamNpcSupplyQuestInteractionOutcome::Satisfied;
            break;
        case EHuwamNpcSupplyQuestInteractionState::Unavailable:
        default:
            OutInteractionResult.Outcome = EHuwamNpcSupplyQuestInteractionOutcome::Unavailable;
            break;
    }

    OutInteractionResult.SnapshotAfter = GetInteractionSnapshotForPrototypePlayer(PrototypePlayer);
    return OutInteractionResult.bSuccess;
}

bool AHuwamNpcSupplyQuestOfferActor::InteractWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, FHuwamQuestOffer& OutQuestOffer)
{
    OutQuestOffer = FHuwamQuestOffer();

    if (!PrototypePlayer
        || !NpcSupply
        || !PrototypePlayer->GetGatherFieldSuppliesQuestOfferFromSupply(NpcSupply, OutQuestOffer))
    {
        return false;
    }

    OnQuestOfferPreviewed.Broadcast(PrototypePlayer, OutQuestOffer);
    return true;
}

bool AHuwamNpcSupplyQuestOfferActor::AcceptQuestOfferWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, FHuwamQuestOffer& OutAcceptedQuestOffer)
{
    OutAcceptedQuestOffer = FHuwamQuestOffer();

    if (!PrototypePlayer
        || !NpcSupply
        || !PrototypePlayer->GetGatherFieldSuppliesQuestOfferFromSupply(NpcSupply, OutAcceptedQuestOffer)
        || !PrototypePlayer->StartGatherFieldSuppliesQuestFromSupply(NpcSupply))
    {
        OutAcceptedQuestOffer = FHuwamQuestOffer();
        return false;
    }

    OnQuestOfferAccepted.Broadcast(PrototypePlayer, OutAcceptedQuestOffer);
    return true;
}

bool AHuwamNpcSupplyQuestOfferActor::CanTurnInQuestWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    const UHuwamQuestComponent* Quests = PrototypePlayer ? PrototypePlayer->GetQuestComponent() : nullptr;
    return PrototypePlayer
        && NpcSupply
        && Quests
        && Quests->CanTurnInQuest(GatherFieldSuppliesQuestId);
}

bool AHuwamNpcSupplyQuestOfferActor::TurnInQuestWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, FHuwamQuestRuntimeState& OutTurnedInQuest, FHuwamNpcSupplyDeliveryResult& OutDeliveryResult)
{
    OutTurnedInQuest = FHuwamQuestRuntimeState();
    OutDeliveryResult = FHuwamNpcSupplyDeliveryResult();

    if (!CanTurnInQuestWithPrototypePlayer(PrototypePlayer)
        || !PrototypePlayer->TurnInGatherFieldSuppliesQuest(NpcSupply, OutTurnedInQuest, OutDeliveryResult))
    {
        return false;
    }

    OnQuestTurnedIn.Broadcast(PrototypePlayer, OutTurnedInQuest, OutDeliveryResult);
    return true;
}

FText AHuwamNpcSupplyQuestOfferActor::GetInteractionPrompt() const
{
    return InteractionPrompt;
}

UHuwamNpcSupplyComponent* AHuwamNpcSupplyQuestOfferActor::GetNpcSupplyComponent() const
{
    return NpcSupply;
}
