#include "Gameplay/HuwamSurvivalServiceActor.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/GameInstance.h"
#include "Gameplay/HuwamCharacterCreationComponent.h"
#include "Gameplay/HuwamEconomyLedgerSubsystem.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"
#include "Gameplay/HuwamRewardComponent.h"
#include "Gameplay/HuwamTimeSubsystem.h"
#include "UObject/ConstructorHelpers.h"

AHuwamSurvivalServiceActor::AHuwamSurvivalServiceActor()
{
    PrimaryActorTick.bCanEverTick = false;

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
    SetRootComponent(VisualMesh);
    VisualMesh->SetRelativeScale3D(FVector(1.2f, 1.2f, 0.35f));
    VisualMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        VisualMesh->SetStaticMesh(CubeMesh.Object);
    }

    ApplyDefaultEconomyForServiceType();
}

void AHuwamSurvivalServiceActor::SetServiceType(EHuwamSurvivalServiceType InServiceType)
{
    ServiceType = InServiceType;
    ApplyDefaultEconomyForServiceType();
}

EHuwamSurvivalServiceType AHuwamSurvivalServiceActor::GetServiceType() const
{
    return ServiceType;
}

FString AHuwamSurvivalServiceActor::GetServiceIdentityId() const
{
    if (!ServiceInstanceId.IsEmpty())
    {
        return ServiceInstanceId;
    }

    return FString::Printf(
        TEXT("service.%s.%s.%s.%s"),
        *SettlementId,
        *OwnerNpcId,
        *OwningFactionId,
        *BuildServiceRevenueSourceTag()
    );
}

FString AHuwamSurvivalServiceActor::GetServiceInstanceId() const
{
    return ServiceInstanceId;
}

void AHuwamSurvivalServiceActor::SetServiceInstanceId(const FString& InServiceInstanceId)
{
    ServiceInstanceId = InServiceInstanceId;
}

int64 AHuwamSurvivalServiceActor::GetCostCopper() const
{
    return CostCopper;
}

void AHuwamSurvivalServiceActor::SetCostCopper(int64 InCostCopper)
{
    CostCopper = FMath::Max<int64>(0, InCostCopper);
}

int64 AHuwamSurvivalServiceActor::GetUpkeepCostCopper() const
{
    return UpkeepCostCopper;
}

void AHuwamSurvivalServiceActor::SetUpkeepCostCopper(int64 InUpkeepCostCopper)
{
    UpkeepCostCopper = FMath::Max<int64>(0, InUpkeepCostCopper);
}

bool AHuwamSurvivalServiceActor::IsUnderfunded() const
{
    return bUnderfunded;
}

FString AHuwamSurvivalServiceActor::GetOwnerNpcId() const
{
    return OwnerNpcId;
}

FString AHuwamSurvivalServiceActor::GetOwningFactionId() const
{
    return OwningFactionId;
}

void AHuwamSurvivalServiceActor::SetOwnership(const FString& InOwnerNpcId, const FString& InOwningFactionId)
{
    OwnerNpcId = InOwnerNpcId;
    OwningFactionId = InOwningFactionId;
}

FString AHuwamSurvivalServiceActor::GetSettlementId() const
{
    return SettlementId;
}

void AHuwamSurvivalServiceActor::SetSettlementId(const FString& InSettlementId)
{
    SettlementId = InSettlementId;
}

EHuwamSurvivalServiceAccessPolicy AHuwamSurvivalServiceActor::GetAccessPolicy() const
{
    return AccessPolicy;
}

void AHuwamSurvivalServiceActor::SetAccessPolicy(EHuwamSurvivalServiceAccessPolicy InAccessPolicy)
{
    AccessPolicy = InAccessPolicy;
}

FText AHuwamSurvivalServiceActor::GetServicePrompt() const
{
    const FText BasePrompt = BuildBasePrompt();
    if (CostCopper <= 0)
    {
        return BasePrompt;
    }

    return FText::Format(
        NSLOCTEXT("HuwamSurvivalService", "PaidServicePrompt", "{0} ({1} copper)"),
        BasePrompt,
        FText::AsNumber(CostCopper)
    );
}

FText AHuwamSurvivalServiceActor::BuildBasePrompt() const
{
    switch (ServiceType)
    {
    case EHuwamSurvivalServiceType::WellWater:
        return NSLOCTEXT("HuwamSurvivalService", "WellWaterPrompt", "Drink from the town well");
    case EHuwamSurvivalServiceType::CampfireChat:
        return NSLOCTEXT("HuwamSurvivalService", "CampfireChatPrompt", "Rest by the campfire");
    case EHuwamSurvivalServiceType::InnRoom:
        return NSLOCTEXT("HuwamSurvivalService", "InnRoomPrompt", "Use the inn room");
    case EHuwamSurvivalServiceType::TavernMeal:
    default:
        return NSLOCTEXT("HuwamSurvivalService", "TavernMealPrompt", "Eat a tavern meal");
    }
}

bool AHuwamSurvivalServiceActor::CanUseServiceWithPrototypePlayer(const AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    return bServiceEnabled
        && PrototypePlayer
        && PrototypePlayer->GetSurvivalVitalsComponent()
        && !bUnderfunded
        && DoesPrototypePlayerHaveAccess(PrototypePlayer)
        && CanPrototypePlayerAffordService(PrototypePlayer);
}

bool AHuwamSurvivalServiceActor::UseServiceWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, FHuwamSurvivalServiceResult& OutResult)
{
    OutResult = FHuwamSurvivalServiceResult();
    OutResult.ServiceType = ServiceType;
    OutResult.AccessPolicy = AccessPolicy;
    OutResult.OwnerNpcId = OwnerNpcId;
    OutResult.OwningFactionId = OwningFactionId;
    OutResult.SettlementId = SettlementId;
    OutResult.CostCopper = CostCopper;

    UHuwamRewardComponent* Rewards = PrototypePlayer ? PrototypePlayer->GetRewardComponent() : nullptr;
    OutResult.CurrencyBefore = Rewards ? Rewards->GetCurrencyBalanceCopper() : 0;
    OutResult.CurrencyAfter = OutResult.CurrencyBefore;

    if (!CanUseServiceWithPrototypePlayer(PrototypePlayer))
    {
        OutResult.Message = BuildFailureMessage(PrototypePlayer);
        return false;
    }

    UHuwamSurvivalVitalsComponent* SurvivalVitals = PrototypePlayer->GetSurvivalVitalsComponent();
    if (CostCopper > 0)
    {
        if (!Rewards || !Rewards->SpendCopper(CostCopper))
        {
            OutResult.Message = NSLOCTEXT("HuwamSurvivalService", "ServicePaymentFailed", "Payment failed.");
            OutResult.CurrencyAfter = Rewards ? Rewards->GetCurrencyBalanceCopper() : OutResult.CurrencyBefore;
            return false;
        }

        OutResult.bCharged = true;
    }

    OutResult.CurrencyAfter = Rewards ? Rewards->GetCurrencyBalanceCopper() : OutResult.CurrencyBefore;
    if (OutResult.bCharged)
    {
        UGameInstance* GameInstance = GetGameInstance();
        UHuwamEconomyLedgerSubsystem* EconomyLedger = GameInstance ? GameInstance->GetSubsystem<UHuwamEconomyLedgerSubsystem>() : nullptr;
        const UHuwamTimeSubsystem* TimeSubsystem = GameInstance ? GameInstance->GetSubsystem<UHuwamTimeSubsystem>() : nullptr;
        const int32 CurrentInGameDay = TimeSubsystem ? TimeSubsystem->GetCurrentTimeSnapshot().TotalInGameDays : 0;
        if (EconomyLedger)
        {
            const FHuwamEconomyLedgerEntry RevenueEntry = EconomyLedger->RecordServiceRevenue(
                BuildServiceRevenueSourceTag(),
                OwnerNpcId,
                OwningFactionId,
                SettlementId,
                CostCopper,
                CurrentInGameDay,
                BuildBasePrompt()
            );
            OutResult.bRevenueRecorded = RevenueEntry.bRecorded;
        }
    }

    OutResult.SnapshotBefore = SurvivalVitals->RefreshSurvivalVitals();
    OutResult.SnapshotAfter = SurvivalVitals->UseSurvivalService(ServiceType);
    OutResult.Message = OutResult.SnapshotAfter.LastSurvivalMessage;
    OutResult.bSuccess = true;
    PrototypePlayer->RefreshPrototypeHud();
    return true;
}

bool AHuwamSurvivalServiceActor::PayPrototypeUpkeepFromLedger(FHuwamSurvivalServiceUpkeepResult& OutResult)
{
    OutResult = FHuwamSurvivalServiceUpkeepResult();
    OutResult.ServiceType = ServiceType;
    OutResult.UpkeepCostCopper = UpkeepCostCopper;

    UGameInstance* GameInstance = GetGameInstance();
    UHuwamEconomyLedgerSubsystem* EconomyLedger = GameInstance ? GameInstance->GetSubsystem<UHuwamEconomyLedgerSubsystem>() : nullptr;
    if (!EconomyLedger)
    {
        bUnderfunded = true;
        if (bDisableWhenUpkeepUnpaid)
        {
            bServiceEnabled = false;
        }

        OutResult.bServiceEnabledAfter = bServiceEnabled;
        OutResult.bUnderfundedAfter = bUnderfunded;
        OutResult.Message = NSLOCTEXT("HuwamSurvivalService", "UpkeepLedgerMissing", "No economy ledger is available for upkeep.");
        return false;
    }

    OutResult.EconomyResult = EconomyLedger->PayServiceUpkeep(OwnerNpcId, OwningFactionId, SettlementId, UpkeepCostCopper);
    if (OutResult.EconomyResult.bSuccess)
    {
        bUnderfunded = false;
        bServiceEnabled = true;
        OutResult.bSuccess = true;
        OutResult.Message = NSLOCTEXT("HuwamSurvivalService", "UpkeepPaid", "Service upkeep paid.");
    }
    else
    {
        bUnderfunded = true;
        if (bDisableWhenUpkeepUnpaid)
        {
            bServiceEnabled = false;
        }
        OutResult.Message = NSLOCTEXT("HuwamSurvivalService", "UpkeepFailed", "Service upkeep could not be paid.");
    }

    OutResult.bServiceEnabledAfter = bServiceEnabled;
    OutResult.bUnderfundedAfter = bUnderfunded;
    return OutResult.bSuccess;
}

FHuwamNpcSupplyNeedState AHuwamSurvivalServiceActor::GetServicePressureNeedState() const
{
    FHuwamNpcSupplyNeedState NeedState;
    NeedState.NpcId = OwnerNpcId;
    NeedState.NeedId = FString::Printf(TEXT("need.service.%s.upkeep"), *BuildServiceRevenueSourceTag());
    NeedState.DisplayName = NSLOCTEXT("HuwamSurvivalService", "ServiceUpkeepNeedName", "Service Upkeep");
    NeedState.TotalStockQuantity = bUnderfunded ? 0 : UpkeepCostCopper;
    NeedState.LowStockThreshold = UpkeepCostCopper;
    NeedState.TargetStockQuantity = UpkeepCostCopper;
    NeedState.NeededQuantity = bUnderfunded ? static_cast<int32>(FMath::Min<int64>(MAX_int32, UpkeepCostCopper)) : 0;
    NeedState.bNeedSatisfied = !bUnderfunded;
    NeedState.bShouldRequestSupplyHelp = bUnderfunded;

    if (bUnderfunded && !bServiceEnabled)
    {
        NeedState.Reason = NSLOCTEXT("HuwamSurvivalService", "ServiceClosedPressureReason", "This service is closed because upkeep could not be paid.");
        NeedState.Urgency = EHuwamNpcSupplyNeedUrgency::Critical;
        NeedState.NextAction = EHuwamNpcSupplyNeedNextAction::RequestPlayerHelp;
    }
    else if (bUnderfunded)
    {
        NeedState.Reason = NSLOCTEXT("HuwamSurvivalService", "ServiceUnderfundedPressureReason", "This service is underfunded and needs money, materials, or repair support.");
        NeedState.Urgency = EHuwamNpcSupplyNeedUrgency::Urgent;
        NeedState.NextAction = EHuwamNpcSupplyNeedNextAction::RequestPlayerHelp;
    }
    else
    {
        NeedState.Reason = NSLOCTEXT("HuwamSurvivalService", "ServiceStablePressureReason", "This service has enough upkeep support for now.");
        NeedState.Urgency = EHuwamNpcSupplyNeedUrgency::Stable;
        NeedState.NextAction = EHuwamNpcSupplyNeedNextAction::ContinueRoutine;
    }

    return NeedState;
}

bool AHuwamSurvivalServiceActor::ApplyRestorationQuestTurnIn(const FHuwamQuestRuntimeState& TurnedInQuest, FHuwamSurvivalServiceRestorationResult& OutResult)
{
    OutResult = FHuwamSurvivalServiceRestorationResult();
    OutResult.ServiceType = ServiceType;
    OutResult.QuestId = TurnedInQuest.QuestId;
    OutResult.NeedId = TurnedInQuest.NeedRequest.NeedId;

    const FString ExpectedNeedId = FString::Printf(TEXT("need.service.%s.upkeep"), *BuildServiceRevenueSourceTag());
    const bool bMatchesThisService = TurnedInQuest.bTurnedIn
        && TurnedInQuest.NeedRequest.NeedId == ExpectedNeedId
        && TurnedInQuest.PrimaryObjective.bCompleted;
    if (!bMatchesThisService)
    {
        OutResult.bServiceEnabledAfter = bServiceEnabled;
        OutResult.bUnderfundedAfter = bUnderfunded;
        OutResult.Message = NSLOCTEXT("HuwamSurvivalService", "RestorationQuestMismatch", "The completed quest does not restore this service.");
        return false;
    }

    bUnderfunded = false;
    bServiceEnabled = true;

    OutResult.bSuccess = true;
    OutResult.bServiceEnabledAfter = bServiceEnabled;
    OutResult.bUnderfundedAfter = bUnderfunded;
    OutResult.Message = NSLOCTEXT("HuwamSurvivalService", "RestorationQuestApplied", "Service restored.");
    return true;
}

FHuwamSurvivalServiceSaveState AHuwamSurvivalServiceActor::GetServiceSaveState() const
{
    FHuwamSurvivalServiceSaveState State;
    State.ServiceIdentityId = GetServiceIdentityId();
    State.ServiceInstanceId = ServiceInstanceId;
    State.ServiceType = ServiceType;
    State.AccessPolicy = AccessPolicy;
    State.OwnerNpcId = OwnerNpcId;
    State.OwningFactionId = OwningFactionId;
    State.SettlementId = SettlementId;
    State.CostCopper = CostCopper;
    State.UpkeepCostCopper = UpkeepCostCopper;
    State.bServiceEnabled = bServiceEnabled;
    State.bUnderfunded = bUnderfunded;
    return State;
}

bool AHuwamSurvivalServiceActor::RestoreServiceStateForPrototypeSave(const FHuwamSurvivalServiceSaveState& SavedState)
{
    if (!SavedState.ServiceIdentityId.IsEmpty() && SavedState.ServiceIdentityId != GetServiceIdentityId())
    {
        return false;
    }

    ServiceType = SavedState.ServiceType;
    ServiceInstanceId = SavedState.ServiceInstanceId;
    AccessPolicy = SavedState.AccessPolicy;
    OwnerNpcId = SavedState.OwnerNpcId;
    OwningFactionId = SavedState.OwningFactionId;
    SettlementId = SavedState.SettlementId;
    CostCopper = FMath::Max<int64>(0, SavedState.CostCopper);
    UpkeepCostCopper = FMath::Max<int64>(0, SavedState.UpkeepCostCopper);
    bServiceEnabled = SavedState.bServiceEnabled;
    bUnderfunded = SavedState.bUnderfunded;
    return true;
}

void AHuwamSurvivalServiceActor::ApplyDefaultEconomyForServiceType()
{
    AccessPolicy = EHuwamSurvivalServiceAccessPolicy::Public;
    SettlementId = TEXT("town.eldoria");
    bUnderfunded = false;

    switch (ServiceType)
    {
    case EHuwamSurvivalServiceType::WellWater:
        CostCopper = 0;
        UpkeepCostCopper = 1;
        OwnerNpcId = TEXT("npc.eldoria.guard_01");
        OwningFactionId = TEXT("guild.knights");
        break;
    case EHuwamSurvivalServiceType::CampfireChat:
        CostCopper = 0;
        UpkeepCostCopper = 1;
        OwnerNpcId = TEXT("npc.eldoria.robin_quickfoot");
        OwningFactionId = TEXT("guild.archers");
        break;
    case EHuwamSurvivalServiceType::InnRoom:
        CostCopper = 20;
        UpkeepCostCopper = 4;
        OwnerNpcId = TEXT("npc.eldoria.market_merchant_01");
        OwningFactionId = TEXT("guild.merchants");
        break;
    case EHuwamSurvivalServiceType::TavernMeal:
    default:
        CostCopper = 8;
        UpkeepCostCopper = 2;
        OwnerNpcId = TEXT("npc.eldoria.market_merchant_01");
        OwningFactionId = TEXT("guild.merchants");
        break;
    }
}

bool AHuwamSurvivalServiceActor::DoesPrototypePlayerHaveAccess(const AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    if (!PrototypePlayer)
    {
        return false;
    }

    switch (AccessPolicy)
    {
    case EHuwamSurvivalServiceAccessPolicy::Public:
        return true;
    case EHuwamSurvivalServiceAccessPolicy::GuildOrFactionMember:
        return OwningFactionId.IsEmpty() || DoesCreatedCharacterMatchOwningFaction(PrototypePlayer);
    case EHuwamSurvivalServiceAccessPolicy::OwnerOnly:
        return OwnerNpcId.IsEmpty();
    case EHuwamSurvivalServiceAccessPolicy::Locked:
    default:
        return false;
    }
}

bool AHuwamSurvivalServiceActor::CanPrototypePlayerAffordService(const AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    if (CostCopper <= 0)
    {
        return true;
    }

    const UHuwamRewardComponent* Rewards = PrototypePlayer ? PrototypePlayer->GetRewardComponent() : nullptr;
    return Rewards && Rewards->GetCurrencyBalanceCopper() >= CostCopper;
}

FText AHuwamSurvivalServiceActor::BuildFailureMessage(const AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    if (!bServiceEnabled)
    {
        return NSLOCTEXT("HuwamSurvivalService", "ServiceClosed", "Service closed.");
    }

    if (!PrototypePlayer || !PrototypePlayer->GetSurvivalVitalsComponent())
    {
        return NSLOCTEXT("HuwamSurvivalService", "ServiceUnavailable", "Service unavailable.");
    }

    if (bUnderfunded)
    {
        return NSLOCTEXT("HuwamSurvivalService", "ServiceUnderfunded", "Service underfunded.");
    }

    if (!DoesPrototypePlayerHaveAccess(PrototypePlayer))
    {
        return NSLOCTEXT("HuwamSurvivalService", "ServiceAccessDenied", "You do not have access to this service.");
    }

    if (!CanPrototypePlayerAffordService(PrototypePlayer))
    {
        return NSLOCTEXT("HuwamSurvivalService", "ServiceInsufficientFunds", "You need more coin for this service.");
    }

    return NSLOCTEXT("HuwamSurvivalService", "ServiceUnavailableFallback", "Service unavailable.");
}

bool AHuwamSurvivalServiceActor::DoesCreatedCharacterMatchOwningFaction(const AHuwamPrototypePlayerActor* PrototypePlayer) const
{
    const UHuwamCharacterCreationComponent* CharacterCreation = PrototypePlayer ? PrototypePlayer->GetCharacterCreationComponent() : nullptr;
    if (!CharacterCreation || !CharacterCreation->HasCreatedCharacter())
    {
        return false;
    }

    const FHuwamCreatedCharacterState CharacterState = CharacterCreation->GetCreatedCharacterState();
    for (const FHuwamCreatedClassState& ClassState : CharacterState.Classes)
    {
        if (ClassState.GuildId == OwningFactionId)
        {
            return true;
        }
    }

    for (const FHuwamCreatedJobState& JobState : CharacterState.Jobs)
    {
        if (JobState.AssociatedGuildId == OwningFactionId)
        {
            return true;
        }
    }

    return false;
}

FString AHuwamSurvivalServiceActor::BuildServiceRevenueSourceTag() const
{
    switch (ServiceType)
    {
    case EHuwamSurvivalServiceType::WellWater:
        return TEXT("survival.service.well_water");
    case EHuwamSurvivalServiceType::CampfireChat:
        return TEXT("survival.service.campfire_chat");
    case EHuwamSurvivalServiceType::InnRoom:
        return TEXT("survival.service.inn_room");
    case EHuwamSurvivalServiceType::TavernMeal:
    default:
        return TEXT("survival.service.tavern_meal");
    }
}
