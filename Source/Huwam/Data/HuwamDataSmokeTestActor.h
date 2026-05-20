#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HuwamDataRegistry.h"
#include "HuwamDataSmokeTestActor.generated.h"

UCLASS()
class HUWAM_API AHuwamDataSmokeTestActor : public AActor
{
    GENERATED_BODY()

public:
    AHuwamDataSmokeTestActor();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Huwam|Data")
    TObjectPtr<UHuwamDataRegistry> DataRegistry;

    void LogSmokeTestResult(const TCHAR* Label, bool bFound, const FText& DisplayName) const;
};
