# NPC Supply Quest Request Prototype

This layer lets the first shortage-gated quest carry the NPC need that caused it.

## Goal

The first need-aware quest pass should prove:

1. The Eldoria supply quest checks a real need state before it starts.
2. The active quest can remember why the NPC asked for help.
3. HUD and later dialogue layers can read that explanation from quest runtime data.

## Runtime Piece

`FHuwamQuestNeedRequestContext` now sits on `FHuwamQuestRuntimeState`.

The first request context stores:

- Need ID
- Need display name
- Need reason
- Quantity missing when the request was accepted

`UHuwamQuestComponent::SetQuestNeedRequestContext` attaches that context to an active quest.

## Field Supplies Flow

`AHuwamPrototypePlayerActor::StartGatherFieldSuppliesQuestFromSupply` now:

1. Reads `FHuwamNpcSupplyNeedState` from the matching `UHuwamNpcSupplyComponent`.
2. Requires that need state to recommend player help.
3. Starts `quest.eldoria.gather_field_supplies`.
4. Copies the need ID, display name, reason, and missing quantity into the quest request context.
5. Refreshes HUD data after the need-aware quest exists.

The field-supplies quest is now repeatable. Once a player turns it in, the same quest can be offered again only when the NPC supply component falls back into help-request territory. Restarting it clears the previous objective progress, so a returning request still requires fresh gathering.

That means the first request is no longer just:

- “Stock is low, start a quest.”

It can also say:

- “Field supplies are low because daily materials are being used for local repairs, care, and ordinary work.”

## HUD Handoff

`FHuwamHudQuestSnapshot` now carries the request need context too:

- `RequestNeedId`
- `RequestNeedDisplayName`
- `RequestNeedReason`
- `RequestNeededQuantity`

The full active quest state already reaches HUD snapshots. The compact quest summary now has the same first request explanation so an offer card, tracker detail, or later dialogue prompt can surface it without re-querying the NPC supply component.

## Blueprint Calls

- `StartGatherFieldSuppliesQuestFromSupply`
- `GetSupplyNeedState`
- `SetQuestNeedRequestContext`
- `RefreshHudSnapshot`

## First Map Test

1. Place the prototype player and an Eldoria NPC actor with `UHuwamNpcSupplyComponent`.
2. Query the empty supply component and confirm its field-supplies need asks for player help.
3. Call `StartGatherFieldSuppliesQuestFromSupply`.
4. Read the active field-supplies quest state.
5. Confirm `NeedRequest.NeedId` is `need.eldoria.field_supplies`.
6. Confirm `NeedRequest.NeedReason` describes the local supply use.
7. Read `ActiveQuestSummaries` from the HUD snapshot.
8. Confirm the compact HUD quest summary carries the same need ID, reason, and missing quantity.
9. Turn in the request and refill the NPC stock.
10. Consume that stock through routine supply use until the need becomes urgent again.
11. Confirm the request can be offered and accepted again.

## Current Boundary

This pass attaches need context after the first quest starts.

It does not yet:

- Rewrite quest objectives or rewards from the missing quantity.
- Pick among multiple NPC needs.
- Turn the need reason into authored NPC dialogue.
- Compare player efficiency against NPC self-resolution before offering help.

The first pre-acceptance offer now continues in:

- `Docs/NpcSupplyQuestOfferPrototype.md`
