# Huwan Inventory Prototype V0.1

## Purpose

The inventory prototype is the first gameplay system built on top of the Huwan data model.

It stores item IDs from the data tables, stacks items by their data-defined stack limit, and exposes simple Blueprint-callable functions for adding, removing, and checking items.

## Added Code

Inventory component:

- `Source/Huwam/Gameplay/HuwamInventoryComponent.h`
- `Source/Huwam/Gameplay/HuwamInventoryComponent.cpp`

Main runtime struct:

- `FHuwamInventoryStack`

Main runtime component:

- `UHuwamInventoryComponent`

## Current Behavior

The component supports:

- Add item by item ID.
- Remove item by item ID.
- Check whether an inventory has enough of an item.
- Count total quantity of an item across stacks.
- Return all inventory stacks.
- Track used slots and remaining slots.
- Clear inventory.
- Broadcast an inventory changed event.
- Refresh spoilage state for perishable stacks.
- Preview item freshness after a number of in-game days.
- Preview storage-adjusted freshness.
- Report the best available storage quality for preservation.
- Report available storage routes and route capacity.
- Add new items directly into a chosen storage route.
- Assign the first matching existing stack into a chosen storage route.

## Stack Logic

Each stack stores:

- `ItemId`
- `Quantity`
- Optional freshness metadata for perishable items.
- Optional storage quality metadata for preserved perishable items.
- `StorageRouteId`, such as `storage.loose`, `storage.pouch`, `storage.chest`, `storage.cooled`, or `storage.magical`.

When adding items:

1. The component checks the item's stack limit from the active Huwan data registry.
2. It fills partial stacks first.
3. It creates new stacks if slots remain.
4. It returns a remainder if the inventory cannot fit the full amount.

If no data registry is active yet, the component uses `FallbackStackLimit`, currently `99`.

This matches the world bible rule that many items stack to 99 before rolling into another slot.

Perishable stacks only merge when they share the same acquisition day, freshness profile, freshness state, and storage quality, so old food and fresh food do not become one misleading stack.

Storage preservation currently uses the best matching storage item in the inventory:

- Loose inventory: normal spoilage.
- Small pouch or food pouch: slower spoilage.
- Basic chest: stronger preservation.
- Cooled storage box: major preservation.
- Magical storage box: freshness stasis in this prototype pass.

Route capacity currently uses simple prototype values:

- Small pouch: 4 stacks.
- Food pouch: 8 stacks.
- Basic chest: 20 stacks.
- Cooled storage box: 12 stacks.
- Magical storage box: 50 stacks.

When a stack is reassigned into better storage, its acquisition day stays the same and its current freshness cannot improve. That means a stale herb placed in magical storage remains stale instead of becoming fresh again.

## Item ID Convention

Gameplay should pass item IDs such as:

- `material.wood.basic`
- `material.slime_core.basic`
- `item.weapon.basic_sword`
- `item.tool.pickaxe`

The component converts these IDs to DataTable row names by replacing periods with underscores.

Example:

- Item ID: `item.weapon.basic_sword`
- Row name: `item_weapon_basic_sword`

## First Prototype Uses

The inventory component can support:

- F-rank resource gathering.
- Slime material drops.
- Quest turn-ins.
- Starter crafting ingredients.
- Basic tool ownership.
- Merchant stock experiments.
- Adventuring backpack tests.
- Food, herb, and organic-material spoilage tests.

## First Smoke Tests

After the data registry is active, test:

1. Add `material.wood.basic` quantity `120`.
2. Confirm it creates two stacks if the item stack limit is `99`.
3. Remove `material.wood.basic` quantity `20`.
4. Confirm total quantity becomes `100`.
5. Add `item.weapon.basic_sword` quantity `2`.
6. Confirm it creates two separate stacks because the sword stack limit is `1`.
7. Turn in `material.slime_core.basic` to a quest by removing it from inventory.
8. Preview `material.herb.lavender` after `10` in-game days and confirm it is spoiled.
9. Preview `material.wood.basic` after `10` in-game days and confirm it is stable.
10. Preview `material.herb.lavender` with pouch storage after `10` in-game days and confirm it is stale.
11. Preview `material.herb.lavender` with cooled or magical storage after `10` in-game days and confirm it is fresh.
12. Add a food pouch, assign a loose lavender stack into pouch storage, and confirm the pouch route reports used capacity.

## Next Inventory Step

The next inventory step is survival effect integration for hunger, thirst, and food quality.

Recommended next component:

- A survival/vitals component that reads food freshness and shared Huwam time.

It should use:

- The shared Huwam time subsystem.
- Perishable stack freshness fields.
- The storage route metadata now stamped on stacks.

Inventory comes first because equipment, crafting, gathering, quests, merchants, loot, and storage all depend on item ownership.
