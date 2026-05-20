#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/HuwamDataTypes.h"
#include "Gameplay/HuwamCombatComponent.h"
#include "HuwamMonsterEncounterActor.generated.h"

class UHuwamCharacterStatsComponent;
class UHuwamInventoryComponent;
class USceneComponent;

UCLASS(Blueprintable)
class HUWAM_API AHuwamMonsterEncounterActor : public AActor
{
    GENERATED_BODY()

public:
    AHuwamMonsterEncounterActor();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Monster")
    bool InitializeFromMonsterData();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Monster")
    bool AttackTarget(UHuwamCombatComponent* Target, FHuwamCombatResult& OutResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Monster")
    bool ReceiveAttackFrom(UHuwamCombatComponent* Attacker, EHuwamCombatAttackType AttackType, const FHuwamCombatRollConfig& RollConfig, FHuwamCombatResult& OutResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Monster")
    bool GrantDefeatRewards(UHuwamInventoryComponent* RecipientInventory, TArray<FHuwamIdQuantity>& OutRewardItems, int32& OutGoldReward, int32& OutExperienceReward);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Monster")
    bool IsDefeated() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Monster")
    FString GetMonsterId() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Monster")
    FText GetMonsterDisplayName() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Monster")
    UHuwamCombatComponent* GetCombatComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Monster")
    UHuwamCharacterStatsComponent* GetCharacterStatsComponent() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    TObjectPtr<UHuwamCharacterStatsComponent> CharacterStats;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    TObjectPtr<UHuwamCombatComponent> Combat;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    FString MonsterId = TEXT("monster.slime.basic");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    FText MonsterDisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    EHuwamRank Rank = EHuwamRank::F;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    bool bTamable = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    bool bEdible = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    bool bAutoInitializeFromData = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    FHuwamCombatRollConfig AttackRollConfig;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    TArray<FHuwamIdQuantity> RewardItems;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    int32 GoldReward = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    int32 ExperienceReward = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Monster")
    bool bRewardsClaimed = false;

private:
    bool ApplyMonsterRow(const FHuwamMonsterRow& MonsterRow);
    void ApplyBasicSlimeFallback();
    bool LoadMonsterRow(FHuwamMonsterRow& OutMonsterRow) const;
    void SetMonsterStats(int32 Strength, int32 Dexterity, int32 Health, int32 Mana, int32 Defense, int32 Luck);
};
