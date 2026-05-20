#include "KingdomEngine.h"

void FKingdomEngine::Initialize()
{
    // Initialize kingdom management systems.
}

void FKingdomEngine::Update(float DeltaTime)
{
    // Kingdom engine updates are handled by the overseer.
}

void FKingdomEngine::ManageAgriculture(FWorldState& WorldState)
{
    // Simulate plant growth and health based on weather and economy.
    WorldState.Agriculture.GrowthRate = FMath::Clamp(WorldState.Agriculture.GrowthRate + (WorldState.Weather.Temperature - 20.0f) * 0.01f, 0.5f, 2.0f);
    WorldState.Agriculture.CropHealth = FMath::Clamp(WorldState.Agriculture.CropHealth - WorldState.Weather.WindIntensity * 0.05f, 0.0f, 1.0f);
    WorldState.Agriculture.HarvestYield = FMath::RoundToInt(100 * WorldState.Agriculture.GrowthRate * WorldState.Agriculture.CropHealth);
}

void FKingdomEngine::ManageBuildings(FWorldState& WorldState)
{
    for (FBuildingState& Building : WorldState.Buildings)
    {
        Building.DecayFactor += 0.001f; // Natural decay
        if (WorldState.Weather.CurrentWeather == EWeatherType::Storm)
        {
            Building.DecayFactor += 0.01f; // Storm damage
        }
        if (Building.DecayFactor >= 1.0f)
        {
            Building.bIsDestroyed = true;
        }
    }
}

void FKingdomEngine::ManageTerrain(FWorldState& WorldState)
{
    // Adjust flora and fauna based on weather and population.
    WorldState.Terrain.FloraDensity = FMath::Clamp(WorldState.Terrain.FloraDensity + (WorldState.Weather.Temperature - 20.0f) * 0.005f, 0.1f, 2.0f);
    WorldState.Terrain.FaunaDensity = FMath::Clamp(WorldState.Terrain.FaunaDensity + WorldState.Terrain.FloraDensity * 0.1f, 0.1f, 2.0f);
    WorldState.Terrain.MonsterPopulation = FMath::RoundToInt(10 * WorldState.Terrain.FaunaDensity);
}

void FKingdomEngine::ManagePopulationDensity(FWorldState& WorldState)
{
    // Adjust NPC and monster density based on resources.
    int32 AliveNPCs = 0;
    for (const FNPCAttributes& NPC : WorldState.NPCPopulation)
    {
        if (NPC.bIsAlive)
        {
            AliveNPCs++;
        }
    }
    // Simple density control: if too many NPCs, reduce growth or increase death rate indirectly.
}

void FKingdomEngine::ApplyWeatherEffects(FWorldState& WorldState)
{
    // Apply specific weather effects.
    if (WorldState.Weather.CurrentWeather == EWeatherType::Storm)
    {
        // Strong winds affect objects.
        for (FBuildingState& Building : WorldState.Buildings)
        {
            Building.DecayFactor += WorldState.Weather.WindIntensity * 0.02f;
        }
        // Lightning affects plants and living matter.
        WorldState.Agriculture.CropHealth -= 0.1f;
        for (FNPCAttributes& NPC : WorldState.NPCPopulation)
        {
            if (NPC.bIsAlive && FMath::FRand() < 0.01f) // 1% chance of lightning strike
            {
                NPC.bIsAlive = false;
                NPC.DeathTick = WorldState.CurrentTick;
            }
        }
    }
}

void FKingdomEngine::UpdateEconomyDetails(FWorldState& WorldState)
{
    for (FTradeState& Trade : WorldState.Trades)
    {
        // Simulate supply and demand.
        Trade.Demand = FMath::Clamp(Trade.Demand + FMath::FRandRange(-0.1f, 0.1f), 0.5f, 2.0f);
        Trade.Supply = FMath::Clamp(Trade.Supply + FMath::FRandRange(-0.1f, 0.1f), 0.5f, 2.0f);
        Trade.PriceMultiplier = Trade.Demand / Trade.Supply;
    }
}

void FKingdomEngine::DistributeSkills(FWorldState& WorldState)
{
    for (FSkillDistribution& Skill : WorldState.Skills)
    {
        if (!Skill.bUnlocked)
        {
            // Unlock skills based on world progress.
            if (WorldState.CurrentTick > 100)
            {
                Skill.bUnlocked = true;
            }
        }
        if (Skill.bUnlocked)
        {
            // Increase skill levels over time.
            Skill.NPCSkillLevel = FMath::Min(Skill.NPCSkillLevel + 1, 10);
            Skill.PlayerSkillLevel = FMath::Min(Skill.PlayerSkillLevel + 1, 10);
        }
    }
}