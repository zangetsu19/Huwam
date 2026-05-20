#pragma once

#include "CoreMinimal.h"
#include "AIEngineBase.h"
#include "WorldState.h"

class FGovernorEngine : public FAIEngineBase
{
public:
    virtual ~FGovernorEngine() override = default;

    virtual void Initialize() override;
    virtual void Update(float DeltaTime) override;

    void ManageQuests(FWorldState& WorldState);
    void HandleQuestCompletion(FQuest& Quest, FHUDState& PlayerHUD);
    void ManageInventories(FWorldState& WorldState);
    void UpdateFarmableAreas(FWorldState& WorldState);
    void UpdateHUDs(FWorldState& WorldState);
    void UpdateLeaderboards(FWorldState& WorldState);
};