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
| Sub-Race | `race.human.common` |
| Class | `class.progression_zero` |
| Job | `job.adventurer` |
| Skills | Basic Weapon Handling, Gathering, Woodworking, Tanning, Sewing |
| Abilities | Born Leader, Indomitable, Quick Hands, Natural Gatherer, Stubborn Survivor |

These are not final balance values. They are stable enough to prove the first playable loop.

## Stat Logic

Starting stats use a bonus map layered on top of Progression Zero defaults.

Examples:

- Humans gain all-rounder bonuses.
- Human sub-races now add first-pass identity bonuses that match their approved art direction.
- High-Humans lean toward Charisma, Intelligence, and Mana.
- Dawn Humans lean toward Wisdom, Mana, and Charisma.
- Dusk Humans lean toward Defense, Strength, and Wisdom.
- Frontier Humans lean toward Wisdom, Dexterity, Health, and Luck.
- Urban Humans lean toward Charisma, Luck, and Intelligence.
- Warborn Humans lean toward Strength, Defense, and Health, but lose some Intelligence.
- Blessed Humans lean toward Mana, Wisdom, and World Popularity.
- Cursed Humans lean toward Dexterity, Luck, and Mana, but lose some Charisma.
- Elves gain Intelligence, Mana, Dexterity, and Wisdom.
- Dwarves gain Strength, Defense, and Wisdom.
- Dwarf sub-races now add first-pass identity bonuses that match their approved art direction.
- Mountain Dwarves lean toward Defense, Strength, and Wisdom.
- Forge Dwarves lean toward Strength, Dexterity, and Mana.
- Quarry Dwarves lean toward Defense, Dexterity, and Charisma.
- Deep Dwarves lean toward Defense, Dexterity, and Wisdom.
- Goldbeard Dwarves lean toward Charisma and Luck.
- Iron Oath Dwarves lean toward Defense and Wisdom.
- Dreamforge Dwarves lean toward Mana and Intelligence.
- Hearth Dwarves lean toward Charisma, Wisdom, and Luck.
- Orcs gain Strength, Defense, and Health, but lose Intelligence.
- Orc sub-races now add first-pass identity bonuses that match their approved art direction.
- Common Orcs lean toward Strength, Defense, and Health, but lose some Intelligence.
- Half-Orcs lean toward Strength, Defense, and Charisma as the half-Human bridge.
- War Orcs lean toward Strength and Health, but lose some Intelligence.
- Stonehide Orcs lean heavily toward Defense, but lose some Dexterity.
- Redtusk Orcs lean toward Strength and Health, but lose some Charisma from their scary reputation.
- Gray Orcs lean toward Wisdom and Intelligence.
- Ironbound Orcs lean toward Strength, Defense, and Wisdom.
- Wild Orcs lean toward Dexterity and Luck, but lose some Intelligence.
- Demons gain Mana, Charisma, and Strength, but lose World Popularity.
- Beastfolk gain Dexterity, Strength, Luck, and Health.
- Fae gain Mana, Wisdom, and Luck, but lose Health.
- Reincarnated characters gain bonus Luck and World Popularity.

This is intentionally data-friendly. Later, these bonuses can move out of code and into race, sub-race, skill, and ability tables.

## Human Visual Baselines

The approved Human sub-race art direction now acts as the first prototype character creation baseline.

| Sub-Race | Visual Identity | Prototype Use |
| --- | --- | --- |
| Common Human | Level 1 adventurer with worn leather, soft cloth, and a used short sword. | Default Human selection. |
| High-Human | Alabaster noble spellcaster with fine fabrics and smug posture. | Noble/magic Human preview. |
| Dawn Human | Yellow-warmed cheerful holy Human with bright garments and gentle aura. | Holy support preview. |
| Dusk Human | Dark-skinned stalwart tank with knight armor, greatsword, kite shield, and void aura. | Guard/tank preview. |
| Frontier Human | Red sunburnt farmer-ranger with wild predator-watchful eyes. | Ranger/farmer preview. |
| Urban Human | Slightly dirty city merchant with softer build, ledgers, pouches, and trade gear. | Merchant/social preview. |
| Warborn Human | Dark tanned scarred frontliner with fierce battle-happy expression. | Barbarian/guard/war-knight preview. |
| Half-Elf | Human-Elf hybrid with very short ears, sword gear, charisma, and mixed-culture appeal. | Hybrid adventurer preview, available from Human hybrid and Elf paths. |
| Blessed Human | Ebony skin, white hair, white eyes, wings, deity tattoos, radiant aura. | Rare divine-touched preview. |
| Cursed Human | Purple-tinted dark tan rogue with black eyes, curse tattoos, and purple aura. | Rogue/curse-bearer preview. |

Half-Blood Human remains a matrix side project overall. The Human-Elf branch is now locked as Half-Elf; the other Human/race mixtures still wait for their parent race baselines.

## Dwarf Visual Baselines

The approved Dwarf sub-race art direction now acts as the third prototype character creation baseline after Humans and Elves.

| Sub-Race | Visual Identity | Prototype Use |
| --- | --- | --- |
| Mountain Dwarf | Classic high-mountain miner, smith, and fortress builder with stone dust, dense beard, heavy boots, and practical armor. | Default classic Dwarf preview for stone, forge, mining, and fortress culture. |
| Forge Dwarf | Soot-worn furnace smith with forge apron, hammer/tongs, ember-lit metal, and singed or ringed beard details. | Smithing, furnace, blacksmith, and craft preview. |
| Quarry Dwarf | Mason and road builder with chisels, measuring tools, work gloves, dusted beard, and civic construction stance. | Builder, contractor, town repair, and masonry preview. |
| Deep Dwarf | Cavern-adapted Dwarf with dark tunnel gear, lamp or rune light, pressure armor, and guarded cave posture. | Dungeon explorer, miner, underground route, and sealed ruin preview. |
| Goldbeard Dwarf | Wealth and contract Dwarf with gold beard ornaments, appraisal tools, ledger or contract case, and merchant confidence. | Merchant, appraiser, banking, and high-value gear preview. |
| Iron Oath Dwarf | Duty-bound Dwarf with oath bands, iron clasps, shield or contract tablet, and disciplined promise-keeper posture. | Defender, officer, oath contract, and duty preview. |
| Dreamforge Dwarf | Ione-touched crafter with dream-blue accents, unusual blueprint props, subtle rune light, and visionary maker expression. | Magical crafting, enchanter, artificer, and dream blueprint preview. |
| Hearth Dwarf | Tavern and homecraft Dwarf with warm apron, bread or tankard props, hearth glow, and friendly sturdy face. | Tavern owner, baker, brewer, morale, food, and community preview. |

All normal Dwarf sub-races preserve the core Dwarf baseline: short broad worker silhouette, dense beard, heavy hands, practical work identity, and job-proud posture.

## Orc Visual Baselines

The approved Orc sub-race art direction now acts as the fourth prototype character creation baseline after Dwarves.

| Sub-Race | Visual Identity | Prototype Use |
| --- | --- | --- |
| Common Orc | Classic green clanned Orc with tusks, ugly brutish face, tall build, battle-ready stance, crude iron/leather, and clan scars. | Default Orc selection for intimidation, guard, escort, and brute frontline identity. |
| Half-Orc | Half-Human Orc with green skin, smaller tusks, shorter leaner build, more Human face structure, and rugged attractive adventurer read. | Mixed Human-Orc bridge preview, separate from protected Raelae. |
| War Orc | Brown-red skin, ugly Orc face, dense chiseled muscle, and mandatory war paint. | War patrol, bounty, officer, and battle-discipline preview. |
| Stonehide Orc | Stone-gray skin, rocky hide protrusions, metal armor, and fortress-like defensive posture. | Tank, guard, escort, and pain-tolerance preview. |
| Redtusk Orc | Blood-red skin, bloodshot eyes, red tusks or tusk markings, and bloodthirsty fight-ready expression. | Aggressive duelist, intimidation, and melee pressure preview. |
| Gray Orc | Gray skin, intelligent neutral persona, studious gear, calm passive posture, scrolls and satchel. | Scholar-guard, peacekeeper, restraint, and negotiation preview. |
| Ironbound Orc | Pink pig-like Orc features, hard-working body, iron bands or broken chain remnants, patched labor gear, and resilient dignity. | Labor history, work endurance, survival, and oppressed-lineage story preview. |
| Wild Orc | Very short green cartoonish build, armored helmet, small axes, and tall-grass ambush behavior. | Tiny ambusher, scout, survival, and chaotic field encounter preview. |

All normal Orc sub-races preserve Orc identity through tusks, visible survival grit, and strong silhouette reads. Ieke and Raelae remain protected special characters, not standard character creation baselines.

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
