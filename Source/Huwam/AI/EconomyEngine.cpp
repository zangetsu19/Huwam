#include "EconomyEngine.h"

void FEconomyEngine::Initialize()
{
    // Initialize economic multipliers and trade values.
}

void FEconomyEngine::Update(float DeltaTime)
{
    // Economic health changes slowly based on active rebuilding tasks.
}

void FEconomyEngine::ApplyPlayerImpact(int32 ImpactLevel, FWorldEconomyState& EconomyState, TArray<EWorldEventType>& PendingEvents)
{
    EconomyState.WealthIndex = FMath::Max(0.1f, EconomyState.WealthIndex - ImpactLevel * 0.02f);
    EconomyState.Inflation = FMath::Clamp(EconomyState.Inflation + ImpactLevel * 0.01f, 0.9f, 3.0f);

    if (ImpactLevel >= 5)
    {
        PendingEvents.Add(EWorldEventType::TradeCrash);
        ScheduleRebuildTask(ImpactLevel, EconomyState);
    }
}

void FEconomyEngine::ScheduleRebuildTask(int32 Severity, FWorldEconomyState& EconomyState)
{
    EconomyState.RebuildingTasks += FMath::Clamp(Severity / 2, 1, 5);
}
