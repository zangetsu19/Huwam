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
- First runtime data subsystem with CSV auto-bootstrap: `Source/Huwam/Data/HuwamDataSubsystem.*`
- First living time subsystem: `Source/Huwam/Gameplay/HuwamTimeSubsystem.*`
- First data smoke test actor: `Source/Huwam/Data/HuwamDataSmokeTestActor.*`
- First inventory component: `Source/Huwam/Gameplay/HuwamInventoryComponent.*`
- First equipment component: `Source/Huwam/Gameplay/HuwamEquipmentComponent.*`
- First map-awareness component: `Source/Huwam/Gameplay/HuwamMapAwarenessComponent.*`
- First character creation runtime and menu data adapter: `Source/Huwam/Gameplay/HuwamCharacterCreation*`
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
3. Learn movement, third-person camera, HUD, inventory, map, and combat basics.
4. Teleport to Eldoria.
5. Accept a simple NPC or guild quest.
6. Gather F-rank resources.
7. Fight or harvest basic slimes.
8. Craft or turn in basic materials.
9. See an NPC inventory, reputation, or quest state update.

If this loop works, the rest of Huwan has a real spine.

## Presentation Target

Huwan is targeting a high-graphics 3D third-person MMORPG experience.

Prototype systems should stay compatible with:

- Third-person character movement and camera readability.
- High-fidelity 3D characters, environments, creatures, gear, and effects later.
- HUD and map layers that support live world navigation instead of replacing the world view.
- Living-server MMO systems where player-facing NPCs can react conversationally without treating every crowd record at the same cost.
- Performance-minded world production as visual density, NPC count, simulation, and multiplayer scale increase.

Early prototypes can use simple visuals while they prove systems, but they should not quietly drift toward a flat or menu-first game.

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
- Keep tutorial, HUD, combat, and navigation systems legible from a third-person camera in 3D spaces.
- Design NPC tiers so foreground conversations can be deep while background crowds stay performance-safe.
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

- Playable third-person test character.
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
- Laucian or tutorial system teaches movement and third-person camera basics.
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
- First foreground NPC conversations plus looped background chatter.
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
- Settlement resource budgets support baseline NPC survival without requiring player harvesting.
- Player gathering, defense, trade, sabotage, overharvesting, and crisis response change surplus, shortages, and settlement outcomes.
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
- Foreground conversational NPC prototype.
- Background NPC bark or loop-conversation prototype.

Conversation depth target:

- Foreground NPCs should eventually support ChatGPT-like conversations with players and other foreground NPCs, grounded in memory, personality, relationships, inventory, current needs, and world state so they can learn and grow.
- Background NPCs should use authored or generated loop sayings, ambient conversations, schedules, and summary simulation until player attention, quest importance, social importance, or world events promote them forward.

First dynamic quest pattern:

1. NPC needs resource.
2. NPC checks whether they can solve it internally.
3. NPC lacks the means, or sees a player as faster, safer, more skilled, or more efficient.
4. NPC creates a player-specific, board, guild, faction, contract, or rumor-routed request.
5. Player, another player, or another NPC completes the request.
6. NPC inventory updates.
7. NPC does not request that resource again until low or circumstances change.

Deliverables:

- NPCs move through basic schedules.
- NPC-generated resource quest works.
- Different eligible players can receive different requests from the same changing NPC state.
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
   - Major parent race creation rows plus Halfling prototype support
   - Approved Human sub-race visual baselines for character creation UI
   - Starter class path rows plus hidden Mage support
   - Starter job rows
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
7. Add a runtime data registry that references the tables. **Done for prototype**
8. Add a Game Instance subsystem to expose the registry to gameplay. **Done for prototype**
9. Add a smoke test actor for imported data. **Done for prototype**
10. Add inventory stack behavior using item IDs and item stack limits. **Started**
11. Add equipment slot behavior using item IDs and item equip slot data. **Started**
12. Auto-load missing DataTables from project CSV files for prototype play. **Done**
13. Import CSV files as Unreal DataTable assets. **Optional production path**

### Sprint 1 Output

By the end of Sprint 1, the game should have a clean design-to-code bridge. That bridge lets the next step become C++ and Unreal implementation instead of more free-floating lore.

## First Prototype Content Target

The first playable verification slice should use a tiny version of Huwan. The creation tables can already expose broader race, class, and job coverage while this first slice stays focused.

### Playable Races

- Human
- Elf
- Dark-Elf

Reason:

- Human proves the all-rounder baseline.
- Elf proves long-life and mana-well identity.
- Dark-Elf proves outlier reputation and Laucian/Dawnspire relevance.

### Human Character Creation Art Baseline

The Human prototype baseline now has approved sub-race art direction:

| Sub-Race | Baseline Read |
| --- | --- |
| Common Human | Level 1 adventurer with worn leather, soft cloth, and a used short sword. |
| High-Human | Alabaster smug noble magic user in fine fabrics. |
| Dawn Human | Yellow-warmed cheerful holy Human with bright garments and gentle aura. |
| Dusk Human | Dark-skinned knight tank with greatsword, kite shield, and slight void aura. |
| Frontier Human | Red sunburnt farmer/ranger with wild predator-watchful eyes. |
| Urban Human | Slightly dirty merchant with softer build, ledgers, pouches, and trade tools. |
| Warborn Human | Dark tanned scarred frontliner with fierce war-happy energy. |
| Half-Elf | Human-Elf hybrid adventurer with very short ears, sword gear, charisma, and mixed-culture appeal. |
| Blessed Human | Ebony skin, white hair and eyes, wings, deity tattoos, and holy aura. |
| Cursed Human | Purple-tinted dark tan rogue with black eyes, curse tattoos, and purple aura. |

Half-Blood Human remains a later matrix side project for most mixtures. Human-Elf is now approved as Half-Elf and should be exposed from both Human hybrid and Elf paths.

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

**Play the first vertical slice in editor, smooth its feel, and replace code-spawned blockout pieces with the first authored white-lab and Eldoria map assets.**

The next systems-side move is:

**Add service-specific repair material requirements and restoration contribution types.**

The data registry and subsystem can now auto-load the starter CSV files into runtime DataTables when no registry asset is configured. Imported DataTable assets and a `UHuwamDataRegistry` Data Asset remain the production-friendly path later, but the prototype no longer blocks on manual import. See `Docs/UnrealDataImport.md` for both paths.

The first inventory component has also been started. Once the imported item table is connected, inventory stack limits will come from Huwan item data. See `Docs/InventoryPrototype.md`.

The first equipment component has been started too. It can equip items from inventory into default Huwan slots and uses `PrimaryEquipSlotId` from item data. See `Docs/EquipmentPrototype.md`.

Equipment now has starter stat totals as well. Item data includes `AttackPower`, `DefensePower`, `ManaPower`, and `UtilityPower`, and the equipment component can total equipped gear for Blueprint. See `Docs/EquipmentStatsPrototype.md`.

The first character stats component has been started. It exposes Progression Zero default stats, permanent and temporary stat bonuses, equipment-aware stat snapshots, and derived combat/utility summaries for Blueprint. See `Docs/CharacterStatsPrototype.md`.

The first combat component has been started. It initializes health and mana from character stats, resolves melee/ranged/spell/true damage attacks, applies armor mitigation, supports critical hits, and tracks defeat state. See `Docs/CombatPrototype.md`.

The first Basic Slime encounter actor has been started. It loads monster data, applies slime stats, uses the combat component, and grants a simple reward after defeat. See `Docs/BasicSlimeEncounterPrototype.md`.

The first reward bridge has been started. It stores copper-native Huwan currency, exposes denomination breakdowns, tracks experience and quest progress, and can claim defeated monster rewards into inventory plus progression. See `Docs/RewardBridgePrototype.md` and `Docs/CurrencyPrototype.md`.

The first starter quest runtime has been started. It can activate "Slimes in the Tall Grass," mirror reward progress into quest state, expose active quest data for HUD/menu use, and turn the quest in for currency and experience. See `Docs/StarterQuestRuntimePrototype.md`.

The first live content layer has been started too. Content pack manifests can describe DLC, cosmetics, pets, mounts, gameplay packs, and world expansions with version, entitlement, server enablement, and feature flag checks. See `Docs/LiveContentPrototype.md`.

The first HUD/menu data adapter has been started. It builds a Blueprint-friendly snapshot for health, mana, Huwan currency, XP, inventory slots/stacks, active quest progress, and live content status. See `Docs/HudMenuDataPrototype.md`.

The first prototype player actor has been started. It bundles inventory, equipment, stats, combat, rewards, quests, live content, and HUD data, with helper calls for the Basic Slime quest loop. See `Docs/PrototypePlayerActor.md`.

The first character creation runtime has been started. It validates race, sub-race, class, job, permanent skills, permanent abilities, Reincarnated limits, and starting stat bonuses, then plugs into the prototype player actor. See `Docs/CharacterCreationPrototype.md`.

The first character creation menu data adapter has been started. It can feed race, filtered sub-race, class, job, skill, ability, slot count, and validation data to a Blueprint UI. See `Docs/CharacterCreationMenuDataPrototype.md`.

The first-pass creation data fill now covers the seven major parent races, representative sub-races, all stated starter class paths, and the first job roster.

The first character creation screen scaffold has been started too. `UHuwamCharacterCreationScreenWidget` shows native prototype choices, validates confirmation, and initializes the prototype player from the confirmed request. See `Docs/CharacterCreationScreenPrototype.md`.

The first Laucian tutorial-lab handoff has been started. Confirmed characters now begin the Progression Zero tutorial quest, can move to a placed white-lab entry marker, and expose Movement and HUD tutorial beats to HUD/menu data. See `Docs/TutorialLabHandoffPrototype.md`.

The first physical lab interaction lesson has been started too. `AHuwamTutorialLabInteractableActor` gives the white lab a reusable training object that advances the player from Interaction Ready into Inventory Basics. See `Docs/TutorialLabInteractionPrototype.md`.

The first inventory lab lesson has been started too. The starter pack now keeps a gathering knife and small pouch loose in inventory, the lab points at the gathering knife for inspection, and the prototype player can advance the lesson into Equipment Basics. See `Docs/TutorialLabInventoryPrototype.md`.

The first equipment lab lesson has been started too. HUD/menu data now carries equipment slots, the lab points at the already-equipped main-hand Basic Sword, and the prototype player can advance the lesson into Combat Basics. See `Docs/TutorialLabEquipmentPrototype.md`.

The first combat lab lesson has been started too. `AHuwamTutorialLabCombatTargetActor` accepts a real player melee hit, advances the tutorial into Map Basics, and avoids Eldoria monster rewards or quest credit. See `Docs/TutorialLabCombatPrototype.md`.

The first map lab lesson has been started too. `UHuwamMapAwarenessComponent` seeds white-lab map state for minimap/full-map UI, HUD snapshots now expose that state, and opening the tutorial map advances Laucian into Quest Basics. See `Docs/TutorialLabMapPrototype.md`.

The first quest lab lesson has been started too. The quest runtime now tracks one active quest, map awareness exposes the tracked objective marker seed, and Laucian advances into Gathering Basics once the Progression Zero quest is tracked. See `Docs/TutorialLabQuestPrototype.md`.

The first gathering lab lesson has been started too. `AHuwamTutorialLabGatheringActor` grants tutorial lavender through inventory, checks for the gathering knife already introduced in the pack lesson, and completes the first Progression Zero tutorial objective. See `Docs/TutorialLabGatheringPrototype.md`.

The first reusable world gathering node has been started too. `AHuwamGatheringResourceActor` loads a material row, grants a field item into inventory, applies current in-game-day respawn timing, and can advance active gather objectives through progress-source IDs such as `material.field_supply`. See `Docs/GatheringResourcePrototype.md`.

The resource loop now also reserves a later player-built node path. Skilled players with matching tools, inputs, legal access, and time can establish new resource nodes such as watermelon fields; valid agricultural nodes mature into NPC-visible settlement production after about `2` in-game days. See `Docs/WorldBible.md` and `Docs/GatheringResourcePrototype.md`.

The first Eldoria field gathering quest path has been started too. The quest component now has a `Gather Field Supplies` fallback, the prototype player can start and turn it in, and field gathering progress can reach its current currency and experience reward loop. See `Docs/GatherFieldSuppliesQuestPrototype.md`.

The first NPC supply handoff has been started too. `UHuwamNpcSupplyComponent` accepts early field materials, updates recipient stock, and the `Gather Field Supplies` turn-in now delivers supplies before the reward resolves. See `Docs/NpcSupplyDeliveryPrototype.md`.

The first NPC supply shortage gate has been started too. The prototype player can now start `Gather Field Supplies` through the matching low-stock NPC supply component instead of treating the living request as universal. See `Docs/NpcSupplyShortageGatePrototype.md`.

The economy rules now also reserve essential NPC surplus buying. Profession-relevant buyers such as blacksmiths and builders can buy useful materials from players before those materials become urgent quest needs, building stock and easing shortage pressure. See `Docs/WorldBible.md`.

The first NPC stock trade and use helpers have been started too. Accepted player surplus can now enter NPC stock through a prototype sale path that pays copper-native item base value, supply storage has a prototype capacity, and stock can be spent through an explicit use call that rechecks shortage pressure. See `Docs/NpcSupplyTradeUsePrototype.md`.

The first NPC routine stock use has been started too. A configured NPC supply component can now repeat one accepted stock use on the current in-game-day timing so refilled stock can fall again through prototype world activity. See `Docs/NpcSupplyRoutineUsePrototype.md`.

The first NPC supply need state has been started too. Supply stock can now expose a named need, a reason, current quantity, missing quantity, urgency, and a next action for Blueprint and later AI layers. See `Docs/NpcSupplyNeedStatePrototype.md`.

The first need-aware NPC quest request has been started too. The shortage-gated field-supplies quest now stores the supply need that caused it, and HUD quest summaries can carry that request reason forward after acceptance. See `Docs/NpcSupplyQuestRequestPrototype.md`.

The first NPC quest offer preview has been started too. Quest content can now be previewed before activation, and the field-supplies offer copies the live NPC supply need into that pre-acceptance data. See `Docs/NpcSupplyQuestOfferPrototype.md`.

The first map-facing NPC supply quest interaction has been started too. `AHuwamNpcSupplyQuestOfferActor` owns the first requester's supply pocket, previews the field-supplies offer on interaction, and accepts the same need-aware offer into the active quest path. See `Docs/NpcSupplyQuestInteractionPrototype.md`.

The first map-facing NPC supply turn-in has been started too. The same placed requester can accept the completed field-supplies delivery into its own stock pocket, then broadcast the quest and delivery result after reward resolution. See `Docs/NpcSupplyQuestTurnInPrototype.md`.

The first NPC supply interaction state snapshot has been started too. The placed Eldoria requester can now report whether it is offering work, waiting for delivery, ready for turn-in, satisfied, or unavailable, with prompt text plus the relevant need and quest data. See `Docs/NpcSupplyQuestInteractionStatePrototype.md`.

The first state-driven NPC supply interaction route has been started too. One actor call can now preview or accept the field-supplies offer, report waiting or satisfied state, or turn in delivery, while returning before-and-after interaction snapshots. See `Docs/NpcSupplyQuestStateDrivenInteractionPrototype.md`.

The first NPC supply quest prompt surface has been started too. `UHuwamNpcSupplyQuestInteractionWidget` reads the routed Eldoria requester snapshot, previews and accepts the supply request, shows progress and reward details, and turns the completed delivery back through the same state route. See `Docs/NpcSupplyQuestWidgetPrototype.md`.

The first shared world interaction route has been started too. `UHuwamWorldInteractionComponent` can focus a nearby Eldoria requester, white-lab interaction object, or field gathering node, exposes prompt state into HUD data, and lets the prototype player open the NPC supply request screen from focused world action. See `Docs/WorldInteractionPrototype.md`.

The first visible world interaction prompt has been started too. `UHuwamWorldInteractionPromptWidget` shows the current supported focus prompt, the interaction component refreshes nearby focus during play, and the placed prototype player now binds a first `E`-key route until the real third-person pawn shell arrives. See `Docs/WorldInteractionPromptPrototype.md`.

The first third-person prototype character has been started too. `AHuwamPrototypeThirdPersonCharacter` adds movement, a spring-arm follow camera, first pawn inputs, camera trace and aimed fallback focus, and an interaction-origin handoff into the existing prototype player state actor. See `Docs/ThirdPersonPrototypeCharacter.md`.

The first third-person startup bootstrap has been started too. `AHuwamPrototypeGameMode` can spawn a character-creation-ready prototype player state actor, link it to the third-person character, and let the white-lab handoff move the playable avatar to Laucian's lab entry marker. See `Docs/ThirdPersonBootstrapPrototype.md`.

The first visible Laucian tutorial prompt has been started too. `UHuwamTutorialLabPromptWidget` shows the current white-lab beat, the third-person pawn advances Movement after the player walks and looks around, and the HUD Basics beat can now advance from a first `Tab` acknowledgment into the existing pedestal interaction. See `Docs/TutorialLabPromptPrototype.md`.

The first playable vertical slice pass has been started too. Native HUD and menu surfaces advance inventory, equipment, map, and quest lab beats; pawn attack and interaction input cover tutorial combat, Basic Slimes, and tutorial gathering; a code-spawned white lab exits into a code-spawned Eldoria starter field; and the first save slot persists prototype transform, inventory, equipment, rewards, quests, map awareness, and tutorial state. See `Docs/PlayableVerticalSlicePrototype.md`.

The first playable-slice follow-up batch has been started too. Eldoria now places an Adventurers Guild contract board for the first slime contract and a visible NPC supply requester for the field-supplies path, the pawn control keys are named in `DefaultInput.ini`, the HUD reads Huwan denominations and turn-in state more clearly, defeated blockout monsters leave interaction focus, and the prototype save snapshot is versioned and preserves current health and mana. See `Docs/GuildContractBoardPrototype.md` and `Docs/PlayableVerticalSlicePrototype.md`.

The next Eldoria slice batch has been started too. Guild boards now read configured contract quest IDs into board entries and prioritize turn-ins over active tracking and offers, their prompts can name the selected contract, the requester panel can sell one accepted surplus item into NPC stock, depleted gatherables hide until respawn, wounded blockout monsters shrink before defeat, and the starter field has first board/supply pads, road spurs, and boundary fences. See `Docs/GuildContractBoardPrototype.md`, `Docs/NpcSupplyQuestWidgetPrototype.md`, and `Docs/PlayableVerticalSlicePrototype.md`.

The runtime data bridge has been hardened too. If no authored registry asset is configured, the data subsystem now creates a transient registry and loads all 15 starter CSV tables from `Content/Data`, while editor validation checks the core race, content pack, class, item, spell, monster, NPC, and quest rows before playing the slice. See `Docs/UnrealDataImport.md`.

The first repeatable living request loop has been started too. Quest data now marks repeatable starter contracts, the quest runtime can restart a turned-in repeatable quest with fresh objective progress, NPC routine stock use can consume any accepted stocked supply when its preferred item is absent, and editor validation now proves that Eldoria's field-supplies request can return after NPC stock drains. See `Docs/NpcSupplyRoutineUsePrototype.md`, `Docs/NpcSupplyQuestRequestPrototype.md`, and `Docs/StarterQuestRuntimePrototype.md`.

The first shared living-server clock has been started too. `UHuwamTimeSubsystem` owns the prototype conversion of 24 real hours into 7 in-game days, exposes a time snapshot for Blueprint and future AI engines, keeps time skipping disabled, and now feeds gathering respawn delays plus NPC routine supply delays. See `Docs/HuwamLivingTimeServicePrototype.md`.

The first inventory spoilage hook has been started too. Inventory stacks can now carry freshness metadata, perishable stacks use the shared Huwam day count, herbs and food-like materials can become stale or spoiled, non-food materials stay stable, and saved inventory restores stack freshness instead of pretending everything was newly acquired. See `Docs/InventorySpoilagePrototype.md` and `Docs/InventoryPrototype.md`.

The first storage preservation layer has been started too. Inventory stacks now remember storage quality and preservation multipliers, pouches and chests slow spoilage, cooled storage greatly extends freshness, and magical storage holds perishables in stasis for the prototype pass. New item data rows reserve food pouches, cooled storage boxes, and magical storage boxes. See `Docs/InventorySpoilagePrototype.md` and `Docs/InventoryPrototype.md`.

The first storage routing layer has been started too. Inventory stacks now carry route IDs, storage routes expose used and remaining capacity, items can be added directly into a target route, existing stacks can be reassigned into pouch/chest/cooled/magical storage, and the prototype menu shows route capacity plus stack freshness. See `Docs/StorageRoutingPrototype.md`.

The first survival vitals layer has been started too. A new survival component tracks hunger, thirst, and social energy against Huwam living time, the HUD shows those meters, food consumption reads inventory freshness, and prototype saves preserve survival state. See `Docs/SurvivalVitalsPrototype.md`.

The first survival consequence layer has been started too. Empty hunger, thirst, and social meters now expose active effect IDs, unsafe food is tracked, and starvation/dehydration/unsafe-food can apply true damage through the combat component while validation restores the slice afterward. See `Docs/SurvivalVitalsPrototype.md`.

The first survival service layer has been started too. Tavern meals, well water, campfire chats, and inn rooms can restore survival needs without advancing Huwam's living server clock, preserving the rule that nobody can speed up time by sleeping or resting. See `Docs/SurvivalVitalsPrototype.md`.

The first placed survival service actors have been started too. Eldoria's prototype blockout now spawns a tavern meal point, town well, campfire chat point, and inn room service, and the world interaction router can focus and use them like other supported world targets. See `Docs/SurvivalServiceActorsPrototype.md`.

The first survival service economy layer has been started too. Service actors now carry copper prices, owner NPC IDs, owning guild/faction IDs, and access policies; paid services check and spend the player's wallet, while locked services are blocked before charging. See `Docs/SurvivalServiceActorsPrototype.md` and `Docs/CurrencyPrototype.md`.

The first economy ledger layer has been started too. Paid survival services now create revenue records with owner NPC, owning guild/faction, settlement, source tag, copper amount, and in-game day, giving later NPC wallets and settlement treasury systems a shared money trail. See `Docs/EconomyLedgerPrototype.md`.

The first spendable service-account layer has been started too. Revenue now splits into owner NPC wallets, guild/faction funds, and settlement treasury balances, and the subsystem can spend from those accounts for future wages, repairs, upkeep, and public works. See `Docs/EconomyLedgerPrototype.md`.

The first service upkeep layer has been started too. Survival service actors now have upkeep costs, can ask the economy ledger to pay from owner, guild/faction, then settlement accounts, and can become underfunded/closed when upkeep cannot be paid. See `Docs/EconomyLedgerPrototype.md` and `Docs/SurvivalServiceActorsPrototype.md`.

The first service pressure bridge has been started too. Underfunded service actors now expose a supply-style need state with owner, need ID, urgency, needed quantity, and request-player-help action so repair and shortage quests can latch onto closed services. See `Docs/SurvivalServiceActorsPrototype.md`.

The first HUD-facing economy debug layer has been started too. HUD/menu data now includes economy ledger totals, account balances, and focused survival-service pressure, and the native prototype HUD displays ledger count plus total recorded revenue beside the wallet line. See `Docs/HudMenuDataPrototype.md` and `Docs/EconomyLedgerPrototype.md`.

The first service restoration quest generation layer has been started too. Quest runtime can now turn an underfunded service-pressure state into a repeatable restoration contract with owner, need context, urgency, objective source, reward, and tracked active state. See `Docs/EconomyLedgerPrototype.md`.

The first service restoration turn-in layer has been started too. Survival service actors can now accept a completed matching restoration quest and use it to clear underfunding, re-enable the service, and complete the first failure-to-repair loop. See `Docs/SurvivalServiceActorsPrototype.md`.

The first economy persistence layer has been started too. Prototype saves now carry economy ledger entries plus NPC, guild/faction, and settlement account balances, and load restores them through the economy ledger subsystem. See `Docs/EconomyLedgerPrototype.md`.

The first survival-service persistence layer has been started too. Service actors now expose identity-based save snapshots for pricing, ownership, access, upkeep, enabled state, and underfunded state, and prototype save/load applies matching service state back onto placed actors. See `Docs/SurvivalServiceActorsPrototype.md`.

The first stable service identity layer has been started too. Eldoria's placed tavern, well, campfire, and inn service actors now have authored instance IDs so save/load and later quest targeting can address specific service points. See `Docs/SurvivalServiceActorsPrototype.md`.
