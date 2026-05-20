#include "FreewillEngine.h"

void FFreewillEngine::Initialize()
{
    // Freewill engine is ready to assign intentions and drive NPC choices.
}

void FFreewillEngine::Update(float DeltaTime)
{
    // In a full simulation this method would iterate over NPCs and adjust goals.
}

void FFreewillEngine::ApplyFreewill(FNPCAttributes& NPC, const FWorldState& WorldState)
{
    if (!NPC.bIsAlive)
    {
        return;
    }

    float Influence = WorldState.Economy.WealthIndex * 0.1f;
    float AlignmentBias = static_cast<float>(static_cast<uint8>(NPC.Alignment)) / 8.0f;
    NPC.Morality = FMath::Clamp(NPC.Morality + (AlignmentBias - 0.5f) * Influence * 0.01f, 0.0f, 1.0f);
}
