# Combat Prototype

This layer gives Huwam its first working damage loop.

## Goal

Starter combat needs to answer:

- How much power does the attacker bring?
- How much armor or resistance does the defender bring?
- Did the roll add a little variance?
- Did the attack critically hit?
- How much health remains?

`UHuwamCombatComponent` handles that first pass.

## Components Involved

| Component | Role |
|---|---|
| `UHuwamCharacterStatsComponent` | Provides health, mana, melee power, ranged power, spell power, armor, utility, and social power. |
| `UHuwamEquipmentComponent` | Provides equipment attack, defense, mana, and utility totals through the stat component. |
| `UHuwamCombatComponent` | Stores current health and mana, resolves attacks, applies damage, heals, and spends or restores mana. |

## Attack Types

| Attack Type | Power Source | Mitigation |
|---|---|---|
| Melee | `MeleePower` | Half of defender armor after armor pierce. |
| Ranged | `RangedPower` | Half of defender armor after armor pierce. |
| Spell | `SpellPower` | One quarter of defender armor after armor pierce. |
| True Damage | Best of melee, ranged, or spell | Ignores armor. |

## Roll Config

`FHuwamCombatRollConfig` controls one attack:

| Field | Purpose |
|---|---|
| FlatBonus | Extra power from skills, abilities, positioning, quest effects, or GM/moderator events. |
| VarianceMin | Lowest random damage roll. |
| VarianceMax | Highest random damage roll. |
| ArmorPierce | Reduces defender armor before mitigation. |
| CriticalChancePercent | Percent chance to critically hit. |
| CriticalMultiplierPercent | Damage multiplier when the attack critically hits. |

## V0.1 Damage Formula

1. Pick attack power from the attack type.
2. Add `FlatBonus`.
3. Add a random roll between `VarianceMin` and `VarianceMax`.
4. Apply critical multiplier if the crit roll succeeds.
5. Calculate mitigation from defender armor.
6. Final damage is `RawDamage - DamageMitigated`, with a minimum of `1` when raw damage is above `0`.
7. Subtract final damage from current health.

## Runtime State

`UHuwamCombatComponent` tracks:

- `CurrentHealth`
- `CurrentMana`
- `bDefeated`

It can:

- Initialize meters from current stats.
- Heal.
- Spend mana.
- Restore mana.
- Apply already resolved damage.
- Attack another combat component.

## First Smoke Test

With two default Progression Zero test characters:

1. Add `UHuwamCharacterStatsComponent`, `UHuwamEquipmentComponent`, and `UHuwamCombatComponent` to both.
2. Give the attacker a Basic Sword.
3. Initialize combat meters.
4. Call `AttackTarget` using `Melee`.

Expected rough result:

| Value | Expected |
|---|---:|
| Attacker MeleePower | 15 |
| Defender ArmorRating | 10 |
| Base Mitigation | 5 |
| Damage Range Before Crit | 10 to 14 |

The exact result can vary because the default roll adds `0` to `4`, and critical hits have a default `5%` chance.

## Basic Slime Step

The first basic enemy encounter layer is now started:

- Spawn or place a Basic Slime.
- Give it stats and combat.
- Let the player attack it.
- Let it attack back.
- Trigger a simple defeat reward hook.

See `Docs/BasicSlimeEncounterPrototype.md`.
