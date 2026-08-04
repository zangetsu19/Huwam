# Reward Bridge Prototype

This layer gives Huwam a first real place to store currency, experience, and quest progress.

## Goal

Defeating a monster should not only return loose numbers. The game needs a bridge that can:

- Grant item drops into inventory.
- Add currency to the character.
- Add experience to the character.
- Advance quest objectives.
- Prevent the same defeated monster from paying rewards twice.

`UHuwamRewardComponent` handles that first pass.

## Components Involved

| Component | Role |
|---|---|
| `UHuwamInventoryComponent` | Receives item drops such as slime cores. |
| `UHuwamRewardComponent` | Stores copper-native currency, experience, and quest progress. |
| `AHuwamMonsterEncounterActor` | Owns monster defeat reward data and blocks double claims. |

## Stored Values

`UHuwamRewardComponent` currently tracks:

| Value | Purpose |
|---|---|
| CurrencyBalanceCopper | Spendable Huwan currency stored exactly as copper. |
| CurrencyBreakdown | Copper, silver, gold, platinum, and Marquis Diamond Print display values. |
| TotalExperience | Lifetime experience earned by the character. |
| QuestProgressRecords | Per-quest objective progress records. |

## Quest Progress Records

Each quest objective record stores:

| Field | Purpose |
|---|---|
| QuestId | The quest being advanced. |
| ObjectiveId | The objective inside that quest. |
| CurrentValue | Current progress amount. |
| TargetValue | Required amount. |
| bCompleted | Whether the target has been reached. |

For the first slime quest, use:

| Field | Value |
|---|---|
| QuestId | `quest.eldoria.slimes_in_tall_grass` |
| ObjectiveId | `objective.defeat_basic_slime` |
| TargetValue | `3` |

## Defeat Reward Flow

1. Player attacks the Basic Slime.
2. Slime reaches `bDefeated`.
3. Player reward component calls `ClaimMonsterDefeatRewards`.
4. Slime grants item rewards into inventory.
5. Reward component adds currency and experience.
6. Reward component advances quest progress if a quest/objective was provided.
7. Slime marks rewards claimed so it cannot pay twice.

## Basic Slime Expected Reward

| Reward | Amount |
|---|---:|
| Slime Core | 1 |
| Copper | 3 |
| Experience | 5 |
| Quest Progress | +1 basic slime defeated |

## Blueprint Test

After placing a Basic Slime encounter actor:

1. Add `UHuwamInventoryComponent` and `UHuwamRewardComponent` to the player actor.
2. Defeat the slime.
3. Call `ClaimMonsterDefeatRewards`.
4. Pass the player's inventory.
5. Pass `quest.eldoria.slimes_in_tall_grass`.
6. Pass `objective.defeat_basic_slime`.
7. Pass progress delta `1`.
8. Pass target value `3`.

Expected result after one slime:

| Value | Expected |
|---|---:|
| Slime Core Quantity | 1 |
| CurrencyBalanceCopper | 3 |
| TotalExperience | 5 |
| Quest Progress | 1 / 3 |
| Quest Complete | false |

After three Basic Slimes, the quest objective should report complete.

## Quest Runtime Step

The first starter quest runtime is now started:

- Activate `quest.eldoria.slimes_in_tall_grass`.
- Show its objective in HUD/menu data.
- Complete it when the reward bridge reports `3 / 3`.
- Grant the quest's own currency reward after turn-in.

See `Docs/StarterQuestRuntimePrototype.md`.
