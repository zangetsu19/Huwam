#pragma once

#include "CoreMinimal.h"

class FAIEngineBase
{
public:
    virtual ~FAIEngineBase() = default;

    virtual void Initialize() = 0;
    virtual void Update(float DeltaTime) = 0;
};
