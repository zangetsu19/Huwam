#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HuwamTimeSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FHuwamTimeSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Time")
    FString RealUtcTimestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Time")
    int64 RealUtcTicks = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Time")
    float SecondsPerInGameDay = 86400.0f / 7.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Time")
    float SecondsPerInGameWeek = 86400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Time")
    int32 TotalInGameDays = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Time")
    int32 CurrentWeekIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Time")
    int32 DayOfWeekIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Time")
    int32 HourOfDay = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Time")
    int32 MinuteOfHour = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Time")
    bool bTimeSkippingAllowed = false;
};

UCLASS(BlueprintType, Config=Game)
class HUWAM_API UHuwamTimeSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Time")
    FHuwamTimeSnapshot GetCurrentTimeSnapshot() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Time")
    float GetSecondsPerInGameDay() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Time")
    float GetSecondsPerInGameWeek() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Time")
    float GetRealSecondsForInGameDays(int32 InGameDays) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Time")
    int32 GetInGameDaysForRealSeconds(float RealSeconds) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Time")
    bool IsTimeSkippingAllowed() const;

private:
    UPROPERTY(Config, EditAnywhere, Category = "Huwam|Time", meta = (ClampMin = "1.0"))
    float RealSecondsPerInGameWeek = 86400.0f;

    UPROPERTY(Config, EditAnywhere, Category = "Huwam|Time", meta = (ClampMin = "1"))
    int32 InGameDaysPerWeek = 7;

    UPROPERTY(Config, EditAnywhere, Category = "Huwam|Time")
    bool bAllowTimeSkipping = false;

    FDateTime WorldEpochUtc;
};
