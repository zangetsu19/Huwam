#pragma once

#include "AIEngineBase.h"
#include "WorldState.h"

class FAlignmentEngine : public FAIEngineBase
{
public:
    virtual void Initialize() override;
    virtual void Update(float DeltaTime) override;

    void AdjustAlignmentFromEvents(FNPCAttributes& NPC, const TArray<EWorldEventType>& PendingEvents);
};
