# Tutorial Lab Equipment Prototype

This layer gives Laucian's lab its first equipment lesson.

## Goal

The player should learn that inventory ownership and active equipment are related, but not the same thing.

After inventory inspection, the tutorial points at the Basic Sword already equipped in:

- `slot.weapon.main_hand`

That uses the starter loadout already prepared by the prototype player instead of moving another item around for the lesson.

## Runtime Flow

After the gathering knife inspection advances the lab into `Equipment Basics`:

1. Equipment UI can read `Snapshot.TutorialLab.EquipmentLessonSlotId`.
2. It can highlight the main-hand slot in `Snapshot.Equipment.Slots`.
3. When the player inspects that slot, the prototype player can call `MarkTutorialEquipmentSlotInspected`.
4. The tutorial lab verifies the slot still holds the expected Basic Sword.
5. The lab records the inspection and advances to `Combat Basics`.

## Main Files

- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.h`
- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`
- `Source/Huwam/Gameplay/HuwamHudDataComponent.h`
- `Source/Huwam/Gameplay/HuwamHudDataComponent.cpp`

## Tutorial State

`FHuwamTutorialLabSnapshot` now includes:

- `bEquipmentLessonComplete`
- `EquipmentLessonSlotId`
- `EquipmentLessonItemId`
- `LastInspectedEquipmentSlotId`
- `LastInspectedEquipmentItemId`

Useful Blueprint calls:

- `UHuwamTutorialLabComponent::MarkEquipmentLessonComplete`
- `AHuwamPrototypePlayerActor::MarkTutorialEquipmentSlotInspected`

## HUD Support

`FHuwamHudSnapshot` now has an `Equipment` snapshot with:

- Current equipment slots.
- Current equipment stat summary.

That lets a first equipment screen highlight the main-hand sword and later show how gear changes current power.

## First Map Test

1. Confirm a character into Laucian's lab.
2. Advance through interaction and inventory inspection.
3. Confirm the tutorial beat is `Equipment Basics`.
4. Confirm HUD equipment slots include `item.weapon.basic_sword` in `slot.weapon.main_hand`.
5. Call `MarkTutorialEquipmentSlotInspected` with `slot.weapon.main_hand`.
6. Confirm the tutorial beat becomes `Combat Basics`.

## Next Step

Combat training now continues in:

- `Docs/TutorialLabCombatPrototype.md`
