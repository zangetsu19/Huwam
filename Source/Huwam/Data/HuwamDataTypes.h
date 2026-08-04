#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HuwamDataTypes.generated.h"

UENUM(BlueprintType)
enum class EHuwamRarity : uint8
{
    Common,
    Uncommon,
    Rare,
    Mythic
};

UENUM(BlueprintType)
enum class EHuwamManaWellDepth : uint8
{
    None,
    Shallow,
    Low,
    Average,
    Deep,
    VeryDeep,
    Extreme
};

UENUM(BlueprintType)
enum class EHuwamRank : uint8
{
    F,
    E,
    D,
    C,
    B,
    A,
    S,
    SS,
    SSS
};

UENUM(BlueprintType)
enum class EHuwamClassType : uint8
{
    Starter,
    Progression,
    Martial,
    Caster,
    Divine,
    Secret
};

UENUM(BlueprintType)
enum class EHuwamItemType : uint8
{
    Weapon,
    Armor,
    Clothing,
    Tool,
    Material,
    Consumable,
    Pouch,
    Container,
    Backpack,
    Relic,
    Currency
};

UENUM(BlueprintType)
enum class EHuwamSpellCategory : uint8
{
    Air,
    Earth,
    Fire,
    Water,
    Nature,
    Light,
    Dark,
    Green,
    Rune,
    Life,
    Death,
    Spirit,
    Forbidden
};

UENUM(BlueprintType)
enum class EHuwamSpellSchool : uint8
{
    Conjuration,
    Evocation,
    Necromancy,
    Abjuration,
    Transmutation,
    Divination,
    Enchantment,
    Illusion
};

UENUM(BlueprintType)
enum class EHuwamCastTime : uint8
{
    Instant,
    Quick,
    Normal,
    Slow,
    Ritual,
    ExtendedRitual
};

UENUM(BlueprintType)
enum class EHuwamDuration : uint8
{
    Instant,
    Short,
    Sustained,
    Timed,
    Permanent,
    Conditional
};

UENUM(BlueprintType)
enum class EHuwamRange : uint8
{
    Self,
    Touch,
    Short,
    Medium,
    Long,
    Sight,
    Area,
    WorldScale
};

UENUM(BlueprintType)
enum class EHuwamQuestType : uint8
{
    Tutorial,
    Gather,
    Combat,
    Craft,
    Delivery,
    Social,
    Contract,
    Dungeon,
    Raid
};

UENUM(BlueprintType)
enum class EHuwamQuestUrgency : uint8
{
    Normal,
    Timed,
    Urgent,
    Emergency
};

UENUM(BlueprintType)
enum class EHuwamSimulationTier : uint8
{
    Dormant,
    StatisticalBackground,
    Background,
    Active,
    Full,
    Critical
};

UENUM(BlueprintType)
enum class EHuwamContentPackType : uint8
{
    Cosmetic,
    Companion,
    Gameplay,
    World,
    Event,
    System
};

UENUM(BlueprintType)
enum class EHuwamContentPackState : uint8
{
    Draft,
    Staged,
    Active,
    Paused,
    Retired
};

USTRUCT(BlueprintType)
struct FHuwamRequirement
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Requirement")
    FString RequirementId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Requirement")
    int32 RequiredValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Requirement")
    FString Notes;
};

USTRUCT(BlueprintType)
struct FHuwamIdQuantity
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Data")
    FString Id;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Data")
    int32 Quantity = 1;
};

USTRUCT(BlueprintType)
struct FHuwamCurrencyBreakdown
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Currency")
    int64 MarquisDiamondPrints = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Currency")
    int64 Platinum = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Currency")
    int64 Gold = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Currency")
    int64 Silver = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Currency")
    int64 Copper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Currency")
    int64 TotalCopper = 0;
};

USTRUCT(BlueprintType)
struct FHuwamContentPackRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Content Pack")
    FString ContentPackId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Content Pack")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Content Pack")
    EHuwamContentPackType PackType = EHuwamContentPackType::Cosmetic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Content Pack")
    EHuwamContentPackState PackState = EHuwamContentPackState::Draft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Content Pack")
    FString Version;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Content Pack")
    FString RequiredGameVersion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Content Pack")
    FString EntitlementId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Content Pack")
    bool bEnabledByDefault = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Content Pack")
    bool bRequiresOwnership = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Content Pack")
    bool bCosmeticOnly = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Content Pack")
    TArray<FString> ContentIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Content Pack")
    TArray<FString> FeatureFlagIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Content Pack")
    FText Description;
};

USTRUCT(BlueprintType)
struct FHuwamRaceRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Race")
    FString RaceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Race")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Race")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Race")
    FText GeneralRole;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Race")
    FText CommonReputation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Race")
    FString ClosestDeityId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Race")
    FString ContinentId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Race")
    FString KingdomId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Race")
    EHuwamRarity Rarity = EHuwamRarity::Common;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Race")
    EHuwamManaWellDepth ManaWellBaseline = EHuwamManaWellDepth::Average;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Race")
    TArray<FString> TraitIds;
};

USTRUCT(BlueprintType)
struct FHuwamSubRaceRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|SubRace")
    FString SubRaceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|SubRace")
    FString ParentRaceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|SubRace")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|SubRace")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|SubRace")
    EHuwamManaWellDepth ManaWellModifier = EHuwamManaWellDepth::Average;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|SubRace")
    TArray<FString> TraitIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|SubRace")
    TArray<FString> WeaknessIds;
};

USTRUCT(BlueprintType)
struct FHuwamClassRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Class")
    FString ClassId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Class")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Class")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Class")
    int32 MaxLevel = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Class")
    EHuwamClassType ClassType = EHuwamClassType::Starter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Class")
    TArray<FHuwamRequirement> UnlockRequirements;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Class")
    TArray<FString> StartingSpellIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Class")
    TArray<FString> StartingSkillIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Class")
    FString GuildId;
};

USTRUCT(BlueprintType)
struct FHuwamJobRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Job")
    FString JobId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Job")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Job")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Job")
    int32 MinSalaryGold = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Job")
    int32 MaxSalaryGold = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Job")
    TArray<FString> DiscountResourceIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Job")
    TArray<FHuwamRequirement> UnlockRequirements;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Job")
    TArray<FString> ProducedItemTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Job")
    FString AssociatedGuildId;
};

USTRUCT(BlueprintType)
struct FHuwamStatRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stat")
    FString StatId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stat")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stat")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stat")
    int32 MinValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stat")
    int32 MaxValue = 999;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stat")
    TArray<FString> GrowthSourceTags;
};

USTRUCT(BlueprintType)
struct FHuwamSkillRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Skill")
    FString SkillId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Skill")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Skill")
    FText Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Skill")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Skill")
    TArray<FString> GoverningStatIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Skill")
    bool bPermanentAtCreation = true;
};

USTRUCT(BlueprintType)
struct FHuwamAbilityRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Ability")
    FString AbilityId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Ability")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Ability")
    FText AbilityType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Ability")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Ability")
    bool bPermanentAtCreation = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Ability")
    TArray<FString> EffectTags;
};

USTRUCT(BlueprintType)
struct FHuwamItemRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    FString ItemId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    EHuwamItemType ItemType = EHuwamItemType::Material;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    int32 StackLimit = 99;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    float Weight = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    int64 BaseValueCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    int32 BaseValueGold = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    int32 AttackPower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    int32 DefensePower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    int32 ManaPower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    int32 UtilityPower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    FString PrimaryEquipSlotId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    TArray<FString> EquipSlotIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Item")
    TArray<FString> Tags;
};

USTRUCT(BlueprintType)
struct FHuwamMaterialRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Material")
    FString MaterialId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Material")
    FString ItemId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Material")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Material")
    EHuwamRank Rank = EHuwamRank::F;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Material")
    TArray<FString> SourceTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Material")
    TArray<FString> HarvestToolIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Material")
    int32 RespawnInGameDays = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Material")
    bool bEdible = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Material")
    TArray<FString> CraftingTags;
};

USTRUCT(BlueprintType)
struct FHuwamRecipeRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Recipe")
    FString RecipeId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Recipe")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Recipe")
    FString OutputItemId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Recipe")
    int32 OutputQuantity = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Recipe")
    TArray<FHuwamIdQuantity> Ingredients;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Recipe")
    TArray<FString> RequiredSkillIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Recipe")
    TArray<FString> RequiredStationIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Recipe")
    float CraftTimeSeconds = 1.0f;
};

USTRUCT(BlueprintType)
struct FHuwamSpellRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Spell")
    FString SpellId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Spell")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Spell")
    EHuwamSpellCategory Category = EHuwamSpellCategory::Air;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Spell")
    EHuwamSpellSchool School = EHuwamSpellSchool::Evocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Spell")
    EHuwamRank Rank = EHuwamRank::F;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Spell")
    int32 ManaCost = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Spell")
    EHuwamCastTime CastTime = EHuwamCastTime::Quick;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Spell")
    EHuwamDuration Duration = EHuwamDuration::Instant;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Spell")
    EHuwamRange Range = EHuwamRange::Short;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Spell")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Spell")
    TArray<FString> RequirementIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Spell")
    TArray<FString> RiskTags;
};

USTRUCT(BlueprintType)
struct FHuwamMonsterRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    FString MonsterId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    EHuwamRank Rank = EHuwamRank::F;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    int32 BaseStrength = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    int32 BaseDexterity = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    int32 BaseHealth = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    int32 BaseMana = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    int32 BaseDefense = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    int32 BaseLuck = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    TArray<FString> BiomeTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    TArray<FString> BehaviorTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    TArray<FHuwamIdQuantity> Drops;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    FString PrimaryDropItemId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    int32 PrimaryDropQuantity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    int64 CurrencyRewardCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    int32 GoldReward = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    int32 ExperienceReward = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    bool bTamable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    bool bEdible = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Monster")
    FText Description;
};

USTRUCT(BlueprintType)
struct FHuwamNpcRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC")
    FString NpcId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC")
    FString RaceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC")
    FString SubRaceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC")
    int32 Age = 18;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC")
    EHuwamSimulationTier SimulationTier = EHuwamSimulationTier::Background;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC")
    TArray<FString> ClassIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC")
    TArray<FString> JobIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC")
    FString GuildId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC")
    FText RoleSummary;
};

USTRUCT(BlueprintType)
struct FHuwamQuestRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FString QuestId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    EHuwamQuestType QuestType = EHuwamQuestType::Gather;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FString QuestGiverId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    EHuwamQuestUrgency Urgency = EHuwamQuestUrgency::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    bool bRepeatable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FString PrimaryObjectiveId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FText PrimaryObjectiveDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    int32 PrimaryObjectiveTargetValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FString PrimaryObjectiveProgressSourceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    TArray<FHuwamIdQuantity> RequiredItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    TArray<FHuwamIdQuantity> RewardItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    int64 RewardCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    int32 RewardGold = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    int32 RewardExperience = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Quest")
    FText WorldStateEffect;
};
