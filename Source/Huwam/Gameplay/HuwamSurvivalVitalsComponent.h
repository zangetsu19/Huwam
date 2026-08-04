#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/HuwamCombatComponent.h"
#include "Gameplay/HuwamInventoryComponent.h"
#include "HuwamSurvivalVitalsComponent.generated.h"

UENUM(BlueprintType)
enum class EHuwamSurvivalNeedState : uint8
{
    Full UMETA(DisplayName = "Full"),
    Comfortable UMETA(DisplayName = "Comfortable"),
    Low UMETA(DisplayName = "Low"),
    Critical UMETA(DisplayName = "Critical"),
    Empty UMETA(DisplayName = "Empty")
};

UENUM(BlueprintType)
enum class EHuwamSurvivalServiceType : uint8
{
    TavernMeal UMETA(DisplayName = "Tavern Meal"),
    WellWater UMETA(DisplayName = "Well Water"),
    CampfireChat UMETA(DisplayName = "Campfire Chat"),
    InnRoom UMETA(DisplayName = "Inn Room")
};

USTRUCT(BlueprintType)
struct FHuwamSurvivalVitalsSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    int32 Hunger = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    int32 MaxHunger = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    float HungerPercent = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    EHuwamSurvivalNeedState HungerState = EHuwamSurvivalNeedState::Full;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    int32 Thirst = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    int32 MaxThirst = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    float ThirstPercent = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    EHuwamSurvivalNeedState ThirstState = EHuwamSurvivalNeedState::Full;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    int32 SocialEnergy = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    int32 MaxSocialEnergy = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    float SocialEnergyPercent = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    EHuwamSurvivalNeedState SocialEnergyState = EHuwamSurvivalNeedState::Full;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    int32 LastUpdatedInGameDay = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    bool bStarving = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    bool bDehydrated = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    bool bIsolated = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    bool bLastFoodWasUnsafe = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    TArray<FString> ActiveEffectIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    int32 LastConsequenceDamage = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival")
    FText LastSurvivalMessage;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHuwamSurvivalVitalsChangedSignature, FHuwamSurvivalVitalsSnapshot, Snapshot);

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamSurvivalVitalsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamSurvivalVitalsComponent();

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Survival")
    FHuwamSurvivalVitalsChangedSignature OnSurvivalVitalsChanged;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival")
    FHuwamSurvivalVitalsSnapshot ResetSurvivalVitals();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival")
    FHuwamSurvivalVitalsSnapshot RefreshSurvivalVitals();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival")
    FHuwamSurvivalVitalsSnapshot ApplyInGameDaysElapsed(int32 InGameDays);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival")
    bool ConsumeFoodFromInventory(UHuwamInventoryComponent* Inventory, const FString& ItemId, FHuwamSurvivalVitalsSnapshot& OutSnapshot);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival")
    FHuwamSurvivalVitalsSnapshot DrinkWater(int32 ThirstRestored = 25);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival")
    FHuwamSurvivalVitalsSnapshot RestoreSocialEnergy(int32 SocialRestored = 15);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival")
    FHuwamSurvivalVitalsSnapshot UseSurvivalService(EHuwamSurvivalServiceType ServiceType);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival")
    bool ApplySurvivalConsequences(UHuwamCombatComponent* Combat, FHuwamCombatResult& OutDamageResult);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival")
    FHuwamSurvivalVitalsSnapshot GetSurvivalVitalsSnapshot() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival")
    void RestoreSurvivalVitalsSnapshot(const FHuwamSurvivalVitalsSnapshot& SavedSnapshot);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival", meta = (ClampMin = "1"))
    int32 MaxHunger = 100;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival", meta = (ClampMin = "1"))
    int32 MaxThirst = 100;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival", meta = (ClampMin = "1"))
    int32 MaxSocialEnergy = 100;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival", meta = (ClampMin = "0"))
    int32 HungerDrainPerInGameDay = 12;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival", meta = (ClampMin = "0"))
    int32 ThirstDrainPerInGameDay = 20;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival", meta = (ClampMin = "0"))
    int32 SocialDrainPerInGameDay = 4;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival", meta = (ClampMin = "0"))
    int32 FreshFoodHungerRestored = 25;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival", meta = (ClampMin = "0"))
    int32 StaleFoodHungerRestored = 10;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival", meta = (ClampMin = "0"))
    int32 StableFoodHungerRestored = 15;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival", meta = (ClampMin = "0"))
    int32 StarvationDamagePerConsequence = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival", meta = (ClampMin = "0"))
    int32 DehydrationDamagePerConsequence = 10;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival", meta = (ClampMin = "0"))
    int32 UnsafeFoodDamagePerConsequence = 3;

private:
    FHuwamSurvivalVitalsSnapshot Snapshot;

    int32 GetCurrentInGameDay() const;
    EHuwamSurvivalNeedState GetNeedState(int32 CurrentValue, int32 MaxValue) const;
    float GetNeedPercent(int32 CurrentValue, int32 MaxValue) const;
    void ClampAndRefreshSnapshot();
    void BroadcastSnapshot();
    EHuwamInventoryFreshness GetBestFreshnessForItem(UHuwamInventoryComponent* Inventory, const FString& ItemId) const;
};
