# Live Content Prototype

This layer prepares Huwam for future DLC, paid cosmetics, pets, mounts, tames, story arcs, and expansion packs.

## Goal

Future content should be added as data-driven content packs where possible, not as hardcoded one-off changes.

The first live content layer answers:

- Does this content pack exist?
- Is it active?
- Is it enabled by the server?
- Does the player own the required entitlement?
- Is the player/client version compatible?
- Are required feature flags enabled?

`UHuwamLiveContentComponent` handles this first access check.

## Content Pack Data

`DT_ContentPacks.csv` uses `FHuwamContentPackRow`.

| Field | Purpose |
|---|---|
| ContentPackId | Stable pack ID. |
| DisplayName | Player or admin-facing name. |
| PackType | Cosmetic, companion, gameplay, world, event, or system. |
| PackState | Draft, staged, active, paused, or retired. |
| Version | Pack version. |
| RequiredGameVersion | Minimum compatible game version. |
| EntitlementId | Store or account ownership ID. |
| bEnabledByDefault | Whether the server enables the pack automatically. |
| bRequiresOwnership | Whether the player must own an entitlement. |
| bCosmeticOnly | Whether the pack avoids gameplay power changes. |
| ContentIds | Future list of item, pet, mount, quest, spell, or world IDs. |
| FeatureFlagIds | Future server flags required for access. |
| Description | Notes for design/admin use. |

## Starter Packs

| Pack | Type | State | Purpose |
|---|---|---|---|
| `content.pack.starter_core` | System | Active | Core launch content for all players. |
| `content.pack.cosmetic.dragonhide_ember` | Cosmetic | Staged | Future paid cosmetic armor set. |
| `content.pack.pet.cleaner_slime` | Companion | Staged | Future paid non-combat slime pet. |
| `content.pack.mount.snowstag` | Companion | Staged | Future paid mount. |
| `content.pack.gameplay.archers_guild` | Gameplay | Draft | Future Archers Guild expansion. |
| `content.pack.world.dawnspire` | World | Draft | Future Dawnspire story/magic expansion. |

## Runtime Component

`UHuwamLiveContentComponent` stores:

- Current game version.
- Server-enabled content pack IDs.
- Server-disabled content pack IDs.
- Enabled feature flag IDs.
- Player-owned entitlement IDs.

It exposes:

- `CanUseContentPack`
- `EnableContentPack`
- `DisableContentPack`
- `EnableFeatureFlag`
- `DisableFeatureFlag`
- `GrantEntitlement`
- `RevokeEntitlement`
- `GetKnownContentPacks`
- `GetAccessibleContentPackIds`

## Access Rules

A pack is usable only when:

1. The pack row exists.
2. `PackState` is `Active`.
3. The server has enabled the pack, either through `bEnabledByDefault` or `EnableContentPack`.
4. The current game version is at least the required game version.
5. Required feature flags are enabled.
6. The player owns the entitlement, unless ownership is not required.

This lets Huwam stage content before release, activate it later, pause broken content, or roll access back without deleting player data.

## No-Downtime Strategy

The long-term no-downtime flow should be:

1. Patch or preload assets on clients.
2. Add content pack data to the server manifest.
3. Leave the pack as `Staged`.
4. Verify compatibility.
5. Flip the pack to `Active`.
6. Enable the server feature flag.
7. Grant or validate player entitlements.

For pure data changes, the server can use the manifest immediately once it reloads or receives the updated data. For new meshes, textures, animation, or audio, clients still need the assets installed before the pack is activated.

## Marketable DLC Rules

- Cosmetic content should stay cosmetic.
- Paid gameplay content should be clearly marked as gameplay or world DLC.
- Any paid legendary-style gear should have an earnable or craftable equivalent if balance matters.
- Store entitlements should be checked server-side.
- Disabling a pack should stop new use without deleting owned history.

## Next Step

The live content layer is ready for future expansion planning.

The next practical gameplay prototype step remains the HUD/menu data adapter:

- Health and mana.
- Gold and experience.
- Inventory stacks.
- Active quest objective progress.
- Ready-to-turn-in indicators.
- Accessible content pack status for admin/debug menus.

That adapter is now started. See `Docs/HudMenuDataPrototype.md`.
