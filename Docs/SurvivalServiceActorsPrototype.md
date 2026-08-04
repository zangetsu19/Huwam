# Survival Service Actors Prototype

This layer makes survival services usable as world objects.

## Added Actor

- `AHuwamSurvivalServiceActor`

The actor can represent:

- Tavern meal.
- Well water.
- Campfire chat.
- Inn room.

Each actor exposes:

- Service type.
- Prompt text.
- Whether the prototype player can use it.
- Copper cost.
- Owner NPC ID.
- Owning guild/faction ID.
- Access policy.
- Before/after survival snapshots when used.

## Service Economy

The first Eldoria service prices are:

| Service | Cost | Owner | Owning Group | Access |
|---|---:|---|---|---|
| Tavern meal | 8 copper | `npc.eldoria.market_merchant_01` | `guild.merchants` | Public |
| Town well | 0 copper | `npc.eldoria.guard_01` | `guild.knights` | Public |
| Campfire chat | 0 copper | `npc.eldoria.robin_quickfoot` | `guild.archers` | Public |
| Inn room | 20 copper | `npc.eldoria.market_merchant_01` | `guild.merchants` | Public |

Paid services check `UHuwamRewardComponent` before use and spend copper on success.

Successful paid uses also create `UHuwamEconomyLedgerSubsystem` revenue entries so NPC, guild/faction, and settlement economy systems can read service income later.

Service actors now also have upkeep costs. When upkeep cannot be paid from the owner wallet, owning group fund, or settlement treasury, the actor can mark itself underfunded and close.

Underfunded service actors expose a supply-style need state. This gives future quest, repair, and NPC simulation layers a clear reason to ask players for help restoring the service.

When a matching restoration quest is completed and turned in, the service actor can apply that quest result to clear underfunding and reopen.

Placed service actors now expose a save-state snapshot with service identity, owner/faction/settlement, pricing, upkeep, enabled state, and underfunded state. Prototype saves can restore that state by matching service identity.

Eldoria's blockout services now have authored stable instance IDs:

| Service | Instance ID |
|---|---|
| Tavern meal | `service.eldoria.tavern_meal.01` |
| Town well | `service.eldoria.town_well.01` |
| Campfire chat | `service.eldoria.campfire_chat.01` |
| Inn room | `service.eldoria.inn_room.01` |

The access policy can currently be:

- Public.
- Guild or faction member.
- Owner only.
- Locked.

Guild/faction access checks the created character's selected class and job guild IDs. This keeps the service actor ready for guild baths, private inns, faction safehouses, noble kitchens, and restricted camp services later.

## World Interaction

`UHuwamWorldInteractionComponent` now recognizes survival service actors.

When focused, they appear as:

- Interaction kind: `SurvivalService`
- Outcome: `SurvivalServiceUsed`

## Eldoria Blockout

The prototype slice now spawns four first service points in Eldoria:

- Tavern meal point.
- Town well.
- Campfire chat point.
- Inn room point.

They are still simple blockout cubes, ready to be replaced by authored 3D assets.

## Rule Preserved

Using these services does not skip time.

They restore needs during the current living-server day instead of advancing the shared Huwam clock.
