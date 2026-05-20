# Huwan Unreal Engine Roadmap

## Current Status

Huwan is now past the first world-bible invention stage.

The world bible is **V1 foundation complete**, which means the major lore, gameplay, simulation, and server-operation systems are defined enough to begin production planning.

The project currently has:

- Unreal C++ project scaffold: `Huwam.uproject`
- Target engine association: Unreal Engine `5.7.4`
- Main C++ module: `Source/Huwam`
- Existing AI engine scaffold: `Source/Huwam/AI`
- First C++ data structs: `Source/Huwam/Data/HuwamDataTypes.h`
- First data registry: `Source/Huwam/Data/HuwamDataRegistry.*`
- First runtime data subsystem: `Source/Huwam/Data/HuwamDataSubsystem.*`
- First data smoke test actor: `Source/Huwam/Data/HuwamDataSmokeTestActor.*`
- First inventory component: `Source/Huwam/Gameplay/HuwamInventoryComponent.*`
- First equipment component: `Source/Huwam/Gameplay/HuwamEquipmentComponent.*`
- Starter CSV data tables: `Content/Data`
- Unreal target files: `Source/Huwam.Target.cs` and `Source/HuwamEditor.Target.cs`
- World bible: `Docs/WorldBible.md`
- Production roadmap: `Docs/UnrealRoadmap.md`

This does not mean the full game is ready to build all at once. It means Huwan now has a strong enough design foundation to become data, prototypes, C++ systems, Blueprints, maps, UI, and eventually multiplayer server logic.

## Production Goal

The next goal is a **first playable Unreal prototype**.

The prototype should not try to build all of Huwan. It should prove the core loop:

1. Create a character.
2. Enter Laucian's tutorial lab.
3. Learn movement, HUD, inventory, map, and combat basics.
4. Teleport to Eldoria.
5. Accept a simple NPC or guild quest.
6. Gather F-rank resources.
7. Fight or harvest basic slimes.
8. Craft or turn in basic materials.
9. See an NPC inventory, reputation, or quest state update.

If this loop works, the rest of Huwan has a real spine.

## Completed Design Foundations

The world bible now defines the following V1 foundations:

| Layer | Status |
|---|---|
| World identity and mature content boundaries | V1 complete |
| Relationship-driven story pillar | V1 complete |
| Pantheon and divine duality | V1 drafted |
| Races, sub-races, traits, reputations, and kingdoms | V1 drafted |
| Playable identity, classes, jobs, stats, skills, abilities | V1 drafted |
| Eldoria starter city and first NPC layer | V1 drafted |
| NPC simulation and AI engine structure | V1 drafted |
| Time, urgency, resource respawn, and spoilage | V1 drafted |
| Magic, mana wells, runes, caster identities, and starter spells | V1 drafted |
| Items, gear, tools, crafting, and equipment slots | V1 drafted |
| Monsters, biomes, resources, slimes, and dungeons | V1 drafted |
| Dragon-born dungeon system | V1 drafted |
| Legendary NPCs and legendary encounter lottery | V1 drafted |
| Factions, settlements, land buying, law, crime, contracts | V1 drafted |
| Reputation, romance, family, and legacy | V1 drafted |
| Admin, AI moderation, and server operations | V1 drafted |
| Story engine, event clocks, artifacts, and server announcements | V1 drafted |

## Production Principles

These rules should guide every Unreal implementation step:

- Build systems small, then let them grow.
- Prefer data-driven design for anything that will have many entries.
- Prototype with simple visuals before spending time on final art.
- Make one working loop before building twenty unfinished systems.
- Keep multiplayer in mind, but do not begin with full MMO-scale networking.
- Treat AI simulation as layered: nearby important actors get detail, distant crowds get summaries.
- Every item, spell, NPC, class, job, monster, and quest should have a stable ID.
- Every major world change should produce a record.
- Laucian/admin tools should be designed early, even if only as debug tools at first.

## Roadmap Overview

| Phase | Name | Target Outcome | Estimated Time |
|---|---|---|---|
| 0 | Build Readiness | Project opens, compiles, and has a clean source/content structure. | 2-5 days |
| 1 | Data Model | World bible concepts become Unreal-friendly IDs and data structs. | 1-2 weeks |
| 2 | Core Player Framework | Player stats, needs, inventory, equipment, and interaction exist. | 2-4 weeks |
| 3 | Character Creation And Tutorial | Character creation and Laucian's white lab tutorial prototype work. | 2-4 weeks |
| 4 | Eldoria Starter Slice | Small playable Eldoria blockout with NPCs, guild board, and first quests. | 1-2 months |
| 5 | Gathering, Crafting, And Slimes | F-rank field loop, resources, slime drops, and starter crafting work. | 3-6 weeks |
| 6 | Combat And Magic V0.1 | Basic combat, starter spells, mana costs, and downed/death states work. | 4-8 weeks |
| 7 | NPC Simulation V0.1 | A small set of NPCs run routines, needs, inventory, and quests. | 6-12 weeks |
| 8 | Verdant Maw Floors 1-10 | First dungeon prototype with floor rules, boss, loot, and map fog. | 2-4 months |
| 9 | Server Persistence And Multiplayer | Early server authority, persistence, party, and admin tools. | 3-6 months |
| 10 | Expansion | More races, classes, jobs, regions, quests, factions, and dungeons. | Ongoing |

Time estimates assume one main developer assisted by AI. A larger team, final art, multiplayer infrastructure, and custom AI services will change the timeline.

## Phase 0: Build Readiness

Goal: Make the project ready for day-to-day Unreal development.

Tasks:

- Confirm Unreal Engine `5.7.4` is installed or update the `.uproject` to the engine version actually available.
- Open `Huwam.uproject` in Unreal Editor.
- Generate project files.
- Compile the base game module.
- Confirm existing AI engine scaffold compiles.
- Add missing target files if Unreal requires them.
- Create baseline folders for code, data, UI, maps, prototypes, and docs.
- Create an empty developer test map.

Deliverables:

- Buildable Unreal project.
- Clean test map.
- Confirmed editor launch.
- Known compile errors list, if any.

Definition of done:

- The editor opens without project-breaking errors.
- The game module compiles.
- A test level can be opened and played.

## Phase 1: Data Model

Goal: Convert the world bible into structured data that Unreal systems can read.

Recommended first data types:

- Race
- Sub-race
- Class
- Job
- Skill
- Ability
- Stat
- Item
- Material
- Recipe
- Spell
- Monster
- NPC
- Quest
- Guild
- Faction
- Deity
- Biome
- Resource node
- Relationship state
- Reputation record
- Contract
- Crime record

Recommended Unreal representation:

| Content Type | Suggested Format |
|---|---|
| Large editable lists | DataTables or CSV import |
| Complex designer-authored entries | Primary Data Assets |
| Runtime state | C++ structs and save-game records |
| Prototype-only values | Blueprints or temporary DataTables |
| Stable IDs | Namespaced string or Gameplay Tag style IDs |

Example IDs:

- `race.human`
- `race.elf.dark`
- `class.progression_zero`
- `class.wizard`
- `job.blacksmith`
- `item.weapon.basic_sword`
- `material.slime_core.basic`
- `spell.fire.spark`
- `npc.eldoria.gorth_runkis`
- `guild.adventurers`
- `quest.eldoria.slimes_in_tall_grass`

Deliverables:

- `Docs/DataModel.md`
- First data tables for races, classes, jobs, stats, items, materials, recipes, spells, monsters, and NPCs.
- A stable ID convention.

Definition of done:

- The first prototype can load starting character, item, spell, material, monster, and quest definitions from data instead of hardcoding everything.

## Phase 2: Core Player Framework

Goal: Create the foundation all player systems will use.

Systems:

- Player character base.
- Player identity record.
- Stats component.
- Needs component for hunger, thirst, and social level.
- Mana well component.
- Inventory component.
- Equipment component.
- Interaction component.
- Basic reputation component.
- Basic quest log component.
- Simple save/load.

Prototype rules:

- Start with simple numbers and debug UI.
- Do not build full balancing yet.
- Make systems readable and testable first.

Deliverables:

- Playable test character.
- Stats update in real time.
- Inventory can add, remove, stack, and inspect items.
- Equipment slots can equip starter gear.
- Basic needs tick over server time.
- Save/load preserves character identity and inventory.

## Phase 3: Character Creation And Tutorial

Goal: Build the first player-facing experience.

Character creation V0.1:

- Race selection.
- Sub-race selection.
- Basic body customization placeholders.
- Mature body customization toggle as a settings-controlled option.
- Progression Zero starting state.
- Choose 5 permanent skills.
- Choose 5 permanent abilities.
- Alignment direction.
- Starter appearance and name.

Tutorial V0.1:

- Player appears in Laucian's white laboratory.
- Laucian or tutorial system teaches movement.
- HUD basics.
- Inventory basics.
- Equipment basics.
- Quest log.
- Map and quest marker.
- Basic combat against harmless dummy or illusion.
- Basic gathering object.
- Basic item pickup.
- Exit portal to Eldoria.

Laucian tutorial tone:

- Helpful, powerful, slightly fourth-wall aware.
- Teaches without overexplaining.
- Makes it clear the player is still Progression Zero.

Deliverables:

- Character creation screen or temporary debug flow.
- Tutorial lab blockout.
- Player can complete tutorial and enter Eldoria starter space.

## Phase 4: Eldoria Starter Slice

Goal: Create a small but real version of Eldoria.

Do not build all 1,500 NPCs visually at first. Build a playable district slice that proves the systems.

Minimum Eldoria slice:

- Arrival plaza.
- Adventurers Guild desk.
- Small market row.
- Poor district edge.
- Basic inn or rest area.
- Gate to F-rank field.
- 5 to 10 active NPCs.
- 20 to 50 background/statistical NPC records.
- Guild board.
- Contract board.
- First map/minimap markers.
- Battle fog in nearby field.

Important NPCs to include early:

- Gorth Runkis
- Robin Quickfoot
- Mara or another poor-district quest giver
- A merchant NPC
- A guard NPC
- A Church representative
- A Thieves Guild hint NPC
- A background Dawnmere-linked NPC

Deliverables:

- Eldoria blockout.
- First NPC conversations.
- First quest acceptance and completion.
- Basic reputation change.
- NPC inventory update after quest completion.

## Phase 5: Gathering, Crafting, And Slimes

Goal: Prove the F-rank survival/economy loop.

F-rank field content:

- Tall grass.
- Small stream.
- Small forest edge.
- Basic wood nodes.
- Stone nodes.
- Herb nodes.
- Flax or grass fiber nodes.
- Small fish source.
- Basic slimes.

First resources:

- Basic wood
- Stone
- Flax
- Grass fiber
- Common herbs
- Slime core
- Slime gel
- Small animal hide
- Small bone
- Small fish

First tools:

- Gathering knife
- Hatchet
- Pickaxe
- Fishing rod
- Skinning knife

First recipes:

- Basic arrows
- Basic quiver
- Basic knife or short sword
- Basic chest
- Waterskin
- Small pouch
- Oaken bow
- Reinforced basic sword level 1
- Slime gear test item

Deliverables:

- Resource nodes respawn on correct time rules.
- Basic slimes drop harvestable materials.
- Items stack correctly.
- Player can craft a starter item from gathered resources.
- NPC or guild quest consumes turned-in items and updates inventory.

## Phase 6: Combat And Magic V0.1

Goal: Make danger work without trying to finish every class.

Combat V0.1:

- Basic melee attack.
- Basic ranged attack placeholder.
- Basic enemy AI for slimes.
- Health, damage, downed, and death states.
- Basic loot and harvesting.
- Simple legal/safe combat zone distinction.

Magic V0.1:

- Mana well value.
- Mana cost.
- Mana depletion causes sleep or collapse state.
- Starter spells:
  - Spark
  - Gust
  - Sprinkle
  - Tiny Lights
  - Minor Heal
  - Warm Rune
  - Alarm Rune
- Simple rune placement prototype.
- Spell data framework.

Deliverables:

- Player can fight basic slimes.
- Player can cast starter spells.
- Mana cost works.
- One rune can be placed and triggered.
- Death/revival placeholder exists.

## Phase 7: NPC Simulation V0.1

Goal: Make a small number of NPCs feel alive.

Start small:

- 5 fully simulated NPCs.
- 20 to 50 background NPC records.
- 1 merchant.
- 1 poor resident.
- 1 guard.
- 1 guild worker.
- 1 crafter.

NPC simulation V0.1 features:

- Daily schedule.
- Inventory.
- Wealth.
- Needs.
- Job role.
- Quest blocker detection.
- Memory summary.
- Relationship value toward player.
- Simple rumor output.

First dynamic quest pattern:

1. NPC needs resource.
2. NPC lacks resource.
3. NPC creates request.
4. Player completes request.
5. NPC inventory updates.
6. NPC does not request that resource again until low.

Deliverables:

- NPCs move through basic schedules.
- NPC-generated resource quest works.
- Market or merchant stock changes from supply.
- NPC remembers whether the player helped.

## Phase 8: Verdant Maw Floors 1-10

Goal: Build the first dungeon proof.

First dungeon slice:

- Entrance region.
- Floors 1 to 10.
- Basic floor generation templates.
- Map fog.
- Slimes, bugs, goblins, low undead, minor plant monsters.
- Chests.
- Traps.
- Ores.
- Floor 10 rest point.
- Floor 10 boss room.
- Thornjaw, Root-Wrapped Boar boss.

Deliverables:

- Dungeon entry and exit.
- Floor progression.
- Rest point.
- Boss fight.
- Loot return loop.
- Map seller or purchased map clears some fog.

## Phase 9: Server Persistence And Multiplayer

Goal: Prepare Huwan for living-server behavior.

Do not begin here until the single-player prototype loop works.

Major systems:

- Dedicated server plan.
- Character persistence.
- Inventory authority.
- Economy authority.
- NPC simulation authority.
- Party quest sharing.
- Contract board state.
- Raid contribution record.
- Moderator/admin commands.
- Laucian admin interface.

Deliverables:

- Multiplayer test map.
- Persistent character record.
- Basic party formation.
- Shared quest visibility.
- Admin debug panel.

## Immediate Sprint: What We Do Next

This is the next practical work package.

### Sprint 1 Goal

Create the first production data model and first prototype scope.

### Sprint 1 Tasks

1. Create `Docs/DataModel.md`. **Done**
2. Define stable ID naming rules. **Done**
3. Define first structs/data tables. **Started**
   - Race
   - Sub-race
   - Class
   - Job
   - Stat
   - Skill
   - Ability
   - Item
   - Material
   - Recipe
   - Spell
   - Monster
   - NPC
   - Quest
4. Create first prototype content list. **Done**
   - 3 races
   - 3 classes
   - 5 jobs
   - 10 skills
   - 10 abilities
   - 20 items/materials
   - 8 recipes
   - 7 starter spells
   - 3 monsters
   - 8 NPCs
   - 5 starter quests
5. Decide which data starts as DataTables and which starts as C++ structs. **Done**
6. Add build-readiness checklist for Unreal. **Done**
7. Add a runtime data registry that references the tables. **Started**
8. Add a Game Instance subsystem to expose the registry to gameplay. **Started**
9. Add a smoke test actor for imported data. **Started**
10. Add inventory stack behavior using item IDs and item stack limits. **Started**
11. Add equipment slot behavior using item IDs and item equip slot data. **Started**
12. Import CSV files as Unreal DataTable assets. **Next**

### Sprint 1 Output

By the end of Sprint 1, the game should have a clean design-to-code bridge. That bridge lets the next step become C++ and Unreal implementation instead of more free-floating lore.

## First Prototype Content Target

The first prototype should use a tiny version of Huwan.

### Playable Races

- Human
- Elf
- Dark-Elf

Reason:

- Human proves the all-rounder baseline.
- Elf proves long-life and mana-well identity.
- Dark-Elf proves outlier reputation and Laucian/Dawnspire relevance.

### Playable Classes

- Progression Zero
- Progression Rank 1
- Mage

Reason:

- Progression Zero proves starting identity.
- Progression Rank 1 proves noob progression.
- Mage proves mana/rune systems without needing every caster type.

### Starter Jobs

- Adventurer
- Hunter/Gatherer
- Blacksmith
- Merchant
- Farmer

Reason:

- These jobs support the first loop: gather, fight, craft, sell, and feed the economy.

### First Monsters

- Basic Slime
- Wood Slime
- Goblin

Reason:

- Basic Slime proves safe F-rank combat and slime materials.
- Wood Slime proves evolution flavor.
- Goblin proves a humanoid enemy later tied to bandits/dungeons.

### First Quests

| Quest | Purpose |
|---|---|
| Welcome to Progression Zero | Tutorial completion. |
| Slimes in the Tall Grass | Combat and harvesting. |
| Gather Field Supplies | Resource gathering. |
| Repair a Basic Tool | Crafting and NPC inventory update. |
| First Guild Registration | Adventurers Guild onboarding. |

## Current Best Next Move

The next best prototype move is:

**Create the character creation menu data adapter that feeds a non-technical UI.**

The data registry and subsystem code have been started. The Unreal editor step is still opening Unreal, importing the CSV files as DataTable assets using the row structs in `FHuwamDataTypes`, and assigning them to a `UHuwamDataRegistry` Data Asset. See `Docs/UnrealDataImport.md` for the exact import map.

The first inventory component has also been started. Once the imported item table is connected, inventory stack limits will come from Huwan item data. See `Docs/InventoryPrototype.md`.

The first equipment component has been started too. It can equip items from inventory into default Huwan slots and uses `PrimaryEquipSlotId` from item data. See `Docs/EquipmentPrototype.md`.

Equipment now has starter stat totals as well. Item data includes `AttackPower`, `DefensePower`, `ManaPower`, and `UtilityPower`, and the equipment component can total equipped gear for Blueprint. See `Docs/EquipmentStatsPrototype.md`.

The first character stats component has been started. It exposes Progression Zero default stats, permanent and temporary stat bonuses, equipment-aware stat snapshots, and derived combat/utility summaries for Blueprint. See `Docs/CharacterStatsPrototype.md`.

The first combat component has been started. It initializes health and mana from character stats, resolves melee/ranged/spell/true damage attacks, applies armor mitigation, supports critical hits, and tracks defeat state. See `Docs/CombatPrototype.md`.

The first Basic Slime encounter actor has been started. It loads monster data, applies slime stats, uses the combat component, and grants a simple reward after defeat. See `Docs/BasicSlimeEncounterPrototype.md`.

The first reward bridge has been started. It stores gold, experience, and quest progress, and can claim defeated monster rewards into inventory plus progression. See `Docs/RewardBridgePrototype.md`.

The first starter quest runtime has been started. It can activate "Slimes in the Tall Grass," mirror reward progress into quest state, expose active quest data for HUD/menu use, and turn the quest in for gold and experience. See `Docs/StarterQuestRuntimePrototype.md`.

The first live content layer has been started too. Content pack manifests can describe DLC, cosmetics, pets, mounts, gameplay packs, and world expansions with version, entitlement, server enablement, and feature flag checks. See `Docs/LiveContentPrototype.md`.

The first HUD/menu data adapter has been started. It builds a Blueprint-friendly snapshot for health, mana, gold, XP, inventory slots/stacks, active quest progress, and live content status. See `Docs/HudMenuDataPrototype.md`.

The first prototype player actor has been started. It bundles inventory, equipment, stats, combat, rewards, quests, live content, and HUD data, with helper calls for the Basic Slime quest loop. See `Docs/PrototypePlayerActor.md`.

The first character creation runtime has been started. It validates race, sub-race, class, job, permanent skills, permanent abilities, Reincarnated limits, and starting stat bonuses, then plugs into the prototype player actor. See `Docs/CharacterCreationPrototype.md`.

With creation rules in place, the code path is ready for a Blueprint-friendly menu adapter.
