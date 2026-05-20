#include "WeatherEngine.h"

void FWeatherEngine::Initialize()
{
    // Seed weather and climate parameters for the world simulation.
}

void FWeatherEngine::Update(float DeltaTime)
{
    // Weather updates are driven by the world tick in this system.
}

void FWeatherEngine::SimulateWeather(FWeatherState& WeatherState, int32 CurrentTick)
{
    if (CurrentTick % 10 == 0)
    {
        switch (WeatherState.CurrentWeather)
        {
        case EWeatherType::Clear:
            WeatherState.CurrentWeather = EWeatherType::Cloudy;
            break;
        case EWeatherType::Cloudy:
            WeatherState.CurrentWeather = EWeatherType::Rain;
            break;
        case EWeatherType::Rain:
            WeatherState.CurrentWeather = EWeatherType::Storm;
            break;
        case EWeatherType::Storm:
            WeatherState.CurrentWeather = EWeatherType::Clear;
            break;
        default:
            WeatherState.CurrentWeather = EWeatherType::Clear;
            break;
        }
    }

    WeatherState.Temperature += FMath::FRandRange(-0.5f, 0.5f);
    WeatherState.WindIntensity = FMath::Clamp(WeatherState.WindIntensity + FMath::FRandRange(-0.05f, 0.05f), 0.0f, 1.0f);
}
