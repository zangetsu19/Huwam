# Survival Vitals Prototype

This layer starts the survival and urgency loop for players.

## Goal

Players should care about everyday needs without turning the game into a chore:

- Hunger.
- Thirst.
- Social energy.
- Food freshness.
- Living-server time.

This first pass gives the systems a real backbone while keeping the numbers easy to tune.

## Runtime Component

Added component:

- `UHuwamSurvivalVitalsComponent`

Snapshot:

- `FHuwamSurvivalVitalsSnapshot`

Need states:

- Full.
- Comfortable.
- Low.
- Critical.
- Empty.

## First Drain Rules

Each in-game day currently drains:

| Need | Drain |
|---|---:|
| Hunger | 12 |
| Thirst | 20 |
| Social energy | 4 |

This uses Huwam living-server time. Sleeping does not skip the world clock.

## First Food Rules

Eating from inventory checks the stack's freshness.

| Freshness | Hunger Restored | Extra Result |
|---|---:|---|
| Fresh | 25 | Good food. |
| Stale | 10 | Helps, but weakly. |
| Spoiled | 0 | Unsafe food flag. |
| Stable | 15 | Preserved/basic food. |

Eating removes one item from inventory.

## HUD And Save

The prototype HUD now exposes:

- Hunger percent.
- Thirst percent.
- Social percent.
- Active survival effect count.

The prototype save file now stores survival vitals.

## First Consequences

Empty needs now create active effect IDs:

| Effect ID | Cause |
|---|---|
| `effect.survival.starving` | Hunger is empty. |
| `effect.survival.dehydrated` | Thirst is empty. |
| `effect.survival.isolated` | Social energy is empty. |
| `effect.survival.unsafe_food` | Last consumed food was spoiled. |

`ApplySurvivalConsequences` can apply true damage through the combat component:

| Condition | Damage |
|---|---:|
| Starving | 5 |
| Dehydrated | 10 |
| Unsafe food | 3 |

Social isolation is exposed as an effect but does not damage health in this pass.

## First Town Services

Survival services restore needs without skipping Huwam's living server clock.

| Service | Effect |
|---|---|
| Tavern meal | Restores hunger heavily and social energy slightly. Clears unsafe-food flag. |
| Well water | Restores thirst. |
| Campfire chat | Restores social energy. |
| Inn room | Restores a smaller amount of hunger, thirst, and social comfort. |

These services do not advance in-game days. They are player actions inside the current server time.

Placed survival service actors can now charge copper and expose owner/faction metadata. Tavern meals cost `8` copper, inn rooms cost `20` copper, and the current well/campfire services are free public services.

## Current Boundary

This pass does not yet:

- Apply deeper sickness, poison, morale, or relationship penalties from survival effects.
- Connect cooking, beds, or relationships.
- Drive mate-finding or social reputation.
- Send paid service revenue into an NPC wallet or settlement ledger.

Those are now straightforward follow-up layers.
