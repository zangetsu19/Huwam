# Tutorial Lab Map Prototype

This layer gives Laucian's lab its first map lesson.

## Goal

Huwan's map teaching should support a high-graphics 3D third-person world:

- The minimap belongs in the HUD over live world movement.
- The full map is a navigation layer, not a replacement for the world.
- Battle fog should keep undiscovered detail unknown later.

The first prototype step is a small map-awareness runtime that gives the tutorial and HUD one shared source of truth.

## Runtime Flow

After the training focus advances the lab into `Map Basics`:

1. `UHuwamMapAwarenessComponent` exposes the current map snapshot.
2. The snapshot says whether the full map is open and whether cardinal markers are visible.
3. The prototype player can call `OpenTutorialMapLesson`.
4. The map opens for `map.laucian.white_lab`.
5. The tutorial verifies the map ID and advances into `Quest Basics`.

## Main Files

- `Source/Huwam/Gameplay/HuwamMapAwarenessComponent.h`
- `Source/Huwam/Gameplay/HuwamMapAwarenessComponent.cpp`
- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.h`
- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`
- `Source/Huwam/Gameplay/HuwamHudDataComponent.h`
- `Source/Huwam/Gameplay/HuwamHudDataComponent.cpp`

## Map Snapshot

`FHuwamMapAwarenessSnapshot` currently includes:

- `MapId`
- `RegionId`
- `DisplayName`
- `FogState`
- `bMinimapAvailable`
- `bFullMapOpen`
- `bCardinalMarkersVisible`
- `bBattleFogEnabled`
- `bQuestMarkersAvailable`

The starting white-lab map uses:

- `map.laucian.white_lab`
- `location.laucian.white_lab`

## Tutorial State

`FHuwamTutorialLabSnapshot` now includes:

- `bMapLessonComplete`
- `MapLessonMapId`
- `LastOpenedMapId`

Useful Blueprint calls:

- `UHuwamMapAwarenessComponent::OpenFullMap`
- `UHuwamMapAwarenessComponent::CloseFullMap`
- `AHuwamPrototypePlayerActor::OpenTutorialMapLesson`
- `UHuwamTutorialLabComponent::MarkMapLessonComplete`

## First Map Test

1. Confirm a character into Laucian's lab.
2. Advance through combat and reach `Map Basics`.
3. Read the HUD snapshot and confirm map awareness exists.
4. Call `OpenTutorialMapLesson`.
5. Confirm the map snapshot reports the full map open.
6. Confirm the tutorial beat becomes `Quest Basics`.

## Next Step

Quest training now continues in:

- `Docs/TutorialLabQuestPrototype.md`
