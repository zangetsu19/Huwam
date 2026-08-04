# NPC Supply Delivery Prototype

This layer lets the first Eldoria gathering quest hand materials into an NPC's stock instead of treating gathered progress as reward-only bookkeeping.

## Goal

The first NPC supply bridge should prove:

1. A quest turn-in can require a matching recipient NPC.
2. Accepted materials can leave the player inventory.
3. Those materials can enter recipient stock.
4. The recipient can report low stock and a satisfied stock target for the shortage-gated request layer.

## Runtime Piece

`UHuwamNpcSupplyComponent` is a Blueprint-spawnable actor component for a prototype NPC supply pocket.

The default recipient is:

- `npc.eldoria.poor_resident_01`

The default accepted early field supplies are:

- Basic wood
- Basic stone
- Flax
- Grass fiber
- Lavender
- Mint
- Rosemary

The default low-stock threshold is `4`.

The default supply target is `10`, matching the first `Gather Field Supplies` objective.

## Turn-In Bridge

`AHuwamPrototypePlayerActor::TurnInGatherFieldSuppliesQuest` now takes a `UHuwamNpcSupplyComponent`.

The helper:

1. Refreshes current gather progress.
2. Confirms the gather quest is ready to turn in.
3. Confirms the quest giver and recipient NPC IDs match when both IDs exist.
4. Delivers the required quantity of accepted inventory materials into NPC stock.
5. Turns in the quest and grants the current reward.

`FHuwamNpcSupplyDeliveryResult` exposes the delivery summary for UI and Blueprint:

- NPC ID
- Delivered item IDs and quantities
- Total quantity delivered
- Recipient stock quantity after delivery
- Whether the current supply target was satisfied

## Why It Matters

This keeps Huwan's early quests grounded in the world state:

- Field gathering gives the player actual materials.
- A request can move those materials into NPC stock.
- Ordinary trade should later move useful surplus into essential NPC stock before a shortage request becomes urgent.
- Later quest generation can ask whether a shortage still exists before offering another version of the request.

The first shortage gate now uses this stock state before the field-supply request starts.

## Main Files

- `Source/Huwam/Gameplay/HuwamNpcSupplyComponent.h`
- `Source/Huwam/Gameplay/HuwamNpcSupplyComponent.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`
- `Docs/GatherFieldSuppliesQuestPrototype.md`

## Useful Blueprint Calls

- `DeliverAcceptedItemsFromInventory`
- `AddStockItemById`
- `AcceptsSupplyItem`
- `GetAcceptedInventoryQuantity`
- `GetStockItems`
- `GetTotalStockQuantity`
- `IsSupplyLow`
- `IsSupplyNeedSatisfied`
- `ShouldRequestSupplyHelp`

Useful event:

- `OnNpcSupplyChanged`

## First Map Test

1. Place `AHuwamPrototypePlayerActor`.
2. Place a prototype NPC actor with `UHuwamNpcSupplyComponent`.
3. Keep the supply component NPC ID at `npc.eldoria.poor_resident_01`.
4. Start `Gather Field Supplies` through the supply-gated quest path.
5. Gather 10 accepted field materials.
6. Call `TurnInGatherFieldSuppliesQuest` with the NPC supply component.
7. Confirm the player inventory loses 10 accepted supply items.
8. Confirm NPC stock gains the delivered item quantities.
9. Confirm the delivery result reports the stock target satisfied.

## Current Boundary

This layer stores supply locally on one NPC component. The first shortage gate continues in:

- `Docs/NpcSupplyShortageGatePrototype.md`

This supply component does not yet:

- Roll stock up into settlement-level inventories.
- Buy with full profession, buyer-wallet, market-price, and settlement-stock rules.
- Explain time-driven stock loss through named NPC needs and AI decisions.
- Persist NPC supply through server saves.

The first stock sale and use helpers continue in:

- `Docs/NpcSupplyTradeUsePrototype.md`
- `Docs/NpcSupplyRoutineUsePrototype.md`
