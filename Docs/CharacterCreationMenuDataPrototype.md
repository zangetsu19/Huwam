# Character Creation Menu Data Prototype

This layer prepares the first character creator UI without forcing us to build the final screen yet.

## Goal

The character creation menu needs one clean data feed for Blueprint widgets.

`UHuwamCharacterCreationMenuDataComponent` builds a snapshot containing:

- Race options.
- Sub-race options filtered by selected race.
- Class options.
- Job options.
- Skill options.
- Ability options.
- Current slot counts.
- Normal or Reincarnated skill and ability limits.
- Validation errors and warnings.
- Whether the current request can be confirmed.

## Why This Layer Matters

This keeps the UI simple.

Blueprint widgets should not need to know how to find DataTables, filter sub-races, count selected skills, or validate the request. They can ask the adapter for a menu snapshot, render the options, and call helper functions when the player clicks something.

## Main Runtime Types

| Type | Purpose |
|---|---|
| `FHuwamCharacterCreationOption` | One race, class, job, skill, ability, or sub-race card. |
| `FHuwamCharacterCreationMenuSnapshot` | Full menu state for one character creation request. |
| `EHuwamCharacterCreationOptionType` | Identifies which option group a click belongs to. |

## Main Functions

| Function | Purpose |
|---|---|
| `BuildDefaultMenuSnapshot` | Builds the starter character creation view. |
| `BuildMenuSnapshot` | Builds a complete UI snapshot from a request. |
| `BuildRequestWithSingleSelection` | Updates race or sub-race selection. |
| `BuildRequestWithToggledSelection` | Toggles class, job, skill, or ability selection. |
| `SetCharacterCreationComponent` | Allows manual linking to a creation component. |

## Data Source

When a `UHuwamDataRegistry` is active, the adapter reads:

- `RacesTable`
- `SubRacesTable`
- `ClassesTable`
- `JobsTable`
- `SkillsTable`
- `AbilitiesTable`

If no registry is active yet, the adapter uses a small fallback option list so UI prototyping can continue.

The first data pass now includes:

- The seven major parent race choices.
- Approved Human baseline sub-races for prototype character creation.
- Approved Dwarf baseline sub-races for prototype character creation.
- Representative Elf, Orc, Demon, Beastfolk, and Fae sub-races.
- Progression, starter guild path, and hidden Mage class rows.
- The first stated starting job roster.

## Human Sub-Race UI Baselines

Human sub-race cards now use the approved 3D art direction as their prototype UI language.

| Sub-Race | UI Hook | Character Creation Read |
| --- | --- | --- |
| Common Human | All-rounder adventurer | Worn leather, soft cloth, used short sword, no supernatural marks. |
| High-Human | Noble magic user | Alabaster skin, smug noble posture, fine fabrics, controlled magic. |
| Dawn Human | Holy optimist | Yellow-warmed skin, cheerful expression, bright holy garments, gentle aura. |
| Dusk Human | Void-touched tank | Dark skin, knight armor, greatsword, kite shield, slight void aura. |
| Frontier Human | Predator-alert survivor | Red sunburnt skin, farmer/ranger tools, wild watchful eyes. |
| Urban Human | Streetwise merchant | Slightly dirty city skin, softer body, ledger, coin pouch, trade gear. |
| Warborn Human | Battle-happy frontliner | Dark tan skin, scars, fierce manic war energy, battered gear. |
| Half-Elf | Hybrid adventurer | Very short elf ears, sword gear, charisma, and Human-Elf bridge identity. |
| Blessed Human | Rare divine-touched | Ebony skin, white hair, white eyes, wings, deity tattoos, holy aura. |
| Cursed Human | Rogue curse-bearer | Purple-tinted dark tan skin, black eyes, curse tattoos, rogue gear. |

Half-Blood Human stays a later Human-plus-race matrix overall, but the Human-Elf branch is now approved as Half-Elf and uses `race.human.half_elf` on the Human path plus `race.elf.half` on the Elf path.

## Dwarf Sub-Race UI Baselines

Dwarf sub-race cards now use the approved 3D art direction as their prototype UI language.

| Sub-Race | UI Hook | Character Creation Read |
| --- | --- | --- |
| Mountain Dwarf | Stone and forge | Mountain miner-smith, fortress gear, dusted beard, heavy boots. |
| Forge Dwarf | Furnace smith | Soot, apron, hammer, ember-lit metal, heat discipline. |
| Quarry Dwarf | Mason builder | Chisels, measuring tools, road-work layers, civic construction stance. |
| Deep Dwarf | Cave delver | Dark tunnel gear, lamp or rune light, pressure-ready armor. |
| Goldbeard Dwarf | Contract appraiser | Gold beard rings, ledger, appraisal tools, polished heavy clothing. |
| Iron Oath Dwarf | Sacred contract guard | Oath bands, shield, iron clasps, disciplined duty posture. |
| Dreamforge Dwarf | Dream smith | Dream-blue runes, odd blueprint, inspired forge focus. |
| Hearth Dwarf | Hearthkeeper | Warm apron, bread or tankard props, tavern comfort, friendly sturdy face. |

The Dwarf path uses `race.dwarf.mountain`, `race.dwarf.forge`, `race.dwarf.quarry`, `race.dwarf.deep`, `race.dwarf.goldbeard`, `race.dwarf.iron_oath`, `race.dwarf.dreamforge`, and `race.dwarf.hearth`.

## Orc Sub-Race UI Baselines

Orc sub-race cards now use the approved 3D art direction as their prototype UI language.

| Sub-Race | UI Hook | Character Creation Read |
| --- | --- | --- |
| Common Orc | Clanned brute | Green skin, tusks, ugly face, tall build, clan scars, battered axe. |
| Half-Orc | Human-Orc bridge | Green skin, smaller tusks, shorter leaner build, more Human attractive features. |
| War Orc | War-painted fighter | Brown-red skin, dense chiseled muscles, ugly face, mandatory war paint. |
| Stonehide Orc | Stone-armored tank | Stone-gray hide, rocky protrusions, metal armor, heavy defensive stance. |
| Redtusk Orc | Bloodthirsty duelist | Blood-red skin, bloodshot eyes, red tusks, ready-to-fight posture. |
| Gray Orc | Studious neutral | Gray skin, intelligent eyes, calm passive stance, scrolls and satchel. |
| Ironbound Orc | Iron laborer | Pink pig-like Orc read, iron bands, patched labor gear, resilient worker posture. |
| Wild Orc | Tall-grass ambusher | Very short green build, armored helmet, small axe, crouched in tall grass. |

The Orc path uses `race.orc.common`, `race.orc.half`, `race.orc.war`, `race.orc.stonehide`, `race.orc.redtusk`, `race.orc.gray`, `race.orc.ironbound`, and `race.orc.wild`.

The native prototype screen still renders text cards. Later Widget Blueprint work can replace those text cards with live 3D preview panes while keeping the same option IDs and selection logic.

## Prototype Actor Link

`AHuwamPrototypePlayerActor` now includes:

- `UHuwamCharacterCreationComponent`
- `UHuwamCharacterCreationMenuDataComponent`

This means the same actor can:

1. Feed the character creation menu.
2. Validate the final request.
3. Apply the created character.
4. Start the slime quest loop.

## First UI Flow

1. Place `AHuwamPrototypePlayerActor` in a test map.
2. Call `GetCharacterCreationMenuDataComponent`.
3. Call `BuildDefaultMenuSnapshot`.
4. Render the option arrays as cards or rows.
5. On race or sub-race click, call `BuildRequestWithSingleSelection`.
6. On class, job, skill, or ability click, call `BuildRequestWithToggledSelection`.
7. Rebuild the menu snapshot from the updated request.
8. Enable the confirm button only when `bCanConfirm` is true.
9. On confirm, call `ApplyCharacterCreationToPrototype`.

## Next Step

The first screen scaffold is now in `UHuwamCharacterCreationScreenWidget`.

See:

- `Docs/CharacterCreationScreenPrototype.md`

The first Laucian tutorial-lab handoff after confirmation is now started.

See:

- `Docs/TutorialLabHandoffPrototype.md`

The next practical layer is the first physical lab interaction lesson.
