#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HuwamCombatComponent.generated.h"

class UHuwamCharacterStatsComponent;
class UHuwamEquipmentComponent;

UENUM(BlueprintType)
enum class EHuwamCombatAttackType : uint8
{
    Melee UMETA(DisplayName = "Melee"),
    Ranged UMETA(DisplayName = "Ranged"),
    Spell UMETA(DisplayName = "Spell"),
    TrueDamage UMETA(DisplayName = "True Damage")
};

USTRUCT(BlueprintType)
struct FHuwamCombatRollConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 FlatBonus = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 VarianceMin = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 VarianceMax = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 ArmorPierce = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 CriticalChancePercent = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 CriticalMultiplierPercent = 150;
};

USTRUCT(BlueprintType)
struct FHuwamCombatResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    EHuwamCombatAttackType AttackType = EHuwamCombatAttackType::Melee;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 AttackPower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 DefensePower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 RollBonus = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 RawDamage = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 DamageMitigated = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 FinalDamage = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 DefenderHealthBefore = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    int32 DefenderHealthAfter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    bool bCritical = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    bool bDefenderDefeated = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Combat")
    FString SourceTag;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHuwamCombatStateChangedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHuwamCombatDamageAppliedSignature, FHuwamCombatResult, Result);

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamCombatComponent();

    virtual void BeginPlay() override;

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Combat")
    FHuwamCombatStateChangedSignature OnCombatStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Combat")
    FHuwamCombatDamageAppliedSignature OnDamageApplied;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Combat", meta=(AdvancedDisplay="Stats,Equipment,bFillMeters"))
    void InitializeFromStats(UHuwamCharacterStatsComponent* Stats = nullptr, UHuwamEquipmentComponent* Equipment = nullptr, bool bFillMeters = true);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Combat", meta=(AdvancedDisplay="Stats,Equipment"))
    int32 GetMaximumHealth(UHuwamCharacterStatsComponent* Stats = nullptr, UHuwamEquipmentComponent* Equipment = nullptr) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Combat", meta=(AdvancedDisplay="Stats,Equipment"))
    int32 GetMaximumMana(UHuwamCharacterStatsComponent* Stats = nullptr, UHuwamEquipmentComponent* Equipment = nullptr) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Combat")
    int32 GetCurrentHealth() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Combat")
    int32 GetCurrentMana() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Combat")
    bool IsDefeated() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Combat")
    int32 Heal(int32 Amount, bool bAllowRevive = false);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Combat")
    bool SpendMana(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Combat", meta=(AdvancedDisplay="Stats,Equipment"))
    int32 RestoreMana(int32 Amount, UHuwamCharacterStatsComponent* Stats = nullptr, UHuwamEquipmentComponent* Equipment = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Combat")
    void RestoreMetersForPrototypeSave(int32 SavedHealth, int32 SavedMana);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Combat")
    bool ApplyResolvedDamage(const FHuwamCombatResult& InResult, FHuwamCombatResult& OutAppliedResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Combat")
    bool AttackTarget(UHuwamCombatComponent* Target, EHuwamCombatAttackType AttackType, const FHuwamCombatRollConfig& RollConfig, FHuwamCombatResult& OutResult);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Combat")
    int32 CurrentHealth = 100;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Combat")
    int32 CurrentMana = 100;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Combat")
    bool bDefeated = false;

private:
    FHuwamCombatResult BuildCombatResult(UHuwamCombatComponent* Target, EHuwamCombatAttackType AttackType, const FHuwamCombatRollConfig& RollConfig) const;
    int32 GetAttackPowerForType(EHuwamCombatAttackType AttackType) const;
    int32 GetDefensePowerAgainstType(EHuwamCombatAttackType AttackType) const;
    int32 CalculateMitigation(EHuwamCombatAttackType AttackType, int32 DefensePower, int32 ArmorPierce) const;
    UHuwamCharacterStatsComponent* ResolveStatsComponent() const;
    UHuwamEquipmentComponent* ResolveEquipmentComponent() const;
};
