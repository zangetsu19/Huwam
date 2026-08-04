# Huwam Living Time Service Prototype

This layer creates the first shared clock for Huwam's living-server systems.

## Goal

The first time service proves:

1. One authoritative prototype rule converts real time into in-game days.
2. Resource respawns and NPC routine stock use can ask the same system for timing.
3. Time skipping remains disabled in the runtime contract.
4. Later spoilage, quest timers, settlement routines, auctions, and AI engines can attach to one clock instead of each owning a private time scale.

## Time Rule

Huwam currently uses the world-bible rule:

| Rule | Value |
|---|---|
| Real time per in-game week | 24 hours |
| In-game days per week | 7 |
| Real seconds per in-game day | `86400 / 7` |
| Time skipping | Disabled |

Sleeping, beds, or player actions should not speed up this server clock.

## Runtime Piece

`UHuwamTimeSubsystem` is a `UGameInstanceSubsystem`.

It exposes:

- `GetCurrentTimeSnapshot`
- `GetSecondsPerInGameDay`
- `GetSecondsPerInGameWeek`
- `GetRealSecondsForInGameDays`
- `GetInGameDaysForRealSeconds`
- `IsTimeSkippingAllowed`

`FHuwamTimeSnapshot` carries:

- Real UTC timestamp
- Real UTC ticks
- Seconds per in-game day
- Seconds per in-game week
- Total in-game days since the current prototype epoch
- Current week index
- Day-of-week index
- Hour and minute within the current in-game day
- Whether time skipping is allowed

## First Integrations

The first systems now resolve timing through the shared service:

- `AHuwamGatheringResourceActor::GetRespawnDelaySeconds`
- `UHuwamNpcSupplyComponent::GetRoutineUseDelaySeconds`

Both keep their older `SecondsPerInGameDay` field as a fallback for legacy placed actors or tests, but default to shared Huwam time.

## Editor Validation

The editor validation run now checks that:

1. The time subsystem exists.
2. One in-game week is 86400 real seconds.
3. One in-game day is `86400 / 7` real seconds.
4. Time skipping is disabled.

That validation runs before the playable tutorial-to-Eldoria slice.

## Current Boundary

This is the first shared clock, not the final MMO time authority.

It does not yet:

- Persist a permanent server epoch across restarts.
- Drive food spoilage.
- Drive quest deadlines.
- Drive auctions or market windows.
- Drive AI engine update buckets.
- Sync multiplayer clients to a server source.

Those can now be added against this single service instead of inventing separate clocks.
