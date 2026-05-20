#pragma once

#include "CoreMinimal.h"
#include "AIEngineBase.h"
#include "WorldState.h"

class FNPCManagerEngine : public FAIEngineBase
{
public:
    virtual ~FNPCManagerEngine() override = default;

    virtual void Initialize() override;
    virtual void Update(float DeltaTime) override;

    void GenerateNPC(FNPCAttributes& NPC);
    void AssignRaceAttributes(FNPCAttributes& NPC);
    void AssignJobAndAmbition(FNPCAttributes& NPC);
    FJobData GetJobData(ENPCJob Job);
    void UpdateNPCJobs(FWorldState& WorldState);
};