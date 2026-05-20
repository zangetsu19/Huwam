#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/HuwamEquipmentComponent.h"
#include "HuwamCharacterStatsComponent.generated.h"

struct FHuwamStatRow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHuwamCharacterStatsChangedSignature);

USTRUCT(BlueprintType)
struct FHuwamStatValue
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    FString StatId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 BaseValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 PermanentBonus = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 TemporaryBonus = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 EquipmentBonus = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 FinalValue = 0;
};

USTRUCT(BlueprintType)
struct FHuwamCharacterPowerSummary
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 Strength = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 Charisma = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 Intelligence = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 Wisdom = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 Dexterity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 Luck = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 Health = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 Mana = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 Defense = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 WorldPopularity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 EquipmentAttackPower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 EquipmentDefensePower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 EquipmentManaPower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 EquipmentUtilityPower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 MaxHealth = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 MaxMana = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 MeleePower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 RangedPower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 SpellPower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 ArmorRating = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 UtilityScore = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Stats")
    int32 SocialPower = 0;
};

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamCharacterStatsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamCharacterStatsComponent();

    virtual void BeginPlay() override;

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Stats")
    FHuwamCharacterStatsChangedSignature OnStatsChanged;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Stats")
    void ResetToProgressionZeroDefaults();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Stats")
    bool SetBaseStat(const FString& StatId, int32 Value);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Stats")
    bool AddBaseStat(const FString& StatId, int32 Delta);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Stats")
    int32 GetBaseStat(const FString& StatId) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Stats")
    bool SetPermanentStatBonus(const FString& StatId, int32 Value);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Stats")
    bool SetTemporaryStatBonus(const FString& StatId, int32 Value);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Stats")
    void ClearTemporaryStatBonuses();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Stats", meta=(AdvancedDisplay="Equipment"))
    FHuwamStatValue GetStatValue(const FString& StatId, UHuwamEquipmentComponent* Equipment = nullptr) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Stats", meta=(AdvancedDisplay="Equipment"))
    TArray<FHuwamStatValue> GetAllStatValues(UHuwamEquipmentComponent* Equipment = nullptr) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Stats", meta=(AdvancedDisplay="Equipment"))
    FHuwamCharacterPowerSummary GetCharacterPowerSummary(UHuwamEquipmentComponent* Equipment = nullptr) const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Stats")
    TMap<FString, int32> BaseStats;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Stats")
    TMap<FString, int32> PermanentStatBonuses;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Stats")
    TMap<FString, int32> TemporaryStatBonuses;

private:
    void EnsureDefaultStats();
    bool IsCoreStatId(const FString& StatId) const;
    int32 GetDefaultBaseValue(const FString& StatId) const;
    int32 GetMapValue(const TMap<FString, int32>& Values, const FString& StatId) const;
    int32 ClampStatValue(const FString& StatId, int32 Value) const;
    int32 GetEquipmentBonusForStat(const FString& StatId, UHuwamEquipmentComponent* Equipment) const;
    bool GetStatRowForId(const FString& StatId, FHuwamStatRow& OutStatRow) const;
    FText GetDisplayNameForStat(const FString& StatId) const;
    const UHuwamEquipmentComponent* ResolveEquipmentComponent(UHuwamEquipmentComponent* Equipment) const;
};
