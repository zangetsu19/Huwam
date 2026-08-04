# Third-Person Bootstrap Prototype

This layer turns the third-person character and gameplay state actor into one first playable startup path.

## Goal

The first bootstrap should prove:

1. A prototype game mode can provide the moving third-person pawn shell.
2. Gameplay state can be spawned as a separate actor prepared for character creation.
3. A confirmed character enters Laucian's white lab through the moving avatar body, not through the hidden state anchor.

## Runtime Pieces

| Piece | Purpose |
|---|---|
| `AHuwamPrototypeGameMode` | Uses the third-person prototype character as the first default pawn and bootstraps the gameplay state actor. |
| `AHuwamPrototypeThirdPersonCharacter` | Owns movement, camera, camera focus, and pawn interaction input. |
| `AHuwamPrototypePlayerActor` | Owns character creation, inventory, equipment, rewards, quests, tutorial state, HUD adapters, and interaction screens. |
| `UHuwamTutorialLabComponent` | Starts the Progression Zero tutorial and can move a supplied arrival actor to the lab entry marker. |

## Bootstrap Flow

`AHuwamPrototypeGameMode::BootstrapFirstPlayablePrototype` now:

1. Finds the first possessed `AHuwamPrototypeThirdPersonCharacter` when available.
2. Finds an existing `AHuwamPrototypePlayerActor` if the map already owns one.
3. Spawns a gameplay state actor when one is missing.
4. Configures the spawned state actor for character creation instead of debug auto-initialize.
5. Links the third-person character to the state actor.

When the spawned state actor is configured for the bootstrap:

- Debug auto-initialize is held back.
- Character creation opens at begin play.
- The old logic-actor interaction key bridge is held back.
- Tutorial start remains enabled after character confirmation.
- The world interaction prompt is still shown after prototype systems initialize.

## Avatar Handoff

`AHuwamPrototypePlayerActor` now tracks a prototype avatar actor.

The third-person character sets itself as that avatar when it links. The same link:

- Uses the third-person body as interaction range origin.
- Lets the pawn own camera focus and `E` interaction input.
- Gives tutorial startup a physical body to move.

`UHuwamTutorialLabComponent::BeginLaucianTutorialLab` now accepts an optional arrival actor. If one is supplied, the lab entry marker moves that actor. Otherwise it keeps the older owner-actor fallback.

## First Map Recipe

1. Set the map game mode override to `AHuwamPrototypeGameMode`.
2. Place `AHuwamTutorialLabEntryActor` where the player should appear in Laucian's white lab.
3. Place the white-lab lesson actors you want to test:
   - `AHuwamTutorialLabInteractableActor`
   - `AHuwamTutorialLabCombatTargetActor`
   - `AHuwamTutorialLabGatheringActor`
4. Do not place a separate prototype player state actor for the clean spawned-state path.
5. Begin play.
6. Confirm the third-person character is possessed and the character creation screen opens.
7. Confirm a character.
8. Confirm the third-person body moves to the lab entry marker.
9. Read the HUD/tutorial snapshot and confirm the first tutorial beat is `Movement`.
10. Move, aim, and interact through the third-person shell while tutorial systems stay on the linked gameplay state actor.

An existing map-owned `AHuwamPrototypePlayerActor` can still be linked, but it should be configured intentionally if character creation needs to open before debug auto-initialize runs.

## Main Files

- `Source/Huwam/Gameplay/HuwamPrototypeGameMode.h`
- `Source/Huwam/Gameplay/HuwamPrototypeGameMode.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypeThirdPersonCharacter.h`
- `Source/Huwam/Gameplay/HuwamPrototypeThirdPersonCharacter.cpp`
- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.h`
- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.cpp`

## Current Boundary

Huwan now has a first clean startup route from pawn spawn into character creation and the white lab.

It does not yet:

- Provide a test-map asset or final white-lab level blockout.
- Advance Movement and HUD tutorial beats from real third-person actions.
- Open the inventory, equipment, map, or quest views from input.
- Add a real HUD for tutorial instructions beyond current HUD data and interaction prompt surfaces.
- promote the tutorial exit portal into Eldoria.

The next practical layer is to make the first third-person tutorial beats advance from real player actions and a simple visible Laucian tutorial prompt.
