#include "Gameplay/HuwamGuildContractBoardActor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gameplay/HuwamMapAwarenessComponent.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
    const TCHAR* StarterSlimeQuestId = TEXT("quest.eldoria.slimes_in_tall_grass");

    int32 GetBoardStatePriority(EHuwamGuildContractBoardState State)
    {
        switch (State)
        {
            case EHuwamGuildContractBoardState::ReadyForTurnIn:
                return 4;
            case EHuwamGuildContractBoardState::TrackingContract:
                return 3;
            case EHuwamGuildContractBoardState::OfferingContract:
                return 2;
            case EHuwamGuildContractBoardState::ContractCleared:
                return 1;
            case EHuwamGuildContractBoardState::Unavailable:
            default:
                return 0;
        }
    }
}

AHuwamGuildContractBoardActor::AHuwamGuildContractBoardActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoardMesh"));
    BoardMesh->SetupAttachment(SceneRoot);
    BoardMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
    BoardMesh->SetRelativeScale3D(FVector(0.18f, 1.15f, 1.35f));

    LeftPostMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftPostMesh"));
    LeftPostMesh->SetupAttachment(SceneRoot);
    LeftPostMesh->SetRelativeLocation(FVector(0.0f, -92.0f, 65.0f));
    LeftPostMesh->SetRelativeScale3D(FVector(0.16f, 0.16f, 1.30f));

    RightPostMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPostMesh"));
    RightPostMesh->SetupAttachment(SceneRoot);
    RightPostMesh->SetRelativeLocation(FVector(0.0f, 92.0f, 65.0f));
    RightPostMesh->SetRelativeScale3D(FVector(0.16f, 0.16f, 1.30f));

    NoticeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NoticeMesh"));
    NoticeMesh->SetupAttachment(BoardMesh);
    NoticeMesh->SetRelativeLocation(FVector(-58.0f, 0.0f, 0.0f));
    NoticeMesh->SetRelativeScale3D(FVector(0.08f, 0.42f, 0.56f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        BoardMesh->SetStaticMesh(CubeMesh.Object);
        LeftPostMesh->SetStaticMesh(CubeMesh.Object);
        RightPostMesh->SetStaticMesh(CubeMesh.Object);
        NoticeMesh->SetStaticMesh(CubeMesh.Object);
    }

    ContractQuestIds.Add(StarterSlimeQuestId);
}

FHuwamGuildContractBoardSnapshot AHuwamGuildContractBoardActor::GetBoardSnapshotForPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    FHuwamGuildContractBoardSnapshot Snapshot;
    Snapshot.PromptText = UnavailablePrompt;

    UHuwamQuestComponent* Quests = PrototypePlayer ? PrototypePlayer->GetQuestComponent() : nullptr;
    if (!PrototypePlayer || !Quests)
    {
        return Snapshot;
    }

    for (const FString& QuestId : ContractQuestIds)
    {
        if (QuestId.IsEmpty())
        {
            continue;
        }

        const FHuwamGuildContractBoardEntry Entry = BuildContractEntry(PrototypePlayer, QuestId);
        Snapshot.Entries.Add(Entry);
        if (Entry.bCanInteract && (!Snapshot.bCanInteract || GetBoardStatePriority(Entry.State) > GetBoardStatePriority(Snapshot.State)))
        {
            Snapshot.State = Entry.State;
            Snapshot.PromptText = BuildPromptForEntry(Entry);
            Snapshot.QuestOffer = Entry.QuestOffer;
            Snapshot.ActiveQuest = Entry.QuestState;
            Snapshot.SelectedQuestId = Entry.QuestId;
            Snapshot.bCanInteract = true;
        }
    }

    return Snapshot;
}

bool AHuwamGuildContractBoardActor::CanInteractWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    return GetBoardSnapshotForPrototypePlayer(PrototypePlayer).bCanInteract;
}

bool AHuwamGuildContractBoardActor::InteractWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, FHuwamGuildContractBoardResult& OutResult)
{
    OutResult = FHuwamGuildContractBoardResult();
    OutResult.SnapshotBefore = GetBoardSnapshotForPrototypePlayer(PrototypePlayer);
    if (!PrototypePlayer || !OutResult.SnapshotBefore.bCanInteract)
    {
        OutResult.SnapshotAfter = OutResult.SnapshotBefore;
        return false;
    }

    switch (OutResult.SnapshotBefore.State)
    {
        case EHuwamGuildContractBoardState::OfferingContract:
            OutResult.bSuccess = PrototypePlayer->GetQuestComponent()
                && PrototypePlayer->GetQuestComponent()->StartQuest(OutResult.SnapshotBefore.SelectedQuestId, PrototypePlayer->GetRewardComponent())
                && TrackQuestForPlayer(PrototypePlayer, OutResult.SnapshotBefore.SelectedQuestId);
            break;
        case EHuwamGuildContractBoardState::TrackingContract:
            OutResult.bSuccess = TrackQuestForPlayer(PrototypePlayer, OutResult.SnapshotBefore.SelectedQuestId);
            break;
        case EHuwamGuildContractBoardState::ReadyForTurnIn:
            OutResult.bSuccess = PrototypePlayer->GetQuestComponent()
                && PrototypePlayer->GetQuestComponent()->TurnInQuest(
                    OutResult.SnapshotBefore.SelectedQuestId,
                    PrototypePlayer->GetRewardComponent(),
                    PrototypePlayer->GetInventoryComponent(),
                    OutResult.TurnedInQuest
                );
            break;
        case EHuwamGuildContractBoardState::ContractCleared:
            OutResult.bSuccess = true;
            break;
        case EHuwamGuildContractBoardState::Unavailable:
        default:
            break;
    }

    PrototypePlayer->RefreshPrototypeHud();
    OutResult.SnapshotAfter = GetBoardSnapshotForPrototypePlayer(PrototypePlayer);
    return OutResult.bSuccess;
}

FHuwamGuildContractBoardEntry AHuwamGuildContractBoardActor::BuildContractEntry(AHuwamPrototypePlayerActor* PrototypePlayer, const FString& QuestId) const
{
    FHuwamGuildContractBoardEntry Entry;
    Entry.QuestId = QuestId;

    UHuwamQuestComponent* Quests = PrototypePlayer ? PrototypePlayer->GetQuestComponent() : nullptr;
    if (!Quests || QuestId.IsEmpty())
    {
        return Entry;
    }

    if (Quests->GetQuestState(QuestId, Entry.QuestState))
    {
        if (Entry.QuestState.bTurnedIn)
        {
            if (Entry.QuestState.bRepeatable && Quests->GetQuestOffer(QuestId, Entry.QuestOffer))
            {
                Entry.State = EHuwamGuildContractBoardState::OfferingContract;
                Entry.bCanInteract = true;
                return Entry;
            }

            Entry.State = EHuwamGuildContractBoardState::ContractCleared;
            Entry.bCanInteract = true;
            return Entry;
        }

        if (Entry.QuestState.bActive)
        {
            Entry.State = Entry.QuestState.bReadyToTurnIn
                ? EHuwamGuildContractBoardState::ReadyForTurnIn
                : EHuwamGuildContractBoardState::TrackingContract;
            Entry.bCanInteract = true;
            return Entry;
        }
    }

    if (Quests->GetQuestOffer(QuestId, Entry.QuestOffer))
    {
        Entry.State = EHuwamGuildContractBoardState::OfferingContract;
        Entry.bCanInteract = true;
    }

    return Entry;
}

bool AHuwamGuildContractBoardActor::TrackQuestForPlayer(AHuwamPrototypePlayerActor* PrototypePlayer, const FString& QuestId) const
{
    UHuwamQuestComponent* Quests = PrototypePlayer ? PrototypePlayer->GetQuestComponent() : nullptr;
    UHuwamMapAwarenessComponent* MapAwareness = PrototypePlayer ? PrototypePlayer->GetMapAwarenessComponent() : nullptr;
    FHuwamQuestRuntimeState QuestState;
    if (!Quests
        || !Quests->GetQuestState(QuestId, QuestState)
        || !QuestState.bActive
        || QuestState.PrimaryObjective.ObjectiveId.IsEmpty())
    {
        return false;
    }

    const bool bTrackedQuest = Quests->TrackQuest(QuestState.QuestId);
    const bool bTrackedMap = !MapAwareness
        || MapAwareness->TrackQuestObjective(QuestState.QuestId, QuestState.PrimaryObjective.ObjectiveId);
    return bTrackedQuest && bTrackedMap;
}

FText AHuwamGuildContractBoardActor::BuildPromptForEntry(const FHuwamGuildContractBoardEntry& Entry) const
{
    const FText ContractName = !Entry.QuestState.DisplayName.IsEmpty()
        ? Entry.QuestState.DisplayName
        : Entry.QuestOffer.DisplayName;

    switch (Entry.State)
    {
        case EHuwamGuildContractBoardState::OfferingContract:
            return ContractName.IsEmpty()
                ? OfferPrompt
                : FText::Format(NSLOCTEXT("HuwamGuildBoard", "OfferNamedContractPrompt", "Accept contract: {0}"), ContractName);
        case EHuwamGuildContractBoardState::TrackingContract:
            return ContractName.IsEmpty()
                ? TrackPrompt
                : FText::Format(NSLOCTEXT("HuwamGuildBoard", "TrackNamedContractPrompt", "Track contract: {0}"), ContractName);
        case EHuwamGuildContractBoardState::ReadyForTurnIn:
            return ContractName.IsEmpty()
                ? TurnInPrompt
                : FText::Format(NSLOCTEXT("HuwamGuildBoard", "TurnInNamedContractPrompt", "Turn in contract: {0}"), ContractName);
        case EHuwamGuildContractBoardState::ContractCleared:
            return ContractName.IsEmpty()
                ? ClearedPrompt
                : FText::Format(NSLOCTEXT("HuwamGuildBoard", "ClearedNamedContractPrompt", "Contract cleared: {0}"), ContractName);
        case EHuwamGuildContractBoardState::Unavailable:
        default:
            return UnavailablePrompt;
    }
}
