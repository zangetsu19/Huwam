# Tutorial Lab Quest Prototype

This layer gives Laucian's lab its first quest-tracking lesson.

## Goal

The player should learn that a quest has both words and direction:

- The quest log shows the active objective.
- A tracked objective can seed a world or map marker.
- Battle fog rules can later decide how exact that marker becomes.

The first lesson uses the already-active tutorial quest:

- `quest.tutorial.progression_zero`
- `objective.complete_tutorial`

## Runtime Flow

After the white-lab map advances the tutorial into `Quest Basics`:

1. `UHuwamQuestComponent` tracks `Welcome to Progression Zero`.
2. `UHuwamMapAwarenessComponent` exposes the tracked objective marker seed.
3. HUD data exposes the tracked quest ID plus the marker-ready map snapshot.
4. `AHuwamPrototypePlayerActor::TrackTutorialQuestLesson` tells the lab the correct quest and objective were tracked.
5. Laucian advances into `Gathering Basics`.

## Main Files

- `Source/Huwam/Gameplay/HuwamQuestComponent.h`
- `Source/Huwam/Gameplay/HuwamQuestComponent.cpp`
- `Source/Huwam/Gameplay/HuwamMapAwarenessComponent.h`
- `Source/Huwam/Gameplay/HuwamMapAwarenessComponent.cpp`
- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.h`
- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`

## Quest And Marker State

`UHuwamQuestComponent` now supports:

- `TrackQuest`
- `IsQuestTracked`
- `GetTrackedQuestId`

`FHuwamMapAwarenessSnapshot` now includes:

- `TrackedQuestId`
- `TrackedObjectiveId`
- `bTrackedQuestMarkerVisible`

`FHuwamTutorialLabSnapshot` now includes:

- `bQuestLessonComplete`
- `QuestLessonQuestId`
- `QuestLessonObjectiveId`
- `LastTrackedQuestId`
- `LastTrackedObjectiveId`

## First Map Test

1. Confirm a character into Laucian's lab.
2. Advance through map training and reach `Quest Basics`.
3. Confirm the active quest list contains `quest.tutorial.progression_zero`.
4. Call `TrackTutorialQuestLesson`.
5. Confirm HUD data reports `TrackedQuestId` as `quest.tutorial.progression_zero`.
6. Confirm map awareness reports a tracked objective marker seed.
7. Confirm the tutorial beat becomes `Gathering Basics`.

## Next Step

Gathering training now continues in:

- `Docs/TutorialLabGatheringPrototype.md`
