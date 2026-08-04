# NPC Supply Quest Offer Prototype

This layer exposes the first need-aware NPC quest before the player accepts it.

## Goal

The first offer pass should prove:

1. Quest content can be previewed without starting an active quest.
2. An NPC supply need can explain the preview before acceptance.
3. Accepting the preview can still use the same need-aware quest start path.

## Runtime Pieces

`FHuwamQuestOffer` is a Blueprint-friendly preview of a quest.

The first offer includes:

- Quest ID, name, description, type, urgency, and giver
- Primary objective preview
- Need request context when an NPC need caused the offer
- Reward items, copper, legacy gold, and experience
- World-state effect text

`UHuwamQuestComponent::GetQuestOffer` builds that preview from the same imported quest row or fallback quest state used when the quest starts. It does not activate the quest.

## Field Supplies Offer

`AHuwamPrototypePlayerActor::GetGatherFieldSuppliesQuestOfferFromSupply` creates the first NPC need-aware offer.

It requires:

1. The prototype player to have a quest component.
2. The requester supply component to belong to `npc.eldoria.poor_resident_01`.
3. The supply need state to recommend player help.
4. The field-supplies quest to be available for an offer.

When those checks pass, the offer adds:

- `need.eldoria.field_supplies`
- The need display name
- The need reason
- The quantity missing at offer time

That gives an offer UI or NPC interaction enough data to explain the job before the player takes it.

## Accept Path

`StartGatherFieldSuppliesQuestFromSupply` now reads the same offer first. Once the offer is valid, it starts the active quest and copies the offer need context into quest runtime state.

For this first pass:

- Offer preview happens before acceptance.
- Quest progress still begins only after acceptance.
- A quest already active or turned in does not generate another offer preview.

## Blueprint Calls

- `GetQuestOffer`
- `GetGatherFieldSuppliesQuestOfferFromSupply`
- `StartGatherFieldSuppliesQuestFromSupply`
- `GetSupplyNeedState`

## First Map Test

1. Place the prototype player and an Eldoria NPC actor with `UHuwamNpcSupplyComponent`.
2. Keep the NPC supply stock low enough to request player help.
3. Call `GetGatherFieldSuppliesQuestOfferFromSupply`.
4. Confirm the offer shows the field-supplies quest objective and rewards while no active quest has started.
5. Confirm the offer need request explains why field supplies are needed.
6. Call `StartGatherFieldSuppliesQuestFromSupply`.
7. Confirm the active quest starts and carries the same need request context.
8. Query the offer again and confirm it does not preview a duplicate while the quest is active.

## Current Boundary

This is a pre-acceptance data offer.

It does not yet:

- Show an offer card, dialogue choice, or accept/decline prompt.
- Rank multiple simultaneous quest offers.
- Reserve the NPC need while one player considers the offer.
- Rebalance objective quantity or reward from changing stock.

The first map-facing interaction bridge now continues in:

- `Docs/NpcSupplyQuestInteractionPrototype.md`
