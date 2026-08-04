# Storage Routing Prototype

This layer turns preservation from a passive best-container rule into the first assignable storage route system.

## Goal

Players should eventually decide where items live:

- Loose pack.
- Pouches.
- Chests.
- Cooled storage.
- Magical storage.

This first pass gives code and UI enough structure to show storage capacity and route stacks into those categories.

## Runtime Shape

Inventory stacks now carry:

- `StorageQuality`
- `StorageRouteId`
- `SpoilageDurationMultiplier`

The inventory component exposes storage route snapshots through `FHuwamInventoryStorageRoute`.

Each route reports:

- Route ID.
- Display name.
- Container item ID.
- Storage quality.
- Used stacks.
- Maximum stacks.
- Remaining stacks.
- Preservation multiplier.
- Availability.

## Prototype Capacities

| Storage Item | Route | Capacity |
|---|---|---:|
| `item.container.small_pouch` | `storage.pouch` | 4 stacks |
| `item.container.food_pouch` | `storage.pouch` | 8 stacks |
| `item.container.basic_chest` | `storage.chest` | 20 stacks |
| `item.container.cooled_storage_box` | `storage.cooled` | 12 stacks |
| `item.container.magical_storage_box` | `storage.magical` | 50 stacks |

Loose pack space still uses normal inventory slot limits.

## Blueprint Calls

- `GetStorageRoutes`
- `GetUsedStorageRouteStacks`
- `GetMaxStorageRouteStacks`
- `GetRemainingStorageRouteStacks`
- `AddItemByIdToStorage`
- `AssignFirstMatchingStackToStorage`

## HUD Surface

`FHuwamHudInventorySnapshot` now includes storage routes.

The native prototype menu shows:

- Inventory slot usage.
- Storage route capacity.
- Item stack storage quality.
- Item stack freshness.

This is still a code-built prototype surface, not the final polished inventory UI.

## Important Rule

Moving stale or spoiled goods into better storage does not make them fresh again. The acquisition day is preserved, and freshness cannot improve during reassignment.

## Next Layer

The next clean system is survival integration:

- Hunger.
- Thirst.
- Social energy.
- Food quality effects.
- Unsafe/spoiled food consequences.
