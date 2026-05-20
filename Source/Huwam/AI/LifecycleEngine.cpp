#include "LifecycleEngine.h"

static ENPCLifeStage CalculateLifeStage(int32 Age)
{
    if (Age < 5)
    {
        return ENPCLifeStage::Infant;
    }
    if (Age < 13)
    {
        return ENPCLifeStage::Child;
    }
    if (Age < 25)
    {
        return ENPCLifeStage::YoungAdult;
    }
    if (Age < 60)
    {
        return ENPCLifeStage::Adult;
    }
    return ENPCLifeStage::Elder;
}

void FLifecycleEngine::Initialize()
{
    // Lifecycle engine starts with a basic population aging model.
}

void FLifecycleEngine::Update(float DeltaTime)
{
    // This method can be extended to run life-cycle checks on a timer.
}

void FLifecycleEngine::AdvanceLifecycle(FWorldState& WorldState)
{
    for (FNPCAttributes& NPC : WorldState.NPCPopulation)
    {
        if (!NPC.bIsAlive)
        {
            continue;
        }

        NPC.Age += 1;
        NPC.LifeStage = CalculateLifeStage(NPC.Age);

        if (NPC.Age >= NPC.Lifespan || NPC.Morality <= 0.0f)
        {
            NPC.bIsAlive = false;
            NPC.DeathTick = WorldState.CurrentTick;
        }
    }

    if (WorldState.CurrentTick % 20 == 0)
    {
        FNPCAttributes NewNPC;
        // Random race for new births
        TArray<ENPCRace> Races = {ENPCRace::Human, ENPCRace::Elf, ENPCRace::Dwarf, ENPCRace::Halfling, ENPCRace::Orc, ENPCRace::Ogre, ENPCRace::Beastfolk, ENPCRace::Demon, ENPCRace::Angelic, ENPCRace::Spirit, ENPCRace::Fae, ENPCRace::Virus};
        NewNPC.Race = Races[FMath::RandRange(0, Races.Num() - 1)];
        NewNPC.Name = FString::Printf(TEXT("Child_%d"), WorldState.NPCPopulation.Num() + 1);
        NewNPC.Alignment = EAlignment::TrueNeutral;
        NewNPC.Age = 0;
        NewNPC.LifeStage = ENPCLifeStage::Infant;
        // Lifespan will be set in NPCManagerEngine->GenerateNPC
        NewNPC.BirthTick = WorldState.CurrentTick;
        // Generate full attributes
        // Assuming NPCManagerEngine is accessible, but since it's not, set basic lifespan here
        switch (NewNPC.Race)
        {
        case ENPCRace::Human: NewNPC.Lifespan = 70; break;
        case ENPCRace::Elf: NewNPC.Lifespan = 300; break;
        case ENPCRace::Dwarf: NewNPC.Lifespan = 200; break;
        case ENPCRace::Halfling: NewNPC.Lifespan = 100; break;
        case ENPCRace::Orc: NewNPC.Lifespan = 60; break;
        case ENPCRace::Ogre: NewNPC.Lifespan = 80; break;
        case ENPCRace::Beastfolk: NewNPC.Lifespan = 90; break;
        case ENPCRace::Demon: NewNPC.Lifespan = 500; break;
        case ENPCRace::Angelic: NewNPC.Lifespan = 1000; break;
        case ENPCRace::Spirit: NewNPC.Lifespan = 200; break;
        case ENPCRace::Fae: NewNPC.Lifespan = 150; break;
        case ENPCRace::Virus: NewNPC.Lifespan = 200; break;
        default: NewNPC.Lifespan = 80; break;
        }
        WorldState.NPCPopulation.Add(MoveTemp(NewNPC));
    }
}
