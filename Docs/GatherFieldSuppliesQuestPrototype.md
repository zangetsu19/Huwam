# Gather Field Supplies Quest Prototype

This layer starts the first Eldoria gathering quest path around field resource nodes.

## Goal

The first resource loop should now prove:

1. Eldoria can start a gathering quest.
2. Field gathering nodes can advance the active quest.
3. The quest can become ready to turn in.
4. The player can deliver gathered materials into NPC stock at turn-in.
5. The player can receive the current quest reward after the delivery succeeds.

## Quest

| Field | Value |
|---|---|
| Quest ID | `quest.eldoria.gather_field_supplies` |
| Display name | Gather Field Supplies |
| Objective ID | `objective.gather_field_supplies` |
| Objective | Gather 10 field supplies. |
| Progress source | `material.field_supply` |
| Quest giver | `npc.eldoria.poor_resident_01` |
| Currency reward | 12 copper |
| Experience reward | 6 |

`UHuwamQuestComponent` now has a fallback runtime state for this quest if the imported quest DataTable is not connected yet.

## Living Quest Rule

The direct `StartGatherFieldSuppliesQuest` helper starts one known gathering quest so the runtime loop can be tested. The living prototype path now uses `StartGatherFieldSuppliesQuestFromSupply`, which asks the correct NPC supply component whether a shortage request should exist first.

In the living quest generator:

- An NPC should generate a request when they cannot solve the need themselves.
- An NPC may also ask a player when the player is clearly faster, safer, more skilled, more trusted, or more efficient than the NPC's own solution.
- Another player may receive a different request if the need is already solved, the NPC inventory changed, or that player fits a different opportunity better.

## Runtime Flow

1. `AHuwamNpcSupplyQuestOfferActor` gives the first Eldoria requester a placed interaction with an actor-owned NPC supply component.
2. `GetInteractionSnapshotForPrototypePlayer` reports whether that actor is offering work, waiting for delivery, ready for turn-in, or satisfied.
3. `InteractByStateWithPrototypePlayer` routes preview, acceptance, waiting, satisfied, or turn-in behavior from that current state.
4. Offer preview and acceptance still reuse the lower-level need-aware offer functions.
5. The started quest stores the need ID, name, reason, and missing quantity in its first need-request context.
6. `AHuwamGatheringResourceActor` gathers a real field item into inventory.
7. The resource actor advances active quest objectives that listen for `material.field_supply`.
8. `UHuwamQuestComponent` refreshes reward progress into quest state.
9. At `10 / 10`, the placed NPC actor can call `TurnInQuestWithPrototypePlayer`.
10. That actor routes its owned `UHuwamNpcSupplyComponent` into `AHuwamPrototypePlayerActor::TurnInGatherFieldSuppliesQuest`.
11. The supply component removes accepted gathered materials from the player inventory and adds them into the recipient NPC stock.
12. The quest turns in and grants the current reward after the stock handoff succeeds.

## Supply Handoff

The objective progress source is broad on purpose: `material.field_supply` lets early field nodes count as gathering progress.

The turn-in is stricter. The first recipient accepts these actual inventory item IDs:

- `material.wood.basic`
- `material.stone.basic`
- `material.flax`
- `material.fiber.grass`
- `material.herb.lavender`
- `material.herb.mint`
- `material.herb.rosemary`

`FHuwamNpcSupplyDeliveryResult` returns which items were delivered, the delivered total, the recipient stock total after delivery, and whether that stock reached the current supply target.

## Main Files

- `Source/Huwam/Gameplay/HuwamQuestComponent.h`
- `Source/Huwam/Gameplay/HuwamQuestComponent.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`
- `Source/Huwam/Gameplay/HuwamGatheringResourceActor.h`
- `Source/Huwam/Gameplay/HuwamGatheringResourceActor.cpp`
- `Source/Huwam/Gameplay/HuwamNpcSupplyComponent.h`
- `Source/Huwam/Gameplay/HuwamNpcSupplyComponent.cpp`
- `Source/Huwam/Gameplay/HuwamNpcSupplyQuestOfferActor.h`
- `Source/Huwam/Gameplay/HuwamNpcSupplyQuestOfferActor.cpp`

## Blueprint Calls

- `UHuwamQuestComponent::StartGatherFieldSuppliesQuest`
- `AHuwamPrototypePlayerActor::StartGatherFieldSuppliesQuest`
- `AHuwamPrototypePlayerActor::CanStartGatherFieldSuppliesQuestFromSupply`
- `AHuwamPrototypePlayerActor::GetGatherFieldSuppliesQuestOfferFromSupply`
- `AHuwamPrototypePlayerActor::StartGatherFieldSuppliesQuestFromSupply`
- `AHuwamNpcSupplyQuestOfferActor::InteractWithPrototypePlayer`
- `AHuwamNpcSupplyQuestOfferActor::AcceptQuestOfferWithPrototypePlayer`
- `AHuwamNpcSupplyQuestOfferActor::GetInteractionSnapshotForPrototypePlayer`
- `AHuwamNpcSupplyQuestOfferActor::InteractByStateWithPrototypePlayer`
- `AHuwamNpcSupplyQuestOfferActor::TurnInQuestWithPrototypePlayer`
- `AHuwamGatheringResourceActor::GatherWithPrototypePlayer`
- `AHuwamPrototypePlayerActor::TurnInGatherFieldSuppliesQuest`

## First Map Test

1. Place `AHuwamPrototypePlayerActor` in a non-tutorial test map.
2. Place `AHuwamNpcSupplyQuestOfferActor` as the first Eldoria supply requester.
3. Confirm the actor-owned supply component starts low enough to request help.
4. Preview the job with `InteractWithPrototypePlayer`.
5. Accept it with `AcceptQuestOfferWithPrototypePlayer`.
6. Confirm the active quest need request points back to `need.eldoria.field_supplies`.
7. Place enough `AHuwamGatheringResourceActor` instances for 10 harvests, or respawn the same node between harvests.
8. Gather field resources until the objective reaches `10 / 10`.
9. Confirm the quest state reports ready to turn in.
10. Call `TurnInQuestWithPrototypePlayer` on the same NPC offer actor.
11. Confirm accepted materials leave the player inventory and appear in NPC stock.
12. Confirm the quest leaves the active list and grants 12 copper plus 6 experience.

## Current Boundary

This layer proves shortage-gated start, gathering quest progress, material delivery, NPC stock update, and reward turn-in.

The first routine stock-use timer now lets that shortage cycle move again:

- A refilled recipient can fall low again through a configured in-game-time stock use instead of a manual reset.
- The supply component now exposes that returned shortage as a named NPC need with urgency and a next action, and the quest keeps that request context after it starts.
- Settlement self-sufficiency and player efficiency rules should stay visible in the request path.

See `Docs/NpcSupplyRoutineUsePrototype.md`.
See `Docs/NpcSupplyNeedStatePrototype.md`.
See `Docs/NpcSupplyQuestRequestPrototype.md`.
See `Docs/NpcSupplyQuestOfferPrototype.md`.
See `Docs/NpcSupplyQuestInteractionPrototype.md`.
See `Docs/NpcSupplyQuestTurnInPrototype.md`.
See `Docs/NpcSupplyQuestInteractionStatePrototype.md`.
See `Docs/NpcSupplyQuestStateDrivenInteractionPrototype.md`.
See `Docs/NpcSupplyQuestWidgetPrototype.md`.
