# Economy Ledger Prototype

This layer starts the money trail for the living economy.

## Added Subsystem

- `UHuwamEconomyLedgerSubsystem`

The subsystem records in-memory prototype transactions so paid services can feed later NPC wealth, guild funds, taxes, settlement quality, and market pressure.

## Ledger Entry

Each entry stores:

- Transaction ID.
- Source tag.
- Owner NPC ID.
- Owning guild or faction ID.
- Settlement ID.
- Copper amount.
- Owner, faction, and settlement copper shares.
- In-game day.
- Human-readable note.

## Account Balances

Service revenue is also split into spendable prototype accounts:

| Account | First Split Rule |
|---|---|
| Owner NPC wallet | Keeps the remaining service income after guild/faction and settlement shares. |
| Guild/faction fund | Receives about one fifth of paid service income when a group owns the service. |
| Settlement treasury | Receives about one tenth of paid service income, with a minimum `1` copper on small paid services. |

For the current `8` copper tavern meal, the split is:

| Account | Copper |
|---|---:|
| Merchant owner wallet | 6 |
| Merchants Guild fund | 1 |
| Eldoria treasury | 1 |

The subsystem can spend from NPC wallets, faction funds, and settlement treasuries. This is a first balance layer for wages, repairs, taxes, bribes, service upkeep, and future market pressure.

## Service Upkeep

The ledger can now pay service upkeep in priority order:

1. Owner NPC wallet.
2. Owning guild/faction fund.
3. Settlement treasury.

If none of those accounts can afford the requested upkeep, the payment fails. Survival service actors can use that result to mark themselves underfunded and close until money returns.

Closed or underfunded service actors now expose a supply-style pressure state:

- Need ID: `need.service.<service-source>.upkeep`
- Owner NPC: the service owner.
- Urgency: urgent or critical depending on whether the service is still usable.
- Next action: request player help.

That pressure state is the bridge toward repair quests, material requests, public-work funding, and shortage escalation.

## Restoration Quest Generation

`UHuwamQuestComponent` can now turn a service-pressure state into a prototype restoration quest offer.

The generated quest uses:

- Quest ID prefix: `quest.eldoria.service_restoration.`
- Quest giver: the service owner NPC.
- Objective progress source: the service pressure need ID.
- Urgency mapped from the pressure urgency.
- Need context copied into the quest summary.

This is the first bridge from economy failure into player-visible repair work.

## Restoration Turn-In Effect

Survival service actors can now accept a turned-in restoration quest result that matches their service pressure need ID.

When the completed quest matches:

- The service is marked funded again.
- The service is re-enabled.
- The underfunded flag is cleared.

This closes the first loop from service failure to player repair work to restored public service.

## Prototype Save

`UHuwamPrototypeSaveGame` now stores:

- Economy ledger entries.
- NPC wallet, guild/faction fund, and settlement treasury account balances.
- Survival service actor funding/enabled state snapshots.

Loading a prototype save restores those economy records through `UHuwamEconomyLedgerSubsystem`, so the living economy does not forget paid service activity between sessions.

## HUD/Menu Data

`UHuwamHudDataComponent` now exposes:

- Economy transaction count.
- Total recorded revenue.
- NPC wallet, guild/faction fund, and settlement treasury balances.
- Focused survival service pressure when the player is aiming at a service actor.

The native prototype HUD shows ledger count and total revenue beside the wallet line.

Current prototype upkeep seeds:

| Service | Upkeep |
|---|---:|
| Tavern meal | 2 copper |
| Town well | 1 copper |
| Campfire chat | 1 copper |
| Inn room | 4 copper |

## Current Integration

Paid survival service actors now record revenue when payment succeeds.

The first source tags are:

| Service | Source Tag |
|---|---|
| Tavern meal | `survival.service.tavern_meal` |
| Inn room | `survival.service.inn_room` |
| Well water | `survival.service.well_water` |
| Campfire chat | `survival.service.campfire_chat` |

Only paid services create revenue records right now. Free public services still restore needs, but they do not create ledger income.

## Query Helpers

Blueprint and validation code can ask for:

- Full ledger snapshot.
- Revenue by NPC.
- Revenue by guild/faction.
- Revenue by settlement.

## Current Boundary

The ledger records that money moved, but it does not yet:

- Apply taxes, rent, wages, or service upkeep.
- Change prices or shortages based on revenue.
- Persist ledger/account state into the prototype save.

Those are the next economy layers.
