# Third-Person Prototype Character

This layer gives Huwan its first movable third-person player shell.

## Goal

The first character shell should prove:

1. A possessed Unreal character can move and drive a third-person camera in the prototype map.
2. Camera direction can choose the world interaction focus instead of relying only on the old nearest-target pass.
3. Existing player-state systems can stay on `AHuwamPrototypePlayerActor` while the new character owns movement, camera, and first pawn input.

## Runtime Piece

`AHuwamPrototypeThirdPersonCharacter` is a native `ACharacter` with:

- Capsule-based character movement.
- Spring-arm third-person camera boom.
- Follow camera.
- First direct prototype input bindings.
- A link to the existing prototype player state actor.
- Camera-driven interaction focus.

The first shell auto-possesses Player 0 when it is placed in a map.

## First Controls

| Input | Prototype Action |
|---|---|
| `W` and `S` | Move forward and backward relative to camera yaw. |
| `A` and `D` | Strafe relative to camera yaw. |
| Mouse movement | Turn and pitch the third-person camera. |
| Space | Jump. |
| `E` | Interact with the current camera-focused supported world target. |
| Left mouse | Strike a camera-aimed tutorial combat target or Basic Slime. |
| `Tab` | Acknowledge the first HUD tutorial beat when Laucian asks for it. |
| `I`, `M`, and `J` | Open the first pack/equipment, map, and quest menu pages. |
| `F5` and `F9` | Save and load the first prototype slice slot. |

These are direct prototype bindings. A later player controller/input settings pass should turn them into the final remappable input layer.

## Gameplay State Pair

For now the character and the gameplay state actor are separate on purpose:

| Actor | Owns |
|---|---|
| `AHuwamPrototypeThirdPersonCharacter` | Movement, camera, focus aim, pawn input. |
| `AHuwamPrototypePlayerActor` | Character creation, inventory, equipment, combat, rewards, quests, tutorial state, HUD data, interaction widgets. |

The character can be linked explicitly with `SetPrototypePlayerActor`, or it can find the first prototype player actor in the world at begin play.

When linked, it:

- Sets itself as the world interaction origin actor.
- Disables the old logic-actor interaction key bridge so `E` routes once from the pawn.
- Shows the existing world interaction prompt through the linked prototype player when allowed.

## Camera Focus

The character tries to find a supported interaction target from camera direction:

1. It traces forward on the visibility channel from the follow camera.
2. If a supported target is hit, it hands that target into `UHuwamWorldInteractionComponent`.
3. If the trace is clear, it can choose an aimed fallback target in a camera cone for early prototype actors that do not yet have useful collision.
4. If a blocking trace hits something unsupported, the focus does not jump through it.

Current supported focus targets remain:

- `AHuwamNpcSupplyQuestOfferActor`
- `AHuwamTutorialLabInteractableActor`
- `AHuwamGatheringResourceActor`

## Interaction Origin

`UHuwamWorldInteractionComponent` now supports `SetInteractionOriginActor`.

That lets:

- The old placed prototype player actor remain a logic/state anchor.
- Range checks and focus distance use the moving character instead.
- A future pawn/controller path replace this first shell without moving quest and inventory state again.

When camera focus drives the route, the character disables nearest-target auto refresh on the linked interaction component so camera focus does not compete with it.

## Tutorial Input

The pawn now proves the first white-lab action handoff too:

1. During Laucian's Movement beat it records a starting location, watched travel distance, and camera look input.
2. Walking a few steps while looking around advances the tutorial into HUD Basics.
3. Pressing `Tab` on the HUD Basics beat routes through the linked prototype player actor and advances Laucian into Interaction Ready.

The native tutorial prompt and those early actions continue in:

- `Docs/TutorialLabPromptPrototype.md`

## Main Files

- `Source/Huwam/Gameplay/HuwamPrototypeThirdPersonCharacter.h`
- `Source/Huwam/Gameplay/HuwamPrototypeThirdPersonCharacter.cpp`
- `Source/Huwam/Gameplay/HuwamWorldInteractionComponent.h`
- `Source/Huwam/Gameplay/HuwamWorldInteractionComponent.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`

## First Map Test

1. Place `AHuwamPrototypePlayerActor` in a test map.
2. Place `AHuwamPrototypeThirdPersonCharacter` as the pawn shell.
3. Keep the character auto-link option enabled, or assign its prototype player actor reference explicitly.
4. Place an Eldoria supply requester, white-lab interaction object, or field gathering node in front of the camera path.
5. Begin play.
6. Move with `W`, `A`, `S`, and `D`, move the camera with the mouse, and jump with Space.
7. Aim at a supported target.
8. Confirm the interaction prompt follows the camera-focused target.
9. Press `E`.
10. Confirm the existing request screen, tutorial interaction, or gathering route still resolves through the linked prototype player state.

## Current Boundary

The prototype now has a real moving third-person shell.

It now also has:

- A simple visible body and head mesh from engine blockout shapes.
- A light movement bob/lean pass for the blockout avatar.
- Camera collision testing plus follow-camera lag.

It does not yet:

- Add final skeletal character art, an animation Blueprint, crosshair, outline, or final interaction marker.
- Use the final Enhanced Input/remappable control layer.
- Replicate movement or interaction focus for multiplayer.

The clean character-plus-state startup path now continues through the first game-mode bootstrap, and the next tutorial layer is to make inventory and equipment lessons player-facing after the first pedestal interaction.

See:

- `Docs/ThirdPersonBootstrapPrototype.md`
- `Docs/TutorialLabPromptPrototype.md`
