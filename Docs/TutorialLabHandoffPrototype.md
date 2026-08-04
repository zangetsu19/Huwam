# Tutorial Lab Handoff Prototype

This layer carries a confirmed character out of character creation and into Laucian's first tutorial state.

## Goal

The player should not leave character creation and fall straight into Eldoria systems.

`UHuwamTutorialLabComponent` now starts the first white-lab flow:

1. Start the Progression Zero tutorial quest.
2. Move the prototype player or supplied playable avatar to a placed lab entry marker when one exists.
3. Expose the first Movement tutorial beat.
4. Advance to the first HUD tutorial beat.
5. Expose tutorial state to HUD/menu data.

The lab room itself is still a future map blockout. The runtime can begin even before that room exists.

## Main Files

- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.h`
- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.cpp`
- `Source/Huwam/Gameplay/HuwamTutorialLabEntryActor.h`
- `Source/Huwam/Gameplay/HuwamTutorialLabEntryActor.cpp`

## Runtime Pieces

| Piece | Purpose |
|---|---|
| `UHuwamTutorialLabComponent` | Stores the current Laucian tutorial beat. |
| `AHuwamTutorialLabEntryActor` | Placeable entry marker for the white lab. |
| `FHuwamTutorialLabSnapshot` | Blueprint-friendly state for tutorial UI and HUD. |
| `EHuwamTutorialLabBeat` | First beat ladder: Movement, HUD Basics, Interaction Ready, Inventory Basics, Equipment Basics, Combat Basics, Map Basics, Quest Basics, Gathering Basics, Complete. |

## Character Creation Handoff

`AHuwamPrototypePlayerActor::InitializePrototypePlayerFromCharacterCreation` now begins the tutorial lab by default.

When that path is used:

- The confirmed character is applied.
- Starter prototype systems initialize.
- The first Eldoria slime quest is held back.
- `quest.tutorial.progression_zero` starts instead.
- The tutorial lab snapshot becomes available to HUD data.
- A linked third-person prototype avatar is moved to the lab entry marker while the state actor keeps the tutorial component.

The debug auto-initialize path still supports the existing slime quest loop.

## First Lab Beats

| Beat | Instruction |
|---|---|
| Movement | Move and look around Laucian's white lab. |
| HUD Basics | Inspect health, mana, quest, and inventory signals. |
| Interaction Ready | Movement and HUD basics are done, so the first physical tutorial object can take over. |
| Inventory Basics | The first interaction is complete and starter inventory inspection can begin. |
| Equipment Basics | The focused inventory item was inspected and equipped gear can be taught next. |
| Combat Basics | The focused equipment slot was inspected and the first combat lesson can begin. |
| Map Basics | The lab target was struck and world navigation can be taught next. |
| Quest Basics | The full map was opened and quest tracking can be taught next. |
| Gathering Basics | The tutorial quest is tracked and a first pickup or gathering task can begin. |

Blueprint or a later player controller layer can call:

- `MarkMovementLessonComplete`
- `MarkHudLessonComplete`
- `MarkInteractionLessonComplete`
- `MarkInventoryLessonComplete`
- `MarkEquipmentLessonComplete`
- `MarkCombatLessonComplete`
- `MarkMapLessonComplete`
- `MarkQuestLessonComplete`
- `MarkGatheringLessonComplete`
- `CompleteLaucianTutorialLab`

Completing the tutorial lab advances the tutorial quest objective once. The full quest completion and Eldoria exit flow will grow with later lab objects.

The first third-person pawn layer now advances the opening beats without debug calls:

- Walking a few steps while looking around completes Movement.
- Pressing `Tab` during HUD Basics acknowledges that lesson.
- `UHuwamTutorialLabPromptWidget` keeps Laucian's beat title, instruction, and line visible while the beat changes.

## First Map Test

1. Place `AHuwamPrototypePlayerActor` in a test map, or use `AHuwamPrototypeGameMode` to spawn one for the third-person bootstrap.
2. Enable `bShowCharacterCreationScreenOnBeginPlay` on a placed state actor, or let the game mode configure its spawned state actor.
3. Place `AHuwamTutorialLabEntryActor` where the white lab arrival should happen.
4. Begin play and confirm a character.
5. Confirm that the player actor or linked third-person avatar moves to the entry marker.
6. Read `GetTutorialLabSnapshot` or `RefreshPrototypeHud`.
7. Confirm the current beat is Movement.
8. With the linked third-person pawn, move and look around until the HUD beat becomes active.
9. Press `Tab`.
10. Confirm Interaction Ready becomes active.

If no lab entry actor exists yet, the tutorial state still begins at the prototype player's current transform.

For the first clean moving startup route, see:

- `Docs/ThirdPersonBootstrapPrototype.md`

## Quest And HUD Support

`UHuwamQuestComponent` now has a fallback for `quest.tutorial.progression_zero`, so the first lab quest can start before imported quest DataTables are linked.

`FHuwamHudSnapshot` now includes:

- `TutorialLab`
- `bHasTutorialLab`

That gives a future HUD or tutorial prompt widget one clean place to read Laucian's current beat.

## Interaction Lesson

The first placeable lab object now exists:

- `AHuwamTutorialLabInteractableActor`

See:

- `Docs/TutorialLabPromptPrototype.md`
- `Docs/TutorialLabInteractionPrototype.md`
- `Docs/TutorialLabInventoryPrototype.md`
- `Docs/TutorialLabEquipmentPrototype.md`
- `Docs/TutorialLabCombatPrototype.md`
- `Docs/TutorialLabMapPrototype.md`
- `Docs/TutorialLabQuestPrototype.md`
- `Docs/TutorialLabGatheringPrototype.md`

## Gathering Lesson

The first placeable lab gatherable now exists:

- `AHuwamTutorialLabGatheringActor`

It adds tutorial lavender to inventory through the real inventory component and closes the first tutorial objective after quest tracking reaches `Gathering Basics`.

See:

- `Docs/TutorialLabGatheringPrototype.md`

## Combat Lesson

The first placeable lab combat target now exists:

- `AHuwamTutorialLabCombatTargetActor`

It uses the real combat component but stays separate from monster rewards and Eldoria quest credit.

See:

- `Docs/TutorialLabCombatPrototype.md`

## Next Step

The next player-facing tutorial layer is a first inventory and equipment lesson surface after the training pedestal interaction.
