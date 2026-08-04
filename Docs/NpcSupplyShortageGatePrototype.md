# NPC Supply Shortage Gate Prototype

This layer makes the first Eldoria field-supply request depend on an NPC shortage state.

## Goal

The first shortage gate should prove:

1. A recipient NPC can decide it should ask for supply help.
2. A refilled NPC can withhold that request while stock is healthy.
3. The prototype player can start the known gathering quest through the recipient instead of assuming every player gets it.

## Supply Gate

`UHuwamNpcSupplyComponent::ShouldRequestSupplyHelp` is the first reusable shortage check.

For the current component it returns true when:

- Total stock is at or below the low-supply threshold.
- The supply need target is not already satisfied.

The default prototype supply pocket starts empty, so the first Eldoria recipient can ask for help immediately in a test map.

## Player Start Path

`AHuwamPrototypePlayerActor` now has:

- `CanStartGatherFieldSuppliesQuestFromSupply`
- `StartGatherFieldSuppliesQuestFromSupply`

The first start path requires:

- A valid quest component.
- A valid NPC supply component.
- The supply component NPC ID to be `npc.eldoria.poor_resident_01`.
- That supply component to report `ShouldRequestSupplyHelp`.

`StartGatherFieldSuppliesQuest` still exists as a direct prototype test helper. The supply-gated helper is the living-world path for this first request.

## Living Quest Meaning

This is the first small proof of the Huwan rule:

- Not every player receives the same NPC request.
- A request should exist because an NPC need exists or because using a player is the better solution.
- Updated inventory state should change future quest availability.

This gate only proves shortage-state availability. It does not yet compare player skill, risk, speed, trust, travel distance, settlement labor capacity, competing demand, or ordinary surplus sales from players into essential NPC stock.

## Main Files

- `Source/Huwam/Gameplay/HuwamNpcSupplyComponent.h`
- `Source/Huwam/Gameplay/HuwamNpcSupplyComponent.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`
- `Docs/GatherFieldSuppliesQuestPrototype.md`

## First Map Test

1. Place `AHuwamPrototypePlayerActor`.
2. Place an NPC actor with `UHuwamNpcSupplyComponent`.
3. Keep the supply component NPC ID at `npc.eldoria.poor_resident_01`.
4. Confirm the empty stock reports `ShouldRequestSupplyHelp`.
5. Call `StartGatherFieldSuppliesQuestFromSupply`.
6. Gather and deliver 10 accepted field supplies.
7. Confirm the delivery result reports the current supply target satisfied.
8. Confirm `ShouldRequestSupplyHelp` is false while the new stock remains above the low threshold.

## Current Boundary

The first supply request can now be withheld by stock state. Stock can now be filled by one prototype NPC purchase path, spent by one first routine timer, and described through one first supply need snapshot.

The first stock sale and use helpers continue in:

- `Docs/NpcSupplyTradeUsePrototype.md`
- `Docs/NpcSupplyRoutineUsePrototype.md`
- `Docs/NpcSupplyNeedStatePrototype.md`
