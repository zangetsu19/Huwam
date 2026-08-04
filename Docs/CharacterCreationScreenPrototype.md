# Character Creation Screen Prototype

This layer puts the first character creation choices on screen.

## Goal

The first prototype needs a working screen before it needs final character creator art.

`UHuwamCharacterCreationScreenWidget` is a native Unreal widget that:

- Shows a name field.
- Shows race and filtered sub-race choices.
- Shows class path, job, skill, and ability choices.
- Shows class, job, skill, and ability slot counts.
- Disables confirmation until the request validates.
- Confirms into `AHuwamPrototypePlayerActor`.
- Restores game input after a successful confirmation.

The native version is intentionally simple. A later Widget Blueprint can subclass it for the final layout and listen to the same snapshot and confirmation events.

## Human Baseline Preview Direction

The first Human sub-race art baselines are now documented and exposed through the character creation data feed.

The current native screen remains text-first, but its Human sub-race cards are now intended to stand in for future 3D preview panes:

| Sub-Race | Future Preview |
| --- | --- |
| Common Human | Baseline level 1 adventurer with worn leather, soft cloth, and a used short sword. |
| High-Human | Alabaster noble magic user in fine fabrics with smug spellcaster posture. |
| Dawn Human | Cheerful yellow-warmed holy Human with bright garments and gentle aura. |
| Dusk Human | Dark-skinned guard/tank in knight armor with greatsword, kite shield, and slight void aura. |
| Frontier Human | Red sunburnt farmer/ranger with wild predator-alert eyes and rugged tools. |
| Urban Human | Slightly dirty city merchant with softer body type, ledger, pouches, and trade gear. |
| Warborn Human | Dark tanned scarred frontliner with fierce nearly manic war energy. |
| Half-Elf | Human-Elf hybrid adventurer with very short ears, sword gear, charisma, and mixed-culture appeal. |
| Blessed Human | Ebony skin, white hair, white eyes, wings, deity tattoos, radiant holy aura. |
| Cursed Human | Purple-tinted dark tan rogue with pure black eyes, curse tattoos, and purple aura. |

Half-Blood Human remains a later matrix side project, but the Human-Elf branch is now approved as Half-Elf and can be shown from either Human hybrid or Elf selection.

## Main Files

- `Source/Huwam/UI/HuwamCharacterCreationScreenWidget.h`
- `Source/Huwam/UI/HuwamCharacterCreationScreenWidget.cpp`

The widget depends on the existing menu adapter:

- `UHuwamCharacterCreationMenuDataComponent`

It does not read race or class tables directly. That stays in the data adapter.

## Prototype Player Hook

`AHuwamPrototypePlayerActor` now exposes:

| Function | Purpose |
|---|---|
| `ShowCharacterCreationScreen` | Creates the native screen and switches the player to UI input. |
| `HideCharacterCreationScreen` | Removes the screen and can restore game input. |
| `InitializePrototypePlayerFromCharacterCreation` | Applies the confirmed request and starts the first prototype systems. |
| `GetCharacterCreationScreen` | Returns the active screen instance, if one is open. |

The actor keeps its existing auto-initialize path by default. For the character creator flow, enable `bShowCharacterCreationScreenOnBeginPlay` on the placed actor or call `ShowCharacterCreationScreen` from a test Blueprint.

The first third-person bootstrap can now configure and spawn that state actor for character creation through `AHuwamPrototypeGameMode`, then link it to the moving pawn shell before the tutorial handoff resolves.

## First Map Test

1. Place `AHuwamPrototypePlayerActor` in a test map.
2. Enable `bShowCharacterCreationScreenOnBeginPlay`.
3. Begin play.
4. Enter a name.
5. Change race and confirm that the sub-race section changes with it.
6. Toggle class, job, skill, and ability choices.
7. Confirm only when the screen says the request is ready.
8. Confirm that the screen closes and the prototype player loop initializes.

If the imported DataTable registry is not linked yet, the screen can still use the menu adapter fallback choices for testing.

## Blueprint Extension

A Widget Blueprint can subclass `UHuwamCharacterCreationScreenWidget` later.

Useful events:

- `OnMenuSnapshotChanged`
- `OnCharacterCreationConfirmed`

Useful callable actions:

- `SelectOption`
- `SetCharacterName`
- `RefreshMenu`
- `ConfirmCharacter`

This keeps the selection rules in C++ while allowing the final UMG layout to become richer.

## Next Step

The first Laucian tutorial-lab handoff now starts after confirmation through `UHuwamTutorialLabComponent`.

See:

- `Docs/TutorialLabHandoffPrototype.md`
- `Docs/ThirdPersonBootstrapPrototype.md`

The next practical layer is the first physical lab interaction lesson.
