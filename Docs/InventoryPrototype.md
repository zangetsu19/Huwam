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

## Stack Logic

Each stack stores:

- `ItemId`
- `Quantity`

When adding items:

1. The component checks the item's stack limit from the active Huwan data registry.
2. It fills partial stacks first.
3. It creates new stacks if slots remain.
4. It returns a remainder if the inventory cannot fit the full amount.

If no data registry is active yet, the component uses `FallbackStackLimit`, currently `99`.

This matches the world bible rule that many items stack to 99 before rolling into another slot.

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

## First Smoke Tests

After the data registry is active, test:

1. Add `material.wood.basic` quantity `120`.
2. Confirm it creates two stacks if the item stack limit is `99`.
3. Remove `material.wood.basic` quantity `20`.
4. Confirm total quantity becomes `100`.
5. Add `item.weapon.basic_sword` quantity `2`.
6. Confirm it creates two separate stacks because the sword stack limit is `1`.
7. Turn in `material.slime_core.basic` to a quest by removing it from inventory.

## Next Inventory Step

The next inventory step is equipment.

Recommended next component:

- `UHuwamEquipmentComponent`

It should use:

- Equipment slot IDs from item data.
- Character slot rules from the world bible.
- Validation against class/job/race requirements later.

Inventory comes first because equipment, crafting, gathering, quests, merchants, loot, and storage all depend on item ownership.
