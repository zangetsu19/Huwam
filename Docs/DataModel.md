# Huwan Data Model V0.1

## Purpose

This document converts the Huwan World Bible into production-friendly data structures for Unreal Engine.

The goal is not to define every final stat, spell, item, NPC, or quest yet. The goal is to create a stable bridge between the world bible and the first playable prototype.

Core rule:

- **Every gameplay concept that can appear many times should become data.**

This includes races, sub-races, classes, jobs, skills, abilities, items, materials, recipes, spells, monsters, NPCs, quests, guilds, factions, contracts, reputation, and relationship states.

## Stable ID Rules

Every important record needs a stable ID. Names can change later, but IDs should remain stable.

Recommended format:

`category.subcategory.name.variant`

Examples:

- `race.human`
- `race.elf.dark`
- `class.progression_zero`
- `class.progression_rank_1`
- `class.mage`
- `job.blacksmith`
- `stat.strength`
- `skill.smithing`
- `ability.born_leader`
- `item.weapon.basic_sword`
- `material.slime_core.basic`
- `recipe.weapon.basic_sword`
- `spell.fire.spark`
- `monster.slime.basic`
- `npc.eldoria.gorth_runkis`
- `quest.eldoria.slimes_in_tall_grass`
- `guild.adventurers`
- `faction.peace_between_us`

ID rules:

- Use lowercase.
- Use underscores for multi-word names.
- Do not include spaces.
- Do not reuse IDs after deletion.
- Keep display names separate from IDs.
- Keep lore text separate from mechanical fields.
- Prefer specific IDs over vague IDs.

## Unreal Data Strategy

| Data Type | Suggested Unreal Form | Reason |
|---|---|---|
| Static lists | DataTables or CSV | Easy to edit, import, and expand. |
| Complex authored objects | Primary Data Assets | Better for rich records with nested values. |
| Runtime state | C++ structs and SaveGame records | Needed for persistence and server authority. |
| Tags and filters | Gameplay Tags or ID strings | Useful for requirements, categories, and search. |
| Prototype-only values | DataTables or Blueprint defaults | Fast iteration before final architecture. |

V0.1 should favor simple DataTables and C++ structs. Once the shape stabilizes, complex records can move into Primary Data Assets.

## Core Structs

These are the first data structures to implement.

### Race Data

Used for playable race selection, NPC generation, reputation, birth traits, mana well baselines, and cultural modifiers.

| Field | Type | Purpose |
|---|---|---|
| RaceId | ID | Stable race identifier. |
| DisplayName | Text | Player-facing name. |
| Description | Text | Short summary. |
| GeneralRole | Text | Role in Huwan society. |
| CommonReputation | Text | How people commonly view the race. |
| ClosestDeityId | ID | Common deity connection. |
| ContinentId | ID | Major homeland continent. |
| KingdomId | ID | Major kingdom or power. |
| Rarity | Enum | Common, uncommon, rare, mythic. |
| FearRespectProfile | Text or tags | Feared, respected, loved, hated, etc. |
| ManaWellBaseline | Enum | None, shallow, low, average, deep, very_deep, extreme. |
| TraitIds | ID list | Starting race traits. |
| MustNotHaveTraitIds | ID list | Traits blocked by lore. |
| ReputationModifiers | Struct | Starting social effects by region/faction. |

### Sub-Race Data

Used for more specific race identities.

| Field | Type | Purpose |
|---|---|---|
| SubRaceId | ID | Stable sub-race identifier. |
| ParentRaceId | ID | Main race. |
| DisplayName | Text | Player-facing name. |
| Description | Text | Short identity summary. |
| ManaWellModifier | Enum or number | Adjusts parent race mana baseline. |
| TraitIds | ID list | Added traits. |
| WeaknessIds | ID list | Added weaknesses. |
| BonusTags | Tags | Lore and gameplay bonuses. |
| ReputationModifiers | Struct | Social reactions. |
| UnlockRule | Text or tags | Default, hidden, reincarnated, story, etc. |

### Class Data

Used for player class progression and class unlocks.

| Field | Type | Purpose |
|---|---|---|
| ClassId | ID | Stable class identifier. |
| DisplayName | Text | Player-facing name. |
| Description | Text | Class fantasy. |
| MaxLevel | Number | Usually 20. |
| ClassType | Enum | Starter, martial, caster, divine, secret, progression. |
| UnlockRequirements | Requirement list | Stats, quests, guild, race, skills, etc. |
| StartingSpellIds | ID list | Starter spells if any. |
| StartingSkillIds | ID list | Class-linked skills. |
| AbilityUnlocks | Unlock table | Level-based class abilities. |
| StatGrowthWeights | Struct | Which stats grow through class actions. |
| GuildId | ID | Associated guild, if any. |
| EquipmentSlotOverrides | Struct | Special slots like Wizard book slot. |

### Job Data

Used for player and NPC work, wages, discounts, production, and social role.

| Field | Type | Purpose |
|---|---|---|
| JobId | ID | Stable job identifier. |
| DisplayName | Text | Player-facing name. |
| Description | Text | Work identity. |
| RankTrack | Enum | Beginner to Master, hidden God rank where allowed. |
| SalaryRange | Number range | Expected pay. |
| DiscountRules | Struct | Resources or services discounted by job. |
| FreeResourceRules | Struct | Resources that can become free at high rank. |
| UnlockRequirements | Requirement list | Stats, skills, guild, tools, property, reputation. |
| ProducedItemTags | Tags | What this job can produce. |
| NeededResourceTags | Tags | What this job consumes. |
| QuestTypesGenerated | Tags | What requests this job creates. |
| AssociatedGuildId | ID | Guild link, if any. |

### Stat Data

Used for player growth, NPC generation, requirements, scaling, and progression.

| Field | Type | Purpose |
|---|---|---|
| StatId | ID | Stable stat identifier. |
| DisplayName | Text | Player-facing name. |
| Description | Text | What it affects. |
| MinValue | Number | Usually 0 or 1. |
| MaxValue | Number | Demi-god target uses 999. |
| GrowthSources | Tags | Actions that improve the stat. |

Confirmed stats:

- Strength
- Charisma
- Intelligence
- Wisdom
- Dexterity
- Luck
- Health
- Mana
- Defense
- World Popularity

### Skill Data

Used for learned proficiencies and action improvement.

| Field | Type | Purpose |
|---|---|---|
| SkillId | ID | Stable skill identifier. |
| DisplayName | Text | Player-facing name. |
| Category | Tags | Combat, crafting, gathering, magic, social, stealth, etc. |
| Description | Text | What it does. |
| GoverningStats | ID list | Stats that help progression or use. |
| PermanentAtCreation | Boolean | Whether it can be chosen as one of the first 5. |
| UnlockRequirements | Requirement list | Optional unlock gates. |
| ProgressionSources | Tags | Actions that train it. |

### Ability Data

Used for innate or special powers.

| Field | Type | Purpose |
|---|---|---|
| AbilityId | ID | Stable ability identifier. |
| DisplayName | Text | Player-facing name. |
| AbilityType | Enum | Birth, unique, passive, active, class, job, divine, bloodline, secret. |
| Description | Text | What it does. |
| PermanentAtCreation | Boolean | Whether it can be selected at creation. |
| Cost | Struct | Mana, stamina, cooldown, material, condition, etc. |
| Effects | Effect list | Mechanical effects. |
| Risks | Risk list | Misfire, exhaustion, curse, social consequence. |
| UnlockRequirements | Requirement list | Race, class, job, quest, deity, etc. |

### Item Data

Used for inventory, equipment, crafting, market, loot, storage, and quest demands.

| Field | Type | Purpose |
|---|---|---|
| ItemId | ID | Stable item identifier. |
| DisplayName | Text | Player-facing name. |
| ItemType | Enum | Weapon, armor, clothing, tool, material, consumable, pouch, container, relic, currency. |
| Description | Text | Short text. |
| StackLimit | Number | Basic items often 99 per slot. |
| Weight | Number | Carry or storage use. |
| BaseValueCopper | Number | Copper-native market baseline for all denominations. |
| AttackPower | Number | Starter offensive contribution when equipped or used. |
| DefensePower | Number | Starter defensive contribution when equipped. |
| ManaPower | Number | Starter magical contribution when equipped or used as focus. |
| UtilityPower | Number | Starter non-combat value for tools, storage, gathering, and support gear. |
| PrimaryEquipSlotId | ID | First equipment slot the item should use in V0.1. |
| EquipSlotIds | ID list | Slots this item can use. |
| Tags | Tags | Wood, metal, slime, basic, magical, food, etc. |
| Durability | Number | Optional. |
| SpoilageProfileId | ID | For food or organic goods. |
| VisualAssetRef | Asset ref | Later Unreal asset link. |

### Material Data

Materials can be regular items, but they need harvesting and crafting fields.

| Field | Type | Purpose |
|---|---|---|
| MaterialId | ID | Stable material identifier. |
| ItemId | ID | Inventory item link. |
| SourceType | Tags | Tree, monster, ore, plant, fish, dungeon, ruin. |
| HarvestToolIds | ID list | Required or preferred tools. |
| Rank | Enum | F, E, D, C, B, A, S, SS. |
| RespawnRuleId | ID | Respawn timing. |
| Edible | Boolean | Whether logic says it can be eaten. |
| CraftingTags | Tags | Recipe uses. |

### Recipe Data

Used for crafting.

| Field | Type | Purpose |
|---|---|---|
| RecipeId | ID | Stable recipe identifier. |
| DisplayName | Text | Player-facing name. |
| OutputItemId | ID | Crafted result. |
| OutputQuantity | Number | Amount produced. |
| IngredientList | Item quantity list | Required resources. |
| RequiredSkillIds | ID list | Skills needed. |
| RequiredJobIds | ID list | Jobs that can use or improve recipe. |
| RequiredStationIds | ID list | Furnace, anvil, sewing kit, etc. |
| RequiredToolIds | ID list | Hammer, knife, tongs, etc. |
| GoverningStats | ID list | Strength, Dexterity, Intelligence, Wisdom. |
| CraftTime | Time | Time needed. |
| QualityRules | Struct | How quality improves. |

### Spell Data

Used for magic, classes, items, runes, NPC casters, and spell creation.

| Field | Type | Purpose |
|---|---|---|
| SpellId | ID | Stable spell identifier. |
| DisplayName | Text | Player-facing name. |
| Category | Enum | Air, Earth, Fire, Water, Nature, Light, Dark, Green, Rune, Life, Death, Spirit, Forbidden. |
| School | Enum | Conjuration, Evocation, Necromancy, Abjuration, Transmutation, Divination, Enchantment, Illusion. |
| Rank | Enum | F, E, D, C, B, A, S, SS. |
| ManaCost | Number | Base cost. |
| CastTime | Enum | Instant, quick, normal, slow, ritual, extended ritual. |
| Duration | Enum | Instant, short, sustained, timed, permanent, conditional. |
| Range | Enum | Self, touch, short, medium, long, sight, area, world-scale. |
| Targeting | Enum | Self, ally, enemy, object, ground, area, line, cone, aura, ritual. |
| Requirements | Requirement list | Class, job, skill, tool, focus, rune, relic, mana well. |
| Risks | Risk list | Misfire, exhaustion, backlash, corruption, overload. |
| Scaling | Struct | How stats or skill improve it. |
| LegalNotes | Text | Law focuses on misuse, not mere possession. |

### Monster Data

Used for combat, ecology, harvesting, taming, dungeons, quests, and economy.

| Field | Type | Purpose |
|---|---|---|
| MonsterId | ID | Stable monster identifier. |
| DisplayName | Text | Player-facing name. |
| Rank | Enum | F to SS. |
| BaseStrength | Number | Starter strength value applied to encounter stats. |
| BaseDexterity | Number | Starter dexterity value applied to encounter stats. |
| BaseHealth | Number | Starter health value applied to encounter stats. |
| BaseMana | Number | Starter mana value applied to encounter stats. |
| BaseDefense | Number | Starter defense value applied to encounter stats. |
| BaseLuck | Number | Starter luck value applied to encounter stats. |
| BiomeTags | Tags | Where it appears. |
| BehaviorTags | Tags | Passive, hostile, territorial, social, tamable, intelligent. |
| Drops | Loot table | Materials and items. |
| PrimaryDropItemId | ID | Simple V0.1 monster drop. |
| PrimaryDropQuantity | Number | Quantity for the simple V0.1 monster drop. |
| CurrencyRewardCopper | Number | Copper-native currency payout after defeat. |
| ExperienceReward | Number | Experience payout after defeat. |
| EdibleParts | ID list | Which parts can be eaten. |
| HarvestRequirements | Tool/skill list | Skinning knife, alchemy, etc. |
| TamingRules | Struct | Whether and how it can be tamed. |
| EvolutionRules | Struct | For slimes and other evolvable monsters. |
| DungeonFloorRange | Range | Optional dungeon use. |

### NPC Data

Used for Eldoria citizens, important NPCs, merchants, nobles, guild leaders, and simulation.

| Field | Type | Purpose |
|---|---|---|
| NpcId | ID | Stable NPC identifier. |
| DisplayName | Text | Player-facing name. |
| RaceId | ID | Race. |
| SubRaceId | ID | Optional sub-race. |
| Age | Number | Current age. |
| SimulationTier | Enum | Background, active, full, critical. |
| ClassIds | ID list | Classes. |
| JobIds | ID list | Jobs. |
| GuildId | ID | Guild link. |
| FactionIds | ID list | Faction links. |
| HomeLocationId | ID | Where they live. |
| WorkLocationId | ID | Where they work. |
| InventoryProfileId | ID | Starting or generated inventory. |
| WealthState | Struct | Gold, income, debt, wages. |
| PersonalityProfileId | ID | Values and behavior. |
| RelationshipProfile | Struct | Starting attitudes. |
| QuestPoolIds | ID list | Quests they can generate. |
| MemoryRules | Struct | What they remember deeply. |

### Content Pack Data

Used for future DLC, cosmetics, pets, mounts, tames, events, and larger expansion packs.

| Field | Type | Purpose |
|---|---|---|
| ContentPackId | ID | Stable content pack identifier. |
| DisplayName | Text | Player/admin-facing name. |
| PackType | Enum | Cosmetic, companion, gameplay, world, event, or system. |
| PackState | Enum | Draft, staged, active, paused, or retired. |
| Version | Version text | Pack version. |
| RequiredGameVersion | Version text | Minimum game version required. |
| EntitlementId | ID | Store/account ownership ID. |
| bEnabledByDefault | Boolean | Whether the server enables it automatically. |
| bRequiresOwnership | Boolean | Whether ownership is required. |
| bCosmeticOnly | Boolean | Whether it avoids gameplay power changes. |
| ContentIds | ID list | Items, pets, mounts, quests, spells, maps, or other content in the pack. |
| FeatureFlagIds | ID list | Server feature flags required to use it. |
| Description | Text | Design/admin notes. |

### Quest Data

Used for guild boards, NPC requests, contracts, tutorials, and world events.

| Field | Type | Purpose |
|---|---|---|
| QuestId | ID | Stable quest identifier. |
| DisplayName | Text | Player-facing name. |
| QuestType | Enum | Tutorial, gather, combat, craft, delivery, social, contract, dungeon, raid. |
| QuestGiverId | ID | NPC, guild, board, faction, system. |
| Description | Text | Short summary. |
| PrimaryObjectiveId | ID | First runtime objective ID. |
| PrimaryObjectiveDescription | Text | HUD/menu objective text. |
| PrimaryObjectiveTargetValue | Number | Completion target for the primary objective. |
| PrimaryObjectiveProgressSourceId | ID | Source that advances the objective, such as a monster or material. |
| Objectives | Objective list | Later multi-objective expansion. |
| TimeLimit | Time or none | Uses living server time. |
| Urgency | Enum | Normal, timed, urgent, emergency. |
| Rewards | Reward list | Currency, items, XP, reputation, relationship, map, unlock. |
| RewardCopper | Number | First-pass copper-native currency reward. |
| RewardExperience | Number | First-pass experience reward. |
| FailureRules | Struct | What happens if ignored or failed. |
| PartySharingRules | Struct | Who can see and complete it. |
| WorldStateEffects | Effect list | NPC inventory, market, reputation, faction, etc. |

## First Prototype Data Set

This is the recommended first content set for V0.1.

The first playable slice can still stay narrow, but character creation now carries a broader first-pass menu set so the initial UI can show Huwan's major identity choices without waiting on a later data pass.

### Races

| ID | Display Name | Purpose |
|---|---|---|
| `race.human` | Human | All-rounder baseline. |
| `race.elf` | Elf | Long life, mana, knowledge culture. |
| `race.dwarf` | Dwarf | Craft, labor, defense, and fortress culture. |
| `race.orc` | Orc | Fear, strength, law, and escort power. |
| `race.demon` | Demon | Deep mana, prejudice, and dangerous reputation. |
| `race.beastfolk` | Beastfolk | Animal senses, exploitation pressure, and nature paths. |
| `race.fae` | Fae | Dreamlike strangeness and pact logic. |
| `race.halfling` | Halfling | Extra prototype support for Eldoria-linked Halfling content. |

`Dark-Elf` now belongs in `Sub-Race Data` under `race.elf` for character creation.

### Prototype Sub-Races

The first character creation data pass now includes approved Human baselines, representative Elf/Orc/Demon/Beastfolk/Fae rows, and the approved Dwarf baseline set.

| ID | Display Name | Parent Race | Purpose |
|---|---|---|---|
| `race.dwarf.mountain` | Mountain Dwarf | `race.dwarf` | Classic mountain miner, smith, and fortress builder baseline. |
| `race.dwarf.forge` | Forge Dwarf | `race.dwarf` | Furnace, smithing, heat discipline, and metal craft baseline. |
| `race.dwarf.quarry` | Quarry Dwarf | `race.dwarf` | Masonry, civic construction, roads, walls, and repair baseline. |
| `race.dwarf.deep` | Deep Dwarf | `race.dwarf` | Cavern, tunnel, mine, sealed ruin, and pressure endurance baseline. |
| `race.dwarf.goldbeard` | Goldbeard Dwarf | `race.dwarf` | Appraisal, contracts, trade, banking, and high-value gear baseline. |
| `race.dwarf.iron_oath` | Iron Oath Dwarf | `race.dwarf` | Duty, sacred contracts, defense, and disciplined labor baseline. |
| `race.dwarf.dreamforge` | Dreamforge Dwarf | `race.dwarf` | Ione-touched dream blueprints and magical crafting baseline. |
| `race.dwarf.hearth` | Hearth Dwarf | `race.dwarf` | Tavern, food, brewing, comfort, morale, and community work baseline. |
| `race.orc.common` | Common Orc | `race.orc` | Green clanned brute, tusks, ugly face, and battle-ready baseline. |
| `race.orc.half` | Half-Orc | `race.orc` | Half-Human Orc bridge with green skin, smaller tusks, and Human-influenced features. |
| `race.orc.war` | War Orc | `race.orc` | Brown-red war-painted fighter with dense chiseled muscle baseline. |
| `race.orc.stonehide` | Stonehide Orc | `race.orc` | Stone-gray rocky hide, protrusions, metal armor, and tank baseline. |
| `race.orc.redtusk` | Redtusk Orc | `race.orc` | Blood-red bloodshot duelist with red tusk and fight-ready baseline. |
| `race.orc.gray` | Gray Orc | `race.orc` | Gray-skinned intelligent neutral scholar-guard baseline. |
| `race.orc.ironbound` | Ironbound Orc | `race.orc` | Pink pig-like laborer with ironbound history and resilient dignity baseline. |
| `race.orc.wild` | Wild Orc | `race.orc` | Very short green helmeted tall-grass ambusher baseline. |

### Classes

| ID | Display Name | Purpose |
|---|---|---|
| `class.progression_zero` | Progression Zero | Starting state. |
| `class.progression_rank_1` | Progression Rank 1 | Generalist noob progression. |
| `class.archer` | Archer | Bow, scouting, and Archers Guild starter path. |
| `class.priest` | Priest | Faith, healing rites, and divine support. |
| `class.knight` | Knight | Protection, duty, and escort path. |
| `class.adventurer_rank_f` | Adventurer Rank F | Entry quest and F-rank field path. |
| `class.thief` | Thief | Locks, stealth, and shadowed contract work. |
| `class.mage` | Mage | First rune and mana prototype class. |

### Jobs

| ID | Display Name | Purpose |
|---|---|---|
| `job.builder` | Builder | Repair and construction loop. |
| `job.adventurer` | Adventurer | Guild quests and F-rank combat. |
| `job.hunter_gatherer` | Hunter/Gatherer | Resource loop. |
| `job.officer` | Officer | Law, patrol, and authority loop. |
| `job.blacksmith` | Blacksmith | Starter weapon/tool crafting. |
| `job.merchant` | Merchant | Market and inventory loop. |
| `job.tavern_owner` | Tavern Owner | Food, rooms, staff, and rumor loop. |
| `job.farmer` | Farmer | Food economy and herb/field loop. |
| `job.fencer` | Fencer | Dueling and precision weapon loop. |
| `job.dungeon_explorer` | Dungeon Explorer | Dungeon route and map loop. |
| `job.craftsman` | Craftsman | Everyday goods production loop. |
| `job.baker` | Baker | Food production and town supply loop. |
| `job.contractor` | Contractor | Crews, permits, and build-order loop. |

### Starter Spells

| ID | Display Name | Category |
|---|---|---|
| `spell.fire.spark` | Spark | Fire |
| `spell.air.gust` | Gust | Air |
| `spell.water.sprinkle` | Sprinkle | Water |
| `spell.light.tiny_lights` | Tiny Lights | Light |
| `spell.green.minor_heal` | Minor Heal | Green |
| `spell.rune.warm_rune` | Warm Rune | Rune |
| `spell.rune.alarm_rune` | Alarm Rune | Rune |

### First Monsters

| ID | Display Name | Rank |
|---|---|---|
| `monster.slime.basic` | Basic Slime | F |
| `monster.slime.wood` | Wood Slime | F |
| `monster.goblin.basic` | Goblin | E |

### First Materials

| ID | Display Name | Source |
|---|---|---|
| `material.wood.basic` | Basic Wood | Trees |
| `material.stone.basic` | Stone | Rocky fields |
| `material.flax` | Flax | Fields |
| `material.fiber.grass` | Grass Fiber | Fields |
| `material.herb.lavender` | Lavender | Fields/forests |
| `material.herb.mint` | Mint | Fields/rivers |
| `material.herb.rosemary` | Rosemary | Fields |
| `material.slime_core.basic` | Basic Slime Core | Basic Slime |
| `material.slime_gel.basic` | Basic Slime Gel | Basic Slime |
| `material.hide.small` | Small Animal Hide | Small animals |

### First Items

| ID | Display Name | Type |
|---|---|---|
| `item.weapon.basic_sword` | Basic Sword | Weapon |
| `item.weapon.basic_knife` | Basic Knife | Weapon/tool |
| `item.weapon.basic_bow` | Basic Bow | Weapon |
| `item.ammo.basic_arrows` | Basic Arrows | Ammo |
| `item.container.basic_quiver` | Basic Quiver | Container |
| `item.tool.gathering_knife` | Gathering Knife | Tool |
| `item.tool.hatchet` | Hatchet | Tool |
| `item.tool.pickaxe` | Pickaxe | Tool |
| `item.tool.fishing_rod` | Fishing Rod | Tool |
| `item.container.small_pouch` | Small Pouch | Container |
| `item.container.basic_chest` | Basic Chest | Storage |
| `item.bag.adventuring_backpack` | Adventuring Backpack | Backpack |

### First Recipes

| ID | Output | Ingredients |
|---|---|---|
| `recipe.weapon.basic_sword` | Basic Sword | Iron bar, basic wood |
| `recipe.weapon.basic_knife` | Basic Knife | Iron bar, basic wood |
| `recipe.weapon.basic_bow` | Basic Bow | Basic wood, flax bowstring |
| `recipe.ammo.basic_arrows` | Basic Arrows | Stone, basic wood |
| `recipe.container.basic_quiver` | Basic Quiver | Grass fiber, hide |
| `recipe.container.small_pouch` | Small Pouch | Small hide |
| `recipe.container.basic_chest` | Basic Chest | Basic wood |
| `recipe.gear.slime_test_piece` | Slime Gear Test Piece | Slime gel, slime core, fiber |

### First NPCs

| ID | Display Name | Role |
|---|---|---|
| `npc.eldoria.gorth_runkis` | Gorth Runkis | Adventurers Guild leader. |
| `npc.eldoria.robin_quickfoot` | Robin Quickfoot | Archers Guild founder. |
| `npc.eldoria.market_merchant_01` | Placeholder Merchant | First shop and supply test. |
| `npc.eldoria.poor_resident_01` | Placeholder Poor Resident | First urgent resource quest. |
| `npc.eldoria.guard_01` | Placeholder Guard | Law and safe-zone test. |
| `npc.eldoria.church_acolyte_01` | Placeholder Church Acolyte | Healing/revival hint. |
| `npc.eldoria.thieves_contact_01` | Placeholder Thieves Contact | Hidden contract hint. |
| `npc.eldoria.dawnmere_agent_01` | Placeholder Dawnmere Agent | Political pressure hint. |

### First Quests

| ID | Display Name | Repeatable | Purpose |
|---|---|---|---|
| `quest.tutorial.progression_zero` | Welcome to Progression Zero | No | Tutorial flow. |
| `quest.eldoria.guild_registration` | First Guild Registration | No | Adventurers Guild onboarding. |
| `quest.eldoria.slimes_in_tall_grass` | Slimes in the Tall Grass | Yes | Basic combat and harvesting. |
| `quest.eldoria.gather_field_supplies` | Gather Field Supplies | Yes | Resource gathering. |
| `quest.eldoria.repair_basic_tool` | Repair a Basic Tool | Yes | Crafting and NPC inventory update. |

## Implementation Order

Recommended next technical order:

1. Create C++ enums and structs for the data above. **Done for V0.1**
2. Create the first DataTables or CSV files. **Done for V0.1**
3. Create a data registry that can reference imported DataTables. **Done for prototype**
4. Auto-load missing runtime tables from `Content/Data` CSV files. **Done for prototype**
5. Import CSV files into Unreal as DataTable assets. **Optional production path**
6. Create a `UHuwamDataRegistry` Data Asset and assign the imported tables. **Optional production path**
7. Show loaded records in a debug screen or log. **Done for prototype**
8. Build inventory and item stack behavior using the loaded item data. **Started**
9. Build equipment slots and equipment stat totals. **Started**
10. Build character stats and derived power summaries. **Started**
11. Build starter combat resolution. **Started**
12. Build the first Basic Slime enemy encounter. **Started**
13. Build the reward bridge for currency, experience, and quest progress. **Started**
14. Build starter quest runtime using quest data. **Started**
15. Build live content pack manifests for DLC and expansion planning. **Started**
16. Build the HUD/menu data adapter. **Started**
17. Build a starter prototype player actor for the first playable loop. **Started**
18. Build character creation using race/class/job data. **Started**
19. Build a character creation menu data adapter. **Started**
20. Expand first-pass creation data for major races and starter paths. **Done**
21. Add approved Human sub-race art baselines to character creation data and UI card language. **Done**
22. Build the first simple character creation screen. **Started**
23. Hand the confirmed character into Laucian's tutorial lab. **Started**
24. Add the first physical tutorial-lab interaction lesson. **Started**
25. Add the first inventory tutorial lesson. **Started**
26. Add the first equipment tutorial lesson. **Started**
27. Add the first combat tutorial lesson. **Started**
28. Add the first map tutorial lesson. **Started**
29. Add the first quest tutorial lesson. **Started**
30. Add the first gathering or pickup tutorial lesson. **Started**
31. Build the first reusable F-rank gathering object outside the tutorial lab. **Started**
32. Add the first Eldoria gathering quest start and turn-in path. **Started**
33. Add gathered-material delivery and NPC stock updates on quest turn-in. **Started**
34. Gate the first generated shortage request from NPC supply state. **Started**
35. Add a first NPC supply trade and use path that can accept useful surplus and lower stock through NPC activity. **Started**
36. Connect the first NPC stock use to a timed routine or need. **Started**
37. Let repeatable NPC and guild requests restart when their world condition returns. **Started**
38. Build the first centralized living-server time service for resources, NPC routines, and quest timers. **Started**
39. Connect food spoilage to shared Huwam time. **Started**
40. Add container/storage modifiers for spoilage and preservation. **Started**
41. Add dedicated storage assignment UI for pouch, chest, cooled, and magical storage routing. **Started**
42. Add survival vitals for hunger, thirst, and social energy. **Started**
43. Add starvation, dehydration, unsafe-food, and social-isolation effects. **Started**
44. Add tavern, well, food, and rest services that restore survival needs without time skipping. **Started**
45. Add placed Eldoria service actors for taverns, wells, campfires, and inn rooms. **Started**
46. Add prices, ownership, and faction access rules for survival services. **Started**
47. Route paid service revenue into NPC, guild/faction, and settlement ledger records. **Started**
48. Add spendable NPC wallets, guild/faction funds, and settlement treasury balances. **Started**
49. Connect treasury and wallet balances to service upkeep. **Started**
50. Connect underfunded services to repair, shortage, and quest pressure. **Started**
51. Surface economy ledger and service pressure in HUD/menu debug data. **Started**
52. Add prototype repair/service-restoration quest generation from service pressure. **Started**
53. Add service restoration quest turn-in effects that reopen or stabilize services. **Started**
54. Persist economy ledger and account balances into prototype saves. **Started**
55. Persist placed survival-service funding/underfunded state by service identity. **Started**
56. Add authored stable service instance IDs for all placed Eldoria services. **Started**
57. Add service-specific repair material requirements and restoration contribution types. **Next**

## Implementation Started

The first Unreal-facing data layer now exists.

The registry now has a prototype-safe runtime bootstrap. If no registry asset is configured, `UHuwamDataSubsystem` creates an active transient registry and loads all missing tables from the CSV files in `Content/Data`. The editor validation runner checks known race, content pack, class, item, spell, monster, NPC, and quest rows before playing through the vertical slice.

Created C++ header:

- `Source/Huwam/Data/HuwamDataTypes.h`

Created C++ data registry:

- `Source/Huwam/Data/HuwamDataRegistry.h`
- `Source/Huwam/Data/HuwamDataRegistry.cpp`

Created runtime data subsystem:

- `Source/Huwam/Data/HuwamDataSubsystem.h`
- `Source/Huwam/Data/HuwamDataSubsystem.cpp`

Created data smoke test actor:

- `Source/Huwam/Data/HuwamDataSmokeTestActor.h`
- `Source/Huwam/Data/HuwamDataSmokeTestActor.cpp`

Created first gameplay component:

- `Source/Huwam/Gameplay/HuwamTimeSubsystem.h`
- `Source/Huwam/Gameplay/HuwamTimeSubsystem.cpp`
- `Source/Huwam/Gameplay/HuwamInventoryComponent.h`
- `Source/Huwam/Gameplay/HuwamInventoryComponent.cpp`
- `Source/Huwam/Gameplay/HuwamEquipmentComponent.h`
- `Source/Huwam/Gameplay/HuwamEquipmentComponent.cpp`
- `Source/Huwam/Gameplay/HuwamCharacterStatsComponent.h`
- `Source/Huwam/Gameplay/HuwamCharacterStatsComponent.cpp`
- `Source/Huwam/Gameplay/HuwamCombatComponent.h`
- `Source/Huwam/Gameplay/HuwamCombatComponent.cpp`
- `Source/Huwam/Gameplay/HuwamMonsterEncounterActor.h`
- `Source/Huwam/Gameplay/HuwamMonsterEncounterActor.cpp`
- `Source/Huwam/Gameplay/HuwamRewardComponent.h`
- `Source/Huwam/Gameplay/HuwamRewardComponent.cpp`
- `Source/Huwam/Gameplay/HuwamQuestComponent.h`
- `Source/Huwam/Gameplay/HuwamQuestComponent.cpp`
- `Source/Huwam/Gameplay/HuwamLiveContentComponent.h`
- `Source/Huwam/Gameplay/HuwamLiveContentComponent.cpp`
- `Source/Huwam/Gameplay/HuwamHudDataComponent.h`
- `Source/Huwam/Gameplay/HuwamHudDataComponent.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`
- `Source/Huwam/Gameplay/HuwamCharacterCreationComponent.h`
- `Source/Huwam/Gameplay/HuwamCharacterCreationComponent.cpp`
- `Source/Huwam/Gameplay/HuwamCharacterCreationMenuDataComponent.h`
- `Source/Huwam/Gameplay/HuwamCharacterCreationMenuDataComponent.cpp`

Created starter CSV data tables:

- `Content/Data/DT_Races.csv`
- `Content/Data/DT_ContentPacks.csv`
- `Content/Data/DT_SubRaces.csv`
- `Content/Data/DT_Classes.csv`
- `Content/Data/DT_Jobs.csv`
- `Content/Data/DT_Stats.csv`
- `Content/Data/DT_Skills.csv`
- `Content/Data/DT_Abilities.csv`
- `Content/Data/DT_Items.csv`
- `Content/Data/DT_Materials.csv`
- `Content/Data/DT_Recipes.csv`
- `Content/Data/DT_Spells.csv`
- `Content/Data/DT_Monsters.csv`
- `Content/Data/DT_NPCs.csv`
- `Content/Data/DT_Quests.csv`

Created Unreal target files:

- `Source/Huwam.Target.cs`
- `Source/HuwamEditor.Target.cs`

The registry has now been started. The next step is to import the CSV files into Unreal as DataTable assets, create a `UHuwamDataRegistry` Data Asset, assign the tables to it, and then use the registry from prototype systems.

Import guide:

- `Docs/UnrealDataImport.md`

Once the import is complete, place `HuwamDataSmokeTestActor` in a test map, assign the registry asset, and run the map to confirm the first known rows load correctly.

Inventory prototype notes:

- `Docs/InventoryPrototype.md`

Equipment prototype notes:

- `Docs/EquipmentPrototype.md`

Equipment stats prototype notes:

- `Docs/EquipmentStatsPrototype.md`

Character stats prototype notes:

- `Docs/CharacterStatsPrototype.md`

Combat prototype notes:

- `Docs/CombatPrototype.md`

Basic Slime encounter prototype notes:

- `Docs/BasicSlimeEncounterPrototype.md`

Reward bridge prototype notes:

- `Docs/RewardBridgePrototype.md`

Starter quest runtime prototype notes:

- `Docs/StarterQuestRuntimePrototype.md`

Live content prototype notes:

- `Docs/LiveContentPrototype.md`

HUD/menu data prototype notes:

- `Docs/HudMenuDataPrototype.md`

Prototype player actor notes:

- `Docs/PrototypePlayerActor.md`

Character creation prototype notes:

- `Docs/CharacterCreationPrototype.md`

Character creation menu data prototype notes:

- `Docs/CharacterCreationMenuDataPrototype.md`

Race 3D art bible notes:

- `Docs/Race3DArtBible.md`

Character creation screen prototype notes:

- `Docs/CharacterCreationScreenPrototype.md`

Tutorial lab handoff prototype notes:

- `Docs/TutorialLabHandoffPrototype.md`

Tutorial lab interaction prototype notes:

- `Docs/TutorialLabInteractionPrototype.md`

Tutorial lab inventory prototype notes:

- `Docs/TutorialLabInventoryPrototype.md`

Tutorial lab equipment prototype notes:

- `Docs/TutorialLabEquipmentPrototype.md`

Tutorial lab combat prototype notes:

- `Docs/TutorialLabCombatPrototype.md`

Tutorial lab map prototype notes:

- `Docs/TutorialLabMapPrototype.md`

Tutorial lab quest prototype notes:

- `Docs/TutorialLabQuestPrototype.md`

Tutorial lab gathering prototype notes:

- `Docs/TutorialLabGatheringPrototype.md`

World gathering resource prototype notes:

- `Docs/GatheringResourcePrototype.md`

Gather Field Supplies quest prototype notes:

- `Docs/GatherFieldSuppliesQuestPrototype.md`

NPC supply delivery prototype notes:

- `Docs/NpcSupplyDeliveryPrototype.md`

NPC supply shortage gate prototype notes:

- `Docs/NpcSupplyShortageGatePrototype.md`

NPC supply trade and use prototype notes:

- `Docs/NpcSupplyTradeUsePrototype.md`

Currency prototype notes:

- `Docs/CurrencyPrototype.md`

Guild contract board prototype notes:

- `Docs/GuildContractBoardPrototype.md`

## V0.1 Definition Of Done

Data Model V0.1 is complete when:

- Every first prototype record has a stable ID.
- The core struct fields are agreed enough to implement.
- The first content list is small enough to build.
- The first playable loop can use data instead of hardcoded lore.

Final V0.1 rule:

- **If a future system needs hundreds or thousands of entries, define the data shape before writing the gameplay logic.**
