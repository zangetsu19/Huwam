# Tutorial Lab Prompt Prototype

This layer puts Laucian's current white-lab lesson on screen during the first third-person handoff.

## Goal

The tutorial state already had a beat ladder. The player now needs a visible guide that follows it while early lessons begin advancing from real pawn actions.

## Runtime Pieces

| Piece | Purpose |
|---|---|
| `UHuwamTutorialLabPromptWidget` | Native first tutorial prompt for title, instruction, and Laucian line. |
| `AHuwamPrototypePlayerActor` | Creates and positions the prompt when the Laucian lab begins. |
| `AHuwamPrototypeThirdPersonCharacter` | Completes the first movement beat from walking plus camera look input, then acknowledges the HUD beat from the pawn. |

## First Prompt Data

The widget listens to `UHuwamTutorialLabComponent::OnTutorialLabChanged`.

It displays:

- `BeatTitle`
- `InstructionText`
- `LaucianLine`

That keeps Laucian's voice and the tutorial objective sourced from the same `FHuwamTutorialLabSnapshot` already available to HUD/menu data.

## First Action Route

The opening tutorial beats now move like this:

| Beat | Real Prototype Action |
|---|---|
| Movement | Walk roughly a few steps and move the third-person camera. |
| HUD Basics | Read the visible lesson and press `Tab`. |
| Interaction Ready | Aim at the placed training pedestal and press `E`. |

The movement thresholds are editable on `AHuwamPrototypeThirdPersonCharacter`:

- `TutorialMovementRequiredDistance`
- `TutorialLookInputRequiredAmount`

The `Tab` acknowledgment is a first pawn route for the HUD lesson, not the final remappable input scheme.

## Main Files

- `Source/Huwam/UI/HuwamTutorialLabPromptWidget.h`
- `Source/Huwam/UI/HuwamTutorialLabPromptWidget.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypeThirdPersonCharacter.h`
- `Source/Huwam/Gameplay/HuwamPrototypeThirdPersonCharacter.cpp`

## First Map Test

1. Use the third-person bootstrap path and place `AHuwamTutorialLabEntryActor`.
2. Confirm a new character.
3. Confirm the tutorial prompt shows Laucian's Movement beat.
4. Walk around the white lab while looking with the mouse.
5. Confirm the prompt changes to HUD Basics.
6. Press `Tab`.
7. Confirm the prompt changes to Interaction Ready and the placed tutorial pedestal can take the next `E` interaction.

## Current Boundary

The first visible tutorial prompt is live, and the first two lab beats can now move through actual third-person input.

The first pack/equipment, map, quest, attack, gathering, and Eldoria handoff surfaces now continue in:

- `Docs/PlayableVerticalSlicePrototype.md`
