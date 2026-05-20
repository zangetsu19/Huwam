# HUD/Menu Data Prototype

This layer gives Huwam a first Blueprint-friendly data adapter for HUDs, menus, and debug screens.

## Goal

UI widgets should not need to know every gameplay system directly.

Instead of a widget separately reading combat, inventory, rewards, quests, and live content, it can ask one component for one snapshot.

`UHuwamHudDataComponent` handles that first pass.

## Snapshot Data

`FHuwamHudSnapshot` currently includes:

| Area | Data |
|---|---|
| Vitals | Current health, max health, health percent, current mana, max mana, mana percent, defeated state. |
| Wallet | Gold balance and total experience. |
| Inventory | Used slots, max slots, remaining slots, and item stacks. |
| Active Quests | Full active quest states plus smaller HUD quest summaries. |
| Live Content | Known content pack status, accessibility, and access reason for debug/admin menus. |
| Component Flags | Whether combat, inventory, rewards, quests, and live content were found. |

## Component Flow

The HUD data component can find these components on the same actor:

- `UHuwamCombatComponent`
- `UHuwamInventoryComponent`
- `UHuwamRewardComponent`
- `UHuwamQuestComponent`
- `UHuwamLiveContentComponent`

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
| Gold | `Snapshot.Wallet.GoldBalance` |
| XP | `Snapshot.Wallet.TotalExperience` |
| Inventory slot count | `Snapshot.Inventory.UsedSlots / Snapshot.Inventory.MaxSlots` |
| Active quest | First entry in `Snapshot.ActiveQuestSummaries` |
| Quest progress | `CurrentValue / TargetValue` |
| Turn-in marker | `bReadyToTurnIn` |

## Slime Quest Example

After starting `Slimes in the Tall Grass`, defeating one Basic Slime, and claiming its reward:

| Snapshot Field | Expected |
|---|---:|
| GoldBalance | 3 |
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
- HUD data component.
- Helper functions for the first slime quest loop.

See `Docs/PrototypePlayerActor.md`.
