# World Interaction Prototype

This layer gives the prototype player one first route into nearby world objects.

## Goal

The first world interaction pass should prove:

1. The player can hold one focus snapshot for a nearby supported actor.
2. HUD or Blueprint can read a prompt without knowing each target actor type.
3. One interaction call can open the Eldoria NPC supply request screen, use the white-lab interaction object, or gather a field resource.

This is still a prototype player route, not the final MMORPG input system.

## Runtime Piece

`UHuwamWorldInteractionComponent` lives on `AHuwamPrototypePlayerActor`.

It currently supports:

| Focus Kind | Routed Action |
|---|---|
| `NpcSupplyQuestRequester` | Open the player-owned `UHuwamNpcSupplyQuestInteractionWidget` for the focused requester. |
| `TutorialLabObject` | Use `AHuwamTutorialLabInteractableActor` and advance the first interaction lesson when allowed. |
| `GatheringResource` | Gather from `AHuwamGatheringResourceActor` when the resource, tool, and inventory rules allow it. |

## Focus Snapshot

`FHuwamWorldInteractionSnapshot` carries:

- Whether a target exists.
- Whether the player can currently interact.
- Interaction kind.
- Focused actor reference.
- Prompt text.
- Current target distance.

The component can:

- Search for the nearest supported target inside its prototype focus radius.
- Accept an explicit target from Blueprint with `SetInteractionFocusActor`.
- Clear the current focus snapshot.

The explicit-target path is useful for a later third-person camera trace. The nearby-search path keeps a test map useful while the final camera/input shell is still forming.

## Interaction Result

`FHuwamWorldInteractionResult` carries:

- Success flag.
- Routed outcome.
- Snapshot before the action.
- Snapshot after the action.

Current outcomes are:

- NPC supply quest screen opened.
- Tutorial interaction completed.
- Resource gathered.
- Unavailable.

## Player Hooks

`AHuwamPrototypePlayerActor` now exposes:

| Function | Purpose |
|---|---|
| `RefreshWorldInteractionFocus` | Refreshes the world focus snapshot and HUD data. |
| `InteractWithFocusedWorldTarget` | Routes the focused first-world action. |
| `ShowNpcSupplyQuestInteractionScreen` | Opens the Eldoria supply-request screen for a chosen requester actor. |
| `HideNpcSupplyQuestInteractionScreen` | Closes that screen and can restore game input. |

The NPC supply request screen now has a native close button too.

## HUD Bridge

`FHuwamHudSnapshot` now includes:

- `WorldInteraction`
- `bHasWorldInteraction`

That gives the next HUD prompt layer access to the current text and target state without making it cast against every supported actor itself.

## Main Files

- `Source/Huwam/Gameplay/HuwamWorldInteractionComponent.h`
- `Source/Huwam/Gameplay/HuwamWorldInteractionComponent.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`
- `Source/Huwam/Gameplay/HuwamHudDataComponent.h`
- `Source/Huwam/Gameplay/HuwamHudDataComponent.cpp`

## First Map Test

1. Place `AHuwamPrototypePlayerActor` in a non-tutorial test map.
2. Place one `AHuwamNpcSupplyQuestOfferActor` close enough for the prototype focus radius.
3. Call `RefreshWorldInteractionFocus`.
4. Confirm HUD data reports an NPC supply request focus and prompt.
5. Call `InteractWithFocusedWorldTarget`.
6. Confirm the NPC supply request screen opens for that requester.
7. Close the request screen from its native close button.
8. Repeat with `AHuwamGatheringResourceActor` and confirm the routed interaction gathers when the player has the required tool.
9. In the white lab, repeat with `AHuwamTutorialLabInteractableActor` when the tutorial beat is `Interaction Ready`.
10. Confirm the routed tutorial interaction advances the beat to `Inventory Basics`.

## Current Boundary

The prototype player can now act on first supported world targets from one focus route.

It does not yet:

- Generalize line-of-sight, overlap priority, and focus rules beyond the first camera shell.
- Generalize every future NPC, harvestable, door, container, and contract board behind one final interface.
- Replicate authoritative interaction choice across a live server.

The first native prompt and camera-driven third-person shell now continue in:

- `Docs/WorldInteractionPromptPrototype.md`
- `Docs/ThirdPersonPrototypeCharacter.md`
