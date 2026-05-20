#pragma once

#include "AIEngineBase.h"
#include "WorldState.h"

class FEconomyEngine : public FAIEngineBase
{
public:
    virtual void Initialize() override;
    virtual void Update(float DeltaTime) override;

    void ApplyPlayerImpact(int32 ImpactLevel, FWorldEconomyState& EconomyState, TArray<EWorldEventType>& PendingEvents);
    void ScheduleRebuildTask(int32 Severity, FWorldEconomyState& EconomyState);
};
