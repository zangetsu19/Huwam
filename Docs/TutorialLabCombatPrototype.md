# Tutorial Lab Combat Prototype

This layer gives Laucian's lab its first combat lesson.

## Goal

The player needs to strike something safe before Eldoria starts attaching danger, loot, and quest progress to fights.

`AHuwamTutorialLabCombatTargetActor` is a placeable white-lab target that:

- Uses the normal combat component.
- Accepts a melee attack from the prototype player.
- Advances the tutorial after a successful hit.
- Does not behave like a monster encounter.
- Does not grant loot, gold, experience, or Eldoria quest progress.

## Runtime Flow

After equipment inspection advances the lab into `Combat Basics`:

1. Place a training target with target ID `target.laucian.training_focus`.
2. Let the prototype player call `AttackTutorialLabCombatTarget`.
3. The target uses the player's real combat component and starter melee attack roll config.
4. A successful strike records the combat lesson.
5. The tutorial lab advances into `Map Basics`.

The first lesson completes on a successful strike rather than a kill. That keeps the target safe and keeps the tutorial focused on input, equipped power, and feedback.

## Main Files

- `Source/Huwam/Gameplay/HuwamTutorialLabCombatTargetActor.h`
- `Source/Huwam/Gameplay/HuwamTutorialLabCombatTargetActor.cpp`
- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.h`
- `Source/Huwam/Gameplay/HuwamTutorialLabComponent.cpp`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.h`
- `Source/Huwam/Gameplay/HuwamPrototypePlayerActor.cpp`

## Tutorial State

`FHuwamTutorialLabSnapshot` now includes:

- `bCombatLessonComplete`
- `CombatLessonTargetId`
- `LastCombatTargetId`

Useful Blueprint calls:

- `AHuwamPrototypePlayerActor::AttackTutorialLabCombatTarget`
- `AHuwamTutorialLabCombatTargetActor::CanReceiveAttackFromPrototypePlayer`
- `AHuwamTutorialLabCombatTargetActor::ReceiveAttackFromPrototypePlayer`
- `UHuwamTutorialLabComponent::MarkCombatLessonComplete`

Useful target event:

- `OnTutorialLabCombatTargetHit`

## First Map Test

1. Confirm a character into Laucian's lab.
2. Advance through interaction, inventory, and equipment inspection.
3. Place `AHuwamTutorialLabCombatTargetActor` in the lab.
4. Confirm the tutorial beat is `Combat Basics`.
5. Call `AttackTutorialLabCombatTarget` with the placed target.
6. Confirm the combat result reports successful damage.
7. Confirm the tutorial beat becomes `Map Basics`.

## Next Step

Map training now continues in:

- `Docs/TutorialLabMapPrototype.md`
