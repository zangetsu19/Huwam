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

It also now has a fallback start path for the first tutorial quest:

- `quest.tutorial.progression_zero`
- `StartProgressionZeroTutorialQuest`

It also now has a fallback start path for the first gathering quest:

- `quest.eldoria.gather_field_supplies`
- `StartGatherFieldSuppliesQuest`

## Data Shape

`DT_Quests.csv` now has first-pass objective fields:

| Field | Purpose |
|---|---|
| PrimaryObjectiveId | Stable objective ID. |
| PrimaryObjectiveDescription | Text for HUD/menu display. |
| PrimaryObjectiveTargetValue | Completion target. |
| PrimaryObjectiveProgressSourceId | What source advances the objective. |
| bRepeatable | Whether a turned-in quest can be offered again when the world condition returns. |
| RewardCopper | Copper-native currency paid at turn-in. |
| RewardExperience | Experience paid at turn-in. |

## Slimes In The Tall Grass

| Field | Value |
|---|---|
| QuestId | `quest.eldoria.slimes_in_tall_grass` |
| ObjectiveId | `objective.defeat_basic_slime` |
| Objective | Defeat or harvest 3 basic slimes. |
| Target | 3 |
| Progress Source | `monster.slime.basic` |
| Turn-in Copper | 15 |
| Turn-in Experience | 10 |

## Runtime State

`FHuwamQuestRuntimeState` stores:

- Quest ID.
- Display name.
- Description.
- Quest type.
- Urgency.
- Repeatable flag.
- Quest giver.
- Primary objective state.
- Need request context for NPC-driven quest explanations when present.
- Reward items.
- Reward currency.
- Reward experience.
- World state effect.
- Active, ready-to-turn-in, and turned-in flags.

The quest component now also stores one tracked active quest ID so HUD and map systems can agree on the current focus.

`FHuwamQuestOffer` previews authored quest content before a quest becomes active. The first NPC supply offer uses that preview to explain a field-supplies request before the player accepts it.

Repeatable quests can restart after turn-in. When a repeatable quest starts again, the quest component removes the old turned-in blocker and resets the matching reward-progress record so the player has to complete the objective again instead of instantly cashing in old progress.

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

| Source | Copper | Experience |
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
- Currency and XP readouts.
- Health and mana readouts from combat.

See `Docs/HudMenuDataPrototype.md`.

## Tutorial Quest Step

The first Laucian white-lab handoff can start `Welcome to Progression Zero` even before imported quest DataTables are linked.

Quest tracking for that tutorial path is now started too:

- `TrackQuest`
- `GetTrackedQuestId`
- `Docs/TutorialLabQuestPrototype.md`

See:

- `Docs/TutorialLabHandoffPrototype.md`

## Field Gathering Step

The first reusable field resource node can now advance active gather objectives by progress source.

`AHuwamGatheringResourceActor` defaults to `material.field_supply`, and `AHuwamPrototypePlayerActor` can now start the existing `Gather Field Supplies` quest through a low-stock `UHuwamNpcSupplyComponent`. Its turn-in path passes accepted gathered materials into that supply component before the currency and experience reward resolves.

See:

- `Docs/GatheringResourcePrototype.md`
- `Docs/GatherFieldSuppliesQuestPrototype.md`
- `Docs/NpcSupplyDeliveryPrototype.md`
- `Docs/NpcSupplyShortageGatePrototype.md`
- `Docs/NpcSupplyTradeUsePrototype.md`
