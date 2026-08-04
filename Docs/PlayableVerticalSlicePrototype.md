# Playable Vertical Slice Prototype

This pass joins the first Huwan prototype systems into one short playable loop.

## Current Loop

1. Begin play with `AHuwamPrototypeGameMode`.
2. Confirm a character.
3. Arrive in Laucian's code-spawned white lab.
4. Move, read the HUD, use the training pedestal, inspect pack and equipment, strike the training focus, read the map, track the tutorial quest, and gather tutorial lavender.
5. Complete the first Progression Zero tutorial objective.
6. Arrive in a code-spawned Eldoria starter field with a marked Adventurers Guild board pad, a visible NPC supply requester pad, starter herbs, and Basic Slime targets.

This is a blockout slice. It is meant to make iteration playable before authored levels, art assets, network play, and final input mapping exist.

## Player-Facing Pieces

| Piece | Prototype Role |
|---|---|
| `UHuwamPrototypeHudWidget` | Shows vitals, pack use, wallet, current map, tracked marker state, and the first active quest. |
| `UHuwamPrototypeMenuWidget` | Gives inventory/equipment, map, and quest pages real buttons for the white-lab lesson steps. |
| `AHuwamPrototypeThirdPersonCharacter` | Owns movement, camera, interaction, attack, menu, and first save/load input. |
| `AHuwamPrototypeSliceBlockoutActor` | Spawns the white lab floor/walls/light, tutorial actors, Eldoria starter field, herbs, and Basic Slimes. |
| `AHuwamGuildContractBoardActor` | Reads configured contract quest IDs, selects the next relevant board entry, and accepts, tracks, or turns it in. |
| `UHuwamPrototypeSaveGame` | Stores a versioned first prototype snapshot for transform, combat meters, pack, equipment, rewards, quests, map awareness, tutorial state, and economy ledger state. |

## Prototype Controls

| Input | Action |
|---|---|
| `W`, `A`, `S`, `D` | Move. |
| Mouse | Aim the third-person camera. |
| Space | Jump. |
| `E` | Interact with focused tutorial or gathering actor. |
| Left mouse | Attack Laucian's combat target or aimed Basic Slime. |
| `Tab` | Acknowledge the first HUD tutorial beat. |
| `I` | Open pack and equipment page. |
| `M` | Open map page. |
| `J` | Open quest page. |
| `F5` | Save the first prototype slice slot. |
| `F9` | Load the first prototype slice slot. |

The menu pages also expose Pack, Map, Quests, lesson actions, and Close as buttons. The control names now live in `Config/DefaultInput.ini` so the blockout route no longer keeps every first key in pawn code.

## Lab Order

| Beat | Current Player Action |
|---|---|
| Movement | Walk and look around. |
| HUD Basics | Read the HUD, then acknowledge the beat. |
| Interaction Ready | Interact with the training pedestal. |
| Inventory Basics | Open pack/equipment and inspect the gathering knife. |
| Equipment Basics | Inspect the main-hand starter sword. |
| Combat Basics | Aim at the training focus and attack. |
| Map Basics | Open the map page and read the full-map lesson. |
| Quest Basics | Open quests and track the tutorial quest. |
| Gathering Basics | Aim at tutorial lavender and gather it. |

The lavender completion triggers the Eldoria handoff. The first guild board in the field starts the slime contract after arrival.

## Main Files

- `Source/Huwam/UI/HuwamPrototypeHudWidget.h`
- `Source/Huwam/UI/HuwamPrototypeHudWidget.cpp`
- `Source/Huwam/UI/HuwamPrototypeMenuWidget.h`
- `Source/Huwam/UI/HuwamPrototypeMenuWidget.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypeSliceBlockoutActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypeSliceBlockoutActor.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypeSaveGame.h`
- `Source/Huwam/Gameplay/HuwamPrototypeSaveGame.cpp`
- `Source/Huwam/Gameplay/HuwamEldoriaStarterEntryActor.h`
- `Source/Huwam/Gameplay/HuwamEldoriaStarterEntryActor.cpp`

## Playtest Focus

The fastest manual test is:

1. Start play and confirm a character.
2. Follow the Laucian prompt until the Eldoria starter field appears.
3. Save in the lab or Eldoria.
4. Move and load.
5. Confirm location, current health/mana, pack/equipment, HUD quest data, map state, and tutorial progress restore.
6. Accept the slime contract at the Adventurers Guild board, defeat Basic Slimes, and turn the contract back in at the board.
7. Meet the visible NPC requester, preview the field-supplies path, and sell one accepted surplus item from the same panel.

## Next Boundary

This slice is now playable enough to iterate. The next work should improve feel and presentation: real map assets, skeletal character art and animation, Enhanced Input assets, richer combat feedback, authored Eldoria objects, data-driven board entries, and multiplayer-safe state ownership.
