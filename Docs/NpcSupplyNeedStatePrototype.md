# NPC Supply Need State Prototype

This layer makes the first NPC supply shortage readable as a named need instead of a bare stock number.

## Goal

The first visible need pass should prove:

1. A supply pocket can explain what kind of need its stock represents.
2. Blueprint and later AI layers can read urgency from the same stock that gates quests.
3. A returning shortage can expose a next action before deeper NPC reasoning exists.

## Runtime Piece

`UHuwamNpcSupplyComponent::GetSupplyNeedState` builds `FHuwamNpcSupplyNeedState`.

The first snapshot exposes:

- NPC ID
- Need ID
- Need display name
- Need reason
- Current total stock
- Low-stock threshold
- Target stock quantity
- Quantity still needed
- Whether the need is satisfied
- Whether player help should be requested
- Urgency
- Next action

The default first need is:

| Field | Value |
|---|---|
| Need ID | `need.eldoria.field_supplies` |
| Display name | Field Supplies |
| Reason | Daily materials are being used for local repairs, care, and ordinary work. |

Those fields are editable on each placed supply component so later NPCs can represent meal stock, healer herbs, workshop fuel, repair parts, caravan feed, and other concrete needs without changing the first runtime shape.

## Need Logic

The current urgency and next action are derived from the existing stock thresholds:

| Stock State | Urgency | Next Action |
|---|---|---|
| Target is satisfied | Stable | Continue Routine |
| Stock is below target but above low threshold | Low | Accept Surplus |
| Stock is at or below low threshold | Urgent | Request Player Help |
| Stock is empty | Critical | Request Player Help |

That keeps ordinary player selling valuable before a quest becomes urgent while still letting a real shortage drive the player-help path.

## Blueprint Calls

- `GetSupplyNeedState`
- `ShouldRequestSupplyHelp`
- `IsSupplyNeedSatisfied`
- `UseRoutineSupply`
- `OnNpcSupplyChanged`

After a delivery, sale, direct stock addition, or successful routine use, UI and AI can query the need state again from the existing stock-change event.

## First Map Test

1. Place an NPC actor with `UHuwamNpcSupplyComponent`.
2. Query `GetSupplyNeedState` on empty stock.
3. Confirm the default field-supplies need is `Critical` and recommends requesting player help.
4. Add enough accepted materials to move stock above the low threshold but below the target.
5. Query again and confirm the state is `Low` and recommends accepting surplus.
6. Fill the target and confirm the state is `Stable`.
7. Let the routine supply timer consume stock back down.
8. Query again and confirm the same need becomes `Urgent` or `Critical` as stock falls.

## Current Boundary

This pass exposes one stock-derived need state.

It does not yet:

- Hold multiple simultaneous needs on one NPC.
- Pick a quest objective from the need state automatically.
- Compare NPC self-solution against player efficiency.
- Record the exact stock use event that caused the shortage.
- Surface the need through dialogue, pre-acceptance offers, or quest-board text.

The first need-aware quest handoff now continues in:

- `Docs/NpcSupplyQuestRequestPrototype.md`
