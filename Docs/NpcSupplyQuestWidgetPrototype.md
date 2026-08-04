# NPC Supply Quest Widget Prototype

This layer puts the first Eldoria supply request route on screen.

## Goal

The first request widget should prove:

1. A small native Unreal widget can read the NPC supply interaction snapshot directly.
2. The same surface can preview a request, accept it, show waiting progress, and turn it in after completion.
3. Blueprint can subclass the widget later without rebuilding the quest-state routing rules in UI.
4. The same requester surface can buy a first unit of accepted player surplus without waiting for a shortage quest.

## Runtime Piece

`UHuwamNpcSupplyQuestInteractionWidget` is a native Unreal widget backed by `AHuwamNpcSupplyQuestOfferActor`.

The widget shows:

- The current interaction prompt for the placed requester.
- The current quest or supply need title.
- The live supply reason.
- The offered or active objective, including active quest progress.
- The current copper and experience reward preview when an offer exists.
- The last routed result after preview, acceptance, waiting, satisfied, or delivery actions.

The first native panel keeps two interaction choices:

| Button | Purpose |
|---|---|
| Primary routed action | Preview a request, check delivery progress, deliver supplies, or check satisfied stock based on current state. |
| Accept Request | Accept the available offer while the requester is in `OfferingWork`. |
| Sell 1 Surplus | Sell the first accepted field-supply stack found in the player's inventory into requester stock. |
| Close | Return from the focused request screen and restore game input when player-owned. |

The surplus sale button pays copper using the same NPC supply purchase path used by the lower-level trade prototype and reports the sold item, payout, and stock total in the result text.

## Source Linking

The widget can be linked explicitly with:

- `SetInteractionSource`

For the current prototype map pass, it can also find:

- The first `AHuwamPrototypePlayerActor` in the world.
- The first `AHuwamNpcSupplyQuestOfferActor` in the world.

That fallback keeps the panel testable, while `AHuwamPrototypePlayerActor::ShowNpcSupplyQuestInteractionScreen` now links the widget to a focused requester through the first world interaction route.

## Blueprint Hooks

Useful events:

- `OnInteractionSnapshotChanged`
- `OnInteractionRouted`

Useful callable actions:

- `RefreshInteraction`
- `RouteInteraction`
- `SetInteractionSource`

Useful state reads:

- `GetCurrentInteractionSnapshot`
- `GetLastInteractionResult`
- `GetPrototypePlayer`
- `GetQuestRequester`

## Main Files

- `Source/Huwam/UI/HuwamNpcSupplyQuestInteractionWidget.h`
- `Source/Huwam/UI/HuwamNpcSupplyQuestInteractionWidget.cpp`

## First Map Test

1. Place `AHuwamPrototypePlayerActor` and `AHuwamNpcSupplyQuestOfferActor` in a non-tutorial test map.
2. Add `UHuwamNpcSupplyQuestInteractionWidget` to the viewport from a test Blueprint or Widget Blueprint subclass, or open it through the prototype player's focused world interaction route.
3. Confirm the panel shows the placed requester's field-supply shortage prompt.
4. Use the routed action to preview the request.
5. Use `Accept Request` and confirm the objective becomes active progress.
6. Gather enough accepted field supplies to finish the quest objective.
7. Return to the widget and use the routed action to deliver the supplies.
8. Confirm the result text changes after delivery and the requester snapshot becomes satisfied.
9. Gather or keep one accepted surplus item, then use `Sell 1 Surplus`.
10. Confirm player inventory falls by one, wallet copper rises, and requester stock rises.

## Current Boundary

The quest route has its first screen surface now.

It does not yet:

- Bind open behavior to the final third-person interact input and camera trace.
- Offer a final conversation layout, NPC portrait, or voiced response layer.
- Choose among several requester actors or several quest offers.
- Choose the exact item or quantity for a sale yet.
- Refresh from multiplayer replication or shared server shortage ownership.

The first shared player focus route now lives in `UHuwamWorldInteractionComponent`.

See `Docs/WorldInteractionPrototype.md`.

The next practical layer is to show the focus prompt on HUD and bind it to the first third-person interact input shell.
