#pragma once

#include "CoreMinimal.h"
#include "AIEngineBase.h"
#include "WorldState.h"

class FKingdomEngine : public FAIEngineBase
{
public:
    virtual ~FKingdomEngine() override = default;

    virtual void Initialize() override;
    virtual void Update(float DeltaTime) override;

    void ManageAgriculture(FWorldState& WorldState);
    void ManageBuildings(FWorldState& WorldState);
    void ManageTerrain(FWorldState& WorldState);
    void ManagePopulationDensity(FWorldState& WorldState);
    void ApplyWeatherEffects(FWorldState& WorldState);
    void UpdateEconomyDetails(FWorldState& WorldState);
    void DistributeSkills(FWorldState& WorldState);
};