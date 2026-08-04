# NPC Supply Quest Interaction State Prototype

This layer gives the first placed Eldoria supply requester one interaction snapshot for map prompts and UI logic.

## Goal

The first interaction state pass should prove:

1. A map-facing NPC can describe its current field-supplies quest state with one read.
2. Prompt logic can tell the difference between offer, active objective, turn-in, and satisfied supply.
3. The snapshot can carry the supply need and relevant quest data for the next UI pass.

## Runtime Piece

`AHuwamNpcSupplyQuestOfferActor::GetInteractionSnapshotForPrototypePlayer` builds `FHuwamNpcSupplyQuestInteractionSnapshot`.

The snapshot exposes:

- Interaction state
- Current prompt text
- Actor-owned NPC supply need state
- Quest offer preview when available
- Active quest state when active
- Offer preview flag
- Offer acceptance flag
- Turn-in flag

## Interaction States

| State | Meaning | Default Prompt |
|---|---|---|
| `OfferingWork` | The NPC supply need asks for player help and the field-supplies offer is available. | Ask about field supplies |
| `WaitingForDelivery` | The player accepted the field-supplies quest but has not completed its objective. | Gather field supplies |
| `ReadyForTurnIn` | The player's field-supplies objective is ready to deliver to this NPC. | Deliver field supplies |
| `Satisfied` | The NPC does not currently need this field-supplies request. | Field supplies are stocked |
| `Unavailable` | The actor cannot produce a usable first interaction state for this player. | No field supply request available |

The actor keeps those prompts editable per placed requester so later Blueprint work can tune nearby prompt text without changing the snapshot shape.

## State Flow

1. Read the actor-owned supply need.
2. If the player has an active field-supplies quest, report `WaitingForDelivery` or `ReadyForTurnIn`.
3. Otherwise, if the actor can build a current field-supplies offer, report `OfferingWork`.
4. Otherwise, when supply is not asking for help, report `Satisfied`.
5. Leave the fallback as `Unavailable`.

## Blueprint Calls

- `GetInteractionSnapshotForPrototypePlayer`
- `InteractWithPrototypePlayer`
- `AcceptQuestOfferWithPrototypePlayer`
- `TurnInQuestWithPrototypePlayer`

## First Map Test

1. Place the prototype player and `AHuwamNpcSupplyQuestOfferActor`.
2. Query `GetInteractionSnapshotForPrototypePlayer` before accepting the job.
3. Confirm the state is `OfferingWork`.
4. Accept the field-supplies offer.
5. Query again and confirm the state is `WaitingForDelivery`.
6. Gather enough supplies to finish the objective.
7. Query again and confirm the state is `ReadyForTurnIn`.
8. Turn in through the same NPC actor.
9. Query again and confirm the state is `Satisfied` while the supply target remains filled.

## Current Boundary

This snapshot gives prompt and UI logic one clear read.

It does not yet:

- Render the prompt in a widget or world marker.
- Represent repeatable requests after the first quest has already been turned in.
- Surface gratitude, trust, dialogue, or visible stock change after delivery.
- Synchronize the state across several players watching one live need.

The first state-driven actor route now continues in:

- `Docs/NpcSupplyQuestStateDrivenInteractionPrototype.md`
