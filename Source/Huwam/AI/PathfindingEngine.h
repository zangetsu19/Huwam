#pragma once

#include "AIEngineBase.h"
#include "Containers/Array.h"

struct FPathNode
{
    int32 X;
    int32 Y;
};

class FPathfindingEngine : public FAIEngineBase
{
public:
    virtual void Initialize() override;
    virtual void Update(float DeltaTime) override;

    TArray<FPathNode> FindPath(const FPathNode& Start, const FPathNode& Goal);
};
