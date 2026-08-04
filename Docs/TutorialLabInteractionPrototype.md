# Tutorial Lab Interaction Prototype

This layer gives Laucian's lab its first physical lesson.

## Goal

After movement and HUD basics, the player needs one safe object that proves the interact button does something.

`AHuwamTutorialLabInteractableActor` is the first placeable lab object for that lesson.

It:

- Exposes an interaction prompt.
- Only accepts interaction while the lab is at `Interaction Ready`.
- Can be used once by default.
- Advances the tutorial lab into `Inventory Basics`.
- Refreshes the prototype HUD snapshot after the interaction.

## Main Files

- `Source/Huwam/Gameplay/HuwamTutorialLabInteractableActor.h`
- `Source/Huwam/Gameplay/HuwamTutorialLabInteractableActor.cpp`

The interaction advances state through:

- `UHuwamTutorialLabComponent::MarkInteractionLessonComplete`

## New Lab Beat

The tutorial ladder now includes:

| Beat | Purpose |
|---|---|
| Interaction Ready | Tell the player to use the training object. |
| Inventory Basics | Tell the player to inspect the focused starter item still in inventory. |

The interaction snapshot records:

- `bInteractionLessonComplete`
- `LastInteractionId`

That gives future tutorial UI enough context to show that the first object was used.

## First Map Test

1. Place `AHuwamPrototypePlayerActor` in a test map.
2. Enable character creation on begin play.
3. Place `AHuwamTutorialLabEntryActor`.
4. Place `AHuwamTutorialLabInteractableActor` near the lab entry point.
5. Confirm a character.
6. Advance the lab through `MarkMovementLessonComplete`.
7. Advance the lab through `MarkHudLessonComplete`.
8. Call `InteractWithPrototypePlayer` on the placed interactable using the prototype player, or focus it through the prototype player's world interaction component and route `InteractWithFocusedWorldTarget`.
9. Confirm `GetTutorialLabSnapshot().CurrentBeat` is `Inventory Basics`.

The first world interaction component now calls the same actor function for the shared prototype focus route. The later third-person interact input can bind to that route.

## Blueprint Hooks

Useful actor functions:

- `CanInteractWithPrototypePlayer`
- `InteractWithPrototypePlayer`
- `GetInteractionPrompt`
- `HasBeenInteractedWith`

Useful event:

- `OnTutorialLabInteraction`

The actor can later gain a mesh, pedestal Blueprint, highlight logic, and input prompt styling without changing the tutorial-state rule.

The shared route continues in:

- `Docs/WorldInteractionPrototype.md`

## Next Step

Inventory training now continues in:

- `Docs/TutorialLabInventoryPrototype.md`
