# Character Creation Prototype

This layer turns the world bible's playable identity rules into a reusable Unreal component.

## Goal

Character creation should validate the player's first identity choices before they enter Laucian's tutorial lab.

`UHuwamCharacterCreationComponent` handles:

- Race selection.
- Optional sub-race selection.
- Up to 3 class slots.
- Up to 5 job slots.
- 5 permanent creation skills for normal characters.
- 5 permanent creation abilities for normal characters.
- 15 permanent skills and 15 permanent abilities for Reincarnated characters.
- Starting mana-well depth.
- Starting stat bonuses from race, sub-race, class, job, skills, and abilities.

## Creation Paths

| Path | Skill Limit | Ability Limit | Purpose |
|---|---:|---:|---|
| Normal | 5 | 5 | Standard first character creation. |
| Reincarnated | 15 | 15 | Secret path after true death without revival. |

Normal creation supports the early game fantasy: choose identity, lock in permanent traits, then learn the rest through play.

Reincarnated creation supports the second-life fantasy: more detailed choices, stronger starting options, and better long-term momentum.

## Runtime State

When a character is created, the component stores:

- Character name.
- Race and sub-race IDs.
- Race and sub-race display names.
- Mana-well depth.
- Class states.
- Job states.
- Permanent skill states.
- Permanent ability states.
- Starting stat bonuses.
- Starting town ID, currently `town.eldoria`.

This gives future menus a single place to read the created character identity.

## Validation Rules

The component rejects:

- Missing race.
- Missing class.
- More than 3 class slots.
- More than 5 job slots.
- Too many creation skills for the current path.
- Too many creation abilities for the current path.
- Duplicate class, job, skill, or ability IDs.
- Unknown data IDs when a `UHuwamDataRegistry` is active.
- Sub-races that do not belong to the selected parent race.

If no data registry is active, row checks are skipped with a warning so early Blueprint tests can still run.

## Prototype Default

The prototype player actor now creates a default normal character:

| Choice | Default |
|---|---|
| Race | `race.human` |
| Class | `class.progression_zero` |
| Job | `job.adventurer` |
| Skills | Basic Weapon Handling, Gathering, Woodworking, Tanning, Sewing |
| Abilities | Born Leader, Indomitable, Quick Hands, Natural Gatherer, Stubborn Survivor |

These are not final balance values. They are stable enough to prove the first playable loop.

## Stat Logic

Starting stats use a bonus map layered on top of Progression Zero defaults.

Examples:

- Humans gain all-rounder bonuses.
- Elves gain Intelligence, Mana, Dexterity, and Wisdom.
- Orcs gain Strength, Defense, and Health, but lose Intelligence.
- Demons gain Mana, Charisma, and Strength, but lose World Popularity.
- Beastfolk gain Dexterity, Strength, Luck, and Health.
- Fae gain Mana, Wisdom, and Luck, but lose Health.
- Reincarnated characters gain bonus Luck and World Popularity.

This is intentionally data-friendly. Later, these bonuses can move out of code and into race, sub-race, skill, and ability tables.

## Prototype Player Link

`AHuwamPrototypePlayerActor` now includes `UHuwamCharacterCreationComponent`.

On initialization it:

1. Builds the default character creation request.
2. Applies it to the stats component.
3. Resets equipment.
4. Enables starter core content.
5. Grants and equips starter gear.
6. Initializes combat vitals.
7. Starts the first slime quest.
8. Refreshes HUD data.

The actor also exposes `ApplyCharacterCreationToPrototype`, so a future character creation menu can apply a custom request before entering Eldoria.

## Next Step

The next practical layer is a starter character creation menu data adapter:

- Available race cards.
- Available sub-races filtered by race.
- Available class/job choices.
- Skill and ability selection counts.
- Validation errors and warnings for UI.
- Confirm button output into `ApplyCharacterCreationToPrototype`.
