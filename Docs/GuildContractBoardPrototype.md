# Guild Contract Board Prototype

This pass gives Eldoria its first placed Adventurers Guild contract surface.

## Current Board Loop

`AHuwamGuildContractBoardActor` is the first blockout board actor. The code-spawned Eldoria starter field places one board beside the route out of Laucian's tutorial lab.

The board exposes configured `ContractQuestIds`, builds a contract entry snapshot for each configured quest, and selects the most urgent usable entry in this order:

1. Ready for turn-in.
2. Active and tracking.
3. Available offer.
4. Already cleared.

The first configured board contract is `Slimes in the Tall Grass`.

| Board State | Player Action |
|---|---|
| Offering contract | Interact to accept and track the selected contract. |
| Tracking contract | Interact again to refresh tracking and the map marker. |
| Ready for turn-in | Interact to claim the guild reward. |
| Cleared | The board remembers the configured contract is finished. |

The board uses the shared `E` world-interaction path, so the HUD prompt and camera focus behavior already used by lab objects, gatherables, and NPC supply requests also cover guild contracts. Its interaction prompt now names the selected contract when display text is available.

## Eldoria Requester

The same starter field now spawns the first poor-district supply requester as a visible blockout NPC. This keeps the NPC-generated `Gather Field Supplies` path reachable in the playable field while the guild board owns the starter combat contract.

## Main Files

- `Source/Huwam/Gameplay/HuwamGuildContractBoardActor.h`
- `Source/Huwam/Gameplay/HuwamGuildContractBoardActor.cpp`
- `Source/Huwam/Gameplay/HuwamWorldInteractionComponent.h`
- `Source/Huwam/Gameplay/HuwamPrototypeSliceBlockoutActor.cpp`
- `Source/Huwam/Gameplay/HuwamNpcSupplyQuestOfferActor.cpp`

## Next Boundary

The board now carries generic configured contract entries, but this blockout prop still acts on one selected contract per interaction. The next production pass should add a board screen for choosing from multiple entries, authored Eldoria visuals, rank permissions, board refresh timing, and guild-specific listings.
