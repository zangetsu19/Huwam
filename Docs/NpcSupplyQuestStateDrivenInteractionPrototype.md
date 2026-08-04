# NPC Supply Quest State-Driven Interaction Prototype

This layer gives the first placed Eldoria supply requester one routed interaction call.

## Goal

The first state-driven interaction pass should prove:

1. Blueprint can call one NPC interaction function instead of stitching offer and turn-in checks together.
2. The current interaction snapshot decides whether the routed call previews work, accepts work, waits for delivery, or turns in delivery.
3. The result carries before-and-after state for the next UI pass.

## Runtime Piece

`AHuwamNpcSupplyQuestOfferActor::InteractByStateWithPrototypePlayer` builds `FHuwamNpcSupplyQuestInteractionResult`.

The result exposes:

- Success flag
- Routed outcome
- Snapshot before the routed action
- Snapshot after the routed action
- Quest offer data when an offer was previewed or accepted
- Turned-in quest data when delivery completes
- NPC stock delivery result when delivery completes

## Routed Outcomes

| Snapshot State | Routed Outcome |
|---|---|
| `OfferingWork` with `bAcceptOfferWhenAvailable` false | Preview the field-supplies offer. |
| `OfferingWork` with `bAcceptOfferWhenAvailable` true | Accept the field-supplies offer. |
| `WaitingForDelivery` | Return a waiting reminder result. |
| `ReadyForTurnIn` | Turn in the completed field-supplies delivery. |
| `Satisfied` | Return a satisfied result. |
| `Unavailable` | Return an unavailable result. |

That keeps offer inspection and offer acceptance distinct while still letting a single routed call choose the correct actor path from current state.

## Blueprint Calls

- `InteractByStateWithPrototypePlayer`
- `GetInteractionSnapshotForPrototypePlayer`
- `CanInteractWithPrototypePlayer`

The lower-level calls still remain available:

- `InteractWithPrototypePlayer`
- `AcceptQuestOfferWithPrototypePlayer`
- `TurnInQuestWithPrototypePlayer`

## First Map Test

1. Place the prototype player and `AHuwamNpcSupplyQuestOfferActor`.
2. Call `InteractByStateWithPrototypePlayer` with `bAcceptOfferWhenAvailable` false.
3. Confirm the result outcome is `OfferPreviewed`.
4. Call the same routed function with `bAcceptOfferWhenAvailable` true.
5. Confirm the result outcome is `OfferAccepted` and the after snapshot is `WaitingForDelivery`.
6. Call it again before gathering is complete.
7. Confirm the result outcome is `WaitingForDelivery`.
8. Gather enough accepted field supplies to finish the objective.
9. Call it again and confirm the outcome is `QuestTurnedIn`.
10. Confirm the delivery result reports the NPC stock update and the after snapshot becomes `Satisfied`.

## Current Boundary

The first NPC requester now has a single actor-side route and the next native widget can read it without stitching offer and turn-in checks together.

It does not yet:

- Use a player input trace or interaction component to call the NPC actor.
- Choose among multiple offers on one NPC.
- Support repeatable shortage requests after the first quest is turned in.
- Synchronize this route across several players sharing one live shortage.

The first routed screen surface now lives in `UHuwamNpcSupplyQuestInteractionWidget`.

See `Docs/NpcSupplyQuestWidgetPrototype.md`.

The next practical layer is to open that requester prompt from player world interaction.
