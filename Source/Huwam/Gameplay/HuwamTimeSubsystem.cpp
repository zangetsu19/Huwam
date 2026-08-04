#include "Gameplay/HuwamTimeSubsystem.h"

namespace
{
    constexpr int32 HoursPerDay = 24;
    constexpr int32 MinutesPerHour = 60;
    constexpr int32 SecondsPerMinute = 60;
}

void UHuwamTimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    WorldEpochUtc = FDateTime::UtcNow();
}

FHuwamTimeSnapshot UHuwamTimeSubsystem::GetCurrentTimeSnapshot() const
{
    const FDateTime CurrentUtc = FDateTime::UtcNow();
    const float SecondsPerDay = GetSecondsPerInGameDay();
    const FTimespan ElapsedRealTime = WorldEpochUtc.GetTicks() > 0
        ? CurrentUtc - WorldEpochUtc
        : FTimespan::Zero();

    const double RealSecondsSinceEpoch = FMath::Max(0.0, ElapsedRealTime.GetTotalSeconds());
    const double InGameDaysPrecise = SecondsPerDay > 0.0f
        ? RealSecondsSinceEpoch / static_cast<double>(SecondsPerDay)
        : 0.0;

    const int32 TotalInGameDays = FMath::FloorToInt(InGameDaysPrecise);
    const double DayFraction = FMath::Frac(InGameDaysPrecise);
    const double TotalMinutesInDay = DayFraction * HoursPerDay * MinutesPerHour;

    FHuwamTimeSnapshot Snapshot;
    Snapshot.RealUtcTimestamp = CurrentUtc.ToIso8601();
    Snapshot.RealUtcTicks = CurrentUtc.GetTicks();
    Snapshot.SecondsPerInGameDay = SecondsPerDay;
    Snapshot.SecondsPerInGameWeek = GetSecondsPerInGameWeek();
    Snapshot.TotalInGameDays = TotalInGameDays;
    Snapshot.CurrentWeekIndex = InGameDaysPerWeek > 0 ? TotalInGameDays / InGameDaysPerWeek : 0;
    Snapshot.DayOfWeekIndex = InGameDaysPerWeek > 0 ? TotalInGameDays % InGameDaysPerWeek : 0;
    Snapshot.HourOfDay = FMath::Clamp(FMath::FloorToInt(TotalMinutesInDay / MinutesPerHour), 0, HoursPerDay - 1);
    Snapshot.MinuteOfHour = FMath::Clamp(FMath::FloorToInt(FMath::Fmod(TotalMinutesInDay, static_cast<double>(MinutesPerHour))), 0, MinutesPerHour - 1);
    Snapshot.bTimeSkippingAllowed = IsTimeSkippingAllowed();
    return Snapshot;
}

float UHuwamTimeSubsystem::GetSecondsPerInGameDay() const
{
    const int32 SafeDaysPerWeek = FMath::Max(1, InGameDaysPerWeek);
    return FMath::Max(1.0f, RealSecondsPerInGameWeek / static_cast<float>(SafeDaysPerWeek));
}

float UHuwamTimeSubsystem::GetSecondsPerInGameWeek() const
{
    return FMath::Max(1.0f, RealSecondsPerInGameWeek);
}

float UHuwamTimeSubsystem::GetRealSecondsForInGameDays(int32 InGameDays) const
{
    return static_cast<float>(FMath::Max(0, InGameDays)) * GetSecondsPerInGameDay();
}

int32 UHuwamTimeSubsystem::GetInGameDaysForRealSeconds(float RealSeconds) const
{
    const float SecondsPerDay = GetSecondsPerInGameDay();
    if (RealSeconds <= 0.0f || SecondsPerDay <= 0.0f)
    {
        return 0;
    }

    return FMath::CeilToInt(RealSeconds / SecondsPerDay);
}

bool UHuwamTimeSubsystem::IsTimeSkippingAllowed() const
{
    return bAllowTimeSkipping;
}
