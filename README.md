# Huwam: The First Adventure Life

This workspace contains a starter Unreal Engine C++ project scaffold with custom AI engine modules.

## What's included

- `Huwam.uproject` — Unreal project file
- `Source/Huwam` — C++ game module
- `Source/Huwam/AI` — custom AI engine interfaces and sample systems
- `Source/Huwam/AI/AIOverseer.*` — master AI engine that controls freewill, weather, economy, lifecycle, and moral alignment

## Engine architecture

- `AIOverseer` — authoritative manager that coordinates other AI engines and applies world events in real time
- `FreewillEngine` — drives NPC choice and behavior influence
- `WeatherEngine` — simulates dynamic weather and climate
- `EconomyEngine` — manages wealth, inflation, and rebuild tasks after player or NPC actions
- `LifecycleEngine` — controls NPC birth, aging, death, and life stages
- `AlignmentEngine` — applies Dungeons & Dragons moral alignment rules to NPC decision-making
- `KingdomEngine` — oversees agriculture, buildings, terrain, population density, weather effects, trade economics, and skill distribution
- `GovernorEngine` — manages quests, inventories, farmable areas, HUD systems, and leaderboards
- `NPCManagerEngine` — handles NPC generation with race, sub-race, jobs, ambitions, and attributes based on fantasy lore. Includes comprehensive job classes inspired by D&D/Pathfinder with stats, skills, inventory, wealth, alignments, gear, and difficulty ratings.

## Next steps

1. Install Unreal Engine locally (matching the target version written in the `.uproject`).
2. Open `Huwam.uproject` in Unreal Editor.
3. Implement gameplay logic, AI behavior, and asset content.

## NPC Job System

The NPC system includes a comprehensive job class system with the following features:

### Job Classes
- **D&D Inspired**: Thief, Sorcerer, Wizard, Warlock, Barbarian, Fighter, Cleric, Paladin, Assassin, Mage, Priest, Knight, Guard, Spy
- **Homebrew Classes**: Inventor, Farmer, Blacksmith, Demonlord, Hero, AntiHero, Villain, Criminal, Adventurer, SlowLife, Nobility, Tamer, Archer, Construction, Pirate, SeaCaptain, Breeder, ShopKeeper, Merchant, Waitress

### Job Attributes
Each job includes:
- **Stats**: Strength, Dexterity, Constitution, Intelligence, Wisdom, Charisma (D&D style)
- **Skills**: Combat, Magic, Survival, Crafting, Farming, Leadership, Diplomacy, Knowledge
- **Inventory**: Starting materials and items
- **Wealth**: Base gold amount
- **Alignment**: Typical moral alignment (Lawful/Chaotic Good/Neutral/Evil)
- **Gear**: Equipment and tools
- **Difficulty**: Challenge rating (1-8 scale)

### Player Job Progression
Players always begin as **Newbie** and must complete quests to build relevant skill experience before graduating to selected jobs.
- Players can choose up to **3 jobs at a time**.
- Quests grant progression in job-related skill fields.
- Once a selected job reaches the required skill threshold, the player unlocks and can transition from Newbie into that job.
- Player HUD state tracks selected jobs, job progress, current job, and personal skill levels.

### Race-Job Compatibility
Jobs are assigned based on racial tendencies:
- Elves: Magic-focused (Wizard, Sorcerer, Archer)
- Dwarves: Crafting and combat (Blacksmith, Fighter, Barbarian)
- Humans: Versatile (Farmer, Blacksmith, Thief, Fighter)
- Demons: Dark magic (Warlock, Sorcerer, Demonlord)
- Angels: Divine (Cleric, Paladin, Hero)
- Virus (cursed): Stealthy evil (Assassin, Warlock, Villain)
