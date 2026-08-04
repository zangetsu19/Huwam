# Prototype Player Actor

This layer bundles the first playable-loop systems into one Blueprint-placeable actor.

## Goal

The early prototype needs a simple actor that can prove the full loop without manually adding every component each time.

`AHuwamPrototypePlayerActor` includes:

- `UHuwamInventoryComponent`
- `UHuwamSurvivalVitalsComponent`
- `UHuwamEquipmentComponent`
- `UHuwamCharacterStatsComponent`
- `UHuwamCharacterCreationComponent`
- `UHuwamCharacterCreationMenuDataComponent`
- `UHuwamCombatComponent`
- `UHuwamRewardComponent`
- `UHuwamQuestComponent`
- `UHuwamLiveContentComponent`
- `UHuwamMapAwarenessComponent`
- `UHuwamTutorialLabComponent`
- `UHuwamWorldInteractionComponent`
- `UHuwamHudDataComponent`

## Auto Initialization

On begin play, the actor can:

1. Apply a default character creation request.
2. Reset equipment slots.
3. Enable the starter core content pack.
4. Grant starter loadout.
5. Equip starter gear.
6. Initialize health and mana.
7. Reset survival vitals.
8. Start `Slimes in the Tall Grass`.
9. Refresh the HUD snapshot.

## Starter Loadout

| Item | Use |
|---|---|
| `item.weapon.basic_sword` | Main hand weapon. |
| `item.weapon.basic_knife` | Off-hand weapon. |
| `item.bag.adventuring_backpack` | Backpack slot. |
| `item.tool.gathering_knife` | Loose starter tool for inventory training. |
| `item.container.small_pouch` | Loose starter storage item for pack testing. |

## Helper Functions

| Function | Purpose |
|---|---|
| `InitializePrototypePlayer` | Resets the actor into the first playable-loop state. |
| `ConfigureForThirdPersonBootstrap` | Holds back debug auto-initialize and prepares a spawned state actor to open character creation for the pawn bootstrap. |
| `InitializePrototypePlayerFromCharacterCreation` | Applies a confirmed character request and starts the first playable-loop state. |
| `GrantStarterLoadout` | Adds basic starter items if missing. |
| `EquipStarterGear` | Equips sword, knife, and backpack. |
| `StartSlimeQuest` | Starts `Slimes in the Tall Grass`. |
| `StartGatherFieldSuppliesQuest` | Starts the first Eldoria field gathering quest directly for prototype tests. |
| `CanStartGatherFieldSuppliesQuestFromSupply` | Checks whether the first Eldoria supply recipient is low enough to ask for the gathering quest. |
| `StartGatherFieldSuppliesQuestFromSupply` | Starts the gathering quest through the first NPC shortage gate. |
| `BeginLaucianTutorialLab` | Starts the Progression Zero tutorial state and moves to a lab entry marker when available. |
| `AcknowledgeTutorialHudLesson` | Completes the first HUD tutorial beat through a pawn or UI acknowledgment route. |
| `MarkTutorialInventoryItemInspected` | Completes the first inventory lesson when its focused pack item is inspected. |
| `MarkTutorialEquipmentSlotInspected` | Completes the first equipment lesson when its focused slot is inspected. |
| `AttackTutorialLabCombatTarget` | Strikes the first white-lab combat target without using Eldoria monster rewards. |
| `OpenTutorialMapLesson` | Opens the first white-lab map state and advances the tutorial after cardinal markers are available. |
| `TrackTutorialQuestLesson` | Tracks the active white-lab tutorial quest and exposes its objective marker seed. |
| `EnterEldoriaStarterArea` | Moves the playable avatar to the first Eldoria starter entry after tutorial completion and starts the slime quest. |
| `ApplyCharacterCreationToPrototype` | Applies a custom character creation request before play. |
| `ShowCharacterCreationScreen` | Opens the native first character creator screen. |
| `HideCharacterCreationScreen` | Closes the character creator screen and can restore game input. |
| `AttackMonster` | Attacks a monster encounter actor with starter melee config. |
| `ClaimMonsterRewards` | Claims monster rewards and advances slime quest progress. |
| `AttackMonsterAndClaimIfDefeated` | Convenience helper for the first loop. |
| `TurnInSlimeQuest` | Turns in the slime quest after progress reaches `3 / 3`. |
| `TurnInGatherFieldSuppliesQuest` | Delivers accepted field materials into the matching NPC supply component, then turns in the gathering quest after harvest progress reaches `10 / 10`. |
| `SellAcceptedSupplyItemToNpc` | Sells one accepted surplus item stack into an NPC supply component and refreshes player currency/HUD state. |
| `RefreshPrototypeHud` | Refreshes and returns the HUD snapshot. |
| `RefreshWorldInteractionFocus` | Refreshes the first nearby world-target prompt snapshot and HUD bridge. |
| `InteractWithFocusedWorldTarget` | Routes the first supported NPC, white-lab object, or field-gather interaction. |
| `ShowNpcSupplyQuestInteractionScreen` | Opens the focused Eldoria supply-request screen for a requester actor. |
| `HideNpcSupplyQuestInteractionScreen` | Closes the Eldoria supply-request screen and can restore game input. |
| `ShowWorldInteractionPromptScreen` | Shows the native HUD prompt for the active first world interaction focus. |
| `HideWorldInteractionPromptScreen` | Removes the native world interaction prompt. |
| `ShowTutorialLabPromptScreen` | Shows the native Laucian prompt for the active white-lab tutorial beat. |
| `HideTutorialLabPromptScreen` | Removes the native Laucian tutorial prompt. |
| `ShowPrototypeHudScreen` | Shows the first vitals, wallet, map, and quest HUD panel. |
| `ShowPrototypeMenuScreen` | Opens the first pack/equipment, map, or quest menu page. |
| `SavePrototypeProgress` | Writes the first vertical-slice save slot. |
| `LoadPrototypeProgress` | Restores the first vertical-slice save slot. |
| `EnablePrototypeWorldInteractionInput` | Binds the first prototype interaction key through the current player controller. |
| `DisablePrototypeWorldInteractionInput` | Releases the logic-actor key bridge when a possessed pawn shell owns interaction input. |
| `SetPrototypeAvatarActor` | Links the moving avatar used for interaction origin and tutorial arrival. |
| `GetPrototypeAvatarActor` | Returns the linked avatar or the state actor fallback. |

## First Map Test

1. Place `AHuwamPrototypePlayerActor` in a test map.
2. Place three `AHuwamMonsterEncounterActor` instances, each using `monster.slime.basic`.
3. Begin play.
4. Call `AttackMonsterAndClaimIfDefeated` until each slime is defeated.
5. Call `RefreshPrototypeHud`.
6. Confirm the active quest reaches `3 / 3`.
7. Call `TurnInSlimeQuest`.

Expected final totals:

| Source | Copper | Experience |
|---|---:|---:|
| Three Basic Slime rewards | 9 | 15 |
| Quest turn-in | 15 | 10 |
| Final total | 24 | 25 |

The player should also have three slime cores if each defeated slime reward was claimed.

## Character Creation Screen

The actor can now open `UHuwamCharacterCreationScreenWidget`.

For the first test path:

1. Place the actor in a map.
2. Enable `bShowCharacterCreationScreenOnBeginPlay`.
3. Begin play.
4. Confirm the character creator screen.

Confirmation uses `InitializePrototypePlayerFromCharacterCreation`, so the same actor keeps the chosen identity and then readies inventory, equipment, combat, live content, and the Laucian tutorial-lab handoff.

See:

- `Docs/CharacterCreationMenuDataPrototype.md`
- `Docs/CharacterCreationScreenPrototype.md`
- `Docs/TutorialLabHandoffPrototype.md`
- `Docs/TutorialLabPromptPrototype.md`
- `Docs/PlayableVerticalSlicePrototype.md`
- `Docs/TutorialLabInteractionPrototype.md`
- `Docs/TutorialLabInventoryPrototype.md`
- `Docs/TutorialLabEquipmentPrototype.md`
- `Docs/TutorialLabCombatPrototype.md`
- `Docs/TutorialLabMapPrototype.md`
- `Docs/TutorialLabQuestPrototype.md`
- `Docs/TutorialLabGatheringPrototype.md`
- `Docs/GatheringResourcePrototype.md`
- `Docs/GatherFieldSuppliesQuestPrototype.md`
- `Docs/NpcSupplyDeliveryPrototype.md`
- `Docs/NpcSupplyShortageGatePrototype.md`
- `Docs/NpcSupplyTradeUsePrototype.md`
- `Docs/WorldInteractionPrototype.md`
- `Docs/WorldInteractionPromptPrototype.md`
- `Docs/ThirdPersonPrototypeCharacter.md`
- `Docs/ThirdPersonBootstrapPrototype.md`

## Next Step

The vertical slice now has first native HUD/menu/save surfaces. The next player-facing layer is stronger authored level and feedback work on top of this playable route.
