#include "AlignmentEngine.h"

void FAlignmentEngine::Initialize()
{
    // Alignment engine defines Dungeons & Dragons moral and ethical behavior.
}

void FAlignmentEngine::Update(float DeltaTime)
{
    // In a full implementation this would iterate through NPCs and adjust their alignment.
}

void FAlignmentEngine::AdjustAlignmentFromEvents(FNPCAttributes& NPC, const TArray<EWorldEventType>& PendingEvents)
{
    if (!NPC.bIsAlive)
    {
        return;
    }

    for (EWorldEventType EventType : PendingEvents)
    {
        switch (EventType)
        {
        case EWorldEventType::VillageDestroyed:
            NPC.Morality = FMath::Clamp(NPC.Morality - 0.05f, 0.0f, 1.0f);
            break;
        case EWorldEventType::VillageRebuilt:
            NPC.Morality = FMath::Clamp(NPC.Morality + 0.07f, 0.0f, 1.0f);
            break;
        case EWorldEventType::Festival:
            NPC.Morality = FMath::Clamp(NPC.Morality + 0.02f, 0.0f, 1.0f);
            break;
        default:
            break;
        }
    }

    if (NPC.Morality > 0.75f)
    {
        NPC.Alignment = EAlignment::NeutralGood;
    }
    else if (NPC.Morality < 0.25f)
    {
        NPC.Alignment = EAlignment::NeutralEvil;
    }
}
