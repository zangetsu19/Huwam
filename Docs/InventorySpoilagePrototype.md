# Inventory Spoilage Prototype

This layer adds the first freshness state to inventory stacks.

## Goal

The first spoilage pass should prove:

1. Perishable goods can carry freshness state.
2. Spoilage uses the shared Huwam living time service.
3. Spoiled goods remain visible instead of silently vanishing.
4. Later cooking, storage, hunger, taverns, farms, trade, and NPC food requests can build on the same stack data.

## Runtime Piece

`FHuwamInventoryStack` now carries optional freshness fields:

- `bTracksFreshness`
- `AcquiredInGameDay`
- `StaleAfterInGameDays`
- `SpoilsAfterInGameDays`
- `Freshness`
- `StorageQuality`
- `SpoilageDurationMultiplier`

Freshness states are:

| State | Meaning |
|---|---|
| Stable | Item does not currently spoil. |
| Fresh | Perishable and still good. |
| Stale | Edible or usable at reduced quality later. |
| Spoiled | Unsafe, unpleasant, or suitable for other systems later. |

## First Rules

The first profile uses simple item/material identity:

| Item Type | Stale | Spoiled |
|---|---:|---:|
| Fish or meat | 1 day | 1 day |
| Wild fruit | 1 day | 2 days |
| Wild vegetables | 2 days | 3 days |
| Herbs and other edible materials | 5 days | 10 days |
| Honey | 30 days | Does not spoil in this pass |
| Non-food materials | Stable | Stable |

Material rows marked `bEdible` also track freshness.

## Storage Preservation

New perishable stacks are stamped with the best storage quality currently available in that inventory.

| Storage | Multiplier | Result |
|---|---:|---|
| Loose | 1.0x | Normal spoilage timing. |
| Pouch | 1.5x | Small pouch or food pouch slows spoilage. |
| Chest | 2.0x | Basic chest keeps goods organized and protected. |
| Cooled | 5.0x | Cooled storage greatly extends freshness. |
| Magical | Stasis | Perishable goods stay fresh in this prototype pass. |

Recognized storage item IDs:

- `item.container.small_pouch`
- `item.container.food_pouch`
- `item.container.basic_chest`
- `item.container.cooled_storage_box`
- `item.container.magical_storage_box`

Storage now has route capacity:

| Route | Capacity |
|---|---:|
| Small pouch | 4 stacks each |
| Food pouch | 8 stacks each |
| Basic chest | 20 stacks each |
| Cooled storage box | 12 stacks each |
| Magical storage box | 50 stacks each |

The first route assignment API can move an existing stack into a storage route without refreshing its acquisition day. Cooling or magically storing something stale does not make it fresh again.

## Blueprint Calls

- `RefreshSpoilageState`
- `PreviewItemFreshnessAfterInGameDays`
- `PreviewItemFreshnessAfterInGameDaysWithStorage`
- `GetBestStorageQuality`
- `GetStorageRoutes`
- `GetUsedStorageRouteStacks`
- `GetMaxStorageRouteStacks`
- `GetRemainingStorageRouteStacks`
- `AddItemByIdToStorage`
- `AssignFirstMatchingStackToStorage`
- `GetSpoilageDurationMultiplierForStorage`
- `GetInventoryStacks`

## Stack Behavior

Perishable stacks only merge when their item ID, acquisition day, freshness profile, freshness state, storage quality, and storage multiplier match. That avoids mixing old food with fresh food, or loose food with carefully preserved food, in the same stack.

Saved inventory stacks now restore exact stack metadata instead of re-adding everything as newly acquired items.

## Current Boundary

This pass does not yet:

- Convert spoiled items into separate spoiled-item IDs.
- Apply sickness, poison, morale, value, or relationship effects.
- Provide drag-and-drop UI for choosing exact individual containers.
- Drive hunger or thirst.
- Attach spoilage to NPC meal services.

Those are now clean next layers because the stack data has a freshness shape.
