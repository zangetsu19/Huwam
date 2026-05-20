#include "AIOverseer.h"
#include "FreewillEngine.h"
#include "WeatherEngine.h"
#include "EconomyEngine.h"
#include "LifecycleEngine.h"
#include "AlignmentEngine.h"
#include "KingdomEngine.h"
#include "GovernorEngine.h"
#include "NPCManagerEngine.h"

void FAIOverseer::Initialize()
{
    FreewillEngine = MakeUnique<FFreewillEngine>();
    WeatherEngine = MakeUnique<FWeatherEngine>();
    EconomyEngine = MakeUnique<FEconomyEngine>();
    LifecycleEngine = MakeUnique<FLifecycleEngine>();
    AlignmentEngine = MakeUnique<FAlignmentEngine>();
    KingdomEngine = MakeUnique<FKingdomEngine>();
    GovernorEngine = MakeUnique<FGovernorEngine>();
    NPCManagerEngine = MakeUnique<FNPCManagerEngine>();

    SetupInitialWorld();

    FreewillEngine->Initialize();
    WeatherEngine->Initialize();
    EconomyEngine->Initialize();
    LifecycleEngine->Initialize();
    AlignmentEngine->Initialize();
    KingdomEngine->Initialize();
    GovernorEngine->Initialize();
    NPCManagerEngine->Initialize();
}

void FAIOverseer::Update(float DeltaTime)
{
    WorldState.CurrentTick++;

    WeatherEngine->SimulateWeather(WorldState.Weather, WorldState.CurrentTick);
    LifecycleEngine->AdvanceLifecycle(WorldState);

    for (FNPCAttributes& NPC : WorldState.NPCPopulation)
    {
        if (!NPC.bIsAlive)
        {
            continue;
        }

        AlignmentEngine->AdjustAlignmentFromEvents(NPC, WorldState.PendingEvents);
        FreewillEngine->ApplyFreewill(NPC, WorldState);
    }

    KingdomEngine->ManageAgriculture(WorldState);
    KingdomEngine->ManageBuildings(WorldState);
    KingdomEngine->ManageTerrain(WorldState);
    KingdomEngine->ManagePopulationDensity(WorldState);
    KingdomEngine->ApplyWeatherEffects(WorldState);
    KingdomEngine->UpdateEconomyDetails(WorldState);
    KingdomEngine->DistributeSkills(WorldState);

    GovernorEngine->ManageQuests(WorldState);
    GovernorEngine->ManageInventories(WorldState);
    GovernorEngine->UpdateFarmableAreas(WorldState);
    GovernorEngine->UpdateHUDs(WorldState);
    GovernorEngine->UpdateLeaderboards(WorldState);

    NPCManagerEngine->UpdateNPCJobs(WorldState);

    AlignmentEngine->Update(DeltaTime);
    FreewillEngine->Update(DeltaTime);
    EconomyEngine->Update(DeltaTime);
    KingdomEngine->Update(DeltaTime);
    GovernorEngine->Update(DeltaTime);
    NPCManagerEngine->Update(DeltaTime);

    ProcessPendingEvents();
}

void FAIOverseer::RecordPlayerAction(const FString& ActionDescription, int32 ImpactLevel)
{
    if (ImpactLevel >= 5)
    {
        WorldState.PendingEvents.Add(EWorldEventType::VillageDestroyed);
        EconomyEngine->ApplyPlayerImpact(ImpactLevel, WorldState.Economy, WorldState.PendingEvents);
    }
}

void FAIOverseer::TriggerWorldEvent(EWorldEventType EventType)
{
    WorldState.PendingEvents.Add(EventType);
}

const FWorldState& FAIOverseer::GetWorldState() const
{
    return WorldState;
}

void FAIOverseer::SetupInitialWorld()
{
    WorldState.CurrentTick = 0;
    WorldState.Weather.CurrentWeather = EWeatherType::Clear;
    WorldState.Weather.Temperature = 21.0f;
    WorldState.Weather.WindIntensity = 0.1f;
    WorldState.Economy.WealthIndex = 1.0f;
    WorldState.Economy.Inflation = 1.0f;
    WorldState.Economy.RebuildingTasks = 0;

    // Initialize kingdom state
    WorldState.Agriculture.CropHealth = 1.0f;
    WorldState.Agriculture.GrowthRate = 1.0f;
    WorldState.Agriculture.HarvestYield = 100;

    WorldState.Buildings.Add(FBuildingState{EBuildingType::House, 0.0f, false});
    WorldState.Buildings.Add(FBuildingState{EBuildingType::Farm, 0.0f, false});

    WorldState.Terrain.FloraDensity = 1.0f;
    WorldState.Terrain.FaunaDensity = 1.0f;
    WorldState.Terrain.MonsterPopulation = 10;

    WorldState.Trades.Add(FTradeState{ETradeType::Agriculture, 1.0f, 1.0f, 1.0f});
    WorldState.Trades.Add(FTradeState{ETradeType::Blacksmith, 1.0f, 1.0f, 1.0f});

    WorldState.Skills.Add(FSkillDistribution{ESkillType::Farming, 1, 1, false});
    WorldState.Skills.Add(FSkillDistribution{ESkillType::Combat, 1, 1, false});

    // Initialize governor state
    FQuest InitialQuest;
    InitialQuest.Description = TEXT("Welcome quest: Explore the village.");
    InitialQuest.Type = EQuestType::Explore;
    InitialQuest.RewardExperience = 50;
    InitialQuest.RewardGold = 25;
    WorldState.ActiveQuests.Add(MoveTemp(InitialQuest));

    FInventory NPCInv;
    NPCInv.OwnerId = FGuid::NewGuid();
    NPCInv.Items.Add(FInventoryItem{EMaterialType::Wood, 10});
    WorldState.NPCInventories.Add(MoveTemp(NPCInv));

    FInventory ChestInv;
    ChestInv.OwnerId = FGuid::NewGuid();
    ChestInv.Items.Add(FInventoryItem{EMaterialType::Gold, 5});
    WorldState.ChestInventories.Add(MoveTemp(ChestInv));

    WorldState.FarmableAreas.Add(FFarmableArea{FVector(100, 100, 0), EMaterialType::Wood, true, 0, 100});
    WorldState.FarmableAreas.Add(FFarmableArea{FVector(200, 200, 0), EMaterialType::Stone, true, 0, 150});

    FHUDState PlayerHUD;
    PlayerHUD.PlayerId = FGuid::NewGuid();
    PlayerHUD.PlayerName = TEXT("Player1");
    PlayerHUD.CurrentJob = ENPCJob::Newbie;
    PlayerHUD.SelectedJobs = {};
    PlayerHUD.Skills.Add(FSkillDistribution{ESkillType::Farming, 1, 1, false});
    PlayerHUD.Skills.Add(FSkillDistribution{ESkillType::Crafting, 1, 1, false});
    PlayerHUD.Skills.Add(FSkillDistribution{ESkillType::Combat, 1, 1, false});
    PlayerHUD.Skills.Add(FSkillDistribution{ESkillType::Magic, 1, 1, false});
    PlayerHUD.Skills.Add(FSkillDistribution{ESkillType::Diplomacy, 1, 1, false});
    PlayerHUD.Skills.Add(FSkillDistribution{ESkillType::Survival, 1, 1, false});
    PlayerHUD.Skills.Add(FSkillDistribution{ESkillType::Trade, 1, 1, false});
    PlayerHUD.Skills.Add(FSkillDistribution{ESkillType::Leadership, 1, 1, false});
    WorldState.PlayerHUDs.Add(MoveTemp(PlayerHUD));

    FLeaderboard ExpBoard;
    ExpBoard.Type = ELeaderboardType::Experience;
    WorldState.Leaderboards.Add(MoveTemp(ExpBoard));

    for (int32 Index = 0; Index < 20; ++Index)
    {
        FNPCAttributes NPC;
        NPC.Name = FString::Printf(TEXT("NPC_%d"), Index + 1);
        // Random race
        TArray<ENPCRace> Races = {ENPCRace::Human, ENPCRace::Elf, ENPCRace::Dwarf, ENPCRace::Halfling, ENPCRace::Orc, ENPCRace::Ogre, ENPCRace::Beastfolk, ENPCRace::Demon, ENPCRace::Angelic, ENPCRace::Spirit, ENPCRace::Fae, ENPCRace::Virus};
        NPC.Race = Races[FMath::RandRange(0, Races.Num() - 1)];
        NPC.Age = 18 + Index;
        NPC.LifeStage = ENPCLifeStage::Adult;
        NPC.BirthTick = 0;
        NPCManagerEngine->GenerateNPC(NPC);
        WorldState.NPCPopulation.Add(MoveTemp(NPC));
    }
}

void FAIOverseer::ProcessPendingEvents()
{
    for (EWorldEventType Event : WorldState.PendingEvents)
    {
        switch (Event)
        {
        case EWorldEventType::VillageDestroyed:
            WorldState.Economy.RebuildingTasks += 1;
            WorldState.Economy.WealthIndex *= 0.98f;
            break;

        case EWorldEventType::VillageRebuilt:
            WorldState.Economy.RebuildingTasks = FMath::Max(0, WorldState.Economy.RebuildingTasks - 1);
            WorldState.Economy.WealthIndex *= 1.02f;
            break;

        default:
            break;
        }
    }

    WorldState.PendingEvents.Empty();
}
