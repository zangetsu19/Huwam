# World Interaction Prompt Prototype

This layer makes the first world focus route visible and usable from a prototype key press.

## Goal

The first prompt pass should prove:

1. A small HUD widget can show the active interaction prompt without owning target logic.
2. Nearby focus can refresh while the prototype is running.
3. The current prototype actor can route one real key press into the same NPC, tutorial, and gathering interaction path.

## Prompt Widget

`UHuwamWorldInteractionPromptWidget` is a native HUD prompt.

It:

- Reads `FHuwamWorldInteractionSnapshot`.
- Listens to `UHuwamWorldInteractionComponent::OnInteractionFocusChanged`.
- Shows only when a focused target is currently interactable and has prompt text.
- Stays non-blocking on the HUD so world input can continue around it.

The first native prompt shows the target-provided prompt text. Final HUD styling can replace it with a richer Widget Blueprint later.

## Focus Refresh

`UHuwamWorldInteractionComponent` now refreshes its prototype focus on a short interval by default.

That keeps nearby prompt state alive while:

- A requester becomes available or satisfied.
- A gathering node depletes or respawns.
- The player moves into or out of the first focus radius.

The interval remains configurable for later performance and targeting work.

## Prototype Input Hook

`AHuwamPrototypePlayerActor` now has a first interaction key bridge:

| Hook | Purpose |
|---|---|
| `EnablePrototypeWorldInteractionInput` | Enables actor input through the current player controller and binds the prototype interaction key. |
| `PrototypeWorldInteractKey` | Defaults to `E` for the first test shell. |
| `HandlePrototypeWorldInteractInput` | Routes into `InteractWithFocusedWorldTarget`. |

This is intentionally attached to the current prototype actor because the project does not yet have a real third-person pawn/controller shell.

## Player Prompt Hooks

`AHuwamPrototypePlayerActor` also exposes:

| Function | Purpose |
|---|---|
| `ShowWorldInteractionPromptScreen` | Creates and shows the player-owned prompt screen. |
| `HideWorldInteractionPromptScreen` | Removes the prompt screen. |
| `GetWorldInteractionPromptScreen` | Returns the current native prompt widget if it exists. |

The prompt is shown automatically after prototype system initialization by default.

## Main Files

- `Source/Huwam/UI/HuwamWorldInteractionPromptWidget.h`
- `Source/Huwam/UI/HuwamWorldInteractionPromptWidget.cpp`
- `Source/Huwam/Gameplay/HuwamWorldInteractionComponent.h`
- `Source/Huwam/Gameplay/HuwamWorldInteractionComponent.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`

## First Map Test

1. Place `AHuwamPrototypePlayerActor` in a test map.
2. Leave `bShowWorldInteractionPromptOnInitialize` and `bEnablePrototypeWorldInteractionInputOnBeginPlay` enabled.
3. Place an `AHuwamNpcSupplyQuestOfferActor` inside the prototype focus radius.
4. Begin play and confirm the requester prompt appears when the requester can interact.
5. Press `E`.
6. Confirm the NPC supply request screen opens.
7. Close it, then repeat around a field gathering node.
8. Confirm `E` gathers the node when the required tool is available.
9. In the white lab, repeat near the tutorial interaction object when the lesson is ready.
10. Confirm the same key route advances the tutorial interaction beat.

## Current Boundary

The interaction route is visible and has a first usable key bridge.

It does not yet:

- Display controller or remappable-input hints.
- Provide final crosshair, outline, occlusion, priority, or line-of-sight rules.

The first moving camera shell now continues in:

- `Docs/ThirdPersonPrototypeCharacter.md`

The next practical layer is to bootstrap character creation and Laucian's tutorial lab around that character-plus-state pair.
