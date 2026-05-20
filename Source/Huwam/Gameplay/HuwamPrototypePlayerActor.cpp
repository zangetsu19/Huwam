#include "Gameplay/HuwamPrototypePlayerActor.h"

#include "Components/SceneComponent.h"
#include "Gameplay/HuwamCharacterStatsComponent.h"
#include "Gameplay/HuwamEquipmentComponent.h"
#include "Gameplay/HuwamInventoryComponent.h"
#include "Gameplay/HuwamLiveContentComponent.h"
#include "Gameplay/HuwamMonsterEncounterActor.h"

namespace
{
    const TCHAR* BasicSwordId = TEXT("item.weapon.basic_sword");
    const TCHAR* BasicKnifeId = TEXT("item.weapon.basic_knife");
    const TCHAR* AdventuringBackpackId = TEXT("item.bag.adventuring_backpack");
    const TCHAR* SlimeQuestId = TEXT("quest.eldoria.slimes_in_tall_grass");
    const TCHAR* SlimeObjectiveId = TEXT("objective.defeat_basic_slime");
}

AHuwamPrototypePlayerActor::AHuwamPrototypePlayerActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    Inventory = CreateDefaultSubobject<UHuwamInventoryComponent>(TEXT("Inventory"));
    Equipment = CreateDefaultSubobject<UHuwamEquipmentComponent>(TEXT("Equipment"));
    CharacterStats = CreateDefaultSubobject<UHuwamCharacterStatsComponent>(TEXT("CharacterStats"));
    CharacterCreation = CreateDefaultSubobject<UHuwamCharacterCreationComponent>(TEXT("CharacterCreation"));
    Combat = CreateDefaultSubobject<UHuwamCombatComponent>(TEXT("Combat"));
    Rewards = CreateDefaultSubobject<UHuwamRewardComponent>(TEXT("Rewards"));
    Quests = CreateDefaultSubobject<UHuwamQuestComponent>(TEXT("Quests"));
    LiveContent = CreateDefaultSubobject<UHuwamLiveContentComponent>(TEXT("LiveContent"));
    HudData = CreateDefaultSubobject<UHuwamHudDataComponent>(TEXT("HudData"));

    StarterMeleeAttackConfig.VarianceMin = 0;
    StarterMeleeAttackConfig.VarianceMax = 4;
    StarterMeleeAttackConfig.CriticalChancePercent = 5;
    StarterMeleeAttackConfig.CriticalMultiplierPercent = 150;
}

void AHuwamPrototypePlayerActor::BeginPlay()
{
    Super::BeginPlay();

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
        CharacterCreation->ApplyCharacterCreation(
            CharacterCreation->BuildDefaultCharacterCreationRequest(),
            CharacterStats,
            CreationResult
        );
    }
    else if (CharacterStats)
    {
        CharacterStats->ResetToProgressionZeroDefaults();
    }

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

    if (bStartSlimeQuestOnInitialize)
    {
        StartSlimeQuest();
    }

    RefreshPrototypeHud();
}

bool AHuwamPrototypePlayerActor::GrantStarterLoadout()
{
    bool bChanged = false;
    bChanged |= AddInventoryItemIfMissing(BasicSwordId, 1);
    bChanged |= AddInventoryItemIfMissing(BasicKnifeId, 1);
    bChanged |= AddInventoryItemIfMissing(AdventuringBackpackId, 1);
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

FHuwamHudSnapshot AHuwamPrototypePlayerActor::RefreshPrototypeHud()
{
    return HudData ? HudData->RefreshHudSnapshot() : FHuwamHudSnapshot();
}

UHuwamInventoryComponent* AHuwamPrototypePlayerActor::GetInventoryComponent() const
{
    return Inventory;
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
