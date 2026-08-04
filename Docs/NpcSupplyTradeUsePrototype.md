# NPC Supply Trade And Use Prototype

This layer gives NPC stock its first ordinary trade entry path and its first explicit use path.

## Goal

The first trade and use pass should prove:

1. A player can sell accepted surplus into an NPC supply pocket without waiting for a shortage quest.
2. The sale removes inventory from the player and pays Huwan currency.
3. NPC stock has a first capacity boundary.
4. NPC work can spend stored items and recheck whether shortage pressure returned.

## Surplus Sale

`UHuwamNpcSupplyComponent::PurchaseAcceptedItemFromInventory` purchases one accepted item ID and quantity from a seller inventory.

For the first pass it:

1. Checks that the item is accepted by the supply component.
2. Checks the seller owns the quantity.
3. Checks the NPC supply pocket has capacity.
4. Removes the sold item from inventory.
5. Adds the item into NPC stock.
6. Pays seller copper-native currency through `UHuwamRewardComponent`.

The unit value uses the item's `BaseValueCopper` from item data when available. If that data is missing or has no positive value yet, the component uses its prototype fallback purchase value. Legacy gold item values still have a compatibility conversion path while content moves over.

`AHuwamPrototypePlayerActor::SellAcceptedSupplyItemToNpc` wraps that call for the first player loop and refreshes HUD state after currency changes.

## Stock Use

`UHuwamNpcSupplyComponent::UseStockItemById` removes a stored item quantity for a future NPC need such as:

- Meals
- Repairs
- Crafting inputs
- Worker supplies
- Local care
- Trade fulfillment

`FHuwamNpcSupplyUseResult` reports:

- Which item was used
- The remaining quantity for that item
- The total stock remaining
- Whether the NPC should ask for supply help after the use

That gives AI routines and Blueprint tests a small hook before we wire the full server-time need system.

The first timer-driven bridge now continues in:

- `Docs/NpcSupplyRoutineUsePrototype.md`

## Capacity

The prototype NPC supply pocket now has:

- `StockCapacity`
- `GetRemainingStockCapacity`

The current default capacity is `50` total accepted items. It keeps the first surplus buyer from accepting endless stock while profession storage, buyer wealth, settlement warehouses, and dynamic demand are still future layers.

## Main Files

- `Source/Huwam/Gameplay/HuwamNpcSupplyComponent.h`
- `Source/Huwam/Gameplay/HuwamNpcSupplyComponent.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`
- `Content/Data/DT_Items.csv`

## Useful Blueprint Calls

- `PurchaseAcceptedItemFromInventory`
- `UseStockItemById`
- `UseRoutineSupply`
- `StartRoutineSupplyUse`
- `StopRoutineSupplyUse`
- `GetRoutineUseDelaySeconds`
- `GetPurchaseValueCopper`
- `GetRemainingStockCapacity`
- `SellAcceptedSupplyItemToNpc`

Useful result structs:

- `FHuwamNpcSupplyPurchaseResult`
- `FHuwamNpcSupplyUseResult`

## First Map Test

1. Place `AHuwamPrototypePlayerActor`.
2. Place an NPC actor with `UHuwamNpcSupplyComponent`.
3. Give the player accepted supply material such as `material.wood.basic`.
4. Call `SellAcceptedSupplyItemToNpc` with the item ID and quantity.
5. Confirm player inventory loses the sold quantity.
6. Confirm player copper-native currency increases by item base value times quantity.
7. Confirm NPC stock gains the item and remaining stock capacity falls.
8. Call `UseStockItemById` for part of that NPC stock.
9. Confirm the use result reports the new stock amount and shortage-help state.
10. Configure the first routine timer when you want stock use to repeat on the prototype in-game clock.

## Current Boundary

This pass is deliberately small:

- Accepted item IDs stand in for profession buy lists.
- Copper-native item base value stands in for dynamic market pricing.
- Stock capacity stands in for future storage and buyer limits.
- The NPC buyer does not yet spend its own wallet balance.
- The first routine timer can repeat one configured stock use, but it is not yet a full schedule, AI goal, recipe, hunger, repair, or authoritative server-time system.

The next practical layer is to give that routine-driven stock loss a visible NPC need state.
