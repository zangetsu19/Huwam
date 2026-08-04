#include "Gameplay/HuwamPrototypePlayerActor.h"

#include "Blueprint/UserWidget.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "Gameplay/HuwamCharacterCreationMenuDataComponent.h"
#include "Gameplay/HuwamCharacterStatsComponent.h"
#include "Gameplay/HuwamEldoriaStarterEntryActor.h"
#include "Gameplay/HuwamEquipmentComponent.h"
#include "Gameplay/HuwamEconomyLedgerSubsystem.h"
#include "Gameplay/HuwamInventoryComponent.h"
#include "Gameplay/HuwamLiveContentComponent.h"
#include "Gameplay/HuwamMapAwarenessComponent.h"
#include "Gameplay/HuwamMonsterEncounterActor.h"
#include "Gameplay/HuwamPrototypeSaveGame.h"
#include "Gameplay/HuwamSurvivalServiceActor.h"
#include "Gameplay/HuwamSurvivalVitalsComponent.h"
#include "Gameplay/HuwamTutorialLabComponent.h"
#include "Gameplay/HuwamTutorialLabCombatTargetActor.h"
#include "UI/HuwamCharacterCreationScreenWidget.h"
#include "UI/HuwamNpcSupplyQuestInteractionWidget.h"
#include "UI/HuwamPrototypeHudWidget.h"
#include "UI/HuwamPrototypeMenuWidget.h"
#include "UI/HuwamTutorialLabPromptWidget.h"
#include "UI/HuwamWorldInteractionPromptWidget.h"
#include "Kismet/GameplayStatics.h"

namespace
{
    const TCHAR* BasicSwordId = TEXT("item.weapon.basic_sword");
    const TCHAR* BasicKnifeId = TEXT("item.weapon.basic_knife");
    const TCHAR* AdventuringBackpackId = TEXT("item.bag.adventuring_backpack");
    const TCHAR* GatheringKnifeId = TEXT("item.tool.gathering_knife");
    const TCHAR* SmallPouchId = TEXT("item.container.small_pouch");
    const TCHAR* SlimeQuestId = TEXT("quest.eldoria.slimes_in_tall_grass");
    const TCHAR* SlimeObjectiveId = TEXT("objective.defeat_basic_slime");
    const TCHAR* GatherFieldSuppliesQuestId = TEXT("quest.eldoria.gather_field_supplies");
    const TCHAR* GatherFieldSuppliesRequesterNpcId = TEXT("npc.eldoria.poor_resident_01");
}

AHuwamPrototypePlayerActor::AHuwamPrototypePlayerActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    Inventory = CreateDefaultSubobject<UHuwamInventoryComponent>(TEXT("Inventory"));
    SurvivalVitals = CreateDefaultSubobject<UHuwamSurvivalVitalsComponent>(TEXT("SurvivalVitals"));
    Equipment = CreateDefaultSubobject<UHuwamEquipmentComponent>(TEXT("Equipment"));
    CharacterStats = CreateDefaultSubobject<UHuwamCharacterStatsComponent>(TEXT("CharacterStats"));
    CharacterCreation = CreateDefaultSubobject<UHuwamCharacterCreationComponent>(TEXT("CharacterCreation"));
    CharacterCreationMenuData = CreateDefaultSubobject<UHuwamCharacterCreationMenuDataComponent>(TEXT("CharacterCreationMenuData"));
    Combat = CreateDefaultSubobject<UHuwamCombatComponent>(TEXT("Combat"));
    Rewards = CreateDefaultSubobject<UHuwamRewardComponent>(TEXT("Rewards"));
    Quests = CreateDefaultSubobject<UHuwamQuestComponent>(TEXT("Quests"));
    LiveContent = CreateDefaultSubobject<UHuwamLiveContentComponent>(TEXT("LiveContent"));
    MapAwareness = CreateDefaultSubobject<UHuwamMapAwarenessComponent>(TEXT("MapAwareness"));
    TutorialLab = CreateDefaultSubobject<UHuwamTutorialLabComponent>(TEXT("TutorialLab"));
    WorldInteraction = CreateDefaultSubobject<UHuwamWorldInteractionComponent>(TEXT("WorldInteraction"));
    HudData = CreateDefaultSubobject<UHuwamHudDataComponent>(TEXT("HudData"));

    StarterMeleeAttackConfig.VarianceMin = 0;
    StarterMeleeAttackConfig.VarianceMax = 4;
    StarterMeleeAttackConfig.CriticalChancePercent = 5;
    StarterMeleeAttackConfig.CriticalMultiplierPercent = 150;
    CharacterCreationScreenClass = UHuwamCharacterCreationScreenWidget::StaticClass();
    NpcSupplyQuestInteractionScreenClass = UHuwamNpcSupplyQuestInteractionWidget::StaticClass();
    PrototypeHudScreenClass = UHuwamPrototypeHudWidget::StaticClass();
    PrototypeMenuScreenClass = UHuwamPrototypeMenuWidget::StaticClass();
    TutorialLabPromptScreenClass = UHuwamTutorialLabPromptWidget::StaticClass();
    WorldInteractionPromptScreenClass = UHuwamWorldInteractionPromptWidget::StaticClass();
    PrototypeWorldInteractKey = EKeys::E;
}

void AHuwamPrototypePlayerActor::BeginPlay()
{
    Super::BeginPlay();

    if (TutorialLab)
    {
        TutorialLab->OnTutorialLabChanged.AddUniqueDynamic(this, &AHuwamPrototypePlayerActor::HandleTutorialLabChanged);
    }

    if (bEnablePrototypeWorldInteractionInputOnBeginPlay)
    {
        EnablePrototypeWorldInteractionInput();
    }

    if (bShowCharacterCreationScreenOnBeginPlay && ShowCharacterCreationScreen())
    {
        return;
    }

    if (bAutoInitializeOnBeginPlay)
    {
        InitializePrototypePlayer();
    }
}

void AHuwamPrototypePlayerActor::InitializePrototypePlayer()
{
    if (CharacterCreation)
    {
        FHuwamCharacterCreationResult CreationResult;
        if (ApplyCharacterCreationToPrototype(CharacterCreation->BuildDefaultCharacterCreationRequest(), CreationResult))
        {
            InitializePrototypeSystemsAfterCharacterCreation(true);
            return;
        }
    }

    if (CharacterStats)
    {
        CharacterStats->ResetToProgressionZeroDefaults();
    }

    InitializePrototypeSystemsAfterCharacterCreation(true);
}

void AHuwamPrototypePlayerActor::ConfigureForThirdPersonBootstrap()
{
    bConfiguredForThirdPersonBootstrap = true;
    bAutoInitializeOnBeginPlay = false;
    bShowCharacterCreationScreenOnBeginPlay = true;
    bEnablePrototypeWorldInteractionInputOnBeginPlay = false;
    bBeginTutorialLabAfterCharacterCreation = true;
    bShowWorldInteractionPromptOnInitialize = true;
}

bool AHuwamPrototypePlayerActor::InitializePrototypePlayerFromCharacterCreation(const FHuwamCharacterCreationRequest& Request, FHuwamCharacterCreationResult& OutResult)
{
    if (!ApplyCharacterCreationToPrototype(Request, OutResult))
    {
        return false;
    }

    const bool bBeginTutorial = bBeginTutorialLabAfterCharacterCreation && TutorialLab;
    InitializePrototypeSystemsAfterCharacterCreation(!bBeginTutorial);
    if (bBeginTutorial)
    {
        BeginLaucianTutorialLab();
    }

    return true;
}

void AHuwamPrototypePlayerActor::InitializePrototypeSystemsAfterCharacterCreation(bool bStartFirstSlimeQuest)
{
    if (Equipment)
    {
        Equipment->ResetToDefaultSlots();
    }

    if (LiveContent)
    {
        LiveContent->SetCurrentGameVersion(TEXT("0.1.0"));
        LiveContent->EnableContentPack(TEXT("content.pack.starter_core"));
    }

    if (bGrantStarterLoadoutOnInitialize)
    {
        GrantStarterLoadout();
        EquipStarterGear();
    }

    if (Combat)
    {
        Combat->InitializeFromStats(CharacterStats, Equipment, true);
    }

    if (SurvivalVitals)
    {
        SurvivalVitals->ResetSurvivalVitals();
    }

    if (bStartFirstSlimeQuest && bStartSlimeQuestOnInitialize)
    {
        StartSlimeQuest();
    }

    if (bShowWorldInteractionPromptOnInitialize)
    {
        ShowWorldInteractionPromptScreen();
    }

    if (bShowPrototypeHudOnInitialize)
    {
        ShowPrototypeHudScreen();
    }

    RefreshPrototypeHud();
}

bool AHuwamPrototypePlayerActor::GrantStarterLoadout()
{
    bool bChanged = false;
    bChanged |= AddInventoryItemIfMissing(BasicSwordId, 1);
    bChanged |= AddInventoryItemIfMissing(BasicKnifeId, 1);
    bChanged |= AddInventoryItemIfMissing(AdventuringBackpackId, 1);
    bChanged |= AddInventoryItemIfMissing(GatheringKnifeId, 1);
    bChanged |= AddInventoryItemIfMissing(SmallPouchId, 1);
    return bChanged;
}

bool AHuwamPrototypePlayerActor::EquipStarterGear()
{
    bool bChanged = false;
    bChanged |= EquipItemIfAvailable(BasicSwordId, TEXT("slot.weapon.main_hand"));
    bChanged |= EquipItemIfAvailable(BasicKnifeId, TEXT("slot.weapon.off_hand"));
    bChanged |= EquipItemIfAvailable(AdventuringBackpackId, TEXT("slot.backpack"));

    if (Combat)
    {
        Combat->InitializeFromStats(CharacterStats, Equipment, false);
    }

    RefreshPrototypeHud();
    return bChanged;
}

bool AHuwamPrototypePlayerActor::StartSlimeQuest()
{
    if (!Quests)
    {
        return false;
    }

    const bool bStarted = Quests->StartSlimesInTallGrassQuest(Rewards);
    RefreshPrototypeHud();
    return bStarted;
}

bool AHuwamPrototypePlayerActor::StartGatherFieldSuppliesQuest()
{
    if (!Quests)
    {
        return false;
    }

    const bool bStarted = Quests->StartGatherFieldSuppliesQuest(Rewards);
    RefreshPrototypeHud();
    return bStarted;
}

bool AHuwamPrototypePlayerActor::CanStartGatherFieldSuppliesQuestFromSupply(UHuwamNpcSupplyComponent* RequesterSupply) const
{
    FHuwamQuestOffer QuestOffer;
    return GetGatherFieldSuppliesQuestOfferFromSupply(RequesterSupply, QuestOffer);
}

bool AHuwamPrototypePlayerActor::GetGatherFieldSuppliesQuestOfferFromSupply(UHuwamNpcSupplyComponent* RequesterSupply, FHuwamQuestOffer& OutQuestOffer) const
{
    OutQuestOffer = FHuwamQuestOffer();

    if (!Quests
        || !RequesterSupply
        || RequesterSupply->GetNpcId() != GatherFieldSuppliesRequesterNpcId)
    {
        return false;
    }

    const FHuwamNpcSupplyNeedState NeedState = RequesterSupply->GetSupplyNeedState();
    if (NeedState.NeedId.IsEmpty()
        || !NeedState.bShouldRequestSupplyHelp
        || !Quests->GetQuestOffer(GatherFieldSuppliesQuestId, OutQuestOffer))
    {
        OutQuestOffer = FHuwamQuestOffer();
        return false;
    }

    OutQuestOffer.NeedRequest.NeedId = NeedState.NeedId;
    OutQuestOffer.NeedRequest.NeedDisplayName = NeedState.DisplayName;
    OutQuestOffer.NeedRequest.NeedReason = NeedState.Reason;
    OutQuestOffer.NeedRequest.NeededQuantityAtRequest = NeedState.NeededQuantity;
    return true;
}

bool AHuwamPrototypePlayerActor::StartGatherFieldSuppliesQuestFromSupply(UHuwamNpcSupplyComponent* RequesterSupply)
{
    FHuwamQuestOffer QuestOffer;
    if (!GetGatherFieldSuppliesQuestOfferFromSupply(RequesterSupply, QuestOffer))
    {
        RefreshPrototypeHud();
        return false;
    }

    const bool bStarted = Quests->StartGatherFieldSuppliesQuest(Rewards);
    const bool bNeedRequestSet = bStarted && Quests->SetQuestNeedRequestContext(GatherFieldSuppliesQuestId, QuestOffer.NeedRequest);
    RefreshPrototypeHud();
    return bNeedRequestSet;
}

bool AHuwamPrototypePlayerActor::BeginLaucianTutorialLab()
{
    if (!TutorialLab)
    {
        return false;
    }

    const bool bStarted = TutorialLab->BeginLaucianTutorialLab(Quests, Rewards, nullptr, GetPrototypeAvatarActor());
    if (bStarted)
    {
        ShowTutorialLabPromptScreen();
    }

    RefreshPrototypeHud();
    return bStarted;
}

bool AHuwamPrototypePlayerActor::AcknowledgeTutorialHudLesson()
{
    if (!TutorialLab)
    {
        return false;
    }

    const bool bCompleted = TutorialLab->MarkHudLessonComplete();
    RefreshPrototypeHud();
    return bCompleted;
}

bool AHuwamPrototypePlayerActor::MarkTutorialInventoryItemInspected(const FString& ItemId)
{
    if (!TutorialLab || !Inventory || !Inventory->HasItemQuantity(ItemId, 1))
    {
        return false;
    }

    const bool bCompleted = TutorialLab->MarkInventoryLessonComplete(ItemId);
    RefreshPrototypeHud();
    return bCompleted;
}

bool AHuwamPrototypePlayerActor::MarkTutorialEquipmentSlotInspected(const FString& SlotId)
{
    if (!TutorialLab || !Equipment || SlotId.IsEmpty())
    {
        return false;
    }

    const FString EquippedItemId = Equipment->GetEquippedItemInSlot(SlotId);
    if (EquippedItemId.IsEmpty())
    {
        return false;
    }

    const bool bCompleted = TutorialLab->MarkEquipmentLessonComplete(SlotId, EquippedItemId);
    RefreshPrototypeHud();
    return bCompleted;
}

bool AHuwamPrototypePlayerActor::AttackTutorialLabCombatTarget(AHuwamTutorialLabCombatTargetActor* Target, FHuwamCombatResult& OutCombatResult)
{
    if (!Target)
    {
        OutCombatResult = FHuwamCombatResult();
        return false;
    }

    const bool bAttacked = Target->ReceiveAttackFromPrototypePlayer(this, StarterMeleeAttackConfig, OutCombatResult);
    RefreshPrototypeHud();
    return bAttacked;
}

bool AHuwamPrototypePlayerActor::OpenTutorialMapLesson()
{
    if (!TutorialLab || !MapAwareness)
    {
        return false;
    }

    MapAwareness->OpenFullMap();
    const FHuwamMapAwarenessSnapshot MapSnapshot = MapAwareness->GetMapAwarenessSnapshot();
    if (!MapSnapshot.bFullMapOpen || !MapSnapshot.bCardinalMarkersVisible)
    {
        RefreshPrototypeHud();
        return false;
    }

    const bool bCompleted = TutorialLab->MarkMapLessonComplete(MapSnapshot.MapId);
    RefreshPrototypeHud();
    return bCompleted;
}

bool AHuwamPrototypePlayerActor::TrackTutorialQuestLesson()
{
    if (!TutorialLab || !Quests || !MapAwareness)
    {
        return false;
    }

    const FHuwamTutorialLabSnapshot TutorialSnapshot = TutorialLab->GetTutorialLabSnapshot();
    FHuwamQuestRuntimeState QuestState;
    if (!Quests->GetQuestState(TutorialSnapshot.QuestLessonQuestId, QuestState)
        || !QuestState.bActive
        || QuestState.bTurnedIn
        || QuestState.PrimaryObjective.ObjectiveId.IsEmpty())
    {
        RefreshPrototypeHud();
        return false;
    }

    const bool bTrackedQuest = Quests->TrackQuest(QuestState.QuestId);
    const bool bTrackedMarker = MapAwareness->TrackQuestObjective(QuestState.QuestId, QuestState.PrimaryObjective.ObjectiveId);
    const bool bCompleted = bTrackedQuest
        && bTrackedMarker
        && TutorialLab->MarkQuestLessonComplete(QuestState.QuestId, QuestState.PrimaryObjective.ObjectiveId);

    RefreshPrototypeHud();
    return bCompleted;
}

bool AHuwamPrototypePlayerActor::EnterEldoriaStarterArea()
{
    if (bHasEnteredEldoriaStarterArea)
    {
        return true;
    }

    AHuwamEldoriaStarterEntryActor* Entry = FindEldoriaStarterEntry();
    AActor* ArrivalActor = GetPrototypeAvatarActor();
    if (Entry && ArrivalActor)
    {
        ArrivalActor->SetActorTransform(Entry->GetEntryTransform());
    }

    if (MapAwareness)
    {
        MapAwareness->SetCurrentMap(
            TEXT("map.nanira.eldoria_starter_fields"),
            TEXT("location.eldoria.starter_fields"),
            NSLOCTEXT("HuwamPrototype", "EldoriaStarterFieldsMap", "Eldoria Starter Fields"),
            EHuwamMapFogState::Explored
        );
    }

    bHasEnteredEldoriaStarterArea = true;
    RefreshPrototypeHud();
    return Entry != nullptr;
}

bool AHuwamPrototypePlayerActor::ApplyCharacterCreationToPrototype(const FHuwamCharacterCreationRequest& Request, FHuwamCharacterCreationResult& OutResult)
{
    if (!CharacterCreation)
    {
        OutResult = FHuwamCharacterCreationResult();
        OutResult.Errors.Add(FText::FromString(TEXT("This prototype player has no character creation component.")));
        return false;
    }

    const bool bApplied = CharacterCreation->ApplyCharacterCreation(Request, CharacterStats, OutResult);
    if (bApplied && Combat)
    {
        Combat->InitializeFromStats(CharacterStats, Equipment, true);
    }

    RefreshPrototypeHud();
    return bApplied;
}

bool AHuwamPrototypePlayerActor::ShowCharacterCreationScreen(APlayerController* OwningPlayer)
{
    if (!CharacterCreationScreenClass)
    {
        return false;
    }

    if (CharacterCreationScreen && CharacterCreationScreen->IsInViewport())
    {
        return true;
    }

    if (!OwningPlayer && GetWorld())
    {
        OwningPlayer = GetWorld()->GetFirstPlayerController();
    }

    if (!OwningPlayer)
    {
        return false;
    }

    CharacterCreationScreen = CreateWidget<UHuwamCharacterCreationScreenWidget>(OwningPlayer, CharacterCreationScreenClass);
    if (!CharacterCreationScreen)
    {
        return false;
    }

    CharacterCreationScreen->SetPrototypePlayer(this);
    CharacterCreationScreen->SetIsFocusable(true);
    CharacterCreationScreen->AddToViewport(20);

    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(CharacterCreationScreen->TakeWidget());
    OwningPlayer->SetInputMode(InputMode);
    OwningPlayer->bShowMouseCursor = true;
    return true;
}

void AHuwamPrototypePlayerActor::HideCharacterCreationScreen(bool bRestoreGameInput)
{
    APlayerController* OwningPlayer = CharacterCreationScreen ? CharacterCreationScreen->GetOwningPlayer() : nullptr;
    if (CharacterCreationScreen)
    {
        CharacterCreationScreen->RemoveFromParent();
        CharacterCreationScreen = nullptr;
    }

    if (bRestoreGameInput && OwningPlayer)
    {
        OwningPlayer->SetInputMode(FInputModeGameOnly());
        OwningPlayer->bShowMouseCursor = false;
    }
}

bool AHuwamPrototypePlayerActor::AttackMonster(AHuwamMonsterEncounterActor* Monster, FHuwamCombatResult& OutCombatResult)
{
    if (!Monster || !Combat)
    {
        OutCombatResult = FHuwamCombatResult();
        return false;
    }

    const bool bAttacked = Monster->ReceiveAttackFrom(Combat, EHuwamCombatAttackType::Melee, StarterMeleeAttackConfig, OutCombatResult);
    RefreshPrototypeHud();
    return bAttacked;
}

bool AHuwamPrototypePlayerActor::ClaimMonsterRewards(AHuwamMonsterEncounterActor* Monster, FHuwamRewardGrantResult& OutRewardResult)
{
    if (!Rewards || !Monster)
    {
        OutRewardResult = FHuwamRewardGrantResult();
        return false;
    }

    const bool bClaimed = Rewards->ClaimMonsterDefeatRewards(
        Monster,
        Inventory,
        SlimeQuestId,
        SlimeObjectiveId,
        1,
        3,
        OutRewardResult
    );

    if (Quests)
    {
        Quests->RefreshQuestProgressFromRewards(SlimeQuestId, Rewards);
    }

    RefreshPrototypeHud();
    return bClaimed;
}

bool AHuwamPrototypePlayerActor::AttackMonsterAndClaimIfDefeated(AHuwamMonsterEncounterActor* Monster, FHuwamCombatResult& OutCombatResult, FHuwamRewardGrantResult& OutRewardResult)
{
    OutRewardResult = FHuwamRewardGrantResult();

    const bool bAttacked = AttackMonster(Monster, OutCombatResult);
    if (bAttacked && Monster && Monster->IsDefeated())
    {
        ClaimMonsterRewards(Monster, OutRewardResult);
    }

    return bAttacked;
}

bool AHuwamPrototypePlayerActor::TurnInSlimeQuest(FHuwamQuestRuntimeState& OutTurnedInQuest)
{
    if (!Quests)
    {
        OutTurnedInQuest = FHuwamQuestRuntimeState();
        return false;
    }

    if (Rewards)
    {
        Quests->RefreshQuestProgressFromRewards(SlimeQuestId, Rewards);
    }

    const bool bTurnedIn = Quests->TurnInQuest(SlimeQuestId, Rewards, Inventory, OutTurnedInQuest);
    RefreshPrototypeHud();
    return bTurnedIn;
}

bool AHuwamPrototypePlayerActor::TurnInGatherFieldSuppliesQuest(UHuwamNpcSupplyComponent* RecipientSupply, FHuwamQuestRuntimeState& OutTurnedInQuest, FHuwamNpcSupplyDeliveryResult& OutDeliveryResult)
{
    OutTurnedInQuest = FHuwamQuestRuntimeState();
    OutDeliveryResult = FHuwamNpcSupplyDeliveryResult();

    if (!Quests || !Rewards || !Inventory || !RecipientSupply)
    {
        return false;
    }

    Quests->RefreshQuestProgressFromRewards(GatherFieldSuppliesQuestId, Rewards);

    FHuwamQuestRuntimeState QuestState;
    if (!Quests->GetQuestState(GatherFieldSuppliesQuestId, QuestState)
        || !Quests->CanTurnInQuest(GatherFieldSuppliesQuestId)
        || QuestState.PrimaryObjective.TargetValue <= 0
        || (!QuestState.QuestGiverId.IsEmpty()
            && !RecipientSupply->GetNpcId().IsEmpty()
            && QuestState.QuestGiverId != RecipientSupply->GetNpcId()))
    {
        RefreshPrototypeHud();
        return false;
    }

    if (!RecipientSupply->DeliverAcceptedItemsFromInventory(Inventory, QuestState.PrimaryObjective.TargetValue, OutDeliveryResult))
    {
        RefreshPrototypeHud();
        return false;
    }

    const bool bTurnedIn = Quests->TurnInQuest(GatherFieldSuppliesQuestId, Rewards, Inventory, OutTurnedInQuest);
    RefreshPrototypeHud();
    return bTurnedIn;
}

bool AHuwamPrototypePlayerActor::SellAcceptedSupplyItemToNpc(UHuwamNpcSupplyComponent* BuyerSupply, const FString& ItemId, int32 Quantity, FHuwamNpcSupplyPurchaseResult& OutPurchaseResult)
{
    OutPurchaseResult = FHuwamNpcSupplyPurchaseResult();

    if (!BuyerSupply || !Inventory || !Rewards)
    {
        return false;
    }

    const bool bSold = BuyerSupply->PurchaseAcceptedItemFromInventory(Inventory, Rewards, ItemId, Quantity, OutPurchaseResult);
    RefreshPrototypeHud();
    return bSold;
}

FHuwamHudSnapshot AHuwamPrototypePlayerActor::RefreshPrototypeHud()
{
    return HudData ? HudData->RefreshHudSnapshot() : FHuwamHudSnapshot();
}

FHuwamWorldInteractionSnapshot AHuwamPrototypePlayerActor::RefreshWorldInteractionFocus()
{
    const FHuwamWorldInteractionSnapshot Snapshot = WorldInteraction
        ? WorldInteraction->RefreshInteractionFocus()
        : FHuwamWorldInteractionSnapshot();
    RefreshPrototypeHud();
    return Snapshot;
}

bool AHuwamPrototypePlayerActor::InteractWithFocusedWorldTarget(FHuwamWorldInteractionResult& OutResult, APlayerController* OwningPlayer)
{
    if (!WorldInteraction)
    {
        OutResult = FHuwamWorldInteractionResult();
        return false;
    }

    return WorldInteraction->InteractWithFocusedTarget(OutResult, OwningPlayer);
}

bool AHuwamPrototypePlayerActor::ShowNpcSupplyQuestInteractionScreen(AHuwamNpcSupplyQuestOfferActor* QuestRequester, APlayerController* OwningPlayer)
{
    if (!QuestRequester || !NpcSupplyQuestInteractionScreenClass)
    {
        return false;
    }

    if (!OwningPlayer && GetWorld())
    {
        OwningPlayer = GetWorld()->GetFirstPlayerController();
    }

    if (!OwningPlayer)
    {
        return false;
    }

    if (!NpcSupplyQuestInteractionScreen)
    {
        NpcSupplyQuestInteractionScreen = CreateWidget<UHuwamNpcSupplyQuestInteractionWidget>(OwningPlayer, NpcSupplyQuestInteractionScreenClass);
    }

    if (!NpcSupplyQuestInteractionScreen)
    {
        return false;
    }

    NpcSupplyQuestInteractionScreen->SetInteractionSource(this, QuestRequester);
    if (!NpcSupplyQuestInteractionScreen->IsInViewport())
    {
        NpcSupplyQuestInteractionScreen->AddToViewport(15);
    }

    FInputModeGameAndUI InputMode;
    InputMode.SetWidgetToFocus(NpcSupplyQuestInteractionScreen->TakeWidget());
    OwningPlayer->SetInputMode(InputMode);
    OwningPlayer->bShowMouseCursor = true;
    return true;
}

void AHuwamPrototypePlayerActor::HideNpcSupplyQuestInteractionScreen(bool bRestoreGameInput)
{
    APlayerController* OwningPlayer = NpcSupplyQuestInteractionScreen ? NpcSupplyQuestInteractionScreen->GetOwningPlayer() : nullptr;
    if (NpcSupplyQuestInteractionScreen)
    {
        NpcSupplyQuestInteractionScreen->RemoveFromParent();
        NpcSupplyQuestInteractionScreen = nullptr;
    }

    if (bRestoreGameInput && OwningPlayer)
    {
        OwningPlayer->SetInputMode(FInputModeGameOnly());
        OwningPlayer->bShowMouseCursor = false;
    }
}

bool AHuwamPrototypePlayerActor::ShowWorldInteractionPromptScreen(APlayerController* OwningPlayer)
{
    if (!WorldInteractionPromptScreenClass)
    {
        return false;
    }

    if (!OwningPlayer && GetWorld())
    {
        OwningPlayer = GetWorld()->GetFirstPlayerController();
    }

    if (!OwningPlayer)
    {
        return false;
    }

    if (!WorldInteractionPromptScreen)
    {
        WorldInteractionPromptScreen = CreateWidget<UHuwamWorldInteractionPromptWidget>(OwningPlayer, WorldInteractionPromptScreenClass);
    }

    if (!WorldInteractionPromptScreen)
    {
        return false;
    }

    WorldInteractionPromptScreen->SetPrototypePlayer(this);
    if (!WorldInteractionPromptScreen->IsInViewport())
    {
        WorldInteractionPromptScreen->AddToViewport(5);
        WorldInteractionPromptScreen->SetAnchorsInViewport(FAnchors(0.5f, 0.88f));
        WorldInteractionPromptScreen->SetAlignmentInViewport(FVector2D(0.5f, 1.0f));
        WorldInteractionPromptScreen->SetPositionInViewport(FVector2D::ZeroVector);
    }

    WorldInteractionPromptScreen->RefreshPrompt();
    return true;
}

void AHuwamPrototypePlayerActor::HideWorldInteractionPromptScreen()
{
    if (WorldInteractionPromptScreen)
    {
        WorldInteractionPromptScreen->RemoveFromParent();
        WorldInteractionPromptScreen = nullptr;
    }
}

bool AHuwamPrototypePlayerActor::ShowTutorialLabPromptScreen(APlayerController* OwningPlayer)
{
    if (!TutorialLabPromptScreenClass)
    {
        return false;
    }

    if (!OwningPlayer && GetWorld())
    {
        OwningPlayer = GetWorld()->GetFirstPlayerController();
    }

    if (!OwningPlayer)
    {
        return false;
    }

    if (!TutorialLabPromptScreen)
    {
        TutorialLabPromptScreen = CreateWidget<UHuwamTutorialLabPromptWidget>(OwningPlayer, TutorialLabPromptScreenClass);
    }

    if (!TutorialLabPromptScreen)
    {
        return false;
    }

    TutorialLabPromptScreen->SetPrototypePlayer(this);
    if (!TutorialLabPromptScreen->IsInViewport())
    {
        TutorialLabPromptScreen->AddToViewport(6);
        TutorialLabPromptScreen->SetAnchorsInViewport(FAnchors(0.5f, 0.08f));
        TutorialLabPromptScreen->SetAlignmentInViewport(FVector2D(0.5f, 0.0f));
        TutorialLabPromptScreen->SetDesiredSizeInViewport(FVector2D(540.0f, 170.0f));
        TutorialLabPromptScreen->SetPositionInViewport(FVector2D::ZeroVector, false);
    }

    TutorialLabPromptScreen->RefreshPrompt();
    return true;
}

void AHuwamPrototypePlayerActor::HideTutorialLabPromptScreen()
{
    if (TutorialLabPromptScreen)
    {
        TutorialLabPromptScreen->RemoveFromParent();
        TutorialLabPromptScreen = nullptr;
    }
}

bool AHuwamPrototypePlayerActor::ShowPrototypeHudScreen(APlayerController* OwningPlayer)
{
    if (!PrototypeHudScreenClass)
    {
        return false;
    }

    if (!OwningPlayer && GetWorld())
    {
        OwningPlayer = GetWorld()->GetFirstPlayerController();
    }

    if (!OwningPlayer)
    {
        return false;
    }

    if (!PrototypeHudScreen)
    {
        PrototypeHudScreen = CreateWidget<UHuwamPrototypeHudWidget>(OwningPlayer, PrototypeHudScreenClass);
    }

    if (!PrototypeHudScreen)
    {
        return false;
    }

    PrototypeHudScreen->SetPrototypePlayer(this);
    if (!PrototypeHudScreen->IsInViewport())
    {
        PrototypeHudScreen->AddToViewport(1);
        PrototypeHudScreen->SetAnchorsInViewport(FAnchors(0.0f, 0.0f));
        PrototypeHudScreen->SetAlignmentInViewport(FVector2D::ZeroVector);
        PrototypeHudScreen->SetDesiredSizeInViewport(FVector2D(390.0f, 240.0f));
        PrototypeHudScreen->SetPositionInViewport(FVector2D(18.0f, 18.0f), false);
    }

    PrototypeHudScreen->RefreshHud();
    return true;
}

void AHuwamPrototypePlayerActor::HidePrototypeHudScreen()
{
    if (PrototypeHudScreen)
    {
        PrototypeHudScreen->RemoveFromParent();
        PrototypeHudScreen = nullptr;
    }
}

bool AHuwamPrototypePlayerActor::ShowPrototypeMenuScreen(EHuwamPrototypeMenuPage Page, APlayerController* OwningPlayer)
{
    if (!PrototypeMenuScreenClass)
    {
        return false;
    }

    if (!OwningPlayer && GetWorld())
    {
        OwningPlayer = GetWorld()->GetFirstPlayerController();
    }

    if (!OwningPlayer)
    {
        return false;
    }

    if (!PrototypeMenuScreen)
    {
        PrototypeMenuScreen = CreateWidget<UHuwamPrototypeMenuWidget>(OwningPlayer, PrototypeMenuScreenClass);
    }

    if (!PrototypeMenuScreen)
    {
        return false;
    }

    PrototypeMenuScreen->SetPrototypePlayer(this);
    PrototypeMenuScreen->SetIsFocusable(true);
    PrototypeMenuScreen->OpenPage(Page);
    if (!PrototypeMenuScreen->IsInViewport())
    {
        PrototypeMenuScreen->AddToViewport(12);
        PrototypeMenuScreen->SetAnchorsInViewport(FAnchors(0.5f, 0.5f));
        PrototypeMenuScreen->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
        PrototypeMenuScreen->SetPositionInViewport(FVector2D::ZeroVector);
    }

    FInputModeGameAndUI InputMode;
    InputMode.SetWidgetToFocus(PrototypeMenuScreen->TakeWidget());
    OwningPlayer->SetInputMode(InputMode);
    OwningPlayer->bShowMouseCursor = true;
    return true;
}

void AHuwamPrototypePlayerActor::HidePrototypeMenuScreen(bool bRestoreGameInput)
{
    APlayerController* OwningPlayer = PrototypeMenuScreen ? PrototypeMenuScreen->GetOwningPlayer() : nullptr;
    if (PrototypeMenuScreen)
    {
        PrototypeMenuScreen->RemoveFromParent();
        PrototypeMenuScreen = nullptr;
    }

    if (bRestoreGameInput && OwningPlayer)
    {
        OwningPlayer->SetInputMode(FInputModeGameOnly());
        OwningPlayer->bShowMouseCursor = false;
    }
}

bool AHuwamPrototypePlayerActor::SavePrototypeProgress(const FString& SlotName, int32 UserIndex)
{
    if (SlotName.IsEmpty())
    {
        return false;
    }

    UHuwamPrototypeSaveGame* SaveGame = Cast<UHuwamPrototypeSaveGame>(UGameplayStatics::CreateSaveGameObject(UHuwamPrototypeSaveGame::StaticClass()));
    if (!SaveGame)
    {
        return false;
    }

    SaveGame->SaveVersion = UHuwamPrototypeSaveGame::LatestSaveVersion;
    if (AActor* Avatar = GetPrototypeAvatarActor())
    {
        SaveGame->bHasAvatarTransform = true;
        SaveGame->AvatarTransform = Avatar->GetActorTransform();
    }

    SaveGame->InventoryStacks = Inventory ? Inventory->GetInventoryStacks() : TArray<FHuwamInventoryStack>();
    SaveGame->EquipmentSlots = Equipment ? Equipment->GetEquipmentSlots() : TArray<FHuwamEquipmentSlotState>();
    SaveGame->SurvivalVitals = SurvivalVitals ? SurvivalVitals->GetSurvivalVitalsSnapshot() : FHuwamSurvivalVitalsSnapshot();
    if (Rewards)
    {
        SaveGame->CurrencyBalanceCopper = Rewards->GetCurrencyBalanceCopper();
        SaveGame->TotalExperience = Rewards->GetTotalExperience();
        SaveGame->QuestProgressRecords = Rewards->GetQuestProgressRecords();
    }

    if (Combat)
    {
        SaveGame->CurrentHealth = Combat->GetCurrentHealth();
        SaveGame->CurrentMana = Combat->GetCurrentMana();
    }

    if (Quests)
    {
        SaveGame->QuestStates = Quests->GetQuestStates();
        SaveGame->TurnedInQuestIds = Quests->GetTurnedInQuestIds();
        SaveGame->TrackedQuestId = Quests->GetTrackedQuestId();
    }

    if (MapAwareness)
    {
        SaveGame->MapAwareness = MapAwareness->GetMapAwarenessSnapshot();
    }

    if (TutorialLab)
    {
        SaveGame->TutorialLab = TutorialLab->GetTutorialLabSnapshot();
    }

    if (GetWorld() && GetWorld()->GetGameInstance())
    {
        if (UHuwamEconomyLedgerSubsystem* EconomyLedger = GetWorld()->GetGameInstance()->GetSubsystem<UHuwamEconomyLedgerSubsystem>())
        {
            SaveGame->EconomyLedgerEntries = EconomyLedger->GetLedgerEntries();
            SaveGame->EconomyAccountBalances = EconomyLedger->GetAccountBalances();
        }

        for (TActorIterator<AHuwamSurvivalServiceActor> It(GetWorld()); It; ++It)
        {
            if (const AHuwamSurvivalServiceActor* ServiceActor = *It)
            {
                SaveGame->SurvivalServiceStates.Add(ServiceActor->GetServiceSaveState());
            }
        }
    }

    SaveGame->bEnteredEldoriaStarterArea = bHasEnteredEldoriaStarterArea;
    return UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, UserIndex);
}

bool AHuwamPrototypePlayerActor::LoadPrototypeProgress(const FString& SlotName, int32 UserIndex)
{
    if (SlotName.IsEmpty() || !UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
    {
        return false;
    }

    UHuwamPrototypeSaveGame* SaveGame = Cast<UHuwamPrototypeSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
    if (!SaveGame)
    {
        return false;
    }

    if (SaveGame->SaveVersion <= 0 || SaveGame->SaveVersion > UHuwamPrototypeSaveGame::LatestSaveVersion)
    {
        return false;
    }

    if (Inventory)
    {
        Inventory->RestoreInventoryStacksForPrototypeSave(SaveGame->InventoryStacks);
    }

    if (Equipment)
    {
        Equipment->RestoreEquipmentSlotsForPrototypeSave(SaveGame->EquipmentSlots);
    }

    if (SurvivalVitals)
    {
        SurvivalVitals->RestoreSurvivalVitalsSnapshot(SaveGame->SurvivalVitals);
    }

    if (Rewards)
    {
        Rewards->RestoreRewardStateForPrototypeSave(SaveGame->CurrencyBalanceCopper, SaveGame->TotalExperience, SaveGame->QuestProgressRecords);
    }

    if (Quests)
    {
        Quests->RestoreQuestStateForPrototypeSave(SaveGame->QuestStates, SaveGame->TurnedInQuestIds, SaveGame->TrackedQuestId);
    }

    if (MapAwareness)
    {
        MapAwareness->RestoreMapAwarenessForPrototypeSave(SaveGame->MapAwareness);
    }

    if (TutorialLab)
    {
        TutorialLab->RestoreTutorialLabForPrototypeSave(SaveGame->TutorialLab);
        if (SaveGame->TutorialLab.bActive)
        {
            ShowTutorialLabPromptScreen();
        }
    }

    if (GetWorld() && GetWorld()->GetGameInstance())
    {
        if (UHuwamEconomyLedgerSubsystem* EconomyLedger = GetWorld()->GetGameInstance()->GetSubsystem<UHuwamEconomyLedgerSubsystem>())
        {
            EconomyLedger->RestoreLedgerStateForPrototypeSave(SaveGame->EconomyLedgerEntries, SaveGame->EconomyAccountBalances);
        }

        for (TActorIterator<AHuwamSurvivalServiceActor> It(GetWorld()); It; ++It)
        {
            AHuwamSurvivalServiceActor* ServiceActor = *It;
            if (!ServiceActor)
            {
                continue;
            }

            const FString ServiceIdentityId = ServiceActor->GetServiceIdentityId();
            for (const FHuwamSurvivalServiceSaveState& SavedServiceState : SaveGame->SurvivalServiceStates)
            {
                if (SavedServiceState.ServiceIdentityId == ServiceIdentityId)
                {
                    ServiceActor->RestoreServiceStateForPrototypeSave(SavedServiceState);
                    break;
                }
            }
        }
    }

    if (Combat)
    {
        const bool bHasSavedMeters = SaveGame->CurrentHealth >= 0 && SaveGame->CurrentMana >= 0;
        Combat->InitializeFromStats(CharacterStats, Equipment, !bHasSavedMeters);
        if (bHasSavedMeters)
        {
            Combat->RestoreMetersForPrototypeSave(SaveGame->CurrentHealth, SaveGame->CurrentMana);
        }
    }

    if (SaveGame->bHasAvatarTransform)
    {
        if (AActor* Avatar = GetPrototypeAvatarActor())
        {
            Avatar->SetActorTransform(SaveGame->AvatarTransform);
        }
    }

    bHasEnteredEldoriaStarterArea = SaveGame->bEnteredEldoriaStarterArea;
    ShowPrototypeHudScreen();
    RefreshPrototypeHud();
    return true;
}

bool AHuwamPrototypePlayerActor::EnablePrototypeWorldInteractionInput(APlayerController* OwningPlayer)
{
    if (!OwningPlayer && GetWorld())
    {
        OwningPlayer = GetWorld()->GetFirstPlayerController();
    }

    if (!OwningPlayer)
    {
        return false;
    }

    EnableInput(OwningPlayer);
    if (!InputComponent || bPrototypeWorldInteractionInputBound || !PrototypeWorldInteractKey.IsValid())
    {
        return InputComponent != nullptr;
    }

    InputComponent->BindKey(PrototypeWorldInteractKey, IE_Pressed, this, &AHuwamPrototypePlayerActor::HandlePrototypeWorldInteractInput);
    bPrototypeWorldInteractionInputBound = true;
    return true;
}

void AHuwamPrototypePlayerActor::DisablePrototypeWorldInteractionInput(APlayerController* OwningPlayer)
{
    if (!OwningPlayer && GetWorld())
    {
        OwningPlayer = GetWorld()->GetFirstPlayerController();
    }

    if (OwningPlayer)
    {
        DisableInput(OwningPlayer);
    }

    bPrototypeWorldInteractionInputBound = false;
}

void AHuwamPrototypePlayerActor::SetPrototypeAvatarActor(AActor* InAvatarActor)
{
    PrototypeAvatarActor = InAvatarActor;
    if (WorldInteraction)
    {
        WorldInteraction->SetInteractionOriginActor(InAvatarActor ? InAvatarActor : this);
    }
}

AActor* AHuwamPrototypePlayerActor::GetPrototypeAvatarActor() const
{
    return PrototypeAvatarActor ? PrototypeAvatarActor.Get() : const_cast<AHuwamPrototypePlayerActor*>(this);
}

UHuwamInventoryComponent* AHuwamPrototypePlayerActor::GetInventoryComponent() const
{
    return Inventory;
}

UHuwamSurvivalVitalsComponent* AHuwamPrototypePlayerActor::GetSurvivalVitalsComponent() const
{
    return SurvivalVitals;
}

UHuwamEquipmentComponent* AHuwamPrototypePlayerActor::GetEquipmentComponent() const
{
    return Equipment;
}

UHuwamCharacterStatsComponent* AHuwamPrototypePlayerActor::GetCharacterStatsComponent() const
{
    return CharacterStats;
}

UHuwamCharacterCreationComponent* AHuwamPrototypePlayerActor::GetCharacterCreationComponent() const
{
    return CharacterCreation;
}

UHuwamCharacterCreationMenuDataComponent* AHuwamPrototypePlayerActor::GetCharacterCreationMenuDataComponent() const
{
    return CharacterCreationMenuData;
}

UHuwamCharacterCreationScreenWidget* AHuwamPrototypePlayerActor::GetCharacterCreationScreen() const
{
    return CharacterCreationScreen;
}

UHuwamCombatComponent* AHuwamPrototypePlayerActor::GetCombatComponent() const
{
    return Combat;
}

UHuwamRewardComponent* AHuwamPrototypePlayerActor::GetRewardComponent() const
{
    return Rewards;
}

UHuwamQuestComponent* AHuwamPrototypePlayerActor::GetQuestComponent() const
{
    return Quests;
}

UHuwamLiveContentComponent* AHuwamPrototypePlayerActor::GetLiveContentComponent() const
{
    return LiveContent;
}

UHuwamMapAwarenessComponent* AHuwamPrototypePlayerActor::GetMapAwarenessComponent() const
{
    return MapAwareness;
}

UHuwamTutorialLabComponent* AHuwamPrototypePlayerActor::GetTutorialLabComponent() const
{
    return TutorialLab;
}

UHuwamWorldInteractionComponent* AHuwamPrototypePlayerActor::GetWorldInteractionComponent() const
{
    return WorldInteraction;
}

UHuwamNpcSupplyQuestInteractionWidget* AHuwamPrototypePlayerActor::GetNpcSupplyQuestInteractionScreen() const
{
    return NpcSupplyQuestInteractionScreen;
}

UHuwamWorldInteractionPromptWidget* AHuwamPrototypePlayerActor::GetWorldInteractionPromptScreen() const
{
    return WorldInteractionPromptScreen;
}

UHuwamTutorialLabPromptWidget* AHuwamPrototypePlayerActor::GetTutorialLabPromptScreen() const
{
    return TutorialLabPromptScreen;
}

UHuwamPrototypeHudWidget* AHuwamPrototypePlayerActor::GetPrototypeHudScreen() const
{
    return PrototypeHudScreen;
}

UHuwamPrototypeMenuWidget* AHuwamPrototypePlayerActor::GetPrototypeMenuScreen() const
{
    return PrototypeMenuScreen;
}

UHuwamHudDataComponent* AHuwamPrototypePlayerActor::GetHudDataComponent() const
{
    return HudData;
}

bool AHuwamPrototypePlayerActor::AddInventoryItemIfMissing(const FString& ItemId, int32 Quantity)
{
    if (!Inventory || ItemId.IsEmpty() || Quantity <= 0 || Inventory->HasItemQuantity(ItemId, Quantity))
    {
        return false;
    }

    if (Equipment)
    {
        for (const FHuwamEquipmentSlotState& Slot : Equipment->GetEquipmentSlots())
        {
            if (Slot.EquippedItemId == ItemId)
            {
                return false;
            }
        }
    }

    int32 Remainder = 0;
    return Inventory->AddItemById(ItemId, Quantity, Remainder) && Remainder == 0;
}

bool AHuwamPrototypePlayerActor::EquipItemIfAvailable(const FString& ItemId, const FString& SlotId)
{
    if (!Inventory || !Equipment || ItemId.IsEmpty() || SlotId.IsEmpty())
    {
        return false;
    }

    if (Equipment->GetEquippedItemInSlot(SlotId) == ItemId)
    {
        return false;
    }

    if (!Inventory->HasItemQuantity(ItemId, 1))
    {
        return false;
    }

    FString UnequippedItemId;
    return Equipment->EquipItemFromInventory(ItemId, SlotId, Inventory, UnequippedItemId);
}

void AHuwamPrototypePlayerActor::HandlePrototypeWorldInteractInput()
{
    FHuwamWorldInteractionResult InteractionResult;
    InteractWithFocusedWorldTarget(InteractionResult);
}

void AHuwamPrototypePlayerActor::HandleTutorialLabChanged(FHuwamTutorialLabSnapshot Snapshot)
{
    if (bEnterEldoriaAfterTutorial
        && Snapshot.CurrentBeat == EHuwamTutorialLabBeat::Complete
        && !Snapshot.bActive)
    {
        EnterEldoriaStarterArea();
    }
}

AHuwamEldoriaStarterEntryActor* AHuwamPrototypePlayerActor::FindEldoriaStarterEntry() const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    AHuwamEldoriaStarterEntryActor* FirstEntry = nullptr;
    for (TActorIterator<AHuwamEldoriaStarterEntryActor> It(GetWorld()); It; ++It)
    {
        if (!FirstEntry)
        {
            FirstEntry = *It;
        }

        if (It->IsPrimaryEntry())
        {
            return *It;
        }
    }

    return FirstEntry;
}
