#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/HuwamCharacterCreationComponent.h"
#include "Gameplay/HuwamCombatComponent.h"
#include "Gameplay/HuwamHudDataComponent.h"
#include "Gameplay/HuwamQuestComponent.h"
#include "Gameplay/HuwamRewardComponent.h"
#include "HuwamPrototypePlayerActor.generated.h"

class AHuwamMonsterEncounterActor;
class UHuwamCharacterStatsComponent;
class UHuwamEquipmentComponent;
class UHuwamInventoryComponent;
class UHuwamLiveContentComponent;
class USceneComponent;

UCLASS(Blueprintable)
class HUWAM_API AHuwamPrototypePlayerActor : public AActor
{
    GENERATED_BODY()

public:
    AHuwamPrototypePlayerActor();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    void InitializePrototypePlayer();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool GrantStarterLoadout();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool EquipStarterGear();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool StartSlimeQuest();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool ApplyCharacterCreationToPrototype(const FHuwamCharacterCreationRequest& Request, FHuwamCharacterCreationResult& OutResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool AttackMonster(AHuwamMonsterEncounterActor* Monster, FHuwamCombatResult& OutCombatResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool ClaimMonsterRewards(AHuwamMonsterEncounterActor* Monster, FHuwamRewardGrantResult& OutRewardResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool AttackMonsterAndClaimIfDefeated(AHuwamMonsterEncounterActor* Monster, FHuwamCombatResult& OutCombatResult, FHuwamRewardGrantResult& OutRewardResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool TurnInSlimeQuest(FHuwamQuestRuntimeState& OutTurnedInQuest);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    FHuwamHudSnapshot RefreshPrototypeHud();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamInventoryComponent* GetInventoryComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamEquipmentComponent* GetEquipmentComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamCharacterStatsComponent* GetCharacterStatsComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamCharacterCreationComponent* GetCharacterCreationComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamCombatComponent* GetCombatComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamRewardComponent* GetRewardComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamQuestComponent* GetQuestComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamLiveContentComponent* GetLiveContentComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamHudDataComponent* GetHudDataComponent() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamInventoryComponent> Inventory;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamEquipmentComponent> Equipment;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamCharacterStatsComponent> CharacterStats;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamCharacterCreationComponent> CharacterCreation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamCombatComponent> Combat;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamRewardComponent> Rewards;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamQuestComponent> Quests;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamLiveContentComponent> LiveContent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamHudDataComponent> HudData;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    bool bAutoInitializeOnBeginPlay = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    bool bGrantStarterLoadoutOnInitialize = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    bool bStartSlimeQuestOnInitialize = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    FHuwamCombatRollConfig StarterMeleeAttackConfig;

private:
    bool AddInventoryItemIfMissing(const FString& ItemId, int32 Quantity);
    bool EquipItemIfAvailable(const FString& ItemId, const FString& SlotId);
};
