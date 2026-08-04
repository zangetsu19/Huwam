# HUD/Menu Data Prototype

This layer gives Huwam a first Blueprint-friendly data adapter for HUDs, menus, and debug screens.

## Goal

UI widgets should not need to know every gameplay system directly.

Instead of a widget separately reading combat, survival, inventory, map awareness, rewards, quests, and live content, it can ask one component for one snapshot.

`UHuwamHudDataComponent` handles that first pass.

## Snapshot Data

`FHuwamHudSnapshot` currently includes:

| Area | Data |
|---|---|
| Vitals | Current health, max health, health percent, current mana, max mana, mana percent, defeated state. |
| Survival | Hunger, thirst, social energy, need states, and unsafe-food flag. |
| Wallet | Copper-native currency balance, denomination breakdown, and total experience. |
| Inventory | Used slots, max slots, remaining slots, item stacks, and storage route capacity. |
| Equipment | Current equipment slots and equipment stat summary. |
| Map Awareness | Current map, region, fog state, full-map state, minimap state, cardinal markers, and quest-marker availability. |
| Active Quests | Full active quest states, smaller HUD quest summaries with need-request context when present, and tracked quest ID. |
| Live Content | Known content pack status, accessibility, and access reason for debug/admin menus. |
| Tutorial Lab | Current Laucian white-lab beat, instruction, line, focused inventory item, equipment slot, combat target, and map lesson. |
| World Interaction | Current first-world focus target kind, prompt, distance, and whether interaction is available. |
| Component Flags | Whether combat, equipment, inventory, survival, map awareness, rewards, quests, live content, tutorial lab, and world interaction were found. |

## Component Flow

The HUD data component can find these components on the same actor:

- `UHuwamCombatComponent`
- `UHuwamEquipmentComponent`
- `UHuwamInventoryComponent`
- `UHuwamSurvivalVitalsComponent`
- `UHuwamMapAwarenessComponent`
- `UHuwamRewardComponent`
- `UHuwamQuestComponent`
- `UHuwamLiveContentComponent`
- `UHuwamTutorialLabComponent`
- `UHuwamWorldInteractionComponent`

It can also accept explicit component references when a Blueprint wants to build a snapshot from another actor.

## Blueprint Functions

| Function | Purpose |
|---|---|
| `RefreshHudSnapshot` | Refreshes quest progress from rewards, rebuilds the snapshot, stores it, and broadcasts an event. |
| `GetLastHudSnapshot` | Returns the most recently refreshed snapshot. |
| `BuildHudSnapshot` | Builds a read-only snapshot from resolved or supplied components. |

## First HUD Targets

The first useful HUD can be very simple:

| UI Area | Source |
|---|---|
| Health bar | `Snapshot.Vitals.HealthPercent` |
| Mana bar | `Snapshot.Vitals.ManaPercent` |
| Hunger bar | `Snapshot.Survival.HungerPercent` |
| Thirst bar | `Snapshot.Survival.ThirstPercent` |
| Social bar | `Snapshot.Survival.SocialEnergyPercent` |
| Currency | `Snapshot.Wallet.Currency` |
| XP | `Snapshot.Wallet.TotalExperience` |
| Economy ledger | `Snapshot.Economy.TransactionCount`, `Snapshot.Economy.TotalRevenueCopper`, and account balances |
| Focused service pressure | `Snapshot.Economy.FocusedServicePressure` when aiming at a survival service |
| Inventory slot count | `Snapshot.Inventory.UsedSlots / Snapshot.Inventory.MaxSlots` |
| Storage routes | `Snapshot.Inventory.StorageRoutes` |
| Equipment slots | `Snapshot.Equipment.Slots` |
| Minimap/full-map seed | `Snapshot.MapAwareness` |
| Active quest | First entry in `Snapshot.ActiveQuestSummaries` |
| NPC quest reason | `Snapshot.ActiveQuestSummaries[*].RequestNeedReason` when a need-driven request exists |
| Tracked quest | `Snapshot.TrackedQuestId` |
| Quest progress | `CurrentValue / TargetValue` |
| Turn-in marker | `bReadyToTurnIn` |
| Tutorial prompt | `Snapshot.TutorialLab.InstructionText` |
| World interaction prompt | `Snapshot.WorldInteraction.PromptText` when `bCanInteract` is true |

The first native HUD surfaces for those prompt rows are now:

- `UHuwamTutorialLabPromptWidget`, which listens to the tutorial lab beat snapshot.
- `UHuwamWorldInteractionPromptWidget`, which listens to the world interaction focus snapshot.
- `UHuwamPrototypeHudWidget`, which shows first vitals, wallet, map, and quest data.
- `UHuwamPrototypeMenuWidget`, which gives pack/equipment, map, and quest data first player-facing pages.

The larger HUD/menu adapter remains available for a later full HUD layout.

## Slime Quest Example

After starting `Slimes in the Tall Grass`, defeating one Basic Slime, and claiming its reward:

| Snapshot Field | Expected |
|---|---:|
| CurrencyBalanceCopper | 3 |
| TotalExperience | 5 |
| Slime Core Quantity | 1 |
| Quest CurrentValue | 1 |
| Quest TargetValue | 3 |
| Quest ProgressPercent | 0.33 |
| bReadyToTurnIn | false |

After three Basic Slimes and quest refresh:

| Snapshot Field | Expected |
|---|---:|
| Quest CurrentValue | 3 |
| Quest TargetValue | 3 |
| Quest ProgressPercent | 1.0 |
| bReadyToTurnIn | true |

## Prototype Player Step

The first starter player prototype actor is now started:

- Inventory component.
- Equipment component.
- Character stats component.
- Combat component.
- Reward component.
- Quest component.
- Live content component.
- Tutorial lab component.
- World interaction component.
- HUD data component.
- Helper functions for the first slime quest loop.

See `Docs/PrototypePlayerActor.md`.
See `Docs/PlayableVerticalSlicePrototype.md`.
See `Docs/TutorialLabPromptPrototype.md`.
See `Docs/WorldInteractionPromptPrototype.md`.
