# NPC Supply Quest Turn-In Prototype

This layer lets the first placed Eldoria supply requester receive the completed field-supplies quest.

## Goal

The first in-world turn-in pass should prove:

1. The same NPC actor that offered the job can receive its delivery.
2. Accepted materials enter that NPC actor's own supply stock.
3. Quest reward and stock update stay on the existing verified turn-in path.

## Runtime Piece

`AHuwamNpcSupplyQuestOfferActor` now wraps the field-supplies turn-in path.

It exposes:

- `CanTurnInQuestWithPrototypePlayer`
- `TurnInQuestWithPrototypePlayer`
- `OnQuestTurnedIn`

The actor does not duplicate delivery rules. It forwards its owned `UHuwamNpcSupplyComponent` into `AHuwamPrototypePlayerActor::TurnInGatherFieldSuppliesQuest`.

## Turn-In Flow

1. The player gathers enough accepted field supplies to finish the objective.
2. The placed NPC actor confirms the field-supplies quest can turn in.
3. The player turn-in helper delivers accepted inventory materials into the actor-owned NPC supply pocket.
4. The quest reward resolves only after delivery succeeds.
5. The NPC actor broadcasts the turned-in quest state and the delivery result.

The delivery result reports:

- Which materials were delivered
- How many items moved
- Stock quantity after delivery
- Whether the NPC supply target is satisfied

## Blueprint Calls

- `CanTurnInQuestWithPrototypePlayer`
- `TurnInQuestWithPrototypePlayer`
- `TurnInGatherFieldSuppliesQuest`
- `GetNpcSupplyComponent`

Useful event:

- `OnQuestTurnedIn`

## First Map Test

1. Place `AHuwamPrototypePlayerActor`.
2. Place `AHuwamNpcSupplyQuestOfferActor`.
3. Preview and accept the field-supplies offer from that NPC actor.
4. Gather accepted field supplies until the objective reaches `10 / 10`.
5. Call `CanTurnInQuestWithPrototypePlayer` on the same NPC actor.
6. Call `TurnInQuestWithPrototypePlayer`.
7. Confirm accepted field supplies leave player inventory.
8. Confirm the actor-owned NPC supply stock gains the delivered items.
9. Confirm the quest reward grants 12 copper and 6 experience after the delivery succeeds.

## Current Boundary

The first requester can now complete the full supply loop in data and actor calls.

It does not yet:

- Render a quest offer or completion prompt.
- Show the NPC's changed need state after delivery.
- Support several needs or quest types on one NPC interaction.
- Reserve or synchronize one shortage across multiple players.

The first map-prompt state snapshot now continues in:

- `Docs/NpcSupplyQuestInteractionStatePrototype.md`
