#pragma once

#include "CoreMinimal.h"
#include "AIEngineBase.h"
#include "WorldState.h"

class FFreewillEngine;
class FWeatherEngine;
class FEconomyEngine;
class FLifecycleEngine;
class FAlignmentEngine;
class FKingdomEngine;
class FGovernorEngine;
class FNPCManagerEngine;

class FAIOverseer : public FAIEngineBase
{
public:
    virtual ~FAIOverseer() override = default;

    virtual void Initialize() override;
    virtual void Update(float DeltaTime) override;

    void RecordPlayerAction(const FString& ActionDescription, int32 ImpactLevel);
    void TriggerWorldEvent(EWorldEventType EventType);
    const FWorldState& GetWorldState() const;

private:
    void SetupInitialWorld();
    void ProcessPendingEvents();

    TUniquePtr<FFreewillEngine> FreewillEngine;
    TUniquePtr<FWeatherEngine> WeatherEngine;
    TUniquePtr<FEconomyEngine> EconomyEngine;
    TUniquePtr<FLifecycleEngine> LifecycleEngine;
    TUniquePtr<FAlignmentEngine> AlignmentEngine;
    TUniquePtr<FKingdomEngine> KingdomEngine;
    TUniquePtr<FGovernorEngine> GovernorEngine;
    TUniquePtr<FNPCManagerEngine> NPCManagerEngine;
    FWorldState WorldState;
};
