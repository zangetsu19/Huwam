#pragma once

#include "AIEngineBase.h"
#include "WorldState.h"

class FFreewillEngine : public FAIEngineBase
{
public:
    virtual void Initialize() override;
    virtual void Update(float DeltaTime) override;

    void ApplyFreewill(FNPCAttributes& NPC, const FWorldState& WorldState);
};
