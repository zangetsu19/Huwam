# Huwan Equipment Prototype V0.1

## Purpose

The equipment prototype turns inventory items into worn, held, or slotted gear.

Inventory answers:

- What does the player own?

Equipment answers:

- What is the player using right now?

## Added Code

Equipment component:

- `Source/Huwam/Gameplay/HuwamEquipmentComponent.h`
- `Source/Huwam/Gameplay/HuwamEquipmentComponent.cpp`

Main runtime struct:

- `FHuwamEquipmentSlotState`

Main runtime component:

- `UHuwamEquipmentComponent`

## Current Behavior

The component supports:

- Equip an item from inventory.
- Unequip an item back into inventory.
- Replace an equipped item and return the old item to inventory.
- Check whether an item can equip to a slot.
- Check whether a slot is unlocked.
- Unlock a locked slot.
- Read the item equipped in a slot.
- Reset to default equipment slots.
- Broadcast an equipment-changed event.

## V0.1 Slot List

The component starts with these slots:

- `slot.head`
- `slot.face`
- `slot.neck`
- `slot.ear.left`
- `slot.ear.right`
- `slot.chest`
- `slot.arms`
- `slot.hands`
- `slot.legs`
- `slot.feet`
- `slot.underwear`
- `slot.weapon.main_hand`
- `slot.weapon.off_hand`
- `slot.weapon.ranged`
- `slot.ammo`
- `slot.quiver`
- `slot.tool`
- `slot.backpack`
- `slot.pouch.1`
- `slot.pouch.2`
- `slot.pouch.3`
- `slot.pouch.4`
- `slot.pouch.5`
- `slot.magical_storage`
- `slot.wizard.book`
- `slot.mage.component`

The magical storage, wizard book, and mage component slots begin locked for now. They can be unlocked by progression, class, job, or tutorial logic later.

## Item Slot Data

The item data table now includes:

- `PrimaryEquipSlotId`

Examples:

| Item | Primary Slot |
|---|---|
| `item.weapon.basic_sword` | `slot.weapon.main_hand` |
| `item.weapon.basic_bow` | `slot.weapon.ranged` |
| `item.container.basic_quiver` | `slot.quiver` |
| `item.tool.pickaxe` | `slot.tool` |
| `item.container.small_pouch` | `slot.pouch.1` |
| `item.bag.adventuring_backpack` | `slot.backpack` |
| `item.gear.slime_test_piece` | `slot.chest` |

The component can also fall back to simple ID logic before the data registry is active.

## Equip Flow

When equipping:

1. Check inventory has the item.
2. Resolve the target slot from preferred slot or item data.
3. Check the target slot exists and is unlocked.
4. Check the item can use that slot.
5. Remove one item from inventory.
6. Equip it.
7. Return any replaced item to inventory.
8. Broadcast equipment changed.

If returning the old item to inventory fails, the equip action rolls back.

## First Smoke Tests

After importing data and creating a registry:

1. Add `item.weapon.basic_sword` to inventory.
2. Equip it with no preferred slot.
3. Confirm it lands in `slot.weapon.main_hand`.
4. Add `item.weapon.basic_knife` to inventory.
5. Equip it into `slot.weapon.off_hand`.
6. Add `item.weapon.basic_bow` to inventory.
7. Equip it with no preferred slot.
8. Confirm it lands in `slot.weapon.ranged`.
9. Add `item.container.small_pouch` to inventory.
10. Equip it into `slot.pouch.3`.
11. Unlock `slot.magical_storage` and confirm it becomes available.

## Equipment Stats Step

The first equipment stat step is now started.

Completed V0.1 work:

- Add lightweight item stat modifiers.
- Add equipment-derived stat totals.
- Add basic weapon damage fields to item data.
- Add armor defense fields to item data.

See `Docs/EquipmentStatsPrototype.md`.

The next equipment step is connecting these totals to player stats, combat damage, survival checks, and the character sheet.
