# Huwam Race 3D Art Bible

This document converts the playable race lore into 3D character art direction for Unreal production.

The goal is to make every race readable from a third-person MMORPG camera before armor, weapons, jobs, or class effects are added. A player should be able to see a silhouette across a street in Eldoria and immediately think, "That is Human," "That is Demon," "That is Beastfolk," and so on.

## Art Production Order

Humans stay at the top because they are the playable baseline. Every other race should be measured against them.

1. Humans
2. Elves
3. Dwarves
4. Orcs
5. Demons
6. Beastfolk
7. Fae
8. Special playable lineages and evolvable races

## Global 3D Character Rules

- Huwam is high-graphics, 3D, and third-person.
- Every race needs a strong first-read silhouette.
- Every playable race needs body sliders, face sliders, skin and hair customization, and equipment compatibility.
- Armor, clothing, hair, weapons, backpacks, pouches, jewelry, and guild gear should fit race-specific anatomy.
- Race differences should be visible even in starter clothes.
- Sub-races should usually reuse a shared race skeleton when possible, but can add modular mesh parts.
- Expensive custom skeletons should be saved for races with major anatomy changes.
- Player-facing races need clean animation support for idle, walk, run, sprint, jump, swim, climb, combat, gathering, crafting, emotes, mounted movement, and conversation.
- Foreground NPCs need expressive faces and body language because they will have deep AI conversations.
- Background NPCs can use lighter animation and voice-loop support.
- Mature character customization can include body proportions and nudity settings, but production assets should keep gameplay systems non-explicit.

## Race Roster

| Priority | Race Group | Primary Art Read | Major Visual Hooks |
| --- | --- | --- | --- |
| 1 | Humans | Baseline civilized all-rounders | Normal fantasy-human anatomy, broad clothing variety, authority silhouettes, flexible class reads. |
| 2 | Elves | Beautiful long-lived knowledge race | Long ears, refined posture, graceful bodies, deep-mana visual accents, forest or court identity. |
| 3 | Dwarves | Hard labor, craft, and fortress people | Shorter broad bodies, heavy hands, dense beards, tool belts, forge and stone motifs. |
| 4 | Orcs | Scary tough law, bounty, guard, and escort power | Green skin, tusks, large muscles, intimidating posture, heavy gear. |
| 5 | Demons | Misunderstood, feared, magical outcasts | Intricate horns, colored skin, claws, sharp teeth, deep mana glow, non-innocent face language. |
| 6 | Beastfolk | Animalistic people under social pressure | Upright animal anatomy, non-Human faces, animal skin/fur/scales/eyes, senses, tails, claws. |
| 7 | Fae | Dreamlike strange beings | Wings, hooves, plant bodies, odd proportions, glamour, whimsy, deformity, impossible details. |
| 8 | Special Lineages | Death, monster, divine, artificial, reincarnated paths | Undead bodies, slime forms, angelic radiance, homunculus seams, dragon traits, soul effects. |

## Current Character Data Roster

The prototype data currently exposes these race rows in `Content/Data/DT_Races.csv`:

1. Human
2. Elf
3. Dwarf
4. Orc
5. Demon
6. Beastfolk
7. Fae
8. Halfling

Art note: the World Bible treats Halfling as Fae-adjacent, while the prototype CSV currently lists Halfling as its own race row. That can work either way. If Halfling stays standalone, it needs a full base body and character creation path. If it folds under Fae, it can share more Fae customization and animation logic.

## Sub-Race Art Roster

### Humans

- Common Human
- High-Human
- Dawn Human
- Dusk Human
- Frontier Human
- Urban Human
- Warborn Human
- Half-Blood Human
- Blessed Human
- Cursed Human

### Elves

- High Elf
- Wood Elf
- Moon Elf
- Sun Elf
- Star Elf
- River Elf
- Root Elf
- Silver Elf
- Ironbark Elf
- Half-Elf
- Dark Elf

### Dwarves

- Mountain Dwarf
- Forge Dwarf
- Quarry Dwarf
- Deep Dwarf
- Goldbeard Dwarf
- Iron Oath Dwarf
- Dreamforge Dwarf
- Hearth Dwarf

### Orcs

- Common Orc
- Half-Orc
- War Orc
- Stonehide Orc
- Redtusk Orc
- Gray Orc
- Ironbound Orc
- Wild Orc

### Demons

- Common Demon
- Arch-Demon
- Lesser Demon
- War Demon
- Flame Demon
- Shadow Demon
- Succubus
- Incubus
- Peacebound Demon
- Horn-Crowned Demon

### Beastfolk

- Rabbitoid
- Wolfkin
- Werewolf
- Tigerfolk
- Lionfolk
- Panthera
- Foxfolk
- Bearfolk
- Birdfolk
- Serpentfolk
- Sharkfolk
- Horsefolk

### Fae

- Fairy
- Arch Fae
- Faun
- Halfling
- Sprite
- Briar Fae
- Moth Fae
- Mirror Fae
- Rootling Fae
- Dream Fae

### Special Playable Lineages

- Vampire
- Ghost
- Lich
- Skeleton
- Slime Monster
- Angelic
- Reincarnated
- Homunculus
- Dragon-Blooded
- Lani, protected Half-Elf/Half-Dwarf special NPC/player character

## Shared Unreal Asset Plan

| Asset Layer | Purpose |
| --- | --- |
| Base Body Mesh | Shared race body with morph targets for height, build, age range, muscle, fat, chest, hips, face, and mature customization sliders. |
| Head Mesh | Race-specific face shape, ear, jaw, nose, brow, teeth, horn, or muzzle support. |
| Modular Anatomy | Ears, horns, tusks, tails, wings, claws, hooves, beards, scales, fur patches, plant growth, undead bones, slime cores. |
| Skin Material | Race skin, pores, scars, tattoos, runes, mana glow, color variants, roughness, wetness, blood, dirt. |
| Hair and Beard | Human hair baseline, elven long hair, dwarven beard systems, demon horn-safe styles, beastfolk fur grooming. |
| Clothing Fit Layer | Starter underwear, cloth, leather, plate, robes, cloaks, hoods, pouches, backpacks, guild outfits. |
| Equipment Attachment Points | Weapon sheaths, bow/quiver sockets, pouch sockets, jewelry sockets, rings, earrings, backpacks, magical storage slot visuals. |
| Animation Profile | Shared locomotion plus race overrides for posture, gait, idle behavior, combat stance, emotes, and conversation gestures. |

## Human 3D Art Brief

Humans are the visual baseline of Huwan. They should feel familiar, grounded, and highly customizable, but not plain. Their strength is that they can become anything.

### Core Human Read

- Height: broad normal fantasy-human range.
- Build: slim, average, athletic, heavy, noble-soft, battle-hardened, worker-strong.
- Skin: realistic Human skin tones plus weathered, scarred, sun-touched, pale, freckled, and aged options.
- Hair: full baseline hair system with practical, noble, military, priestly, merchant, rural, and adventurer styles.
- Face: strongest face customization of any race because Humans are common and socially varied.
- Eyes: natural eye colors, no default Darkvision glow.
- Posture: adaptable and social, with variants for noble authority, labor, military discipline, priestly calm, merchant confidence, and frontier grit.
- Magic: subtle rather than extreme. Human mana visuals should usually come from class, gear, faith, or training, not raw birth power.

### Human Must-Haves

- All-Rounder visual identity.
- No natural Darkvision.
- Strong compatibility with every starter class and job.
- Wide clothing and armor compatibility.
- Clear social class range: poor district, common citizen, merchant, priest, soldier, noble, frontier worker, adventurer.
- Strong facial expression support for conversation-heavy NPCs.

### Human Must-Nots

- Do not give normal Humans elf ears, horns, tusks, animal faces, claws, natural wings, or glowing darkvision eyes.
- Do not make them visually superior to every other race by default.
- Do not make every Human noble, clean, or heroic.

## Human Sub-Race Visual Direction

These Human baselines are approved for the prototype character creation direction. Common Human is the plain playable baseline. Half-Blood Human remains a side project because every Human-plus-race mixture can become its own art branch. The Human-Elf branch is now approved as the Half-Elf bridge and should be reachable from both Human and Elf character creation paths.

| Sub-Race | Approved 3D Visual Direction | Prototype Character Creation Baseline |
| --- | --- | --- |
| Common Human | Typical level 1 adventurer with worn leather, very soft cloth, real forged short sword texture, and no special birth effects. | Default Human player preview and baseline for all future Human outfit proportions. |
| High-Human | Alabaster skin, smug noble persona, fine cloths and fabrics, controlled magic-user stance, and clean expensive presentation. | Noble mage preview with high-quality fabric material callouts, magic-user animation idle, and refined social posture. |
| Dawn Human | Yellowish warm skin, cheerful/happy demeanor, bright holy garments, and a slight radiant holy aura. | Holy-support preview with white and gold cloth, gentle aura VFX, and friendly expression presets. |
| Dusk Human | Dark skin, stalwart fierce look, slight void aura, knight armor, greatsword, and kite shield. | Tank/guard preview with shield stance, heavy armor material baseline, and controlled void-edge VFX. |
| Frontier Human | Slight red sunburnt skin, farmer/ranger look, wild predator-watchful eyes, rugged field tools, and worn rural cloth. | Ranger/farmer preview with weathered cloth, work pouches, hunting tools, and alert idle posture. |
| Urban Human | Slightly dirty city skin, softer or plumper merchant body type, street-market clothing, ledgers, pouches, and coin tools. | Merchant preview with trade props, softer body slider example, and market-social expression set. |
| Warborn Human | Dark tanned skin, wild fierce nearly manic look, scars, battered gear, and barbarian/guard/war-knight energy. | Warfighter preview with scars, battle-worn metal/leather, aggressive idle, and weapon-ready posture. |
| Half-Blood Human | Human mixed with another race. | Side project overall. Human-Elf is now locked as the Half-Elf hybrid baseline; the other Human-plus-race branches still wait for their parent race art. |
| Blessed Human | Ebony skin, pure white hair, white eyes, radiant holy aura, pure white fabrics, floating pose, wings, and deity tattoos. | Rare divine preview with wings, floating animation pose, holy tattoo materials, and radiant aura VFX. |
| Cursed Human | Dark tanned skin with sickly purple tint, pure black eyes, purple curse aura, heavy curse tattoos, and rogue styling. | Rogue/antihero preview with black-eye material, purple curse VFX, tattoo overlays, and sun-averse expression set. |

### Human Prototype UI Baselines

The character creation UI should use the approved Human art reads as card-level language:

| Sub-Race | UI Hook | Preview Read |
| --- | --- | --- |
| Common Human | All-rounder adventurer | Worn leather, soft cloth, used short sword. |
| High-Human | Noble magic user | Alabaster skin, fine fabrics, smug spellcaster posture. |
| Dawn Human | Holy optimist | Yellow-warmed skin, bright garments, gentle holy aura. |
| Dusk Human | Void-touched tank | Dark skin, knight armor, greatsword, kite shield. |
| Frontier Human | Predator-alert survivor | Reddish sunburnt skin, farmer/ranger tools, wild eyes. |
| Urban Human | Streetwise merchant | Slightly dirty skin, softer body, trade tools. |
| Warborn Human | Battle-happy frontliner | Dark tan skin, scars, fierce manic war energy. |
| Half-Elf | Hybrid adventurer | Very short elf ears, sword gear, charismatic Human-Elf bridge. |
| Blessed Human | Rare divine-touched | Ebony skin, white hair and eyes, wings, holy tattoos. |
| Cursed Human | Rogue curse-bearer | Purple-tinted skin, black eyes, tattoos, curse aura. |

The first prototype screen does not need final 3D preview meshes yet, but its option cards should already describe the final intended visual identity so later Widget Blueprint work can swap text-only cards for rendered previews without changing the selection logic.

### Approved Human Material And VFX Notes

| Sub-Race | Skin And Face | Clothing And Armor | Weapons And Props | VFX And Animation |
| --- | --- | --- | --- | --- |
| Common Human | Normal Human tones, grounded faces, no supernatural marks. | Worn starter leather over very soft woven cloth; no waxy fabric. | Used short sword with real forged steel texture, edge nicks, scratches, and mild shine. | Basic adventurer idle, neutral confidence, no aura. |
| High-Human | Alabaster skin, smug expressions, polished grooming. | Fine fabrics: silk, velvet, linen, brocade, embroidery, refined belts, polished boots. | Spell focus, wand, or small ceremonial dagger; no dirty tools. | Controlled noble spell idle with restrained white/gold magic. |
| Dawn Human | Warm yellowish skin tint, cheerful face language. | Bright holy garments, white/gold cloth, sacred trims, clean travel layers. | Holy focus, prayer beads, simple pouches. | Gentle holy aura, soft sacred particles, blessing gestures. |
| Dusk Human | Dark skin, fierce stalwart expression. | Knight armor, dark cloth padding, steel plate, chainmail, worn leather straps. | Greatsword and kite shield with realistic scratches and dents. | Subtle void aura, shield-ready tank posture. |
| Frontier Human | Slight red sunburn/windburn, wild watchful eyes. | Farmer-ranger workwear, patched cloth, rugged leather, muddy boots. | Bow, hatchet, gathering knife, hoe/sickle, waterskin, pouches. | Alert predator-watch idle and practical field stance. |
| Urban Human | Slightly dirty city skin, market grime, softer/plumper body options. | Respectable trade clothing, vests, aprons, coats, stained cuffs, polished-but-worn shoes. | Ledger, coin pouch, keys, small scales, contracts. | Social merchant idle, bargaining hand gestures. |
| Warborn Human | Dark tan skin, scars, fierce almost manic eyes. | Battered leather, dented iron, torn war cloth, rough chain, battlefield tokens. | Sword, axe, mace, longsword, or small shield. | Aggressive weapon-ready idle and battle-hungry expression set. |
| Blessed Human | Ebony skin, white hair, white eyes, deity tattoos. | Pure white robes, silk, linen, gauze-like drapes, pale-gold clasps. | Holy focus or open-palm casting; no mundane weapon emphasis. | Floating pose, divine wings, radiant holy aura, glowing tattoo materials. |
| Cursed Human | Dark tan skin with sickly purple tint, pure black eyes, curse tattoos. | Dark rogue gear, hooded cloak, soft black cloth, cracked leather, hidden pockets. | Dagger, short sword, throwing knives, lockpick roll. | Purple curse aura, smoky shadow wisps, sun-averse rogue posture. |

### Prototype Character Creation Asset Slots

The first UI can start as text cards, but the production-ready character creation screen should reserve these slots for every Human sub-race:

- Preview mesh slot: male/female body preview with body slider support.
- Material preset slot: skin, eyes, tattoos, hair, cloth, leather, metal, and aura materials.
- Outfit preset slot: first readable outfit that communicates the sub-race fantasy.
- Prop slot: weapon, shield, focus, ledger, tool, pouch, or other identifying prop.
- VFX slot: aura, glow, curse smoke, holy particles, void edge, or no effect.
- Animation idle slot: noble casting, blessing, tank guard, predator-watch, merchant, war-ready, floating, rogue.
- UI card hook: one short phrase from the Human Prototype UI Baselines table.
- Lore tooltip: one short tooltip explaining why the sub-race looks and behaves that way.

Half-Blood Human should eventually become a matrix rather than one option. The Human-Elf branch is approved as Half-Elf: very short elf ears, tall attractive adventurer bodies, muscular swordfighter male baseline, long golden-haired female baseline, and mixed-culture charisma. Example future branches still include Human-Dwarf, Human-Orc, Human-Demon, Human-Beastfolk, Human-Fae, and rare hidden mixtures. That work should wait until each parent race has an approved art baseline.

## Human Starter Outfit Needs

| Outfit Family | Use |
| --- | --- |
| Progression Zero Clothes | Starter/noob look after character creation and tutorial. |
| Common Town Clothes | Baseline NPC population and player casual wear. |
| Poor District Clothes | Worn, patched, practical, lower wealth. |
| Noble Clothes | High-Human, authority, courts, Marquis-connected NPCs. |
| Frontier Workwear | Farmers, gatherers, builders, hunters, road workers. |
| Guild Starter Gear | Adventurers, Merchants, Church, Thieves, Knights, Archers, Mages. |
| Combat Starter Gear | Basic leather, cloth, chain, plate, robe, cloak, hood, boots, gloves. |

## First Human Model Targets

| Target | Purpose | Notes |
| --- | --- | --- |
| Human Base Body | Proves the full customization baseline. | Needs morph targets and gear compatibility. |
| Human Base Head | Proves face sliders and AI conversation expression quality. | Must support age, scars, makeup, facial hair. |
| Progression Zero Outfit | First player outfit in Laucian's tutorial lab. | Should be simple, neutral, and upgrade-friendly. |
| Eldoria Commoner Outfit | First city NPC population look. | Needs color and wear variations. |
| High-Human Noble Outfit | Proves Human authority identity. | Marquis and noble house direction starts here. |
| Frontier Human Outfit | Proves poor district, farms, and F-rank gathering loops. | Should pair with tools and pouches. |
| Warborn Human Outfit | Proves guard, officer, knight, and militia look. | Works with basic sword, shield, and armor progression. |

## Human Art Questions For The Next Pass

These are the choices that matter before generating or modeling the first Human concept.

1. Should Humans in Huwam lean realistic medieval fantasy, anime-realistic, or stylized high-fantasy?
2. Should Human starter clothing look more like poor commoner clothes, neutral adventurer clothes, or clean tutorial-lab issued clothes?
3. Should High-Humans look obviously noble by body/face, or mostly by clothing, hair, and posture?
4. Should Blessed and Cursed Humans be available at character creation, or locked behind rare birth/event rules?

## Next Race

After Humans are approved, the next art pass should be Elves.

Elf art needs to solve:

- Ear shapes.
- Long-life facial aging.
- Deep mana visual language.
- High Elf, Wood Elf, Moon Elf, Sun Elf, Star Elf, River Elf, Root Elf, Silver Elf, Ironbark Elf, Half-Elf, and Dark Elf differentiation.
- Dark-Elf compatibility with Laucian, Kaneke, Dawnspire, stealth, mystery, and purple skin with white hair.

## Elf 3D Art Prep

Elves are the second playable race art family after Humans. Human art proved the baseline body, cloth, armor, face, and starter-gear direction. Elves should now prove beauty, long life, deep mana wells, knowledge culture, and forest-linked identity without becoming just "Humans with ears."

### Core Elf Read

- Height: usually taller or more elongated than Humans, but not exaggerated.
- Build: graceful, elegant, lean, refined, athletic, scholar-like, or forest-hardened depending on sub-race.
- Skin: broad fantasy-natural range plus sub-race-specific moonlit, sunlit, bark-touched, silver, river, star, dark-purple, and forest-warm options.
- Hair: long, clean, expressive, and culturally meaningful; white hair remains key for Dark Elves.
- Ears: unmistakably long Elf ears, with sub-race variation in length, angle, sharpness, jewelry, and scars.
- Eyes: beautiful and highly expressive, often carrying subtle mana depth; no default monster look.
- Posture: graceful, controlled, and old-soul confident.
- Mana: visible as refined aura, controlled glow, delicate runes, natural motes, starlight, moonlight, sunlight, water shimmer, or shadow-scholarly effects depending on sub-race.

### Elf Must-Haves

- Long life visual language.
- Deep mana well identity.
- Knowledge, education, or refined culture in the face, posture, clothing, or accessories.
- Forest living or nature-link visible somewhere in the race family.
- Beauty and elegance without making every Elf identical.
- Long ears as a core silhouette feature.

### Elf Must-Nots

- Do not make Elves look like Humans with tiny pointy ears.
- Do not make every Elf noble, delicate, or helpless.
- Do not make every Elf use the same skin, hair, robe, or bow silhouette.
- Do not copy Dwarven visual identity; Elf/Dwarf tension should be visually obvious through contrast.

### Elf Sub-Race Art Order

Dark Elf already has creator-defined anchor traits, but High Elf is the cleanest first render because it establishes the public Elf baseline.

1. High Elf
2. Wood Elf
3. Moon Elf
4. Sun Elf
5. Star Elf
6. River Elf
7. Root Elf
8. Silver Elf
9. Ironbark Elf
10. Half-Elf
11. Dark Elf

### Elf Prototype UI Baseline Draft

These are not final until user-approved through renders.

| Sub-Race | Draft UI Hook | Draft Preview Read |
| --- | --- | --- |
| High Elf | Refined academy noble | Elegant scholar-mage, polished robes, deep mana, old education. |
| Wood Elf | Forest archer-druid | Natural leathers, leaf cloth, bow, herbs, animal awareness. |
| Moon Elf | Dreamlit night caster | Pale moon tones, calm eyes, illusion/dream aura. |
| Sun Elf | Radiant public beauty | Warm light, visible pride, gold-white garments, social presence. |
| Star Elf | Cosmic diviner | Starlit eyes, astronomy ornaments, fate-reader calm. |
| River Elf | Waterway traveler | Flowing fabrics, river jewelry, water shimmer, trade/fishing tools. |
| Root Elf | Ancient grove guardian | Bark/leaf textures, old-tree markings, patient nature magic. |
| Silver Elf | Diplomatic social expert | Silver accents, polished etiquette, language and negotiation props. |
| Ironbark Elf | Hardy border defender | Tougher body, rough forest armor, bow discipline, patrol gear. |
| Half-Elf | Culture bridge | Human-Elf blend, softer ears, mixed clothing language. |
| Dark Elf | Mysterious introvert | Purple skin, white hair, stealth, quiet knowledge, shadow-mana restraint. |

### Elf First Render Recommendation

Start with **High Elf** male and female models.

The first High Elf render should establish:

- Long elegant ears.
- Deep mana without uncontrolled glow.
- Beautiful refined faces.
- Old academy or court clothing.
- Scholar-mage posture.
- Etta-linked knowledge identity.
- Real fabric and jewelry textures.
- No Human, Fae, or Demon anatomy leakage.

## Dwarf 3D Art Brief

Dwarves are the third playable race art family after Humans and Elves. The approved Dwarf renders establish Huwam's craft, labor, fortress, forge, stone, contract, dream, and hearth identity. They should read as Dwarves from a third-person camera even before armor or tools are equipped.

### Core Dwarf Read

- Height: shorter than Humans and Elves, with compact mass rather than fragile smallness.
- Build: broad shoulders, thick torso, sturdy hips, heavy forearms, strong hands, grounded legs, and work-shaped posture.
- Skin: realistic earth, stone, soot, forge-warm, cavern-pale, gold-lit, and hearth-warm variants.
- Hair and beard: dense beard identity is mandatory for normal Dwarves. Beard length, braids, rings, ash, dust, oil, gems, and oath bands should carry sub-race identity.
- Face: strong brow, broad nose, practical eyes, expressive work pride, and clear age/wear options.
- Hands: large, calloused, and believable for forging, mining, masonry, cooking, brewing, and tool use.
- Posture: planted, weighty, job-proud, stubborn, and work-ready.
- Mana: steady rather than flashy. Dwarf mana should show through runes, heated metal, dream-blueprints, stone resonance, or hearth warmth more often than raw aura.

### Dwarf Must-Haves

- Short, broad, dense silhouette.
- Beards as a core racial identity for normal Dwarves.
- Work and job identity visible in clothing, tools, hands, posture, or materials.
- Heavy craft, stone, metal, contract, food, tavern, and fortress motifs across the race family.
- Gear compatibility for tool belts, hammers, shields, axes, aprons, heavy boots, rings, beard ornaments, and backpacks.
- Animation weight that makes Dwarves feel grounded, stubborn, and strong.

### Dwarf Must-Nots

- Do not make Dwarves look like short Humans with no craft identity.
- Do not make normal Dwarves beardless.
- Do not copy Elf elegance or Human all-rounder proportions.
- Do not make every Dwarf a grim warrior; craft, trade, cooking, civic building, and comfort are just as important.
- Do not make Forge or Dreamforge effects so bright that they read as Demon or Fae magic.

### Approved Dwarf Visual Baselines

These Dwarf baselines are approved for prototype character creation and data wiring.

| Baseline | Approved 3D Visual Direction | Prototype Character Creation Baseline |
| --- | --- | --- |
| Dwarf Baseline | Short broad worker body, dense beard, heavy hands, sturdy boots, tool belt, practical cloth/leather, stone and metal material language. | Parent Dwarf race preview and skeleton/body reference for all normal Dwarf sub-races. |
| Mountain Dwarf | High-mountain miner, smith, and fortress builder with stone dust, heavy boots, dense beard, practical armor, pick/hammer details, and mountain-cold ruggedness. | Default classic Dwarf sub-race preview for stone, forge, mining, and fortress culture. |
| Forge Dwarf | Soot-worn smith with heat-darkened skin accents, forge apron, hammer/tongs, ember-lit metal, dense beard with singe marks or metal rings. | Smithing and furnace preview with restrained forge glow and craft animation idle. |
| Quarry Dwarf | Mason and road builder with stone chips, measuring tools, chisels, work gloves, dusted beard, civic construction posture, and durable work layers. | Builder/mason preview for roads, walls, town repair, and contractor loops. |
| Deep Dwarf | Cavern-adapted Dwarf with muted skin tones, darker gear, lamp or rune light, pressure-ready armor, compact mining kit, and guarded cave posture. | Dungeon, mine, sealed ruin, and underground route preview. |
| Goldbeard Dwarf | Wealth and contract Dwarf with gold beard ornaments, appraisal tools, contract scrolls, jeweler details, polished-but-heavy clothing, and merchant confidence. | Trade, appraisal, banking, and high-value gear preview. |
| Iron Oath Dwarf | Duty-bound Dwarf with oath bands, iron clasps, shield or contract tablet, disciplined armor, straight posture, and severe promise-keeper expression. | Defender, officer, oath contract, and law-of-work preview. |
| Dreamforge Dwarf | Ione-touched dream crafter with sleep-blue accents, unusual blueprint props, softened forge gear, subtle dream-rune light, and visionary maker expression. | Magical crafting, enchanter, artificer, and dream blueprint preview. |
| Hearth Dwarf | Tavern, food, brewing, comfort, and family-work Dwarf with warm cloth, apron, tankard or bread tools, hearth glow, friendly sturdy face, and homecraft details. | Tavern owner, baker, brewer, morale, food, and community preview. |

### Dwarf Prototype UI Baselines

The character creation UI should use these approved Dwarf art reads as card-level language:

| Sub-Race | UI Hook | Preview Read |
| --- | --- | --- |
| Mountain Dwarf | Stone and forge | Mountain miner-smith, fortress gear, dusted beard, heavy boots. |
| Forge Dwarf | Furnace smith | Soot, apron, hammer, ember-lit metal, heat discipline. |
| Quarry Dwarf | Mason builder | Chisels, measuring tools, road-work layers, civic construction stance. |
| Deep Dwarf | Cave delver | Dark tunnel gear, lamp or rune light, pressure-ready armor. |
| Goldbeard Dwarf | Contract appraiser | Gold beard rings, ledger, appraisal tools, polished heavy clothing. |
| Iron Oath Dwarf | Sacred contract guard | Oath bands, shield, iron clasps, disciplined duty posture. |
| Dreamforge Dwarf | Dream smith | Dream-blue runes, odd blueprint, inspired forge focus. |
| Hearth Dwarf | Hearthkeeper | Warm apron, bread or tankard props, tavern comfort, friendly sturdy face. |

### Approved Dwarf Material And VFX Notes

| Sub-Race | Skin And Face | Clothing And Armor | Weapons And Props | VFX And Animation |
| --- | --- | --- | --- | --- |
| Mountain Dwarf | Weathered mountain skin, stone dust, serious practical face. | Heavy boots, rugged cloth, reinforced leather, stone-gray metal. | Pick, hammer, shield, fortress builder tools. | Grounded mining idle, stone-sense gesture, no bright aura. |
| Forge Dwarf | Soot, heat flush, singed beard edges, focused eyes. | Forge apron, thick gloves, heat-marked leather, metal clasps. | Hammer, tongs, ingot, anvil-side prop. | Restrained ember light, hammer-check idle, heat shimmer near tools. |
| Quarry Dwarf | Dusty skin, stone-chip marks, patient builder expression. | Work gloves, civic work layers, measuring belts, reinforced knees. | Chisel, plumb line, mallet, measuring rod. | Blueprint-check idle, stone-tap gesture, dust motes. |
| Deep Dwarf | Cavern-muted tones, guarded eyes, low-light readability. | Dark work cloth, pressure armor, lamp harness, cave pouches. | Lantern, rune lamp, pick, short axe. | Subtle darkvision/rune light, cautious cave-listen idle. |
| Goldbeard Dwarf | Polished beard grooming, gold rings, calculating eyes. | Heavy merchant coat, gem clasps, rich-but-practical boots. | Ledger, coin scale, appraisal lens, contract case. | Appraisal gesture, coin-weight idle, minimal gold glint. |
| Iron Oath Dwarf | Severe promise-keeper face, beard oath bands. | Iron clasps, disciplined armor, dark contract cloth, shield straps. | Shield, contract tablet, oath hammer. | Oath-brace idle, shield-ready stance, faint iron rune. |
| Dreamforge Dwarf | Sleep-tired inspired eyes, soft dream-blue accents. | Forge gear with cloth wraps, blueprint case, rune trim. | Odd blueprint, dream-forged tool, stylus. | Subtle dream-blue rune drift, visionary planning idle. |
| Hearth Dwarf | Warm cheeks, friendly sturdy face, food-service wear. | Apron, warm cloth, tavern layers, brewer gloves. | Bread paddle, tankard, stew ladle, spice pouch. | Hearth warmth, welcome gesture, morale-building idle. |

### Prototype Character Creation Asset Slots

The first UI can keep using text cards, but production-ready Dwarf character creation should reserve:

- Preview mesh slot: short broad Dwarf body with strong beard customization.
- Material preset slot: skin, beard, hair, soot, dust, metal, stone, cloth, apron, rune, and hearth light materials.
- Outfit preset slot: workwear, armor, apron, merchant coat, construction layers, or tavern clothing.
- Prop slot: hammer, pick, chisel, shield, ledger, contract tablet, blueprint, tankard, bread paddle, or rune lamp.
- VFX slot: none, ember light, stone dust, rune lamp glow, gold glint, oath rune, dream-blue drift, or hearth warmth.
- Animation idle slot: mining, smithing, masonry measuring, cave-listening, appraisal, oath-guard, dream planning, hearth welcome.
- UI card hook: one short phrase from the Dwarf Prototype UI Baselines table.
- Lore tooltip: one short tooltip explaining the sub-race's work identity.

### First Dwarf Model Targets

| Target | Purpose | Notes |
| --- | --- | --- |
| Dwarf Base Body | Proves the short broad silhouette, body sliders, and shared gear fit. | Needs beard and heavy-hand compatibility from the start. |
| Dwarf Base Head And Beard | Proves the mandatory beard system and expressive work-pride faces. | Needs beard rings, braids, age, soot, dust, and scar support. |
| Mountain Dwarf Outfit | Proves classic Dwarf mining, fortress, and smith readability. | Good default selection for `race.dwarf.mountain`. |
| Forge Dwarf Outfit | Proves smithing VFX, apron, heat, and metal material style. | Should connect to blacksmith and crafting loops. |
| Quarry Dwarf Outfit | Proves civic builder and road repair identity. | Should connect to construction, repair, and contractor loops. |
| Goldbeard Dwarf Outfit | Proves trade, appraisal, banking, and contract visuals. | Should pair with economy and merchant systems. |
| Hearth Dwarf Outfit | Proves food, tavern, comfort, and community identity. | Should pair with NPC supply, food, and morale systems. |

## Orc 3D Art Brief

Orcs are the fourth playable race art family after Dwarves. The approved Orc renders establish a full range from classic green clan brutes to Half-Human bridges, war-painted fighters, stone-skinned tanks, blood-red duelists, quiet Gray Orc scholars, ironbound laborers, and tiny Wild Orc ambushers.

Protected Orc-linked characters stay separate: Ieke is a protected Legendary special baseline, and Raelae is a protected Half-Orc/Half-Elf ranger. Normal Orc sub-race art must not overwrite either of them.

### Core Orc Read

- Height: usually tall and physically imposing, except Wild Orcs, who are intentionally very short.
- Build: strong, muscular, dense, intimidating, and hard to knock down.
- Skin: sub-race color is a primary read. Common and Half-Orc stay green, War Orcs are brown-red, Stonehide Orcs are stone-gray, Redtusk Orcs are blood-red, Gray Orcs are gray, Ironbound Orcs are pink/dusty rose, and Wild Orcs are bright green.
- Face: tusks, heavy brows, broad jaws, rough teeth, strong noses, and visible Orc ancestry.
- Posture: threat-ready by default, with Gray Orc as the calm studious exception and Ironbound Orc as the exhausted resilient labor exception.
- Gear: crude iron, leather, clan marks, war paint, helmets, practical work gear, or scholar gear depending on sub-race.
- Mana: usually low to average and physical rather than refined. Visual identity should come from body, skin, tusks, gear, and posture before magical effects.

### Orc Must-Haves

- Tusks as a core silhouette feature.
- Visible physical power or survival grit.
- Strong sub-race color differentiation.
- Clan, labor, war, guard, or survival identity visible through markings and gear.
- Third-person readability from body shape, skin tone, tusks, and outfit.
- Enough facial variation to support ugly brutish, rugged attractive, bloodthirsty, studious, labor-worn, and cartoonish ambusher reads.

### Orc Must-Nots

- Do not copy Ieke for normal Orc baselines.
- Do not use Raelae's Half-Orc/Half-Elf ranger identity for ordinary Half-Orcs.
- Do not make every Orc a barbarian; guards, scholars, laborers, bridge-lineages, and ambushers matter too.
- Do not make Gray Orcs look bloodthirsty.
- Do not make Half-Orcs look like Half-Elves; this approved branch is half-Human.
- Do not sexualize female Orc baselines.

### Approved Orc Visual Baselines

These Orc baselines are approved for prototype character creation and data wiring.

| Baseline | Approved 3D Visual Direction | Prototype Character Creation Baseline |
| --- | --- | --- |
| Common Orc | Classic green clanned Orc from broad fantasy lore: tall, ugly, brutish, tusked, battle-ready, heavy brow, bulbous nose/jaw, crude iron/leather, clan scars, war paint, and battered weapon. | Default Orc preview for intimidation, guard, escort, and brute frontline identity. |
| Half-Orc | Half-Human Orc branch with green skin, smaller tusks, shorter height, leaner build, more Human facial structure, and more attractive rugged adventurer features. | Human-Orc bridge preview for mixed identity, approachable social tension, and adaptable adventurer paths. |
| War Orc | Brownish red skin, ugly Common-Orc-like face, much denser and more chiseled muscle, and mandatory war paint across face/body. | Combat training, war patrol, officer, bounty, and battle-discipline preview. |
| Stonehide Orc | Stone-colored gray/ash skin, unusual rocky hide texture, natural protrusions on brow/shoulders/forearms/back, and prominent metal armor. | Defensive tank, fortress guard, escort, and pain-tolerance preview. |
| Redtusk Orc | Blood-red skin, bloodshot eyes, red-stained or naturally reddish tusks, brutal ugly face, and constant fight-ready bloodthirsty posture. | Aggressive duelist, intimidation, melee pressure, and rage-forward preview. |
| Gray Orc | Gray skin, intelligent neutral persona, studious clothing, passive body language, scroll/satchel gear, and calm observant face. | Scholar-guard, peacekeeper, negotiator, and disciplined restraint preview. |
| Ironbound Orc | Pink/dusty-rose skin, somewhat pig-like Orc features, hard-working body, iron restraints or broken chain remnants, patched labor gear, and resilient dignity. | Labor history, survival, work endurance, and oppressed-lineage story preview. |
| Wild Orc | Very short green cartoonish build, oversized armored helmet, short tusks, small axes, and tall-grass ambush behavior. | Tiny ambusher, plains/canyon survival, scout, and chaotic field encounter preview. |

### Orc Prototype UI Baselines

The character creation UI should use these approved Orc art reads as card-level language:

| Sub-Race | UI Hook | Preview Read |
| --- | --- | --- |
| Common Orc | Clanned brute | Green skin, tusks, ugly face, tall build, clan scars, battered axe. |
| Half-Orc | Human-Orc bridge | Green skin, smaller tusks, shorter leaner build, more Human attractive features. |
| War Orc | War-painted fighter | Brown-red skin, dense chiseled muscles, ugly face, mandatory war paint. |
| Stonehide Orc | Stone-armored tank | Stone-gray hide, rocky protrusions, metal armor, heavy defensive stance. |
| Redtusk Orc | Bloodthirsty duelist | Blood-red skin, bloodshot eyes, red tusks, ready-to-fight posture. |
| Gray Orc | Studious neutral | Gray skin, intelligent eyes, calm passive stance, scrolls and satchel. |
| Ironbound Orc | Iron laborer | Pink pig-like Orc read, iron bands, patched labor gear, resilient worker posture. |
| Wild Orc | Tall-grass ambusher | Very short green build, armored helmet, small axe, crouched in tall grass. |

### Approved Orc Material And VFX Notes

| Sub-Race | Skin And Face | Clothing And Armor | Weapons And Props | VFX And Animation |
| --- | --- | --- | --- | --- |
| Common Orc | Green skin, ugly brutish face, heavy brow, large tusks, scars. | Crude iron plates, leather straps, clan cloth, heavy boots. | Battered axe, cleaver, club, clan scraps. | Battle-ready idle, intimidating stare, no refined aura. |
| Half-Orc | Green skin, smaller tusks, more Human face, rugged attractiveness. | Practical adventurer leather and iron, travel pouches, clan sash. | Sword, axe, dagger, or hand axe. | Guarded social idle, mixed-heritage confidence. |
| War Orc | Brown-red skin, ugly face, dense muscle, war paint. | Crude war armor, iron/leather, trophies, heavy boots. | Heavy axe, war club, blade. | Aggressive war-ready idle, painted battle ritual gestures. |
| Stonehide Orc | Stone-gray cracked hide, rocky protrusions, tusks. | Prominent battered metal armor, rivets, chain, leather underlayers. | Mace, shield, axe. | Heavy defensive stance, stone dust, no bright glow. |
| Redtusk Orc | Blood-red skin, bloodshot eyes, red tusk markings. | Red clan marks, crude iron/leather, scarred war cloth. | Axe, cleaver, heavy blade. | Forward fight-ready idle, snarling pressure, no gore. |
| Gray Orc | Gray skin, thoughtful face, controlled tusks. | Layered robes/leather, metal fittings, scholar-adventurer kit. | Scroll case, satchel, writing tools, staff or sheathed weapon. | Passive observant idle, reading/checking notes. |
| Ironbound Orc | Pink dusty-rose skin, broad flattened nose, worn eyes. | Patched labor clothing, work harness, iron bands or broken chain links. | Pick, mining hammer, hauling hook. | Exhausted but dignified idle, heavy labor posture. |
| Wild Orc | Bright green skin, short tusks, mischievous ugly face. | Oversized battered helmet, tiny armor plates, rough leather. | Small axes. | Crouched ambush idle, tall-grass pop-up movement. |

### Prototype Character Creation Asset Slots

The first UI can keep using text cards, but production-ready Orc character creation should reserve:

- Preview mesh slot: Orc base body plus Half-Orc, Wild Orc, and Stonehide shape variants.
- Material preset slot: skin color, tusks, scars, war paint, rocky hide, red eyes, iron bands, helmets, leather, metal, and grime.
- Outfit preset slot: clan gear, adventurer gear, war armor, metal tank armor, scholar kit, labor harness, or tall-grass ambusher gear.
- Prop slot: axe, cleaver, club, shield, satchel, scroll case, mining hammer, hauling hook, small axe, or helmet.
- VFX slot: mostly none; use dust, sweat, subtle eye irritation, stone grit, or ambient grass movement rather than flashy magic.
- Animation idle slot: brute guard, mixed-lineage adventurer, war-painted fighter, stone tank, bloodthirsty duelist, studious neutral, labor-worn worker, tall-grass ambusher.
- UI card hook: one short phrase from the Orc Prototype UI Baselines table.
- Lore tooltip: one short tooltip explaining the sub-race's social read.

## Next Race

After Orcs are documented and wired, the next art pass should be Demons.

Demon art needs to solve:

- Intricate horns, colored skin, claws, sharp teeth, and very deep mana.
- Fear and discrimination without making every Demon actually evil.
- Common Demon, Arch-Demon, Lesser Demon, War Demon, Flame Demon, Shadow Demon, Succubus, Incubus, Peacebound Demon, and Horn-Crowned Demon differentiation.
- Succubus and Incubus appeal without making the whole Demon race cute or innocent.

## Protected Hybrid Character Art Notes

These characters are not ordinary sub-race baselines. They are protected NPC/player identities that can later become guest logins, companion characters, mentor figures, story-event participants, or special player-facing cameos.

| Character | Lineage | Approved Visual Direction | Gear | Use |
| --- | --- | --- | --- | --- |
| Lani | Half-Elf / Half-Dwarf | Adult female hybrid, short like a Dwarf, sturdy body, no beard, clear Elf ears, attractive feminine face, practical mage-adventurer clothing, Dwarven craft details mixed with Elven elegance. | Maui, a polished dark-wood staff with Dwarven metal bands and a graceful curved top. | Protected special NPC/player slot; not a normal random NPC and not a standard character creation option yet. |
| Raelae | Half-Orc / Half-Elf | Adult female ranger, tall and graceful like an Elf, clear Elf ears, green skin, short tusks, lean athletic muscle, and wilderness-hardened beauty. | Longbow, quiver, ranger knife, forest leathers, and travel cloak. | Protected special NPC/player slot; not a normal random NPC and not a standard character creation option yet. |
| Lahua | Fae Sprite | Child Fae Sprite druid, flower dress, tiny wings, soft innocent face, animal-lover presence, and gentle nature magic. | Tiny druid charm or mini staff, seed pouch, flower dress, and animal companion styling. | Protected child-safe special NPC/player slot; not a normal random NPC and not a mature-system character. |
| Ieke | Full Orc visual baseline / Legendary barbarian | Adult male full Orc barbarian, massive muscular body, green skin, tusks, battle scars, and battle-ready stance. | Huge battle-worn axe for this render; Umbra Pit Broadsword remains earlier legendary gear history unless later replaced. | Protected Legendary True special guest baseline; not a normal random NPC. |
