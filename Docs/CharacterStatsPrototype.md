# Character Stats Prototype

This layer gives Huwam its first runtime character sheet foundation.

## Goal

The prototype needs one place that can answer:

- What are this character's base stats?
- What permanent bonuses have they earned?
- What temporary buffs or debuffs are active?
- What does equipped gear add?
- What are the current derived combat and utility values?

`UHuwamCharacterStatsComponent` is that first bridge.

## Core Stats

Progression Zero defaults are intentionally simple:

| Stat | Default |
|---|---:|
| Strength | 10 |
| Charisma | 10 |
| Intelligence | 10 |
| Wisdom | 10 |
| Dexterity | 10 |
| Luck | 10 |
| Health | 10 |
| Mana | 10 |
| Defense | 10 |
| World Popularity | 0 |

Imported stat table rows can clamp values between their configured minimum and maximum. The current data table uses `0` to `999`.

## Stat Layers

Each stat snapshot is built from:

| Layer | Purpose |
|---|---|
| BaseValue | Starting value plus normal stat growth. |
| PermanentBonus | Race, subrace, class, job, title, reincarnation, or achievement bonuses. |
| TemporaryBonus | Buffs, debuffs, blessings, curses, food, potions, weather, and statuses. |
| EquipmentBonus | Gear-based stat support. V0.1 maps equipment defense to Defense and equipment mana to Mana. |
| FinalValue | The clamped total. |

## Equipment Bridge

The character stats component can find the owner's `UHuwamEquipmentComponent`, or use one passed in manually.

Equipment contributes:

| Equipment Value | Current Use |
|---|---|
| AttackPower | Added to melee and ranged power. |
| DefensePower | Added to Defense, then Armor Rating. |
| ManaPower | Added to Mana, then Max Mana and Spell Power. |
| UtilityPower | Added to Utility Score. |

## Derived Summary

`GetCharacterPowerSummary` returns a compact Blueprint-ready summary.

Current V0.1 formulas:

| Derived Value | Formula |
|---|---|
| MaxHealth | `50 + Health * 10 + Defense * 2` |
| MaxMana | `20 + Mana * 10 + Intelligence * 2` |
| MeleePower | `Strength + EquipmentAttackPower` |
| RangedPower | `Dexterity + EquipmentAttackPower` |
| SpellPower | `Intelligence + Wisdom / 2 + EquipmentManaPower` |
| ArmorRating | `Defense` |
| UtilityScore | `Dexterity + Luck + EquipmentUtilityPower` |
| SocialPower | `Charisma + WorldPopularity / 10` |

These formulas are not final balance. They are the first working hooks.

## Blueprint Functions

- `ResetToProgressionZeroDefaults`
- `SetBaseStat`
- `AddBaseStat`
- `GetBaseStat`
- `SetPermanentStatBonus`
- `SetTemporaryStatBonus`
- `ClearTemporaryStatBonuses`
- `GetStatValue`
- `GetAllStatValues`
- `GetCharacterPowerSummary`

## First Smoke Test

After importing data and assigning the registry:

1. Add `UHuwamCharacterStatsComponent`, `UHuwamInventoryComponent`, and `UHuwamEquipmentComponent` to a test character.
2. Add and equip `item.weapon.basic_sword`.
3. Add and equip `item.bag.adventuring_backpack`.
4. Call `GetCharacterPowerSummary`.

Expected V0.1 result with default stats:

| Value | Expected |
|---|---:|
| Strength | 10 |
| Defense | 11 |
| Mana | 10 |
| EquipmentAttackPower | 5 |
| EquipmentDefensePower | 1 |
| EquipmentUtilityPower | 8 |
| MaxHealth | 172 |
| MaxMana | 140 |
| MeleePower | 15 |
| ArmorRating | 11 |
| UtilityScore | 28 |

## Combat Step

The first starter combat layer is now started:

- Take attacker power.
- Take defender armor.
- Add a small roll or variance.
- Apply damage to current health.
- Leave room for skills, abilities, spells, and status effects later.

See `Docs/CombatPrototype.md`.
