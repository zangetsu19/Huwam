#pragma once

#include "AIEngineBase.h"
#include "WorldState.h"

class FLifecycleEngine : public FAIEngineBase
{
public:
    virtual void Initialize() override;
    virtual void Update(float DeltaTime) override;

    void AdvanceLifecycle(FWorldState& WorldState);
};
