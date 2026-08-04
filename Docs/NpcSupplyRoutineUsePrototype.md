# NPC Supply Routine Use Prototype

This layer gives one NPC supply pocket a first clock-driven stock drain.

## Goal

The first routine use pass should prove:

1. A configured NPC supply component can spend a chosen stored item on the current in-game-day timing.
2. Stock can fall after quest delivery or ordinary player sale without a manual reset.
3. The existing low-stock request gate can become true again after routine life consumes enough supply.

## Runtime Piece

`UHuwamNpcSupplyComponent` now exposes a small routine timer around its existing stock-use call.

The first routine fields are:

- `RoutineUseItemId`
- `RoutineUseQuantity`
- `RoutineUseIntervalInGameDays`
- `bUseSharedHuwamTime`
- `SecondsPerInGameDay`
- `bAutoStartRoutineSupplyUse`

The default item is `material.wood.basic`, the default quantity is `1`, and the default interval is `1` in-game day. Auto-start stays off by default so a test NPC, Blueprint routine, or later NPC Life Engine chooses which supply pockets actually run a schedule.

If the preferred routine item is not stocked, the component can now fall back to any accepted stocked supply item. That lets the first Eldoria requester consume delivered herbs or field materials even when the routine's ideal item is wood.

The timing now resolves through `UHuwamTimeSubsystem` by default, matching the current gathering prototype conversion:

- `1` in-game week equals `24` real hours.
- `1` in-game day uses `86400 / 7` real seconds in this first pass.

The component still exposes `SecondsPerInGameDay` as a fallback for old placed actors and focused tests.

## Runtime Flow

1. A test NPC receives accepted stock through quest delivery, direct addition, or surplus purchase.
2. Blueprint enables `bAutoStartRoutineSupplyUse` or calls `StartRoutineSupplyUse`.
3. The component timer reaches the configured in-game-day interval.
4. `UseRoutineSupply` chooses the preferred routine item when stocked, otherwise an accepted stocked fallback item.
5. Successful use broadcasts the existing stock change event and reports whether shortage-help pressure has returned.

If the configured item is not currently in stock, the routine use attempt simply fails and the looping timer waits for the next interval. That is enough for this layer; later NPC needs can react to failed use with hunger, work delay, substitutions, buying, or quest generation.

## Blueprint Calls

- `StartRoutineSupplyUse`
- `StopRoutineSupplyUse`
- `UseRoutineSupply`
- `IsRoutineSupplyUseActive`
- `GetRoutineUseDelaySeconds`
- `GetRoutineUseCandidateItemId`
- `UseStockItemById`
- `ShouldRequestSupplyHelp`

## First Map Test

1. Place the prototype player and one NPC actor with `UHuwamNpcSupplyComponent`.
2. Keep that NPC supply component on the first accepted field-material list.
3. Add or deliver enough `material.wood.basic` to push stock above the low-supply threshold.
4. Set `RoutineUseIntervalInGameDays` to `1`.
5. Enable `bAutoStartRoutineSupplyUse` before play, or call `StartRoutineSupplyUse` after play starts.
6. For a quick test, temporarily lower `SecondsPerInGameDay` on that placed component.
7. Confirm routine use removes the configured wood quantity when the timer fires.
8. Confirm `OnNpcSupplyChanged` fires on successful use.
9. Confirm `ShouldRequestSupplyHelp` becomes true again once total supply falls low enough.
10. Confirm the NPC can offer the repeatable field-supplies request again.

## Current Boundary

This is a prototype timer, not the final living server clock.

It does not yet:

- Track multiple named NPC needs such as meals, repairs, workshop fuel, patient care, or trade fulfillment.
- Substitute another suitable resource when the configured item is absent.
- Persist elapsed routine time while the server or map is offline.
- Explain the returned shortage through natural dialogue.

The first visible need state now continues in:

- `Docs/NpcSupplyNeedStatePrototype.md`

The first repeatable request proof now lives in the editor validation run: after delivery and one surplus sale, the validation consumes NPC stock through `UseRoutineSupply` until shortage pressure returns, then accepts the field-supplies request again.
