#include "Gameplay/HuwamCombatComponent.h"

#include "GameFramework/Actor.h"
#include "Gameplay/HuwamCharacterStatsComponent.h"
#include "Gameplay/HuwamEquipmentComponent.h"

UHuwamCombatComponent::UHuwamCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHuwamCombatComponent::BeginPlay()
{
    Super::BeginPlay();

    InitializeFromStats();
}

void UHuwamCombatComponent::InitializeFromStats(UHuwamCharacterStatsComponent* Stats, UHuwamEquipmentComponent* Equipment, bool bFillMeters)
{
    const int32 MaxHealth = GetMaximumHealth(Stats, Equipment);
    const int32 MaxMana = GetMaximumMana(Stats, Equipment);

    if (bFillMeters)
    {
        CurrentHealth = MaxHealth;
        CurrentMana = MaxMana;
        bDefeated = CurrentHealth <= 0;
    }
    else
    {
        CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
        CurrentMana = FMath::Clamp(CurrentMana, 0, MaxMana);
        bDefeated = CurrentHealth <= 0;
    }

    OnCombatStateChanged.Broadcast();
}

int32 UHuwamCombatComponent::GetMaximumHealth(UHuwamCharacterStatsComponent* Stats, UHuwamEquipmentComponent* Equipment) const
{
    UHuwamCharacterStatsComponent* ResolvedStats = Stats ? Stats : ResolveStatsComponent();
    UHuwamEquipmentComponent* ResolvedEquipment = Equipment ? Equipment : ResolveEquipmentComponent();

    if (ResolvedStats)
    {
        return FMath::Max(1, ResolvedStats->GetCharacterPowerSummary(ResolvedEquipment).MaxHealth);
    }

    return 100;
}

int32 UHuwamCombatComponent::GetMaximumMana(UHuwamCharacterStatsComponent* Stats, UHuwamEquipmentComponent* Equipment) const
{
    UHuwamCharacterStatsComponent* ResolvedStats = Stats ? Stats : ResolveStatsComponent();
    UHuwamEquipmentComponent* ResolvedEquipment = Equipment ? Equipment : ResolveEquipmentComponent();

    if (ResolvedStats)
    {
        return FMath::Max(0, ResolvedStats->GetCharacterPowerSummary(ResolvedEquipment).MaxMana);
    }

    return 100;
}

int32 UHuwamCombatComponent::GetCurrentHealth() const
{
    return CurrentHealth;
}

int32 UHuwamCombatComponent::GetCurrentMana() const
{
    return CurrentMana;
}

bool UHuwamCombatComponent::IsDefeated() const
{
    return bDefeated;
}

int32 UHuwamCombatComponent::Heal(int32 Amount, bool bAllowRevive)
{
    if (Amount <= 0 || (bDefeated && !bAllowRevive))
    {
        return 0;
    }

    const int32 HealthBefore = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0, GetMaximumHealth());
    bDefeated = CurrentHealth <= 0;

    const int32 AmountHealed = CurrentHealth - HealthBefore;
    if (AmountHealed > 0)
    {
        OnCombatStateChanged.Broadcast();
    }

    return AmountHealed;
}

bool UHuwamCombatComponent::SpendMana(int32 Amount)
{
    if (Amount < 0 || CurrentMana < Amount)
    {
        return false;
    }

    CurrentMana -= Amount;
    OnCombatStateChanged.Broadcast();
    return true;
}

int32 UHuwamCombatComponent::RestoreMana(int32 Amount, UHuwamCharacterStatsComponent* Stats, UHuwamEquipmentComponent* Equipment)
{
    if (Amount <= 0)
    {
        return 0;
    }

    const int32 ManaBefore = CurrentMana;
    CurrentMana = FMath::Clamp(CurrentMana + Amount, 0, GetMaximumMana(Stats, Equipment));

    const int32 AmountRestored = CurrentMana - ManaBefore;
    if (AmountRestored > 0)
    {
        OnCombatStateChanged.Broadcast();
    }

    return AmountRestored;
}

bool UHuwamCombatComponent::ApplyResolvedDamage(const FHuwamCombatResult& InResult, FHuwamCombatResult& OutAppliedResult)
{
    OutAppliedResult = InResult;
    OutAppliedResult.DefenderHealthBefore = CurrentHealth;

    if (bDefeated)
    {
        OutAppliedResult.DefenderHealthAfter = CurrentHealth;
        OutAppliedResult.bDefenderDefeated = true;
        OutAppliedResult.bSuccess = false;
        return false;
    }

    const int32 DamageToApply = FMath::Max(0, InResult.FinalDamage);
    CurrentHealth = FMath::Clamp(CurrentHealth - DamageToApply, 0, GetMaximumHealth());
    bDefeated = CurrentHealth <= 0;

    OutAppliedResult.DefenderHealthAfter = CurrentHealth;
    OutAppliedResult.bDefenderDefeated = bDefeated;
    OutAppliedResult.bSuccess = DamageToApply > 0;

    OnDamageApplied.Broadcast(OutAppliedResult);
    OnCombatStateChanged.Broadcast();

    return OutAppliedResult.bSuccess;
}

bool UHuwamCombatComponent::AttackTarget(UHuwamCombatComponent* Target, EHuwamCombatAttackType AttackType, const FHuwamCombatRollConfig& RollConfig, FHuwamCombatResult& OutResult)
{
    OutResult = FHuwamCombatResult();
    OutResult.AttackType = AttackType;

    if (!Target || bDefeated || Target->IsDefeated())
    {
        return false;
    }

    const FHuwamCombatResult BuiltResult = BuildCombatResult(Target, AttackType, RollConfig);
    FHuwamCombatResult AppliedResult;
    Target->ApplyResolvedDamage(BuiltResult, AppliedResult);
    OutResult = AppliedResult;

    return OutResult.bSuccess;
}

FHuwamCombatResult UHuwamCombatComponent::BuildCombatResult(UHuwamCombatComponent* Target, EHuwamCombatAttackType AttackType, const FHuwamCombatRollConfig& RollConfig) const
{
    FHuwamCombatResult Result;
    Result.AttackType = AttackType;
    Result.AttackPower = FMath::Max(0, GetAttackPowerForType(AttackType) + RollConfig.FlatBonus);
    Result.DefensePower = Target ? Target->GetDefensePowerAgainstType(AttackType) : 0;
    Result.RollBonus = FMath::RandRange(RollConfig.VarianceMin, FMath::Max(RollConfig.VarianceMin, RollConfig.VarianceMax));
    Result.RawDamage = FMath::Max(0, Result.AttackPower + Result.RollBonus);
    Result.SourceTag = TEXT("combat.basic_attack");

    const int32 CriticalChance = FMath::Clamp(RollConfig.CriticalChancePercent, 0, 100);
    Result.bCritical = CriticalChance > 0 && FMath::RandRange(1, 100) <= CriticalChance;
    if (Result.bCritical)
    {
        const int32 CriticalMultiplier = FMath::Max(100, RollConfig.CriticalMultiplierPercent);
        Result.RawDamage = FMath::FloorToInt(static_cast<float>(Result.RawDamage * CriticalMultiplier) / 100.0f);
    }

    Result.DamageMitigated = CalculateMitigation(AttackType, Result.DefensePower, RollConfig.ArmorPierce);
    Result.FinalDamage = Result.RawDamage > 0 ? FMath::Max(1, Result.RawDamage - Result.DamageMitigated) : 0;
    Result.bSuccess = Result.FinalDamage > 0;

    return Result;
}

int32 UHuwamCombatComponent::GetAttackPowerForType(EHuwamCombatAttackType AttackType) const
{
    if (const UHuwamCharacterStatsComponent* Stats = ResolveStatsComponent())
    {
        const FHuwamCharacterPowerSummary Summary = Stats->GetCharacterPowerSummary(ResolveEquipmentComponent());
        switch (AttackType)
        {
            case EHuwamCombatAttackType::Melee:
                return Summary.MeleePower;

            case EHuwamCombatAttackType::Ranged:
                return Summary.RangedPower;

            case EHuwamCombatAttackType::Spell:
                return Summary.SpellPower;

            case EHuwamCombatAttackType::TrueDamage:
                return FMath::Max(Summary.MeleePower, FMath::Max(Summary.RangedPower, Summary.SpellPower));

            default:
                break;
        }
    }

    return 10;
}

int32 UHuwamCombatComponent::GetDefensePowerAgainstType(EHuwamCombatAttackType AttackType) const
{
    if (AttackType == EHuwamCombatAttackType::TrueDamage)
    {
        return 0;
    }

    if (const UHuwamCharacterStatsComponent* Stats = ResolveStatsComponent())
    {
        return Stats->GetCharacterPowerSummary(ResolveEquipmentComponent()).ArmorRating;
    }

    return 0;
}

int32 UHuwamCombatComponent::CalculateMitigation(EHuwamCombatAttackType AttackType, int32 DefensePower, int32 ArmorPierce) const
{
    if (AttackType == EHuwamCombatAttackType::TrueDamage)
    {
        return 0;
    }

    const int32 PiercedDefense = FMath::Max(0, DefensePower - FMath::Max(0, ArmorPierce));
    if (AttackType == EHuwamCombatAttackType::Spell)
    {
        return FMath::FloorToInt(static_cast<float>(PiercedDefense) * 0.25f);
    }

    return FMath::FloorToInt(static_cast<float>(PiercedDefense) * 0.5f);
}

UHuwamCharacterStatsComponent* UHuwamCombatComponent::ResolveStatsComponent() const
{
    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamCharacterStatsComponent>();
    }

    return nullptr;
}

UHuwamEquipmentComponent* UHuwamCombatComponent::ResolveEquipmentComponent() const
{
    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamEquipmentComponent>();
    }

    return nullptr;
}
