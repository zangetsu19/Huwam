#include "Gameplay/HuwamCharacterStatsComponent.h"

#include "Data/HuwamDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

namespace
{
    const TCHAR* StatStrength = TEXT("stat.strength");
    const TCHAR* StatCharisma = TEXT("stat.charisma");
    const TCHAR* StatIntelligence = TEXT("stat.intelligence");
    const TCHAR* StatWisdom = TEXT("stat.wisdom");
    const TCHAR* StatDexterity = TEXT("stat.dexterity");
    const TCHAR* StatLuck = TEXT("stat.luck");
    const TCHAR* StatHealth = TEXT("stat.health");
    const TCHAR* StatMana = TEXT("stat.mana");
    const TCHAR* StatDefense = TEXT("stat.defense");
    const TCHAR* StatWorldPopularity = TEXT("stat.world_popularity");

    TArray<FString> GetCoreStatIds()
    {
        return {
            StatStrength,
            StatCharisma,
            StatIntelligence,
            StatWisdom,
            StatDexterity,
            StatLuck,
            StatHealth,
            StatMana,
            StatDefense,
            StatWorldPopularity
        };
    }

    FName GetRowNameFromId(const FString& Id)
    {
        return FName(*Id.Replace(TEXT("."), TEXT("_")));
    }
}

UHuwamCharacterStatsComponent::UHuwamCharacterStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHuwamCharacterStatsComponent::BeginPlay()
{
    Super::BeginPlay();

    EnsureDefaultStats();
}

void UHuwamCharacterStatsComponent::ResetToProgressionZeroDefaults()
{
    BaseStats.Reset();
    PermanentStatBonuses.Reset();
    TemporaryStatBonuses.Reset();

    for (const FString& StatId : GetCoreStatIds())
    {
        BaseStats.Add(StatId, GetDefaultBaseValue(StatId));
    }

    OnStatsChanged.Broadcast();
}

bool UHuwamCharacterStatsComponent::SetBaseStat(const FString& StatId, int32 Value)
{
    if (StatId.IsEmpty())
    {
        return false;
    }

    BaseStats.FindOrAdd(StatId) = ClampStatValue(StatId, Value);
    OnStatsChanged.Broadcast();
    return true;
}

bool UHuwamCharacterStatsComponent::AddBaseStat(const FString& StatId, int32 Delta)
{
    if (StatId.IsEmpty())
    {
        return false;
    }

    return SetBaseStat(StatId, GetBaseStat(StatId) + Delta);
}

int32 UHuwamCharacterStatsComponent::GetBaseStat(const FString& StatId) const
{
    if (const int32* FoundValue = BaseStats.Find(StatId))
    {
        return *FoundValue;
    }

    return GetDefaultBaseValue(StatId);
}

bool UHuwamCharacterStatsComponent::SetPermanentStatBonus(const FString& StatId, int32 Value)
{
    if (StatId.IsEmpty())
    {
        return false;
    }

    PermanentStatBonuses.FindOrAdd(StatId) = Value;
    OnStatsChanged.Broadcast();
    return true;
}

bool UHuwamCharacterStatsComponent::SetTemporaryStatBonus(const FString& StatId, int32 Value)
{
    if (StatId.IsEmpty())
    {
        return false;
    }

    TemporaryStatBonuses.FindOrAdd(StatId) = Value;
    OnStatsChanged.Broadcast();
    return true;
}

void UHuwamCharacterStatsComponent::ClearTemporaryStatBonuses()
{
    if (TemporaryStatBonuses.IsEmpty())
    {
        return;
    }

    TemporaryStatBonuses.Reset();
    OnStatsChanged.Broadcast();
}

FHuwamStatValue UHuwamCharacterStatsComponent::GetStatValue(const FString& StatId, UHuwamEquipmentComponent* Equipment) const
{
    FHuwamStatValue StatValue;
    StatValue.StatId = StatId;
    StatValue.DisplayName = GetDisplayNameForStat(StatId);
    StatValue.BaseValue = ClampStatValue(StatId, GetBaseStat(StatId));
    StatValue.PermanentBonus = GetMapValue(PermanentStatBonuses, StatId);
    StatValue.TemporaryBonus = GetMapValue(TemporaryStatBonuses, StatId);
    StatValue.EquipmentBonus = GetEquipmentBonusForStat(StatId, Equipment);
    StatValue.FinalValue = ClampStatValue(
        StatId,
        StatValue.BaseValue + StatValue.PermanentBonus + StatValue.TemporaryBonus + StatValue.EquipmentBonus
    );

    return StatValue;
}

TArray<FHuwamStatValue> UHuwamCharacterStatsComponent::GetAllStatValues(UHuwamEquipmentComponent* Equipment) const
{
    TArray<FString> StatIds = GetCoreStatIds();

    auto AddUniqueKeys = [&StatIds](const TMap<FString, int32>& Values)
    {
        for (const TPair<FString, int32>& Pair : Values)
        {
            StatIds.AddUnique(Pair.Key);
        }
    };

    AddUniqueKeys(BaseStats);
    AddUniqueKeys(PermanentStatBonuses);
    AddUniqueKeys(TemporaryStatBonuses);

    TArray<FHuwamStatValue> StatValues;
    for (const FString& StatId : StatIds)
    {
        StatValues.Add(GetStatValue(StatId, Equipment));
    }

    return StatValues;
}

FHuwamCharacterPowerSummary UHuwamCharacterStatsComponent::GetCharacterPowerSummary(UHuwamEquipmentComponent* Equipment) const
{
    FHuwamCharacterPowerSummary Summary;

    Summary.Strength = GetStatValue(StatStrength, Equipment).FinalValue;
    Summary.Charisma = GetStatValue(StatCharisma, Equipment).FinalValue;
    Summary.Intelligence = GetStatValue(StatIntelligence, Equipment).FinalValue;
    Summary.Wisdom = GetStatValue(StatWisdom, Equipment).FinalValue;
    Summary.Dexterity = GetStatValue(StatDexterity, Equipment).FinalValue;
    Summary.Luck = GetStatValue(StatLuck, Equipment).FinalValue;
    Summary.Health = GetStatValue(StatHealth, Equipment).FinalValue;
    Summary.Mana = GetStatValue(StatMana, Equipment).FinalValue;
    Summary.Defense = GetStatValue(StatDefense, Equipment).FinalValue;
    Summary.WorldPopularity = GetStatValue(StatWorldPopularity, Equipment).FinalValue;

    if (const UHuwamEquipmentComponent* ResolvedEquipment = ResolveEquipmentComponent(Equipment))
    {
        const FHuwamEquipmentStatSummary EquipmentSummary = ResolvedEquipment->GetEquipmentStatSummary();
        Summary.EquipmentAttackPower = EquipmentSummary.AttackPower;
        Summary.EquipmentDefensePower = EquipmentSummary.DefensePower;
        Summary.EquipmentManaPower = EquipmentSummary.ManaPower;
        Summary.EquipmentUtilityPower = EquipmentSummary.UtilityPower;
    }

    Summary.MaxHealth = 50 + (Summary.Health * 10) + (Summary.Defense * 2);
    Summary.MaxMana = 20 + (Summary.Mana * 10) + (Summary.Intelligence * 2);
    Summary.MeleePower = Summary.Strength + Summary.EquipmentAttackPower;
    Summary.RangedPower = Summary.Dexterity + Summary.EquipmentAttackPower;
    Summary.SpellPower = Summary.Intelligence + FMath::FloorToInt(static_cast<float>(Summary.Wisdom) * 0.5f) + Summary.EquipmentManaPower;
    Summary.ArmorRating = Summary.Defense;
    Summary.UtilityScore = Summary.Dexterity + Summary.Luck + Summary.EquipmentUtilityPower;
    Summary.SocialPower = Summary.Charisma + FMath::FloorToInt(static_cast<float>(Summary.WorldPopularity) * 0.1f);

    return Summary;
}

void UHuwamCharacterStatsComponent::EnsureDefaultStats()
{
    for (const FString& StatId : GetCoreStatIds())
    {
        if (!BaseStats.Contains(StatId))
        {
            BaseStats.Add(StatId, GetDefaultBaseValue(StatId));
        }
    }
}

bool UHuwamCharacterStatsComponent::IsCoreStatId(const FString& StatId) const
{
    return GetCoreStatIds().Contains(StatId);
}

int32 UHuwamCharacterStatsComponent::GetDefaultBaseValue(const FString& StatId) const
{
    if (StatId == StatWorldPopularity)
    {
        return 0;
    }

    return IsCoreStatId(StatId) ? 10 : 0;
}

int32 UHuwamCharacterStatsComponent::GetMapValue(const TMap<FString, int32>& Values, const FString& StatId) const
{
    if (const int32* FoundValue = Values.Find(StatId))
    {
        return *FoundValue;
    }

    return 0;
}

int32 UHuwamCharacterStatsComponent::ClampStatValue(const FString& StatId, int32 Value) const
{
    FHuwamStatRow StatRow;
    if (GetStatRowForId(StatId, StatRow))
    {
        return FMath::Clamp(Value, StatRow.MinValue, StatRow.MaxValue);
    }

    return FMath::Clamp(Value, 0, 999);
}

int32 UHuwamCharacterStatsComponent::GetEquipmentBonusForStat(const FString& StatId, UHuwamEquipmentComponent* Equipment) const
{
    const UHuwamEquipmentComponent* ResolvedEquipment = ResolveEquipmentComponent(Equipment);
    if (!ResolvedEquipment)
    {
        return 0;
    }

    const FHuwamEquipmentStatSummary EquipmentSummary = ResolvedEquipment->GetEquipmentStatSummary();
    if (StatId == StatDefense)
    {
        return EquipmentSummary.DefensePower;
    }

    if (StatId == StatMana)
    {
        return EquipmentSummary.ManaPower;
    }

    return 0;
}

bool UHuwamCharacterStatsComponent::GetStatRowForId(const FString& StatId, FHuwamStatRow& OutStatRow) const
{
    if (StatId.IsEmpty())
    {
        return false;
    }

    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (const UHuwamDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UHuwamDataSubsystem>())
            {
                return DataSubsystem->GetStatRow(GetRowNameFromId(StatId), OutStatRow);
            }
        }
    }

    return false;
}

FText UHuwamCharacterStatsComponent::GetDisplayNameForStat(const FString& StatId) const
{
    FHuwamStatRow StatRow;
    if (GetStatRowForId(StatId, StatRow))
    {
        return StatRow.DisplayName;
    }

    FString DisplayName = StatId;
    DisplayName.RemoveFromStart(TEXT("stat."));
    DisplayName.ReplaceInline(TEXT("_"), TEXT(" "));
    return FText::FromString(DisplayName);
}

const UHuwamEquipmentComponent* UHuwamCharacterStatsComponent::ResolveEquipmentComponent(UHuwamEquipmentComponent* Equipment) const
{
    if (Equipment)
    {
        return Equipment;
    }

    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamEquipmentComponent>();
    }

    return nullptr;
}
