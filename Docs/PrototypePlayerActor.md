# Prototype Player Actor

This layer bundles the first playable-loop systems into one Blueprint-placeable actor.

## Goal

The early prototype needs a simple actor that can prove the full loop without manually adding every component each time.

`AHuwamPrototypePlayerActor` includes:

- `UHuwamInventoryComponent`
- `UHuwamEquipmentComponent`
- `UHuwamCharacterStatsComponent`
- `UHuwamCharacterCreationComponent`
- `UHuwamCombatComponent`
- `UHuwamRewardComponent`
- `UHuwamQuestComponent`
- `UHuwamLiveContentComponent`
- `UHuwamHudDataComponent`

## Auto Initialization

On begin play, the actor can:

1. Apply a default character creation request.
2. Reset equipment slots.
3. Enable the starter core content pack.
4. Grant starter loadout.
5. Equip starter gear.
6. Initialize health and mana.
7. Start `Slimes in the Tall Grass`.
8. Refresh the HUD snapshot.

## Starter Loadout

| Item | Use |
|---|---|
| `item.weapon.basic_sword` | Main hand weapon. |
| `item.weapon.basic_knife` | Off-hand weapon. |
| `item.bag.adventuring_backpack` | Backpack slot. |

## Helper Functions

| Function | Purpose |
|---|---|
| `InitializePrototypePlayer` | Resets the actor into the first playable-loop state. |
| `GrantStarterLoadout` | Adds basic starter items if missing. |
| `EquipStarterGear` | Equips sword, knife, and backpack. |
| `StartSlimeQuest` | Starts `Slimes in the Tall Grass`. |
| `ApplyCharacterCreationToPrototype` | Applies a custom character creation request before play. |
| `AttackMonster` | Attacks a monster encounter actor with starter melee config. |
| `ClaimMonsterRewards` | Claims monster rewards and advances slime quest progress. |
| `AttackMonsterAndClaimIfDefeated` | Convenience helper for the first loop. |
| `TurnInSlimeQuest` | Turns in the slime quest after progress reaches `3 / 3`. |
| `RefreshPrototypeHud` | Refreshes and returns the HUD snapshot. |

## First Map Test

1. Place `AHuwamPrototypePlayerActor` in a test map.
2. Place three `AHuwamMonsterEncounterActor` instances, each using `monster.slime.basic`.
3. Begin play.
4. Call `AttackMonsterAndClaimIfDefeated` until each slime is defeated.
5. Call `RefreshPrototypeHud`.
6. Confirm the active quest reaches `3 / 3`.
7. Call `TurnInSlimeQuest`.

Expected final totals:

| Source | Gold | Experience |
|---|---:|---:|
| Three Basic Slime rewards | 9 | 15 |
| Quest turn-in | 15 | 10 |
| Final total | 24 | 25 |

The player should also have three slime cores if each defeated slime reward was claimed.

## Next Step

The next practical layer is a character creation menu data adapter:

- Race and sub-race cards.
- Class and job selection panels.
- Skill and ability counters.
- Validation output.
- Confirm action into `ApplyCharacterCreationToPrototype`.

See `Docs/CharacterCreationPrototype.md`.
