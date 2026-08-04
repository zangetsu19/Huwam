# Huwan Unreal Data Import Guide

## Purpose

This guide explains how the starter CSV files in `Content/Data` become Unreal data for gameplay systems.

The project now supports two paths:

- Runtime bootstrap for prototypes: `UHuwamDataSubsystem` creates a transient `UHuwamDataRegistry` and loads missing DataTables directly from `Content/Data/*.csv`.
- Authored asset path for production: imported `.uasset` DataTables can still be assigned to a `DA_HuwamDataRegistry` asset when the content team wants editor-managed tables.

If no registry asset is configured, the runtime bootstrap path is used automatically.

## Import Order

Open `Huwam.uproject`, compile the C++ project, then import each CSV as a DataTable using the matching row struct.

| CSV File | Row Struct |
|---|---|
| `Content/Data/DT_ContentPacks.csv` | `FHuwamContentPackRow` |
| `Content/Data/DT_Races.csv` | `FHuwamRaceRow` |
| `Content/Data/DT_SubRaces.csv` | `FHuwamSubRaceRow` |
| `Content/Data/DT_Classes.csv` | `FHuwamClassRow` |
| `Content/Data/DT_Jobs.csv` | `FHuwamJobRow` |
| `Content/Data/DT_Stats.csv` | `FHuwamStatRow` |
| `Content/Data/DT_Skills.csv` | `FHuwamSkillRow` |
| `Content/Data/DT_Abilities.csv` | `FHuwamAbilityRow` |
| `Content/Data/DT_Items.csv` | `FHuwamItemRow` |
| `Content/Data/DT_Materials.csv` | `FHuwamMaterialRow` |
| `Content/Data/DT_Recipes.csv` | `FHuwamRecipeRow` |
| `Content/Data/DT_Spells.csv` | `FHuwamSpellRow` |
| `Content/Data/DT_Monsters.csv` | `FHuwamMonsterRow` |
| `Content/Data/DT_NPCs.csv` | `FHuwamNpcRow` |
| `Content/Data/DT_Quests.csv` | `FHuwamQuestRow` |

## Recommended Asset Names

Use the same base names for the imported DataTable assets:

- `DT_Races`
- `DT_ContentPacks`
- `DT_SubRaces`
- `DT_Classes`
- `DT_Jobs`
- `DT_Stats`
- `DT_Skills`
- `DT_Abilities`
- `DT_Items`
- `DT_Materials`
- `DT_Recipes`
- `DT_Spells`
- `DT_Monsters`
- `DT_NPCs`
- `DT_Quests`

Recommended destination:

- `/Game/Data`

## Create The Registry Asset

This is now optional for the prototype, but still recommended later for production content workflows. After importing the tables:

1. Create a new Data Asset.
2. Choose `HuwamDataRegistry`.
3. Name it `DA_HuwamDataRegistry`.
4. Place it in `/Game/Data`.
5. Assign every imported DataTable to its matching field.

The registry is the single asset that gameplay systems should use to find Huwan data.

## Use The Runtime Subsystem

The code now includes:

- `UHuwamDataRegistry`
- `UHuwamDataSubsystem`

The subsystem can hold the active registry, expose row lookups to C++ and Blueprints, and auto-load missing tables from `Content/Data`.

Current prototype flow:

1. Get the Game Instance subsystem `HuwamDataSubsystem`.
2. Call `EnsureDataRegistryLoaded` if you need to force the bootstrap.
3. Optionally set `DA_HuwamDataRegistry` as the active registry if authored assets exist.
4. Ask the subsystem for rows such as:
   - Race row
   - Content pack row
   - Class row
   - Job row
   - Item row
   - Spell row
   - Monster row
   - NPC row
   - Quest row

## Smoke Test

After import, test these rows first:

| Table | Row Name |
|---|---|
| `DT_ContentPacks` | `content_pack_starter_core` |
| `DT_Races` | `race_human` |
| `DT_SubRaces` | `subrace_dwarf_dreamforge` |
| `DT_SubRaces` | `subrace_orc_redtusk` |
| `DT_Classes` | `class_progression_zero` |
| `DT_Items` | `item_weapon_basic_sword` |
| `DT_Spells` | `spell_fire_spark` |
| `DT_Monsters` | `monster_slime_basic` |
| `DT_NPCs` | `npc_eldoria_gorth_runkis` |
| `DT_Quests` | `quest_eldoria_slimes_in_tall_grass` |

If these load, the first Huwan data bridge is alive.

The editor validation runner now checks these rows before it plays through the tutorial and Eldoria starter field loop.

## Common Import Issues

- If Unreal cannot find a row struct, compile the project first.
- If enum fields fail, confirm the CSV value matches the enum entry name, such as `Common`, `Deep`, `F`, `Fire`, or `Evocation`.
- If a row field imports blank, check the CSV column name against the C++ property name.
- Omitted optional columns use C++ default values during runtime bootstrap.
- If array fields are needed later, add the matching CSV columns after the first simple table imports are stable.
- If a table imports but lookups fail, confirm the row name in the first CSV column matches the requested row name.

## Next Code Step

After the DataTables and registry asset exist, the next code step is a small debug actor or UI widget that:

1. Reads the active `UHuwamDataSubsystem`.
2. Loads a known row from each table.
3. Prints the display names on screen.

That smoke test actor now exists:

- `Source/Huwam/Data/HuwamDataSmokeTestActor.h`
- `Source/Huwam/Data/HuwamDataSmokeTestActor.cpp`

Use it by placing `HuwamDataSmokeTestActor` in a test map and assigning `DA_HuwamDataRegistry` to its `DataRegistry` field. When the map starts, the actor will try to load known starter rows and write the results to the Unreal log.

If no registry is assigned, the smoke test actor now uses the active runtime registry from `UHuwamDataSubsystem`.

That will prove the pipeline from world bible to CSV to Unreal data to gameplay code.
