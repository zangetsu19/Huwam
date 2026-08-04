# Huwan Currency Prototype

This layer moves the playable prototype away from a gold-only wallet.

## Official Denominations

| Denomination | Exchange |
|---|---|
| Copper | Smallest exact runtime value. |
| Silver | `1000` copper. |
| Gold | `1000` silver. |
| Platinum | `1000` gold. |
| Marquis Diamond Print | `10000` platinum. |

One Marquis Diamond Print equals `10000000000000` copper.

## Marquis Diamond Prints

Marquis Diamond Prints are ultra-high-value currency instruments:

- A full diamond card.
- Stamped with the local Marquis House stamp.

That makes the print valuable and politically legible at the same time. High-level trade, land deals, noble wealth, kingdom treasuries, banks, legendary contracts, and expensive player power can all use them later.

## Runtime Rule

The prototype stores wallet value as one `int64` copper total.

`UHuwamRewardComponent` exposes:

- `AddCopper`
- `SpendCopper`
- `GetCurrencyBalanceCopper`
- `GetCurrencyBreakdown`

`FHuwamCurrencyBreakdown` converts that exact copper total into Marquis Diamond Prints, platinum, gold, silver, and copper.

Gold helper functions still exist as compatibility shims for old prototype code while content moves to copper-native values.

## Content Fields

The first CSV/runtime fields now favor copper:

- Items use `BaseValueCopper`.
- Monster rewards use `CurrencyRewardCopper`.
- Quest rewards use `RewardCopper`.

Using copper at the data layer lets every display format the same exact value without rounding away smaller denominations.

## Current Prototype Values

The early slime and starter quest reward numbers now read as copper for the current slice:

- Basic Slime defeat currency: `3` copper.
- Slimes in the Tall Grass turn-in currency: `15` copper.
- Gather Field Supplies turn-in currency: `12` copper.
- Tavern meal service: `8` copper.
- Inn room service: `20` copper.
- Town well and campfire chat services: free public services for now.

These are starter balancing seeds, not final prices.

## Main Files

- `Source/Huwam/Data/HuwamDataTypes.h`
- `Source/Huwam/Gameplay/HuwamRewardComponent.h`
- `Source/Huwam/Gameplay/HuwamRewardComponent.cpp`
- `Source/Huwam/Gameplay/HuwamHudDataComponent.h`
- `Source/Huwam/Gameplay/HuwamHudDataComponent.cpp`
- `Content/Data/DT_Items.csv`
- `Content/Data/DT_Monsters.csv`
- `Content/Data/DT_Quests.csv`

## Current Boundary

The first currency foundation exists, but later economy work still needs:

- Wallet UI that chooses compact or expanded denomination displays.
- Bank storage, exchange services, treasury records, tax ledgers, and theft rules.
- Market pricing in denominations beyond starter base values.
- Physical coin and print loot visuals and inventory rules if money becomes an itemized object.
