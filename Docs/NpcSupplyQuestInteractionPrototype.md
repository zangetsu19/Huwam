# NPC Supply Quest Interaction Prototype

This layer gives the first need-aware Eldoria supply request a placed in-world interaction surface.

## Goal

The first NPC interaction pass should prove:

1. An NPC actor in the map can own the supply state that creates a quest need.
2. Interacting with that actor can preview the need-aware quest offer.
3. Accepting through that actor starts the same active quest path already used by the prototype player.

## Runtime Piece

`AHuwamNpcSupplyQuestOfferActor` is the first placed NPC quest interaction actor.

It includes:

- A scene root for map placement.
- A `UHuwamNpcSupplyComponent` owned by the NPC actor.
- An interaction prompt.
- A preview event.
- An acceptance event.
- A completion turn-in event.

The actor defaults to the same first supply requester setup as `UHuwamNpcSupplyComponent`, so empty stock can immediately represent Eldoria's first field-supplies shortage in a test map.

## Interaction Flow

`InteractWithPrototypePlayer` previews the field-supplies offer without activating it.

It calls:

1. `GetGatherFieldSuppliesQuestOfferFromSupply` on the prototype player.
2. The actor-owned NPC supply component as the offer source.
3. `OnQuestOfferPreviewed` when the offer is valid.

`AcceptQuestOfferWithPrototypePlayer` accepts the offer.

It:

1. Revalidates the current offer from the actor-owned supply component.
2. Calls `StartGatherFieldSuppliesQuestFromSupply`.
3. Broadcasts `OnQuestOfferAccepted` with the accepted preview data.

`TurnInQuestWithPrototypePlayer` receives the completed field-supplies delivery through the same actor-owned supply component.

It:

1. Confirms the field-supplies quest is ready for turn-in.
2. Calls `TurnInGatherFieldSuppliesQuest` with the actor-owned supply pocket.
3. Broadcasts `OnQuestTurnedIn` with the quest and delivery result after stock and reward resolution succeed.

## Blueprint Calls

- `CanInteractWithPrototypePlayer`
- `GetInteractionSnapshotForPrototypePlayer`
- `InteractByStateWithPrototypePlayer`
- `InteractWithPrototypePlayer`
- `AcceptQuestOfferWithPrototypePlayer`
- `CanTurnInQuestWithPrototypePlayer`
- `TurnInQuestWithPrototypePlayer`
- `GetInteractionPrompt`
- `GetNpcSupplyComponent`

Useful events:

- `OnQuestOfferPreviewed`
- `OnQuestOfferAccepted`
- `OnQuestTurnedIn`

## First Map Test

1. Place `AHuwamPrototypePlayerActor`.
2. Place `AHuwamNpcSupplyQuestOfferActor` as the first Eldoria supply requester.
3. Keep the actor-owned supply stock low enough for its need state to request player help.
4. Call `InteractWithPrototypePlayer`.
5. Confirm the returned offer contains the field-supplies quest, objective, rewards, and need reason.
6. Confirm no active field-supplies quest exists yet.
7. Call `AcceptQuestOfferWithPrototypePlayer`.
8. Confirm the active quest starts and HUD quest data keeps the same need request context.
9. Gather accepted field supplies until the active objective is ready to turn in.
10. Call `TurnInQuestWithPrototypePlayer` on the same NPC actor.
11. Confirm the actor-owned NPC stock receives the delivery.

## Current Boundary

This actor creates the first map-facing offer bridge.

It does not yet:

- Render a quest offer widget or conversation choice by itself.
- Move, animate, speak, or simulate a full NPC body.
- Route several quest types or several needs through one NPC interaction.
- Reserve one live need while multiple players inspect it.

The first actor-owned turn-in bridge now continues in:

- `Docs/NpcSupplyQuestTurnInPrototype.md`
- `Docs/NpcSupplyQuestInteractionStatePrototype.md`
- `Docs/NpcSupplyQuestStateDrivenInteractionPrototype.md`
