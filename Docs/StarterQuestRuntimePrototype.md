# Starter Quest Runtime Prototype

This layer makes the first Adventurers Guild quest usable at runtime.

## Goal

The first quest loop should work like this:

1. Player starts `Slimes in the Tall Grass`.
2. Quest state appears in HUD/menu data.
3. Player defeats Basic Slimes.
4. Reward bridge advances the slime objective.
5. Quest runtime refreshes progress.
6. Quest becomes ready to turn in at `3 / 3`.
7. Player turns in the quest and receives the quest reward.

`UHuwamQuestComponent` handles the quest state.

## Data Shape

`DT_Quests.csv` now has first-pass objective fields:

| Field | Purpose |
|---|---|
| PrimaryObjectiveId | Stable objective ID. |
| PrimaryObjectiveDescription | Text for HUD/menu display. |
| PrimaryObjectiveTargetValue | Completion target. |
| PrimaryObjectiveProgressSourceId | What source advances the objective. |
| RewardGold | Gold paid at turn-in. |
| RewardExperience | Experience paid at turn-in. |

## Slimes In The Tall Grass

| Field | Value |
|---|---|
| QuestId | `quest.eldoria.slimes_in_tall_grass` |
| ObjectiveId | `objective.defeat_basic_slime` |
| Objective | Defeat or harvest 3 basic slimes. |
| Target | 3 |
| Progress Source | `monster.slime.basic` |
| Turn-in Gold | 15 |
| Turn-in Experience | 10 |

## Runtime State

`FHuwamQuestRuntimeState` stores:

- Quest ID.
- Display name.
- Description.
- Quest type.
- Urgency.
- Quest giver.
- Primary objective state.
- Reward items.
- Reward gold.
- Reward experience.
- World state effect.
- Active, ready-to-turn-in, and turned-in flags.

## Blueprint Flow

For the first slime quest:

1. Add `UHuwamQuestComponent`, `UHuwamRewardComponent`, and `UHuwamInventoryComponent` to the player.
2. Call `StartSlimesInTallGrassQuest`.
3. Defeat a Basic Slime encounter.
4. Call `ClaimMonsterDefeatRewards` on the reward component with:
   - `quest.eldoria.slimes_in_tall_grass`
   - `objective.defeat_basic_slime`
   - progress delta `1`
   - target value `3`
5. Call `RefreshQuestProgressFromRewards`.
6. Read `GetActiveQuestStates` for HUD/menu display.
7. When `bReadyToTurnIn` is true, call `TurnInQuest`.

Expected totals after three Basic Slimes and quest turn-in:

| Source | Gold | Experience |
|---|---:|---:|
| Monster rewards | 9 | 15 |
| Quest turn-in | 15 | 10 |
| Total | 24 | 25 |

The player should also have three slime cores if all monster drops were claimed.

## HUD/Menu Step

The first HUD/menu data adapter is now started:

- Active quest list.
- Objective text.
- Current and target progress.
- Ready-to-turn-in indicator.
- Gold and XP readouts.
- Health and mana readouts from combat.

See `Docs/HudMenuDataPrototype.md`.
