# Tutorial Lab Gathering Prototype

This layer gives Laucian's lab its first gathering lesson.

## Goal

The player should learn that a world object can become inventory state without leaving the white lab yet.

The first gatherable uses a safe F-rank material:

- `material.herb.lavender`

It also expects the starter gathering tool the inventory lesson already introduced:

- `item.tool.gathering_knife`

## Runtime Flow

After quest tracking advances the lab into `Gathering Basics`:

1. A placed `AHuwamTutorialLabGatheringActor` becomes gatherable.
2. The actor checks that the player still has the gathering knife.
3. The actor grants tutorial lavender through `UHuwamInventoryComponent::AddItemById`.
4. `UHuwamTutorialLabComponent::MarkGatheringLessonComplete` records the source, item, and quantity.
5. Laucian completes the first tutorial objective and advances the lab to `Complete`.

## Main Files

- `Source/Huwam/Gameplay/HuwamTutorialLabGatheringActor.h`
- `Source/Huwam/Gameplay/HuwamTutorialLabGatheringActor.cpp`
- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.h`
- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.cpp`

## Tutorial State

`FHuwamTutorialLabSnapshot` now includes:

- `bGatheringLessonComplete`
- `GatheringLessonSourceId`
- `GatheringLessonItemId`
- `GatheringLessonQuantity`
- `LastGatheringSourceId`
- `LastGatheredItemId`
- `LastGatheredItemQuantity`

Useful Blueprint calls:

- `AHuwamTutorialLabGatheringActor::CanGatherWithPrototypePlayer`
- `AHuwamTutorialLabGatheringActor::GatherWithPrototypePlayer`
- `UHuwamTutorialLabComponent::MarkGatheringLessonComplete`

Useful gatherable event:

- `OnTutorialLabGathered`

## First Map Test

1. Confirm a character into Laucian's lab.
2. Advance through the map and quest lessons until the beat is `Gathering Basics`.
3. Place `AHuwamTutorialLabGatheringActor` in the white lab.
4. Confirm the prototype player still carries `item.tool.gathering_knife`.
5. Call `GatherWithPrototypePlayer` with the placed gatherable.
6. Confirm inventory now contains `material.herb.lavender`.
7. Confirm the tutorial beat becomes `Complete`.

## Next Step

World gathering now continues in:

- `Docs/GatheringResourcePrototype.md`
