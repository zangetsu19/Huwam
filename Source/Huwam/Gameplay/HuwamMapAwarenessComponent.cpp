#include "Gameplay/HuwamMapAwarenessComponent.h"

UHuwamMapAwarenessComponent::UHuwamMapAwarenessComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UHuwamMapAwarenessComponent::OpenFullMap()
{
    if (MapAwarenessSnapshot.bFullMapOpen)
    {
        return false;
    }

    MapAwarenessSnapshot.bFullMapOpen = true;
    BroadcastSnapshot();
    return true;
}

bool UHuwamMapAwarenessComponent::CloseFullMap()
{
    if (!MapAwarenessSnapshot.bFullMapOpen)
    {
        return false;
    }

    MapAwarenessSnapshot.bFullMapOpen = false;
    BroadcastSnapshot();
    return true;
}

bool UHuwamMapAwarenessComponent::SetCurrentMap(const FString& MapId, const FString& RegionId, const FText& DisplayName, EHuwamMapFogState FogState)
{
    if (MapId.IsEmpty() || RegionId.IsEmpty())
    {
        return false;
    }

    MapAwarenessSnapshot.MapId = MapId;
    MapAwarenessSnapshot.RegionId = RegionId;
    MapAwarenessSnapshot.DisplayName = DisplayName;
    MapAwarenessSnapshot.FogState = FogState;
    BroadcastSnapshot();
    return true;
}

bool UHuwamMapAwarenessComponent::TrackQuestObjective(const FString& QuestId, const FString& ObjectiveId)
{
    if (!MapAwarenessSnapshot.bQuestMarkersAvailable || QuestId.IsEmpty() || ObjectiveId.IsEmpty())
    {
        return false;
    }

    MapAwarenessSnapshot.TrackedQuestId = QuestId;
    MapAwarenessSnapshot.TrackedObjectiveId = ObjectiveId;
    MapAwarenessSnapshot.bTrackedQuestMarkerVisible = true;
    BroadcastSnapshot();
    return true;
}

FHuwamMapAwarenessSnapshot UHuwamMapAwarenessComponent::GetMapAwarenessSnapshot() const
{
    return MapAwarenessSnapshot;
}

void UHuwamMapAwarenessComponent::RestoreMapAwarenessForPrototypeSave(const FHuwamMapAwarenessSnapshot& SavedSnapshot)
{
    MapAwarenessSnapshot = SavedSnapshot;
    BroadcastSnapshot();
}

void UHuwamMapAwarenessComponent::BroadcastSnapshot()
{
    OnMapAwarenessChanged.Broadcast(MapAwarenessSnapshot);
}
