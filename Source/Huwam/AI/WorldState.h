#pragma once

#include "CoreMinimal.h"

enum class EAlignment : uint8
{
    LawfulGood,
    NeutralGood,
    ChaoticGood,
    LawfulNeutral,
    TrueNeutral,
    ChaoticNeutral,
    LawfulEvil,
    NeutralEvil,
    ChaoticEvil
};

enum class ENPCLifeStage : uint8
{
    Infant,
    Child,
    YoungAdult,
    Adult,
    Elder,
    Deceased
};

enum class EWeatherType : uint8
{
    Clear,
    Cloudy,
    Rain,
    Storm,
    Snow,
    Blizzard,
    Heatwave,
    Drought
};

enum class EWorldEventType : uint8
{
    VillageDestroyed,
    VillageRebuilt,
    Famine,
    Festival,
    TradeBoom,
    TradeCrash,
    GreatStorm,
    PeaceTreaty
};

enum class ENPCRace : uint8
{
    Human,
    Elf,
    Dwarf,
    Halfling,
    Orc,
    Ogre,
    Beastfolk,
    Demon,
    Angelic,
    Spirit,
    Fae,
    Virus
};

enum class ENPCSubRace : uint8
{
    None,
    // Human sub-races
    Noble,
    Peasant,
    // Elf sub-races
    HighElf,
    WoodElf,
    DarkElf,
    // Dwarf sub-races
    MountainDwarf,
    ForgeDwarf,
    QuarryDwarf,
    DeepDwarf,
    GoldbeardDwarf,
    IronOathDwarf,
    DreamforgeDwarf,
    HearthDwarf,
    // Halfling sub-races
    Lightfoot,
    Stout,
    // Orc sub-races
    CommonOrc,
    HalfOrc,
    WarOrc,
    StonehideOrc,
    RedtuskOrc,
    GrayOrc,
    IronboundOrc,
    WildOrc,
    // Ogre sub-races
    HillOgre,
    SwampOgre,
    // Beastfolk sub-races
    Catfolk,
    Wolfkin,
    Tigerfolk,
    Lionkin,
    Pantherkin,
    // Demon sub-races
    Imp,
    Fiend,
    // Angelic sub-races
    Seraph,
    Cherub,
    // Spirit sub-races
    Elemental,
    Ghost,
    // Fae sub-races
    Pixie,
    Sprite,
    Faun,
    ArchFey,
    // Virus sub-races
    Vampire,
    Werewolf,
    Hag,
    Warlock
};

enum class ENPCJob : uint8
{
    Newbie,
    // D&D inspired
    Thief,
    Sorcerer,
    Wizard,
    Warlock,
    Barbarian,
    Fighter,
    // Homebrew
    Inventor,
    Cleric,
    Paladin,
    Assassin,
    Farmer,
    Blacksmith,
    Demonlord,
    Hero,
    AntiHero,
    Villain,
    Criminal,
    Adventurer,
    SlowLife,
    Nobility,
    Tamer,
    Archer,
    Construction,
    Pirate,
    SeaCaptain,
    Breeder,
    ShopKeeper,
    Merchant,
    Waitress,
    Guard,
    Knight,
    Mage,
    Priest,
    Spy,
    // Fallback
    Unemployed
};

enum class EMaterialType : uint8
{
    Wood,
    Stone,
    Iron,
    Gold,
    Herbs,
    Food
};

enum class ESkillType : uint8
{
    Farming,
    Crafting,
    Combat,
    Magic,
    Diplomacy,
    Survival,
    Trade,
    Leadership,
    Knowledge
};

struct FInventoryItem
{
    EMaterialType Type = EMaterialType::Wood;
    int32 Quantity = 0;
};

struct FJobData
{
    ENPCJob Job = ENPCJob::Unemployed;
    int32 Strength = 10;
    int32 Dexterity = 10;
    int32 Constitution = 10;
    int32 Intelligence = 10;
    int32 Wisdom = 10;
    int32 Charisma = 10;
    TArray<ESkillType> Skills;
    TArray<FInventoryItem> StartingInventory;
    int32 StartingWealth = 0;
    EAlignment TypicalAlignment = EAlignment::TrueNeutral;
    FString Gear = TEXT("");
    int32 Difficulty = 1; // 1-10 scale
};

enum class ENPCAmbition : uint8
{
    Wealth,
    Power,
    Knowledge,
    Peace,
    Adventure,
    Family,
    None
};

enum class EQuestType : uint8
{
    Gather,
    Deliver,
    Kill,
    Explore,
    Craft,
    Escort
};

enum class ELeaderboardType : uint8
{
    Experience,
    Wealth,
    QuestsCompleted,
    MonstersKilled
};

struct FQuest
{
    FGuid QuestId = FGuid::NewGuid();
    FString Description = TEXT("");
    EQuestType Type = EQuestType::Gather;
    int32 RewardExperience = 0;
    int32 RewardGold = 0;
    bool bIsCompleted = false;
    FGuid AssignedPlayerId = FGuid();
};

struct FInventory
{
    FGuid OwnerId = FGuid::NewGuid();
    TArray<FInventoryItem> Items;
};

struct FFarmableArea
{
    FVector Location = FVector::ZeroVector;
    EMaterialType Material = EMaterialType::Wood;
    bool bIsFarmable = true;
    int32 CooldownTicks = 0;
    int32 MaxCooldownTicks = 100;
};

struct FPlayerJobProgress
{
    ENPCJob Job = ENPCJob::Unemployed;
    int32 Progress = 0;
    int32 RequiredProgress = 5;
    bool bUnlocked = false;
};

struct FSkillDistribution
{
    ESkillType Skill = ESkillType::Farming;
    int32 NPCSkillLevel = 1;
    int32 PlayerSkillLevel = 1;
    bool bUnlocked = false;
};

struct FHUDState
{
    FGuid PlayerId = FGuid::NewGuid();
    FString PlayerName = TEXT("");
    ENPCJob CurrentJob = ENPCJob::Newbie;
    TArray<ENPCJob> SelectedJobs; // Players may choose up to 3 jobs at a time.
    TArray<FPlayerJobProgress> JobProgress;
    TArray<FSkillDistribution> Skills;
    int32 Health = 100;
    int32 Experience = 0;
    int32 Gold = 0;
    bool bHasIsekaiLegacy = false;
    TArray<FString> ActiveQuests;
};

struct FLeaderboardEntry
{
    FGuid PlayerId = FGuid::NewGuid();
    FString PlayerName = TEXT("");
    int32 Score = 0;
};

struct FLeaderboard
{
    ELeaderboardType Type = ELeaderboardType::Experience;
    TArray<FLeaderboardEntry> Entries;
};

enum class ETradeType : uint8
{
    Agriculture,
    Blacksmith,
    Alchemy,
    Tailoring,
    Carpentry,
    Mining,
    Fishing,
    Hunting
};

enum class EBuildingType : uint8
{
    House,
    Farm,
    Shop,
    Temple,
    Castle,
    Wall,
    Bridge
};

struct FAgricultureState
{
    float CropHealth = 1.0f;
    float GrowthRate = 1.0f;
    int32 HarvestYield = 100;
};

struct FBuildingState
{
    EBuildingType Type = EBuildingType::House;
    float DecayFactor = 0.0f;
    bool bIsDestroyed = false;
};

struct FTerrainState
{
    float FloraDensity = 1.0f;
    float FaunaDensity = 1.0f;
    int32 MonsterPopulation = 10;
};

struct FTradeState
{
    ETradeType Type = ETradeType::Agriculture;
    float Demand = 1.0f;
    float Supply = 1.0f;
    float PriceMultiplier = 1.0f;
};

struct FNPCAttributes
{
    FGuid NPCId = FGuid::NewGuid();
    FString Name = TEXT("Unknown");
    ENPCRace Race = ENPCRace::Human;
    ENPCSubRace SubRace = ENPCSubRace::None;
    EAlignment Alignment = EAlignment::TrueNeutral;
    ENPCLifeStage LifeStage = ENPCLifeStage::Infant;
    ENPCJob Job = ENPCJob::Unemployed;
    ENPCAmbition Ambition = ENPCAmbition::None;
    int32 Age = 0;
    int32 Lifespan = 80;
    bool bIsAlive = true;
    float Morality = 0.5f;
    int32 BirthTick = 0;
    int32 DeathTick = -1;
    int32 Wealth = 0;
    int32 Health = 100;
    int32 Experience = 0;
    FJobData JobData;
    TArray<FInventoryItem> Inventory;
    TArray<FQuest> AvailableQuests;
};

struct FWorldEconomyState
{
    float WealthIndex = 1.0f;
    float Inflation = 1.0f;
    int32 RebuildingTasks = 0;
};

struct FWeatherState
{
    EWeatherType CurrentWeather = EWeatherType::Clear;
    float Temperature = 20.0f;
    float WindIntensity = 0.1f;
};

struct FWorldState
{
    TArray<FNPCAttributes> NPCPopulation;
    FWorldEconomyState Economy;
    FWeatherState Weather;
    int32 CurrentTick = 0;
    TArray<EWorldEventType> PendingEvents;

    // Kingdom engine state
    FAgricultureState Agriculture;
    TArray<FBuildingState> Buildings;
    FTerrainState Terrain;
    TArray<FTradeState> Trades;
    TArray<FSkillDistribution> Skills;

    // Governor engine state
    TArray<FQuest> ActiveQuests;
    TArray<FInventory> NPCInventories;
    TArray<FInventory> ChestInventories;
    TArray<FFarmableArea> FarmableAreas;
    TArray<FHUDState> PlayerHUDs;
    TArray<FLeaderboard> Leaderboards;
};
