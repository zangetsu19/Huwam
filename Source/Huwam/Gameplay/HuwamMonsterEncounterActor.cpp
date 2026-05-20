#include "Gameplay/HuwamMonsterEncounterActor.h"

#include "Components/SceneComponent.h"
#include "Data/HuwamDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Gameplay/HuwamCharacterStatsComponent.h"
#include "Gameplay/HuwamInventoryComponent.h"

namespace
{
    const TCHAR* StatStrength = TEXT("stat.strength");
    const TCHAR* StatCharisma = TEXT("stat.charisma");
    const TCHAR* StatIntelligence = TEXT("stat.intelligence");
    const TCHAR* StatWisdom = TEXT("stat.wisdom");
    const TCHAR* StatDexterity = TEXT("stat.dexterity");
    const TCHAR* StatLuck = TEXT("stat.luck");
    const TCHAR* StatHealth = TEXT("stat.health");
    const TCHAR* StatMana = TEXT("stat.mana");
    const TCHAR* StatDefense = TEXT("stat.defense");
    const TCHAR* StatWorldPopularity = TEXT("stat.world_popularity");

    FName GetRowNameFromId(const FString& Id)
    {
        return FName(*Id.Replace(TEXT("."), TEXT("_")));
    }
}

AHuwamMonsterEncounterActor::AHuwamMonsterEncounterActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    CharacterStats = CreateDefaultSubobject<UHuwamCharacterStatsComponent>(TEXT("CharacterStats"));
    Combat = CreateDefaultSubobject<UHuwamCombatComponent>(TEXT("Combat"));

    MonsterDisplayName = NSLOCTEXT("HuwamMonster", "BasicSlimeDisplayName", "Basic Slime");

    AttackRollConfig.VarianceMin = 0;
    AttackRollConfig.VarianceMax = 2;
    AttackRollConfig.CriticalChancePercent = 2;
    AttackRollConfig.CriticalMultiplierPercent = 150;
}

void AHuwamMonsterEncounterActor::BeginPlay()
{
    Super::BeginPlay();

    if (bAutoInitializeFromData)
    {
        InitializeFromMonsterData();
    }
}

bool AHuwamMonsterEncounterActor::InitializeFromMonsterData()
{
    FHuwamMonsterRow MonsterRow;
    if (LoadMonsterRow(MonsterRow))
    {
        return ApplyMonsterRow(MonsterRow);
    }

    ApplyBasicSlimeFallback();
    return false;
}

bool AHuwamMonsterEncounterActor::AttackTarget(UHuwamCombatComponent* Target, FHuwamCombatResult& OutResult)
{
    if (!Combat)
    {
        OutResult = FHuwamCombatResult();
        return false;
    }

    return Combat->AttackTarget(Target, EHuwamCombatAttackType::Melee, AttackRollConfig, OutResult);
}

bool AHuwamMonsterEncounterActor::ReceiveAttackFrom(UHuwamCombatComponent* Attacker, EHuwamCombatAttackType AttackType, const FHuwamCombatRollConfig& RollConfig, FHuwamCombatResult& OutResult)
{
    if (!Attacker || !Combat)
    {
        OutResult = FHuwamCombatResult();
        return false;
    }

    return Attacker->AttackTarget(Combat, AttackType, RollConfig, OutResult);
}

bool AHuwamMonsterEncounterActor::GrantDefeatRewards(UHuwamInventoryComponent* RecipientInventory, TArray<FHuwamIdQuantity>& OutRewardItems, int32& OutGoldReward, int32& OutExperienceReward)
{
    OutRewardItems = RewardItems;
    OutGoldReward = GoldReward;
    OutExperienceReward = ExperienceReward;

    if (!Combat || !Combat->IsDefeated() || bRewardsClaimed)
    {
        return false;
    }

    if (!RewardItems.IsEmpty() && !RecipientInventory)
    {
        return false;
    }

    if (RecipientInventory)
    {
        for (const FHuwamIdQuantity& RewardItem : RewardItems)
        {
            if (RewardItem.Id.IsEmpty() || RewardItem.Quantity <= 0)
            {
                continue;
            }

            int32 Remainder = 0;
            if (!RecipientInventory->AddItemById(RewardItem.Id, RewardItem.Quantity, Remainder) || Remainder > 0)
            {
                return false;
            }
        }
    }

    bRewardsClaimed = true;
    return true;
}

bool AHuwamMonsterEncounterActor::IsDefeated() const
{
    return Combat && Combat->IsDefeated();
}

FString AHuwamMonsterEncounterActor::GetMonsterId() const
{
    return MonsterId;
}

FText AHuwamMonsterEncounterActor::GetMonsterDisplayName() const
{
    return MonsterDisplayName;
}

UHuwamCombatComponent* AHuwamMonsterEncounterActor::GetCombatComponent() const
{
    return Combat;
}

UHuwamCharacterStatsComponent* AHuwamMonsterEncounterActor::GetCharacterStatsComponent() const
{
    return CharacterStats;
}

bool AHuwamMonsterEncounterActor::ApplyMonsterRow(const FHuwamMonsterRow& MonsterRow)
{
    MonsterId = MonsterRow.MonsterId;
    MonsterDisplayName = MonsterRow.DisplayName;
    Rank = MonsterRow.Rank;
    bTamable = MonsterRow.bTamable;
    bEdible = MonsterRow.bEdible;
    GoldReward = MonsterRow.GoldReward;
    ExperienceReward = MonsterRow.ExperienceReward;
    bRewardsClaimed = false;

    RewardItems = MonsterRow.Drops;
    if (!MonsterRow.PrimaryDropItemId.IsEmpty() && MonsterRow.PrimaryDropQuantity > 0)
    {
        FHuwamIdQuantity PrimaryDrop;
        PrimaryDrop.Id = MonsterRow.PrimaryDropItemId;
        PrimaryDrop.Quantity = MonsterRow.PrimaryDropQuantity;
        RewardItems.Add(PrimaryDrop);
    }

    SetMonsterStats(
        MonsterRow.BaseStrength,
        MonsterRow.BaseDexterity,
        MonsterRow.BaseHealth,
        MonsterRow.BaseMana,
        MonsterRow.BaseDefense,
        MonsterRow.BaseLuck
    );

    if (Combat)
    {
        Combat->InitializeFromStats(CharacterStats, nullptr, true);
    }

    return true;
}

void AHuwamMonsterEncounterActor::ApplyBasicSlimeFallback()
{
    MonsterId = TEXT("monster.slime.basic");
    MonsterDisplayName = NSLOCTEXT("HuwamMonster", "BasicSlimeFallbackDisplayName", "Basic Slime");
    Rank = EHuwamRank::F;
    bTamable = true;
    bEdible = false;
    GoldReward = 3;
    ExperienceReward = 5;
    bRewardsClaimed = false;

    RewardItems.Reset();
    FHuwamIdQuantity SlimeCore;
    SlimeCore.Id = TEXT("material.slime_core.basic");
    SlimeCore.Quantity = 1;
    RewardItems.Add(SlimeCore);

    SetMonsterStats(2, 3, 2, 0, 1, 2);

    if (Combat)
    {
        Combat->InitializeFromStats(CharacterStats, nullptr, true);
    }
}

bool AHuwamMonsterEncounterActor::LoadMonsterRow(FHuwamMonsterRow& OutMonsterRow) const
{
    if (MonsterId.IsEmpty())
    {
        return false;
    }

    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (const UHuwamDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UHuwamDataSubsystem>())
            {
                return DataSubsystem->GetMonsterRow(GetRowNameFromId(MonsterId), OutMonsterRow);
            }
        }
    }

    return false;
}

void AHuwamMonsterEncounterActor::SetMonsterStats(int32 Strength, int32 Dexterity, int32 Health, int32 Mana, int32 Defense, int32 Luck)
{
    if (!CharacterStats)
    {
        return;
    }

    CharacterStats->ResetToProgressionZeroDefaults();
    CharacterStats->SetBaseStat(StatStrength, Strength);
    CharacterStats->SetBaseStat(StatCharisma, 0);
    CharacterStats->SetBaseStat(StatIntelligence, 0);
    CharacterStats->SetBaseStat(StatWisdom, 1);
    CharacterStats->SetBaseStat(StatDexterity, Dexterity);
    CharacterStats->SetBaseStat(StatLuck, Luck);
    CharacterStats->SetBaseStat(StatHealth, Health);
    CharacterStats->SetBaseStat(StatMana, Mana);
    CharacterStats->SetBaseStat(StatDefense, Defense);
    CharacterStats->SetBaseStat(StatWorldPopularity, 0);
}
