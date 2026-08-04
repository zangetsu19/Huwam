#include "Gameplay/HuwamPrototypeGameMode.h"

#include "Data/HuwamDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Gameplay/HuwamGatheringResourceActor.h"
#include "Gameplay/HuwamGuildContractBoardActor.h"
#include "Gameplay/HuwamCombatComponent.h"
#include "Gameplay/HuwamEconomyLedgerSubsystem.h"
#include "Gameplay/HuwamInventoryComponent.h"
#include "Gameplay/HuwamMonsterEncounterActor.h"
#include "Gameplay/HuwamNpcSupplyComponent.h"
#include "Gameplay/HuwamNpcSupplyQuestOfferActor.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"
#include "Gameplay/HuwamPrototypeSliceBlockoutActor.h"
#include "Gameplay/HuwamPrototypeThirdPersonCharacter.h"
#include "Gameplay/HuwamQuestComponent.h"
#include "Gameplay/HuwamRewardComponent.h"
#include "Gameplay/HuwamSurvivalServiceActor.h"
#include "Gameplay/HuwamSurvivalVitalsComponent.h"
#include "Gameplay/HuwamTimeSubsystem.h"
#include "Gameplay/HuwamTutorialLabCombatTargetActor.h"
#include "Gameplay/HuwamTutorialLabComponent.h"
#include "Gameplay/HuwamTutorialLabGatheringActor.h"
#include "Gameplay/HuwamTutorialLabInteractableActor.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "TimerManager.h"

namespace
{
    template <typename ActorType>
    ActorType* FindFirstActorOfType(UWorld* World)
    {
        if (!World)
        {
            return nullptr;
        }

        for (TActorIterator<ActorType> It(World); It; ++It)
        {
            return *It;
        }

        return nullptr;
    }

    const TCHAR* MainHandSlotId = TEXT("slot.weapon.main_hand");
    const TCHAR* GatheringKnifeId = TEXT("item.tool.gathering_knife");
}

AHuwamPrototypeGameMode::AHuwamPrototypeGameMode()
{
    DefaultPawnClass = AHuwamPrototypeThirdPersonCharacter::StaticClass();
    PrototypePlayerStateActorClass = AHuwamPrototypePlayerActor::StaticClass();
    PrototypeSliceBlockoutClass = AHuwamPrototypeSliceBlockoutActor::StaticClass();
}

void AHuwamPrototypeGameMode::StartPlay()
{
    Super::StartPlay();
    BootstrapFirstPlayablePrototype();

    if (FParse::Param(FCommandLine::Get(), TEXT("HuwamEditorValidationRun")))
    {
        BeginEditorValidationRun();
    }
}

bool AHuwamPrototypeGameMode::BootstrapFirstPlayablePrototype()
{
    bool bSpawnedStateActorForBootstrap = false;

    PrototypeCharacter = FindPrototypeCharacter();
    PrototypeSliceBlockout = FindPrototypeSliceBlockout();
    if (!PrototypeSliceBlockout && bSpawnPrototypeSliceBlockoutIfMissing)
    {
        PrototypeSliceBlockout = SpawnPrototypeSliceBlockout();
    }

    PrototypePlayerStateActor = FindPrototypePlayerStateActor();

    if (!PrototypePlayerStateActor && bSpawnPrototypePlayerStateActorIfMissing)
    {
        PrototypePlayerStateActor = SpawnPrototypePlayerStateActor(PrototypeCharacter);
        bSpawnedStateActorForBootstrap = PrototypePlayerStateActor != nullptr;
    }

    if (!PrototypePlayerStateActor)
    {
        return false;
    }

    if (bLinkExistingThirdPersonCharacter && PrototypeCharacter)
    {
        PrototypeCharacter->SetPrototypePlayerActor(PrototypePlayerStateActor);
    }

    if (bSpawnedStateActorForBootstrap && bConfigureSpawnedStateForCharacterCreation)
    {
        PrototypePlayerStateActor->ShowCharacterCreationScreen(GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr);
    }

    return true;
}

AHuwamPrototypePlayerActor* AHuwamPrototypeGameMode::GetPrototypePlayerStateActor() const
{
    return PrototypePlayerStateActor;
}

AHuwamPrototypeThirdPersonCharacter* AHuwamPrototypeGameMode::GetPrototypeCharacter() const
{
    return PrototypeCharacter;
}

AHuwamPrototypeSliceBlockoutActor* AHuwamPrototypeGameMode::GetPrototypeSliceBlockout() const
{
    return PrototypeSliceBlockout;
}

AHuwamPrototypePlayerActor* AHuwamPrototypeGameMode::FindPrototypePlayerStateActor() const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    for (TActorIterator<AHuwamPrototypePlayerActor> It(GetWorld()); It; ++It)
    {
        return *It;
    }

    return nullptr;
}

AHuwamPrototypeThirdPersonCharacter* AHuwamPrototypeGameMode::FindPrototypeCharacter() const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    if (APlayerController* FirstController = GetWorld()->GetFirstPlayerController())
    {
        if (AHuwamPrototypeThirdPersonCharacter* PossessedCharacter = Cast<AHuwamPrototypeThirdPersonCharacter>(FirstController->GetPawn()))
        {
            return PossessedCharacter;
        }
    }

    for (TActorIterator<AHuwamPrototypeThirdPersonCharacter> It(GetWorld()); It; ++It)
    {
        return *It;
    }

    return nullptr;
}

AHuwamPrototypeSliceBlockoutActor* AHuwamPrototypeGameMode::FindPrototypeSliceBlockout() const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    for (TActorIterator<AHuwamPrototypeSliceBlockoutActor> It(GetWorld()); It; ++It)
    {
        return *It;
    }

    return nullptr;
}

AHuwamPrototypePlayerActor* AHuwamPrototypeGameMode::SpawnPrototypePlayerStateActor(AHuwamPrototypeThirdPersonCharacter* SpawnAnchor)
{
    if (!GetWorld() || !PrototypePlayerStateActorClass)
    {
        return nullptr;
    }

    const FTransform SpawnTransform = SpawnAnchor ? SpawnAnchor->GetActorTransform() : FTransform::Identity;
    AHuwamPrototypePlayerActor* SpawnedStateActor = GetWorld()->SpawnActorDeferred<AHuwamPrototypePlayerActor>(
        PrototypePlayerStateActorClass,
        SpawnTransform,
        nullptr,
        nullptr,
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn
    );
    if (!SpawnedStateActor)
    {
        return nullptr;
    }

    if (bConfigureSpawnedStateForCharacterCreation)
    {
        SpawnedStateActor->ConfigureForThirdPersonBootstrap();
    }

    UGameplayStatics::FinishSpawningActor(SpawnedStateActor, SpawnTransform);
    return SpawnedStateActor;
}

AHuwamPrototypeSliceBlockoutActor* AHuwamPrototypeGameMode::SpawnPrototypeSliceBlockout()
{
    if (!GetWorld() || !PrototypeSliceBlockoutClass)
    {
        return nullptr;
    }

    return GetWorld()->SpawnActor<AHuwamPrototypeSliceBlockoutActor>(
        PrototypeSliceBlockoutClass,
        FTransform::Identity,
        FActorSpawnParameters()
    );
}

void AHuwamPrototypeGameMode::BeginEditorValidationRun()
{
    if (!GetWorld())
    {
        return;
    }

    EditorValidationStep = 0;
    ValidateRuntimeTimeBootstrap();
    ValidateRuntimeDataBootstrap();
    ValidateRuntimeInventorySpoilageBootstrap();
    ValidateRuntimeSurvivalVitalsBootstrap();
    UE_LOG(LogTemp, Display, TEXT("Huwam editor validation run started."));
    GetWorld()->GetTimerManager().SetTimer(
        EditorValidationTimerHandle,
        this,
        &AHuwamPrototypeGameMode::AdvanceEditorValidationRun,
        1.0f,
        true,
        0.75f
    );
}

void AHuwamPrototypeGameMode::AdvanceEditorValidationRun()
{
    if (!GetWorld())
    {
        return;
    }

    if (!PrototypePlayerStateActor)
    {
        BootstrapFirstPlayablePrototype();
    }

    bool bStepSucceeded = false;
    FString StepName;

    switch (EditorValidationStep)
    {
    case 0:
    {
        StepName = TEXT("confirm default character creation into Laucian tutorial");
        if (PrototypePlayerStateActor && PrototypePlayerStateActor->GetCharacterCreationComponent())
        {
            FHuwamCharacterCreationResult CreationResult;
            bStepSucceeded = PrototypePlayerStateActor->InitializePrototypePlayerFromCharacterCreation(
                PrototypePlayerStateActor->GetCharacterCreationComponent()->BuildDefaultCharacterCreationRequest(),
                CreationResult
            );
            PrototypePlayerStateActor->HideCharacterCreationScreen(false);
        }
        break;
    }
    case 1:
    {
        StepName = TEXT("advance movement lesson");
        UHuwamTutorialLabComponent* TutorialLab = PrototypePlayerStateActor ? PrototypePlayerStateActor->GetTutorialLabComponent() : nullptr;
        bStepSucceeded = TutorialLab && TutorialLab->MarkMovementLessonComplete();
        if (PrototypePlayerStateActor)
        {
            PrototypePlayerStateActor->RefreshPrototypeHud();
        }
        break;
    }
    case 2:
    {
        StepName = TEXT("acknowledge HUD lesson");
        bStepSucceeded = PrototypePlayerStateActor && PrototypePlayerStateActor->AcknowledgeTutorialHudLesson();
        break;
    }
    case 3:
    {
        StepName = TEXT("use tutorial interaction pedestal");
        AHuwamTutorialLabInteractableActor* TutorialObject = FindFirstActorOfType<AHuwamTutorialLabInteractableActor>(GetWorld());
        bStepSucceeded = TutorialObject && PrototypePlayerStateActor && TutorialObject->InteractWithPrototypePlayer(PrototypePlayerStateActor);
        if (PrototypePlayerStateActor)
        {
            PrototypePlayerStateActor->RefreshPrototypeHud();
        }
        break;
    }
    case 4:
    {
        StepName = TEXT("inspect gathering knife in inventory");
        bStepSucceeded = PrototypePlayerStateActor && PrototypePlayerStateActor->MarkTutorialInventoryItemInspected(GatheringKnifeId);
        break;
    }
    case 5:
    {
        StepName = TEXT("inspect equipped main-hand sword");
        bStepSucceeded = PrototypePlayerStateActor && PrototypePlayerStateActor->MarkTutorialEquipmentSlotInspected(MainHandSlotId);
        break;
    }
    case 6:
    {
        StepName = TEXT("strike tutorial combat focus");
        AHuwamTutorialLabCombatTargetActor* CombatTarget = FindFirstActorOfType<AHuwamTutorialLabCombatTargetActor>(GetWorld());
        FHuwamCombatResult CombatResult;
        bStepSucceeded = PrototypePlayerStateActor && CombatTarget && PrototypePlayerStateActor->AttackTutorialLabCombatTarget(CombatTarget, CombatResult);
        break;
    }
    case 7:
    {
        StepName = TEXT("open tutorial map lesson");
        bStepSucceeded = PrototypePlayerStateActor && PrototypePlayerStateActor->OpenTutorialMapLesson();
        break;
    }
    case 8:
    {
        StepName = TEXT("track tutorial quest lesson");
        bStepSucceeded = PrototypePlayerStateActor && PrototypePlayerStateActor->TrackTutorialQuestLesson();
        break;
    }
    case 9:
    {
        StepName = TEXT("gather tutorial lavender and complete tutorial");
        AHuwamTutorialLabGatheringActor* TutorialGathering = FindFirstActorOfType<AHuwamTutorialLabGatheringActor>(GetWorld());
        bStepSucceeded = TutorialGathering && PrototypePlayerStateActor && TutorialGathering->GatherWithPrototypePlayer(PrototypePlayerStateActor);
        break;
    }
    case 10:
    {
        StepName = TEXT("enter Eldoria starter fields");
        bStepSucceeded = PrototypePlayerStateActor && PrototypePlayerStateActor->EnterEldoriaStarterArea();
        break;
    }
    case 11:
    {
        StepName = TEXT("accept Adventurers Guild board contract");
        AHuwamGuildContractBoardActor* GuildBoard = FindFirstActorOfType<AHuwamGuildContractBoardActor>(GetWorld());
        FHuwamGuildContractBoardResult BoardResult;
        bStepSucceeded = GuildBoard && PrototypePlayerStateActor && GuildBoard->InteractWithPrototypePlayer(PrototypePlayerStateActor, BoardResult);
        break;
    }
    case 12:
    {
        StepName = TEXT("accept NPC field supply request");
        AHuwamNpcSupplyQuestOfferActor* SupplyRequester = FindFirstActorOfType<AHuwamNpcSupplyQuestOfferActor>(GetWorld());
        FHuwamNpcSupplyQuestInteractionResult InteractionResult;
        bStepSucceeded = SupplyRequester && PrototypePlayerStateActor && SupplyRequester->InteractByStateWithPrototypePlayer(PrototypePlayerStateActor, true, InteractionResult);
        break;
    }
    case 13:
    {
        StepName = TEXT("gather ten field supplies for NPC request");
        int32 GatheredCount = 0;
        AHuwamGatheringResourceActor* FirstResource = nullptr;
        for (TActorIterator<AHuwamGatheringResourceActor> It(GetWorld()); It && GatheredCount < 10; ++It)
        {
            AHuwamGatheringResourceActor* Resource = *It;
            if (!FirstResource)
            {
                FirstResource = Resource;
            }

            if (Resource && !Resource->IsAvailable())
            {
                Resource->RespawnResource();
            }

            if (Resource && PrototypePlayerStateActor && Resource->GatherWithPrototypePlayer(PrototypePlayerStateActor))
            {
                ++GatheredCount;
            }
        }

        while (FirstResource && PrototypePlayerStateActor && GatheredCount < 10)
        {
            FirstResource->RespawnResource();
            if (!FirstResource->GatherWithPrototypePlayer(PrototypePlayerStateActor))
            {
                break;
            }
            ++GatheredCount;
        }

        bStepSucceeded = GatheredCount >= 10;
        break;
    }
    case 14:
    {
        StepName = TEXT("turn in NPC field supply request");
        AHuwamNpcSupplyQuestOfferActor* SupplyRequester = FindFirstActorOfType<AHuwamNpcSupplyQuestOfferActor>(GetWorld());
        FHuwamNpcSupplyQuestInteractionResult InteractionResult;
        bStepSucceeded = SupplyRequester && PrototypePlayerStateActor && SupplyRequester->InteractByStateWithPrototypePlayer(PrototypePlayerStateActor, true, InteractionResult);
        break;
    }
    case 15:
    {
        StepName = TEXT("sell one surplus field supply to NPC stock");
        AHuwamNpcSupplyQuestOfferActor* SupplyRequester = FindFirstActorOfType<AHuwamNpcSupplyQuestOfferActor>(GetWorld());
        AHuwamGatheringResourceActor* Resource = FindFirstActorOfType<AHuwamGatheringResourceActor>(GetWorld());
        FHuwamNpcSupplyPurchaseResult PurchaseResult;
        if (Resource && PrototypePlayerStateActor)
        {
            Resource->RespawnResource();
            Resource->GatherWithPrototypePlayer(PrototypePlayerStateActor);
        }
        bStepSucceeded = SupplyRequester
            && PrototypePlayerStateActor
            && Resource
            && PrototypePlayerStateActor->SellAcceptedSupplyItemToNpc(SupplyRequester->GetNpcSupplyComponent(), Resource->GetGatheredItemId(), 1, PurchaseResult);
        break;
    }
    case 16:
    {
        StepName = TEXT("drain NPC routine supply and reoffer field request");
        AHuwamNpcSupplyQuestOfferActor* SupplyRequester = FindFirstActorOfType<AHuwamNpcSupplyQuestOfferActor>(GetWorld());
        UHuwamNpcSupplyComponent* SupplyComponent = SupplyRequester ? SupplyRequester->GetNpcSupplyComponent() : nullptr;
        int32 RoutineUses = 0;
        while (SupplyComponent && !SupplyComponent->ShouldRequestSupplyHelp() && RoutineUses < 20)
        {
            FHuwamNpcSupplyUseResult RoutineUseResult;
            if (!SupplyComponent->UseRoutineSupply(RoutineUseResult))
            {
                break;
            }
            ++RoutineUses;
        }

        FHuwamNpcSupplyQuestInteractionResult InteractionResult;
        bStepSucceeded = SupplyRequester
            && SupplyComponent
            && PrototypePlayerStateActor
            && RoutineUses > 0
            && SupplyComponent->ShouldRequestSupplyHelp()
            && SupplyRequester->InteractByStateWithPrototypePlayer(PrototypePlayerStateActor, true, InteractionResult)
            && InteractionResult.Outcome == EHuwamNpcSupplyQuestInteractionOutcome::OfferAccepted;
        break;
    }
    case 17:
    {
        StepName = TEXT("defeat visible Basic Slimes");
        int32 DefeatedCount = 0;
        for (TActorIterator<AHuwamMonsterEncounterActor> It(GetWorld()); It; ++It)
        {
            AHuwamMonsterEncounterActor* Monster = *It;
            int32 AttackCount = 0;
            while (Monster && !Monster->IsDefeated() && AttackCount < 20)
            {
                FHuwamCombatResult CombatResult;
                FHuwamRewardGrantResult RewardResult;
                PrototypePlayerStateActor->AttackMonsterAndClaimIfDefeated(Monster, CombatResult, RewardResult);
                ++AttackCount;
            }

            if (Monster && Monster->IsDefeated())
            {
                ++DefeatedCount;
            }
        }

        bStepSucceeded = DefeatedCount > 0;
        break;
    }
    case 18:
    {
        StepName = TEXT("turn in Adventurers Guild slime contract");
        AHuwamGuildContractBoardActor* GuildBoard = FindFirstActorOfType<AHuwamGuildContractBoardActor>(GetWorld());
        FHuwamGuildContractBoardResult BoardResult;
        bStepSucceeded = GuildBoard && PrototypePlayerStateActor && GuildBoard->InteractWithPrototypePlayer(PrototypePlayerStateActor, BoardResult);
        break;
    }
    default:
        UE_LOG(LogTemp, Display, TEXT("Huwam editor validation run complete."));
        GetWorld()->GetTimerManager().ClearTimer(EditorValidationTimerHandle);
        return;
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("Huwam editor validation step %d: %s -> %s"),
        EditorValidationStep,
        *StepName,
        bStepSucceeded ? TEXT("passed") : TEXT("failed")
    );

    ++EditorValidationStep;
}

bool AHuwamPrototypeGameMode::ValidateRuntimeDataBootstrap() const
{
    UGameInstance* GameInstance = GetGameInstance();
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Huwam data validation skipped: no game instance."));
        return false;
    }

    UHuwamDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UHuwamDataSubsystem>();
    if (!DataSubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Huwam data validation failed: no data subsystem."));
        return false;
    }

    DataSubsystem->EnsureDataRegistryLoaded();
    UHuwamDataRegistry* DataRegistry = DataSubsystem->GetDataRegistry();
    if (!DataRegistry)
    {
        UE_LOG(LogTemp, Warning, TEXT("Huwam data validation failed: no active data registry."));
        return false;
    }

    FHuwamRaceRow RaceRow;
    FHuwamContentPackRow ContentPackRow;
    FHuwamClassRow ClassRow;
    FHuwamItemRow ItemRow;
    FHuwamSpellRow SpellRow;
    FHuwamMonsterRow MonsterRow;
    FHuwamNpcRow NpcRow;
    FHuwamQuestRow QuestRow;

    const bool bCoreRowsLoaded =
        DataRegistry->GetRaceRow(FName(TEXT("race_human")), RaceRow)
        && DataRegistry->GetContentPackRow(FName(TEXT("content_pack_starter_core")), ContentPackRow)
        && DataRegistry->GetClassRow(FName(TEXT("class_progression_zero")), ClassRow)
        && DataRegistry->GetItemRow(FName(TEXT("item_weapon_basic_sword")), ItemRow)
        && DataRegistry->GetSpellRow(FName(TEXT("spell_fire_spark")), SpellRow)
        && DataRegistry->GetMonsterRow(FName(TEXT("monster_slime_basic")), MonsterRow)
        && DataRegistry->GetNpcRow(FName(TEXT("npc_eldoria_gorth_runkis")), NpcRow)
        && DataRegistry->GetQuestRow(FName(TEXT("quest_eldoria_slimes_in_tall_grass")), QuestRow);

    if (bCoreRowsLoaded)
    {
        UE_LOG(LogTemp, Display, TEXT("Huwam data validation passed with %d known rows."), DataRegistry->GetTotalKnownRowCount());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Huwam data validation failed with %d known rows."), DataRegistry->GetTotalKnownRowCount());
    }

    return bCoreRowsLoaded;
}

bool AHuwamPrototypeGameMode::ValidateRuntimeTimeBootstrap() const
{
    UGameInstance* GameInstance = GetGameInstance();
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Huwam time validation skipped: no game instance."));
        return false;
    }

    const UHuwamTimeSubsystem* TimeSubsystem = GameInstance->GetSubsystem<UHuwamTimeSubsystem>();
    if (!TimeSubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Huwam time validation failed: no time subsystem."));
        return false;
    }

    const FHuwamTimeSnapshot Snapshot = TimeSubsystem->GetCurrentTimeSnapshot();
    const bool bValidTimeScale =
        Snapshot.SecondsPerInGameWeek == 86400.0f
        && Snapshot.SecondsPerInGameDay > 0.0f
        && FMath::IsNearlyEqual(Snapshot.SecondsPerInGameDay, 86400.0f / 7.0f, 0.01f)
        && !Snapshot.bTimeSkippingAllowed;

    if (bValidTimeScale)
    {
        UE_LOG(
            LogTemp,
            Display,
            TEXT("Huwam time validation passed: %.2f real seconds per in-game day, time skipping disabled."),
            Snapshot.SecondsPerInGameDay
        );
    }
    else
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Huwam time validation failed: %.2f seconds/day, %.2f seconds/week, skip=%s."),
            Snapshot.SecondsPerInGameDay,
            Snapshot.SecondsPerInGameWeek,
            Snapshot.bTimeSkippingAllowed ? TEXT("true") : TEXT("false")
        );
    }

    return bValidTimeScale;
}

bool AHuwamPrototypeGameMode::ValidateRuntimeInventorySpoilageBootstrap()
{
    UHuwamInventoryComponent* Inventory = PrototypePlayerStateActor ? PrototypePlayerStateActor->GetInventoryComponent() : nullptr;
    if (!Inventory)
    {
        UE_LOG(LogTemp, Warning, TEXT("Huwam inventory spoilage validation skipped: no inventory component."));
        return false;
    }

    const bool bHerbStartsFresh =
        Inventory->PreviewItemFreshnessAfterInGameDays(TEXT("material.herb.lavender"), 0) == EHuwamInventoryFreshness::Fresh;
    const bool bHerbSpoils =
        Inventory->PreviewItemFreshnessAfterInGameDays(TEXT("material.herb.lavender"), 10) == EHuwamInventoryFreshness::Spoiled;
    const bool bWoodStaysStable =
        Inventory->PreviewItemFreshnessAfterInGameDays(TEXT("material.wood.basic"), 10) == EHuwamInventoryFreshness::Stable;
    const bool bPouchSlowsHerbSpoilage =
        Inventory->PreviewItemFreshnessAfterInGameDaysWithStorage(TEXT("material.herb.lavender"), 10, EHuwamInventoryStorageQuality::Pouch) == EHuwamInventoryFreshness::Stale;
    const bool bCooledStorageKeepsHerbsFresh =
        Inventory->PreviewItemFreshnessAfterInGameDaysWithStorage(TEXT("material.herb.lavender"), 10, EHuwamInventoryStorageQuality::Cooled) == EHuwamInventoryFreshness::Fresh;
    const bool bMagicalStorageKeepsHerbsFresh =
        Inventory->PreviewItemFreshnessAfterInGameDaysWithStorage(TEXT("material.herb.lavender"), 100, EHuwamInventoryStorageQuality::Magical) == EHuwamInventoryFreshness::Fresh;

    const TArray<FHuwamInventoryStack> SavedStacks = Inventory->GetInventoryStacks();
    Inventory->ClearInventory();

    int32 Remainder = 0;
    const bool bAddedLooseHerb = Inventory->AddItemById(TEXT("material.herb.lavender"), 1, Remainder) && Remainder == 0;
    const bool bAddedFoodPouch = Inventory->AddItemById(TEXT("item.container.food_pouch"), 1, Remainder) && Remainder == 0;
    const bool bAssignedHerbToPouch = Inventory->AssignFirstMatchingStackToStorage(TEXT("material.herb.lavender"), EHuwamInventoryStorageQuality::Pouch);
    const TArray<FHuwamInventoryStack> RoutedStacks = Inventory->GetInventoryStacks();
    const TArray<FHuwamInventoryStorageRoute> StorageRoutes = Inventory->GetStorageRoutes();

    bool bHerbRoutedToPouch = false;
    for (const FHuwamInventoryStack& Stack : RoutedStacks)
    {
        if (Stack.ItemId == TEXT("material.herb.lavender") && Stack.StorageQuality == EHuwamInventoryStorageQuality::Pouch && Stack.StorageRouteId == TEXT("storage.pouch"))
        {
            bHerbRoutedToPouch = true;
            break;
        }
    }

    bool bPouchRouteCapacityValid = false;
    for (const FHuwamInventoryStorageRoute& Route : StorageRoutes)
    {
        if (Route.StorageQuality == EHuwamInventoryStorageQuality::Pouch && Route.UsedStacks == 1 && Route.MaxStacks >= 8)
        {
            bPouchRouteCapacityValid = true;
            break;
        }
    }

    Inventory->RestoreInventoryStacksForPrototypeSave(SavedStacks);

    const bool bSpoilageRulesValid = bHerbStartsFresh
        && bHerbSpoils
        && bWoodStaysStable
        && bPouchSlowsHerbSpoilage
        && bCooledStorageKeepsHerbsFresh
        && bMagicalStorageKeepsHerbsFresh
        && bAddedLooseHerb
        && bAddedFoodPouch
        && bAssignedHerbToPouch
        && bHerbRoutedToPouch
        && bPouchRouteCapacityValid;

    if (bSpoilageRulesValid)
    {
        UE_LOG(LogTemp, Display, TEXT("Huwam inventory spoilage validation passed, including storage preservation and assignment."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Huwam inventory spoilage validation failed."));
    }

    return bSpoilageRulesValid;
}

bool AHuwamPrototypeGameMode::ValidateRuntimeSurvivalVitalsBootstrap()
{
    UHuwamInventoryComponent* Inventory = PrototypePlayerStateActor ? PrototypePlayerStateActor->GetInventoryComponent() : nullptr;
    UHuwamSurvivalVitalsComponent* SurvivalVitals = PrototypePlayerStateActor ? PrototypePlayerStateActor->GetSurvivalVitalsComponent() : nullptr;
    UHuwamCombatComponent* Combat = PrototypePlayerStateActor ? PrototypePlayerStateActor->GetCombatComponent() : nullptr;
    UHuwamRewardComponent* Rewards = PrototypePlayerStateActor ? PrototypePlayerStateActor->GetRewardComponent() : nullptr;
    UHuwamQuestComponent* Quests = PrototypePlayerStateActor ? PrototypePlayerStateActor->GetQuestComponent() : nullptr;
    UHuwamEconomyLedgerSubsystem* EconomyLedger = GetGameInstance() ? GetGameInstance()->GetSubsystem<UHuwamEconomyLedgerSubsystem>() : nullptr;
    if (!Inventory || !SurvivalVitals || !Combat || !Rewards || !Quests || !EconomyLedger)
    {
        UE_LOG(LogTemp, Warning, TEXT("Huwam survival vitals validation skipped: missing inventory, survival, combat, reward, quest, or economy ledger component."));
        return false;
    }

    const TArray<FHuwamInventoryStack> SavedStacks = Inventory->GetInventoryStacks();
    const FHuwamSurvivalVitalsSnapshot SavedSurvival = SurvivalVitals->GetSurvivalVitalsSnapshot();
    const int32 SavedHealth = Combat->GetCurrentHealth();
    const int32 SavedMana = Combat->GetCurrentMana();
    const int64 SavedCurrencyBalanceCopper = Rewards->GetCurrencyBalanceCopper();
    const int32 SavedTotalExperience = Rewards->GetTotalExperience();
    const TArray<FHuwamQuestProgressRecord> SavedQuestProgressRecords = Rewards->GetQuestProgressRecords();
    const TArray<FHuwamQuestRuntimeState> SavedQuestStates = Quests->GetQuestStates();
    const TArray<FString> SavedTurnedInQuestIds = Quests->GetTurnedInQuestIds();
    const FString SavedTrackedQuestId = Quests->GetTrackedQuestId();
    const TArray<FHuwamEconomyLedgerEntry> SavedLedgerEntries = EconomyLedger->GetLedgerEntries();
    const TArray<FHuwamEconomyAccountBalance> SavedAccountBalances = EconomyLedger->GetAccountBalances();

    SurvivalVitals->ResetSurvivalVitals();
    const FHuwamSurvivalVitalsSnapshot DrainedSnapshot = SurvivalVitals->ApplyInGameDaysElapsed(3);

    int32 Remainder = 0;
    const bool bAddedFood = Inventory->AddItemById(TEXT("material.herb.lavender"), 1, Remainder) && Remainder == 0;

    FHuwamSurvivalVitalsSnapshot AfterFoodSnapshot;
    const bool bConsumedFood = SurvivalVitals->ConsumeFoodFromInventory(Inventory, TEXT("material.herb.lavender"), AfterFoodSnapshot);
    const FHuwamSurvivalVitalsSnapshot AfterWaterSnapshot = SurvivalVitals->DrinkWater(25);
    const FHuwamSurvivalVitalsSnapshot AfterSocialSnapshot = SurvivalVitals->RestoreSocialEnergy(15);

    const bool bDaysDrainNeeds =
        DrainedSnapshot.Hunger == 64
        && DrainedSnapshot.Thirst == 40
        && DrainedSnapshot.SocialEnergy == 88;
    const bool bFoodRestoresHunger = bAddedFood && bConsumedFood && AfterFoodSnapshot.Hunger == 89;
    const bool bWaterRestoresThirst = AfterWaterSnapshot.Thirst == 65;
    const bool bSocialRestoresEnergy = AfterSocialSnapshot.SocialEnergy == 100;

    SurvivalVitals->ResetSurvivalVitals();
    const FHuwamSurvivalVitalsSnapshot BeforeServiceSnapshot = SurvivalVitals->ApplyInGameDaysElapsed(3);
    const FHuwamSurvivalVitalsSnapshot AfterTavernMealSnapshot = SurvivalVitals->UseSurvivalService(EHuwamSurvivalServiceType::TavernMeal);
    const FHuwamSurvivalVitalsSnapshot AfterWellWaterSnapshot = SurvivalVitals->UseSurvivalService(EHuwamSurvivalServiceType::WellWater);
    const FHuwamSurvivalVitalsSnapshot AfterCampfireChatSnapshot = SurvivalVitals->UseSurvivalService(EHuwamSurvivalServiceType::CampfireChat);
    const bool bServicesRestoreNeedsWithoutTimeSkip =
        BeforeServiceSnapshot.LastUpdatedInGameDay == AfterCampfireChatSnapshot.LastUpdatedInGameDay
        && AfterTavernMealSnapshot.Hunger == 100
        && AfterWellWaterSnapshot.Thirst == 80
        && AfterCampfireChatSnapshot.SocialEnergy == 100;

    bool bServiceActorWorks = false;
    bool bPaidServiceActorRulesWork = false;
    if (GetWorld())
    {
        AHuwamSurvivalServiceActor* ServiceActor = GetWorld()->SpawnActor<AHuwamSurvivalServiceActor>(AHuwamSurvivalServiceActor::StaticClass(), FTransform::Identity);
        if (ServiceActor)
        {
            SurvivalVitals->ResetSurvivalVitals();
            SurvivalVitals->ApplyInGameDaysElapsed(3);
            ServiceActor->SetServiceType(EHuwamSurvivalServiceType::WellWater);
            FHuwamSurvivalServiceResult ServiceResult;
            bServiceActorWorks = ServiceActor->UseServiceWithPrototypePlayer(PrototypePlayerStateActor, ServiceResult)
                && ServiceResult.SnapshotAfter.Thirst == 80;
            ServiceActor->Destroy();
        }

        AHuwamSurvivalServiceActor* PaidServiceActor = GetWorld()->SpawnActor<AHuwamSurvivalServiceActor>(AHuwamSurvivalServiceActor::StaticClass(), FTransform::Identity);
        if (PaidServiceActor)
        {
            Rewards->RestoreRewardStateForPrototypeSave(0, SavedTotalExperience, SavedQuestProgressRecords);
            EconomyLedger->ResetLedgerForPrototypeValidation();
            SurvivalVitals->ResetSurvivalVitals();
            SurvivalVitals->ApplyInGameDaysElapsed(3);
            PaidServiceActor->SetServiceType(EHuwamSurvivalServiceType::TavernMeal);

            FHuwamSurvivalServiceResult UnaffordableResult;
            const bool bUnaffordableServiceBlocked = !PaidServiceActor->UseServiceWithPrototypePlayer(PrototypePlayerStateActor, UnaffordableResult)
                && UnaffordableResult.CostCopper == 8
                && UnaffordableResult.CurrencyBefore == 0;

            Rewards->AddCopper(8);
            FHuwamSurvivalServiceResult PaidServiceResult;
            const bool bPaidServiceSucceeded = PaidServiceActor->UseServiceWithPrototypePlayer(PrototypePlayerStateActor, PaidServiceResult)
                && PaidServiceResult.bCharged
                && PaidServiceResult.CostCopper == 8
                && PaidServiceResult.CurrencyBefore == 8
                && PaidServiceResult.CurrencyAfter == 0
                && PaidServiceResult.bRevenueRecorded
                && PaidServiceResult.OwnerNpcId == TEXT("npc.eldoria.market_merchant_01")
                && PaidServiceResult.OwningFactionId == TEXT("guild.merchants")
                && PaidServiceResult.SettlementId == TEXT("town.eldoria")
                && PaidServiceResult.SnapshotAfter.Hunger == 100
                && EconomyLedger->GetRevenueCopperForNpc(TEXT("npc.eldoria.market_merchant_01")) == 8
                && EconomyLedger->GetRevenueCopperForFaction(TEXT("guild.merchants")) == 8
                && EconomyLedger->GetRevenueCopperForSettlement(TEXT("town.eldoria")) == 8
                && EconomyLedger->GetNpcWalletCopper(TEXT("npc.eldoria.market_merchant_01")) == 6
                && EconomyLedger->GetFactionFundCopper(TEXT("guild.merchants")) == 1
                && EconomyLedger->GetSettlementTreasuryCopper(TEXT("town.eldoria")) == 1;

            FHuwamSurvivalServiceUpkeepResult UpkeepResult;
            const bool bServiceUpkeepWorks = PaidServiceActor->PayPrototypeUpkeepFromLedger(UpkeepResult)
                && UpkeepResult.UpkeepCostCopper == 2
                && UpkeepResult.EconomyResult.PayerAccountType == EHuwamEconomyAccountType::NpcWallet
                && UpkeepResult.EconomyResult.PayerAccountId == TEXT("npc.eldoria.market_merchant_01")
                && UpkeepResult.EconomyResult.PaidCopper == 2
                && EconomyLedger->GetNpcWalletCopper(TEXT("npc.eldoria.market_merchant_01")) == 4
                && !PaidServiceActor->IsUnderfunded();

            PaidServiceActor->SetAccessPolicy(EHuwamSurvivalServiceAccessPolicy::Locked);
            Rewards->AddCopper(8);
            FHuwamSurvivalServiceResult LockedServiceResult;
            const bool bLockedServiceBlocked = !PaidServiceActor->UseServiceWithPrototypePlayer(PrototypePlayerStateActor, LockedServiceResult)
                && LockedServiceResult.AccessPolicy == EHuwamSurvivalServiceAccessPolicy::Locked
                && LockedServiceResult.CurrencyBefore == 8
                && LockedServiceResult.CurrencyAfter == 8
                && Rewards->GetCurrencyBalanceCopper() == 8
                && EconomyLedger->GetLedgerSnapshot().TransactionCount == 1
                && EconomyLedger->GetLedgerSnapshot().TotalRevenueCopper == 8;

            PaidServiceActor->SetUpkeepCostCopper(99);
            FHuwamSurvivalServiceUpkeepResult FailedUpkeepResult;
            FHuwamNpcSupplyNeedState FailedServicePressure;
            const bool bFailedUpkeepClosesService = !PaidServiceActor->PayPrototypeUpkeepFromLedger(FailedUpkeepResult)
                && FailedUpkeepResult.UpkeepCostCopper == 99
                && PaidServiceActor->IsUnderfunded()
                && !PaidServiceActor->CanUseServiceWithPrototypePlayer(PrototypePlayerStateActor)
                && EconomyLedger->GetNpcWalletCopper(TEXT("npc.eldoria.market_merchant_01")) == 4
                && EconomyLedger->GetFactionFundCopper(TEXT("guild.merchants")) == 1
                && EconomyLedger->GetSettlementTreasuryCopper(TEXT("town.eldoria")) == 1;

            FailedServicePressure = PaidServiceActor->GetServicePressureNeedState();
            const bool bUnderfundedServiceCreatesQuestPressure =
                FailedServicePressure.NpcId == TEXT("npc.eldoria.market_merchant_01")
                && FailedServicePressure.NeedId == TEXT("need.service.survival.service.tavern_meal.upkeep")
                && FailedServicePressure.Urgency == EHuwamNpcSupplyNeedUrgency::Critical
                && FailedServicePressure.NextAction == EHuwamNpcSupplyNeedNextAction::RequestPlayerHelp
                && FailedServicePressure.NeededQuantity == 99
                && FailedServicePressure.bShouldRequestSupplyHelp;

            FHuwamQuestOffer RestorationQuestOffer;
            FHuwamQuestRuntimeState RestorationQuestState;
            const bool bServiceRestorationQuestGenerationWorks =
                Quests->GetServiceRestorationQuestOfferFromPressure(FailedServicePressure, RestorationQuestOffer)
                && RestorationQuestOffer.QuestId == TEXT("quest.eldoria.service_restoration.need.service.survival.service.tavern_meal.upkeep")
                && RestorationQuestOffer.NeedRequest.NeedId == FailedServicePressure.NeedId
                && RestorationQuestOffer.Urgency == EHuwamQuestUrgency::Emergency
                && RestorationQuestOffer.PrimaryObjective.ProgressSourceId == FailedServicePressure.NeedId
                && RestorationQuestOffer.PrimaryObjective.TargetValue == 99
                && Quests->StartServiceRestorationQuestFromPressure(FailedServicePressure, Rewards)
                && Quests->GetQuestState(RestorationQuestOffer.QuestId, RestorationQuestState)
                && RestorationQuestState.bActive
                && RestorationQuestState.NeedRequest.NeededQuantityAtRequest == 99
                && Quests->GetTrackedQuestId() == RestorationQuestOffer.QuestId;

            int32 RestorationProgressAfter = 0;
            bool bRestorationObjectiveCompleted = false;
            FHuwamQuestRuntimeState TurnedInRestorationQuest;
            FHuwamSurvivalServiceRestorationResult RestorationResult;
            PaidServiceActor->SetAccessPolicy(EHuwamSurvivalServiceAccessPolicy::Public);
            const bool bServiceRestorationTurnInWorks =
                bServiceRestorationQuestGenerationWorks
                && Rewards->AddQuestProgress(
                    RestorationQuestOffer.QuestId,
                    RestorationQuestOffer.PrimaryObjective.ObjectiveId,
                    RestorationQuestOffer.PrimaryObjective.TargetValue,
                    RestorationQuestOffer.PrimaryObjective.TargetValue,
                    RestorationProgressAfter,
                    bRestorationObjectiveCompleted
                )
                && bRestorationObjectiveCompleted
                && Quests->RefreshQuestProgressFromRewards(RestorationQuestOffer.QuestId, Rewards)
                && Quests->TurnInQuest(RestorationQuestOffer.QuestId, Rewards, Inventory, TurnedInRestorationQuest)
                && PaidServiceActor->ApplyRestorationQuestTurnIn(TurnedInRestorationQuest, RestorationResult)
                && RestorationResult.bSuccess
                && !PaidServiceActor->IsUnderfunded()
                && PaidServiceActor->CanUseServiceWithPrototypePlayer(PrototypePlayerStateActor);

            const FString EconomyValidationSlot = TEXT("HuwamValidationEconomySlot");
            UGameplayStatics::DeleteGameInSlot(EconomyValidationSlot, 0);
            PaidServiceActor->SetServiceInstanceId(TEXT("service.validation.tavern_meal.01"));
            PaidServiceActor->SetOwnership(TEXT("npc.validation.service_owner"), TEXT("guild.validation"));
            PaidServiceActor->SetSettlementId(TEXT("town.validation"));
            const bool bSavedEconomyState = bServiceRestorationTurnInWorks
                && PrototypePlayerStateActor->SavePrototypeProgress(EconomyValidationSlot, 0);
            EconomyLedger->ResetLedgerForPrototypeValidation();
            PaidServiceActor->SetUpkeepCostCopper(999);
            FHuwamSurvivalServiceUpkeepResult SaveRestoreMutationResult;
            PaidServiceActor->PayPrototypeUpkeepFromLedger(SaveRestoreMutationResult);
            const bool bMutatedServiceAfterSave = EconomyLedger->GetLedgerSnapshot().TransactionCount == 0
                && PaidServiceActor->IsUnderfunded();
            const bool bLoadedEconomyState = bSavedEconomyState && PrototypePlayerStateActor->LoadPrototypeProgress(EconomyValidationSlot, 0);
            const bool bEconomySaveRestoreWorks =
                bLoadedEconomyState
                && EconomyLedger->GetLedgerSnapshot().TransactionCount == 1
                && EconomyLedger->GetLedgerSnapshot().TotalRevenueCopper == 8
                && EconomyLedger->GetNpcWalletCopper(TEXT("npc.eldoria.market_merchant_01")) == 4
                && EconomyLedger->GetFactionFundCopper(TEXT("guild.merchants")) == 1
                && EconomyLedger->GetSettlementTreasuryCopper(TEXT("town.eldoria")) == 1
                && bMutatedServiceAfterSave
                && PaidServiceActor->GetServiceIdentityId() == TEXT("service.validation.tavern_meal.01")
                && !PaidServiceActor->IsUnderfunded()
                && PaidServiceActor->CanUseServiceWithPrototypePlayer(PrototypePlayerStateActor);
            UGameplayStatics::DeleteGameInSlot(EconomyValidationSlot, 0);

            const FHuwamHudSnapshot HudEconomySnapshot = PrototypePlayerStateActor->RefreshPrototypeHud();
            const bool bHudEconomySnapshotWorks =
                HudEconomySnapshot.bHasEconomyLedger
                && HudEconomySnapshot.Economy.TransactionCount == 1
                && HudEconomySnapshot.Economy.TotalRevenueCopper == 8
                && HudEconomySnapshot.Economy.AccountBalances.Num() >= 3;

            bPaidServiceActorRulesWork = bUnaffordableServiceBlocked
                && bPaidServiceSucceeded
                && bServiceUpkeepWorks
                && bLockedServiceBlocked
                && bFailedUpkeepClosesService
                && bUnderfundedServiceCreatesQuestPressure
                && bServiceRestorationQuestGenerationWorks
                && bServiceRestorationTurnInWorks
                && bEconomySaveRestoreWorks
                && bHudEconomySnapshotWorks;
            PaidServiceActor->Destroy();
        }
    }

    Combat->RestoreMetersForPrototypeSave(Combat->GetMaximumHealth(), Combat->GetCurrentMana());
    SurvivalVitals->ResetSurvivalVitals();
    const FHuwamSurvivalVitalsSnapshot ConsequenceReadySnapshot = SurvivalVitals->ApplyInGameDaysElapsed(9);
    FHuwamCombatResult SurvivalDamageResult;
    const bool bAppliedConsequenceDamage = SurvivalVitals->ApplySurvivalConsequences(Combat, SurvivalDamageResult);
    const bool bSurvivalEffectsValid =
        ConsequenceReadySnapshot.ActiveEffectIds.Contains(TEXT("effect.survival.starving"))
        && ConsequenceReadySnapshot.ActiveEffectIds.Contains(TEXT("effect.survival.dehydrated"));
    const bool bConsequenceDamageValid =
        bAppliedConsequenceDamage
        && SurvivalDamageResult.FinalDamage == 15
        && SurvivalDamageResult.SourceTag == TEXT("survival.consequence");

    const bool bSurvivalRulesValid = bDaysDrainNeeds
        && bFoodRestoresHunger
        && bWaterRestoresThirst
        && bSocialRestoresEnergy
        && bServicesRestoreNeedsWithoutTimeSkip
        && bServiceActorWorks
        && bPaidServiceActorRulesWork
        && bSurvivalEffectsValid
        && bConsequenceDamageValid;

    Inventory->RestoreInventoryStacksForPrototypeSave(SavedStacks);
    SurvivalVitals->RestoreSurvivalVitalsSnapshot(SavedSurvival);
    Combat->RestoreMetersForPrototypeSave(SavedHealth, SavedMana);
    Quests->RestoreQuestStateForPrototypeSave(SavedQuestStates, SavedTurnedInQuestIds, SavedTrackedQuestId);
    Rewards->RestoreRewardStateForPrototypeSave(SavedCurrencyBalanceCopper, SavedTotalExperience, SavedQuestProgressRecords);
    EconomyLedger->RestoreLedgerEntriesForPrototypeValidation(SavedLedgerEntries);
    EconomyLedger->RestoreAccountBalancesForPrototypeValidation(SavedAccountBalances);

    if (bSurvivalRulesValid)
    {
        UE_LOG(LogTemp, Display, TEXT("Huwam survival vitals validation passed."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Huwam survival vitals validation failed."));
    }

    return bSurvivalRulesValid;
}
