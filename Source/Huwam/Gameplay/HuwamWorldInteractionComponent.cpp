#include "Gameplay/HuwamWorldInteractionComponent.h"

#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Gameplay/HuwamGatheringResourceActor.h"
#include "Gameplay/HuwamGuildContractBoardActor.h"
#include "Gameplay/HuwamNpcSupplyQuestOfferActor.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"
#include "Gameplay/HuwamSurvivalServiceActor.h"
#include "Gameplay/HuwamTutorialLabGatheringActor.h"
#include "Gameplay/HuwamTutorialLabInteractableActor.h"

namespace
{
    template <typename TargetType>
    void ConsiderNearestTarget(
        UWorld* World,
        const AActor* InteractionOwner,
        float FocusRadius,
        AActor*& InOutNearestTarget,
        float& InOutNearestDistance
    )
    {
        if (!World || !InteractionOwner)
        {
            return;
        }

        for (TActorIterator<TargetType> It(World); It; ++It)
        {
            AActor* Candidate = *It;
            if (!Candidate || Candidate == InteractionOwner)
            {
                continue;
            }

            const float CandidateDistance = FVector::Dist(InteractionOwner->GetActorLocation(), Candidate->GetActorLocation());
            if (CandidateDistance <= FocusRadius && CandidateDistance < InOutNearestDistance)
            {
                InOutNearestTarget = Candidate;
                InOutNearestDistance = CandidateDistance;
            }
        }
    }
}

UHuwamWorldInteractionComponent::UHuwamWorldInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UHuwamWorldInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bAutoRefreshFocus)
    {
        return;
    }

    FocusRefreshElapsedSeconds += DeltaTime;
    if (FocusRefreshIntervalSeconds > 0.0f && FocusRefreshElapsedSeconds < FocusRefreshIntervalSeconds)
    {
        return;
    }

    FocusRefreshElapsedSeconds = 0.0f;
    RefreshInteractionFocus();
}

FHuwamWorldInteractionSnapshot UHuwamWorldInteractionComponent::RefreshInteractionFocus()
{
    AHuwamPrototypePlayerActor* PrototypePlayer = ResolvePrototypePlayer();
    if (!FocusedTargetActor || !IsTargetInRange(FocusedTargetActor))
    {
        FocusedTargetActor = FindNearestSupportedTarget(PrototypePlayer);
    }

    LastInteractionSnapshot = BuildSnapshotForTarget(PrototypePlayer, FocusedTargetActor);
    if (!LastInteractionSnapshot.bHasTarget)
    {
        FocusedTargetActor = nullptr;
    }

    OnInteractionFocusChanged.Broadcast(LastInteractionSnapshot);
    return LastInteractionSnapshot;
}

FHuwamWorldInteractionSnapshot UHuwamWorldInteractionComponent::SetInteractionFocusActor(AActor* InTargetActor)
{
    if (!InTargetActor)
    {
        return ClearInteractionFocusActor();
    }

    FocusedTargetActor = InTargetActor;
    return RefreshInteractionFocus();
}

FHuwamWorldInteractionSnapshot UHuwamWorldInteractionComponent::ClearInteractionFocusActor()
{
    FocusedTargetActor = nullptr;
    LastInteractionSnapshot = FHuwamWorldInteractionSnapshot();
    OnInteractionFocusChanged.Broadcast(LastInteractionSnapshot);
    return LastInteractionSnapshot;
}

void UHuwamWorldInteractionComponent::SetInteractionOriginActor(AActor* InOriginActor)
{
    InteractionOriginActor = InOriginActor;
    RefreshInteractionFocus();
}

void UHuwamWorldInteractionComponent::SetAutoRefreshFocus(bool bInAutoRefreshFocus)
{
    bAutoRefreshFocus = bInAutoRefreshFocus;
    FocusRefreshElapsedSeconds = 0.0f;
}

bool UHuwamWorldInteractionComponent::InteractWithFocusedTarget(FHuwamWorldInteractionResult& OutResult, APlayerController* OwningPlayer)
{
    OutResult = FHuwamWorldInteractionResult();
    AHuwamPrototypePlayerActor* PrototypePlayer = ResolvePrototypePlayer();
    OutResult.SnapshotBefore = RefreshInteractionFocus();

    if (!PrototypePlayer
        || !OutResult.SnapshotBefore.bCanInteract
        || !OutResult.SnapshotBefore.TargetActor)
    {
        OutResult.Outcome = EHuwamWorldInteractionOutcome::Unavailable;
        OutResult.SnapshotAfter = LastInteractionSnapshot;
        LastInteractionResult = OutResult;
        OnInteractionRouted.Broadcast(LastInteractionResult);
        return false;
    }

    if (AHuwamGuildContractBoardActor* GuildBoard = Cast<AHuwamGuildContractBoardActor>(OutResult.SnapshotBefore.TargetActor))
    {
        FHuwamGuildContractBoardResult BoardResult;
        OutResult.bSuccess = GuildBoard->InteractWithPrototypePlayer(PrototypePlayer, BoardResult);
        OutResult.Outcome = OutResult.bSuccess
            ? EHuwamWorldInteractionOutcome::GuildContractUpdated
            : EHuwamWorldInteractionOutcome::Unavailable;
    }
    else if (AHuwamNpcSupplyQuestOfferActor* QuestRequester = Cast<AHuwamNpcSupplyQuestOfferActor>(OutResult.SnapshotBefore.TargetActor))
    {
        OutResult.bSuccess = PrototypePlayer->ShowNpcSupplyQuestInteractionScreen(QuestRequester, OwningPlayer);
        OutResult.Outcome = OutResult.bSuccess
            ? EHuwamWorldInteractionOutcome::NpcSupplyQuestScreenOpened
            : EHuwamWorldInteractionOutcome::Unavailable;
    }
    else if (AHuwamTutorialLabInteractableActor* TutorialObject = Cast<AHuwamTutorialLabInteractableActor>(OutResult.SnapshotBefore.TargetActor))
    {
        OutResult.bSuccess = TutorialObject->InteractWithPrototypePlayer(PrototypePlayer);
        OutResult.Outcome = OutResult.bSuccess
            ? EHuwamWorldInteractionOutcome::TutorialInteractionCompleted
            : EHuwamWorldInteractionOutcome::Unavailable;
    }
    else if (AHuwamTutorialLabGatheringActor* TutorialGatherable = Cast<AHuwamTutorialLabGatheringActor>(OutResult.SnapshotBefore.TargetActor))
    {
        OutResult.bSuccess = TutorialGatherable->GatherWithPrototypePlayer(PrototypePlayer);
        OutResult.Outcome = OutResult.bSuccess
            ? EHuwamWorldInteractionOutcome::TutorialResourceGathered
            : EHuwamWorldInteractionOutcome::Unavailable;
    }
    else if (AHuwamGatheringResourceActor* GatheringResource = Cast<AHuwamGatheringResourceActor>(OutResult.SnapshotBefore.TargetActor))
    {
        OutResult.bSuccess = GatheringResource->GatherWithPrototypePlayer(PrototypePlayer);
        OutResult.Outcome = OutResult.bSuccess
            ? EHuwamWorldInteractionOutcome::ResourceGathered
            : EHuwamWorldInteractionOutcome::Unavailable;
    }
    else if (AHuwamSurvivalServiceActor* SurvivalService = Cast<AHuwamSurvivalServiceActor>(OutResult.SnapshotBefore.TargetActor))
    {
        FHuwamSurvivalServiceResult ServiceResult;
        OutResult.bSuccess = SurvivalService->UseServiceWithPrototypePlayer(PrototypePlayer, ServiceResult);
        OutResult.Outcome = OutResult.bSuccess
            ? EHuwamWorldInteractionOutcome::SurvivalServiceUsed
            : EHuwamWorldInteractionOutcome::Unavailable;
    }
    else
    {
        OutResult.Outcome = EHuwamWorldInteractionOutcome::Unavailable;
    }

    OutResult.SnapshotAfter = RefreshInteractionFocus();
    LastInteractionResult = OutResult;
    OnInteractionRouted.Broadcast(LastInteractionResult);
    PrototypePlayer->RefreshPrototypeHud();
    return OutResult.bSuccess;
}

FHuwamWorldInteractionSnapshot UHuwamWorldInteractionComponent::GetLastInteractionSnapshot() const
{
    return LastInteractionSnapshot;
}

FHuwamWorldInteractionResult UHuwamWorldInteractionComponent::GetLastInteractionResult() const
{
    return LastInteractionResult;
}

AActor* UHuwamWorldInteractionComponent::GetFocusedTargetActor() const
{
    return FocusedTargetActor;
}

AActor* UHuwamWorldInteractionComponent::GetInteractionOriginActor() const
{
    return InteractionOriginActor ? InteractionOriginActor.Get() : GetOwner();
}

bool UHuwamWorldInteractionComponent::IsAutoRefreshFocusEnabled() const
{
    return bAutoRefreshFocus;
}

AHuwamPrototypePlayerActor* UHuwamWorldInteractionComponent::ResolvePrototypePlayer() const
{
    return Cast<AHuwamPrototypePlayerActor>(GetOwner());
}

const AActor* UHuwamWorldInteractionComponent::ResolveInteractionOriginActor() const
{
    return InteractionOriginActor ? InteractionOriginActor.Get() : GetOwner();
}

AActor* UHuwamWorldInteractionComponent::FindNearestSupportedTarget(AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    const AActor* OriginActor = ResolveInteractionOriginActor();
    if (!PrototypePlayer || !GetWorld() || !OriginActor)
    {
        return nullptr;
    }

    AActor* NearestTarget = nullptr;
    float NearestDistance = FocusRadius + 1.0f;
    ConsiderNearestTarget<AHuwamGuildContractBoardActor>(GetWorld(), OriginActor, FocusRadius, NearestTarget, NearestDistance);
    ConsiderNearestTarget<AHuwamNpcSupplyQuestOfferActor>(GetWorld(), OriginActor, FocusRadius, NearestTarget, NearestDistance);
    ConsiderNearestTarget<AHuwamTutorialLabInteractableActor>(GetWorld(), OriginActor, FocusRadius, NearestTarget, NearestDistance);
    ConsiderNearestTarget<AHuwamTutorialLabGatheringActor>(GetWorld(), OriginActor, FocusRadius, NearestTarget, NearestDistance);
    ConsiderNearestTarget<AHuwamGatheringResourceActor>(GetWorld(), OriginActor, FocusRadius, NearestTarget, NearestDistance);
    ConsiderNearestTarget<AHuwamSurvivalServiceActor>(GetWorld(), OriginActor, FocusRadius, NearestTarget, NearestDistance);
    return NearestTarget;
}

FHuwamWorldInteractionSnapshot UHuwamWorldInteractionComponent::BuildSnapshotForTarget(AHuwamPrototypePlayerActor* PrototypePlayer, AActor* TargetActor) const
{
    FHuwamWorldInteractionSnapshot Snapshot;
    if (!PrototypePlayer || !TargetActor || !IsTargetInRange(TargetActor))
    {
        return Snapshot;
    }

    Snapshot.TargetActor = TargetActor;
    Snapshot.bHasTarget = true;
    Snapshot.DistanceToTarget = FVector::Dist(ResolveInteractionOriginActor()->GetActorLocation(), TargetActor->GetActorLocation());

    if (const AHuwamGuildContractBoardActor* GuildBoard = Cast<AHuwamGuildContractBoardActor>(TargetActor))
    {
        const FHuwamGuildContractBoardSnapshot BoardSnapshot = GuildBoard->GetBoardSnapshotForPrototypePlayer(PrototypePlayer);
        Snapshot.Kind = EHuwamWorldInteractionKind::GuildContractBoard;
        Snapshot.PromptText = BoardSnapshot.PromptText;
        Snapshot.bCanInteract = BoardSnapshot.bCanInteract;
    }
    else if (const AHuwamNpcSupplyQuestOfferActor* QuestRequester = Cast<AHuwamNpcSupplyQuestOfferActor>(TargetActor))
    {
        const FHuwamNpcSupplyQuestInteractionSnapshot QuestSnapshot = QuestRequester->GetInteractionSnapshotForPrototypePlayer(PrototypePlayer);
        Snapshot.Kind = EHuwamWorldInteractionKind::NpcSupplyQuestRequester;
        Snapshot.PromptText = QuestSnapshot.PromptText;
        Snapshot.bCanInteract = QuestRequester->CanInteractWithPrototypePlayer(PrototypePlayer);
    }
    else if (const AHuwamTutorialLabInteractableActor* TutorialObject = Cast<AHuwamTutorialLabInteractableActor>(TargetActor))
    {
        Snapshot.Kind = EHuwamWorldInteractionKind::TutorialLabObject;
        Snapshot.PromptText = TutorialObject->GetInteractionPrompt();
        Snapshot.bCanInteract = TutorialObject->CanInteractWithPrototypePlayer(PrototypePlayer);
    }
    else if (const AHuwamGatheringResourceActor* GatheringResource = Cast<AHuwamGatheringResourceActor>(TargetActor))
    {
        Snapshot.Kind = EHuwamWorldInteractionKind::GatheringResource;
        Snapshot.PromptText = GatheringResource->GetGatherPrompt();
        Snapshot.bCanInteract = GatheringResource->CanGatherWithPrototypePlayer(PrototypePlayer);
    }
    else if (const AHuwamTutorialLabGatheringActor* TutorialGatherable = Cast<AHuwamTutorialLabGatheringActor>(TargetActor))
    {
        Snapshot.Kind = EHuwamWorldInteractionKind::TutorialLabGatherable;
        Snapshot.PromptText = TutorialGatherable->GetGatherPrompt();
        Snapshot.bCanInteract = TutorialGatherable->CanGatherWithPrototypePlayer(PrototypePlayer);
    }
    else if (const AHuwamSurvivalServiceActor* SurvivalService = Cast<AHuwamSurvivalServiceActor>(TargetActor))
    {
        Snapshot.Kind = EHuwamWorldInteractionKind::SurvivalService;
        Snapshot.PromptText = SurvivalService->GetServicePrompt();
        Snapshot.bCanInteract = SurvivalService->CanUseServiceWithPrototypePlayer(PrototypePlayer);
    }
    else
    {
        return FHuwamWorldInteractionSnapshot();
    }

    return Snapshot;
}

bool UHuwamWorldInteractionComponent::IsTargetInRange(const AActor* TargetActor) const
{
    const AActor* OriginActor = ResolveInteractionOriginActor();
    return OriginActor
        && TargetActor
        && FVector::Dist(OriginActor->GetActorLocation(), TargetActor->GetActorLocation()) <= FocusRadius;
}
