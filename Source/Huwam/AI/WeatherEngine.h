#pragma once

#include "AIEngineBase.h"
#include "WorldState.h"

class FWeatherEngine : public FAIEngineBase
{
public:
    virtual void Initialize() override;
    virtual void Update(float DeltaTime) override;

    void SimulateWeather(FWeatherState& WeatherState, int32 CurrentTick);
};
