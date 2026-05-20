# Equipment Stats Prototype

This layer gives equipment a first mechanical effect without trying to solve the full combat system yet.

## Goal

Equipped items now contribute four simple totals:

| Stat | Purpose |
|---|---|
| AttackPower | Weapon damage, ammo help, and offensive gear pressure. |
| DefensePower | Armor, shields, survival gear, and protective bonuses. |
| ManaPower | Spell focus, mana support, enchanted gear, and magical fuel. |
| UtilityPower | Tools, storage, gathering quality, travel help, and non-combat value. |

These are intentionally broad V0.1 values. Later systems can split them into detailed damage types, armor types, enchantments, rarity curves, durability, and class scaling.

## Runtime Behavior

`UHuwamEquipmentComponent` can now total equipped item stats from the active item data table.

Blueprint callable functions:

- `GetEquipmentStatSummary`
- `GetEquipmentAttackPower`
- `GetEquipmentDefensePower`
- `GetEquipmentManaPower`
- `GetEquipmentUtilityPower`

The component checks each equipped slot, loads the matching `FHuwamItemRow`, and adds that item's stat values into a single summary.

## Starter Item Values

Starter equipment now has enough values to test early gameplay:

| Item | Attack | Defense | Mana | Utility |
|---|---:|---:|---:|---:|
| Basic Sword | 5 | 0 | 0 | 0 |
| Basic Knife | 2 | 0 | 0 | 1 |
| Basic Bow | 4 | 0 | 0 | 0 |
| Basic Arrows | 1 | 0 | 0 | 0 |
| Basic Quiver | 0 | 0 | 0 | 2 |
| Gathering Knife | 1 | 0 | 0 | 2 |
| Pickaxe | 2 | 0 | 0 | 3 |
| Hatchet | 2 | 0 | 0 | 3 |
| Fishing Rod | 0 | 0 | 0 | 3 |
| Adventuring Backpack | 0 | 1 | 0 | 8 |
| Slime Test Piece | 0 | 2 | 1 | 1 |

## Smoke Test

After importing `Content/Data/DT_Items.csv` into Unreal and assigning it to the data registry:

1. Give a test character an inventory component and equipment component.
2. Add `item.weapon.basic_sword` and `item.bag.adventuring_backpack` to inventory.
3. Equip both items.
4. Call `GetEquipmentStatSummary`.

Expected V0.1 result:

| Attack | Defense | Mana | Utility |
|---:|---:|---:|---:|
| 5 | 1 | 0 | 8 |

## Next Step

The next layer should be a starter stat or combat component that combines:

- Base player stats.
- Race and subrace modifiers.
- Class and job bonuses.
- Equipment stat summary.
- Temporary buffs, debuffs, blessings, curses, and status effects.

That gives Huwam a clean path from "I equipped a sword" to "my combat math and character sheet changed."
