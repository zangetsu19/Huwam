# Tutorial Lab Inventory Prototype

This layer gives Laucian's lab its first inventory lesson.

## Goal

The player should not open a blank pack and wonder whether the lesson failed.

The starter prototype now keeps these loose items in inventory after starter gear is equipped:

- `item.tool.gathering_knife`
- `item.container.small_pouch`

The tutorial snapshot points at `item.tool.gathering_knife` as the first item to inspect.

## Runtime Flow

After the training pedestal advances the lab into `Inventory Basics`:

1. An inventory UI can read `Snapshot.TutorialLab.InventoryLessonItemId`.
2. It can highlight that item in `Snapshot.Inventory.Stacks`.
3. When the player inspects the item, the prototype player can call `MarkTutorialInventoryItemInspected`.
4. The tutorial lab records the inspected item and advances to `Equipment Basics`.

## Main Files

- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.h`
- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`

## Tutorial State

`FHuwamTutorialLabSnapshot` now includes:

- `bInventoryLessonComplete`
- `InventoryLessonItemId`
- `LastInspectedInventoryItemId`

Useful Blueprint calls:

- `UHuwamTutorialLabComponent::MarkInventoryLessonComplete`
- `AHuwamPrototypePlayerActor::MarkTutorialInventoryItemInspected`

The prototype player helper only accepts an item that is currently in the inventory component, so this first lesson is about pack inspection rather than equipped gear.

## First Map Test

1. Confirm a character into Laucian's lab.
2. Advance Movement and HUD Basics.
3. Use `AHuwamTutorialLabInteractableActor`.
4. Confirm the tutorial beat is `Inventory Basics`.
5. Confirm inventory stacks include the gathering knife and small pouch.
6. Call `MarkTutorialInventoryItemInspected` with `item.tool.gathering_knife`.
7. Confirm the tutorial beat becomes `Equipment Basics`.

## Next Step

Equipment training now continues in:

- `Docs/TutorialLabEquipmentPrototype.md`
