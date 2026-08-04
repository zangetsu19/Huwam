# Basic Slime Encounter Prototype

This layer creates the first enemy encounter path for Huwam.

## Goal

The Basic Slime needs to prove the first combat loop:

- Load monster data.
- Apply monster stats.
- Initialize health and mana.
- Let the player attack it.
- Let it attack back.
- Mark it defeated.
- Grant simple rewards.

`AHuwamMonsterEncounterActor` is the first Blueprint-placeable encounter actor for this.

## Monster Data

`DT_Monsters.csv` now includes starter combat and reward fields:

| Field | Purpose |
|---|---|
| BaseStrength | Monster melee power seed. |
| BaseDexterity | Monster ranged/agility seed. |
| BaseHealth | Monster health seed. |
| BaseMana | Monster mana seed. |
| BaseDefense | Monster armor seed. |
| BaseLuck | Monster luck seed. |
| PrimaryDropItemId | Simple V0.1 drop item. |
| PrimaryDropQuantity | Simple V0.1 drop quantity. |
| CurrencyRewardCopper | Copper-native currency payout after defeat. |
| ExperienceReward | Experience payout after defeat. |

## Basic Slime V0.1

| Value | Setting |
|---|---:|
| Strength | 2 |
| Dexterity | 3 |
| Health | 2 |
| Mana | 0 |
| Defense | 1 |
| Luck | 2 |
| Copper Reward | 3 |
| Experience Reward | 5 |
| Primary Drop | `material.slime_core.basic` x1 |

With the current stat formula, the Basic Slime initializes around `72` max health.

## Encounter Actor

`AHuwamMonsterEncounterActor` creates:

- `UHuwamCharacterStatsComponent`
- `UHuwamCombatComponent`
- A simple scene root

It can:

- Load `monster.slime.basic` from the data subsystem.
- Fall back to Basic Slime values if data is unavailable.
- Attack a target combat component.
- Receive attacks from another combat component.
- Grant defeat rewards into an inventory.
- Shrink its blockout sphere as health falls, then hide its mesh and collision after defeat so the field reads more clearly.

## First Playtest Flow

1. Create or open a test map.
2. Place a Blueprint subclass of `AHuwamMonsterEncounterActor`.
3. Keep `MonsterId` as `monster.slime.basic`.
4. Add stats, equipment, combat, and inventory components to a test player actor.
5. Give the player `item.weapon.basic_sword`.
6. Equip the sword.
7. Call `ReceiveAttackFrom` on the slime using the player's combat component.
8. When the slime is defeated, call `GrantDefeatRewards`.

Expected early fight:

- Player Basic Sword melee damage should chip the slime down in a few hits.
- Slime attacks are weak but visible.
- Reward grants one slime core, three copper, and five experience.

## Reward Bridge Step

The first loot and reward bridge is now started:

- Store currency and experience somewhere real.
- Decide how monster rewards connect to quests.
- Trigger the "Slimes in the Tall Grass" quest progress.
- Feed harvested slime materials into inventory, crafting, and NPC requests.

See `Docs/RewardBridgePrototype.md`.
