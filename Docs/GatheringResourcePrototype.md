# Gathering Resource Prototype

This layer starts Huwan's first reusable F-rank field resource node.

## Goal

World gathering needs to move beyond Laucian's tutorial prop.

`AHuwamGatheringResourceActor` is the first Blueprint-placeable field resource that can:

- Load an F-rank material row by stable material ID.
- Grant the material's item ID into inventory.
- Require an appropriate gathering tool.
- Deplete after harvest.
- Respawn on the material's in-game-day rule.
- Feed matching active quest objectives through progress-source IDs.

## Settlement Sustainment Rule

This actor is the player-facing start of gathering, not the whole supply model.

Cities, towns, villages, and kingdoms must eventually be generated with enough farms, resource nodes, civic stockpiles, NPC labor, and supply routes to sustain their ordinary population without player harvesting. Player gathering should change surplus, scarcity, prices, recovery, guild demand, and crisis outcomes rather than acting as the only thing keeping a settlement alive.

That means later settlement resource code should separate:

- Public or player-facing field nodes.
- NPC production and managed civic resources.
- Stockpiles and trade-route supply.
- Pressure from monsters, weather, war, corruption, overuse, and player decisions.

It should also support player-built resource nodes. A skilled player with legal access, the correct tools, inputs, and work time should be able to create new local production. For agriculture, a watermelon field is the example rule:

- Hoe
- Seeds
- Fertilizer
- Water
- Farming skill and establishment time

After roughly `2` in-game days, a valid new field can mature into a self-sufficient resource node that farmers, other NPC workers, and players recognize as part of the local harvest loop.

## First Node

The default field node starts as lavender:

- Material ID: `material.herb.lavender`
- Item ID: `material.herb.lavender`
- Default tool while the CSV material row has no tool list: `item.tool.gathering_knife`
- Material data respawn rule: `5` in-game days

The actor keeps a fallback if the material DataTable is not connected yet, then adopts material ID, item ID, display name, rank, harvest tool IDs, and respawn days once the material row loads.

## Time Rule

The current world bible rule is:

- `24` real hours equals `7` in-game days.

The actor now defaults to shared Huwam time through `UHuwamTimeSubsystem`.

It still exposes `SecondsPerInGameDay` as a fallback, and computes `GetRespawnDelaySeconds` from `RespawnInGameDays`.

That gives plants such as lavender a respawn-ready runtime path on the same clock NPC routines can use.

## Quest Bridge

The actor does not know one fixed quest. It checks active quest objectives and advances those whose progress source matches:

- One of the actor's `QuestProgressSourceIds`
- The gathered material ID
- The gathered item ID

The default prototype progress source includes:

- `material.field_supply`

That means the existing `Gather Field Supplies` quest data can count early field harvests once the quest is active.

## Main Files

- `Source/Huwam/Gameplay/HuwamGatheringResourceActor.h`
- `Source/Huwam/Gameplay/HuwamGatheringResourceActor.cpp`
- `Content/Data/DT_Materials.csv`
- `Content/Data/DT_Quests.csv`

## Useful Blueprint Calls

- `InitializeFromMaterialData`
- `CanGatherWithPrototypePlayer`
- `GatherWithPrototypePlayer`
- `RespawnResource`
- `IsAvailable`
- `GetRespawnDelaySeconds`

Useful events:

- `OnResourceGathered`
- `OnResourceRespawned`

## First Map Test

1. Place `AHuwamPrototypePlayerActor` in a non-tutorial test map.
2. Confirm the player inventory contains `item.tool.gathering_knife`.
3. Place `AHuwamGatheringResourceActor` near the player.
4. Confirm its default material is lavender.
5. Call `GatherWithPrototypePlayer`, or focus the node with the prototype player's world interaction component and route `InteractWithFocusedWorldTarget`.
6. Confirm inventory gains `material.herb.lavender`.
7. Confirm the node reports unavailable after harvest and its blockout mesh leaves focus while depleted.
8. Either wait for its configured timer or call `RespawnResource`.
9. Confirm the node becomes available again.

## Next Step

The first Eldoria gathering quest path now continues in:

- `Docs/GatherFieldSuppliesQuestPrototype.md`

The current blockout node hides its mesh and collision while depleted, returns them on respawn, and uses named gather prompts such as `Gather Lavender` when display data is available.

Later resource-node work should distinguish seeded natural nodes, civic/NPC-managed nodes, and player-built nodes that mature into settlement production.

The first shared player focus route now continues in:

- `Docs/WorldInteractionPrototype.md`
