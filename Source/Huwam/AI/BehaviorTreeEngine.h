#pragma once

#include "AIEngineBase.h"

class FBehaviorTreeEngine : public FAIEngineBase
{
public:
    virtual void Initialize() override;
    virtual void Update(float DeltaTime) override;

private:
    void EvaluateBehaviorTree();
};
