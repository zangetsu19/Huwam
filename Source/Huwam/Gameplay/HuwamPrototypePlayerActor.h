#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/HuwamCharacterCreationComponent.h"
#include "Gameplay/HuwamCombatComponent.h"
#include "Gameplay/HuwamHudDataComponent.h"
#include "Gameplay/HuwamNpcSupplyComponent.h"
#include "Gameplay/HuwamQuestComponent.h"
#include "Gameplay/HuwamRewardComponent.h"
#include "Gameplay/HuwamWorldInteractionComponent.h"
#include "UI/HuwamPrototypeMenuWidget.h"
#include "HuwamPrototypePlayerActor.generated.h"

class AHuwamEldoriaStarterEntryActor;
class AHuwamMonsterEncounterActor;
class AHuwamNpcSupplyQuestOfferActor;
class AHuwamPrototypeThirdPersonCharacter;
class AHuwamTutorialLabCombatTargetActor;
class APlayerController;
class UHuwamCharacterCreationMenuDataComponent;
class UHuwamCharacterCreationScreenWidget;
class UHuwamCharacterStatsComponent;
class UHuwamEquipmentComponent;
class UHuwamInventoryComponent;
class UHuwamLiveContentComponent;
class UHuwamMapAwarenessComponent;
class UHuwamNpcSupplyQuestInteractionWidget;
class UHuwamPrototypeHudWidget;
class UHuwamSurvivalVitalsComponent;
class UHuwamTutorialLabComponent;
class UHuwamTutorialLabPromptWidget;
class UHuwamWorldInteractionPromptWidget;
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
    void ConfigureForThirdPersonBootstrap();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool InitializePrototypePlayerFromCharacterCreation(const FHuwamCharacterCreationRequest& Request, FHuwamCharacterCreationResult& OutResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool GrantStarterLoadout();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool EquipStarterGear();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool StartSlimeQuest();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool StartGatherFieldSuppliesQuest();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    bool CanStartGatherFieldSuppliesQuestFromSupply(UHuwamNpcSupplyComponent* RequesterSupply) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    bool GetGatherFieldSuppliesQuestOfferFromSupply(UHuwamNpcSupplyComponent* RequesterSupply, FHuwamQuestOffer& OutQuestOffer) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool StartGatherFieldSuppliesQuestFromSupply(UHuwamNpcSupplyComponent* RequesterSupply);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool BeginLaucianTutorialLab();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool AcknowledgeTutorialHudLesson();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool MarkTutorialInventoryItemInspected(const FString& ItemId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool MarkTutorialEquipmentSlotInspected(const FString& SlotId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool AttackTutorialLabCombatTarget(AHuwamTutorialLabCombatTargetActor* Target, FHuwamCombatResult& OutCombatResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool OpenTutorialMapLesson();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool TrackTutorialQuestLesson();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool EnterEldoriaStarterArea();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool ApplyCharacterCreationToPrototype(const FHuwamCharacterCreationRequest& Request, FHuwamCharacterCreationResult& OutResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="OwningPlayer"))
    bool ShowCharacterCreationScreen(APlayerController* OwningPlayer = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="bRestoreGameInput"))
    void HideCharacterCreationScreen(bool bRestoreGameInput = true);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool AttackMonster(AHuwamMonsterEncounterActor* Monster, FHuwamCombatResult& OutCombatResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool ClaimMonsterRewards(AHuwamMonsterEncounterActor* Monster, FHuwamRewardGrantResult& OutRewardResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool AttackMonsterAndClaimIfDefeated(AHuwamMonsterEncounterActor* Monster, FHuwamCombatResult& OutCombatResult, FHuwamRewardGrantResult& OutRewardResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool TurnInSlimeQuest(FHuwamQuestRuntimeState& OutTurnedInQuest);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool TurnInGatherFieldSuppliesQuest(UHuwamNpcSupplyComponent* RecipientSupply, FHuwamQuestRuntimeState& OutTurnedInQuest, FHuwamNpcSupplyDeliveryResult& OutDeliveryResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    bool SellAcceptedSupplyItemToNpc(UHuwamNpcSupplyComponent* BuyerSupply, const FString& ItemId, int32 Quantity, FHuwamNpcSupplyPurchaseResult& OutPurchaseResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    FHuwamHudSnapshot RefreshPrototypeHud();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    FHuwamWorldInteractionSnapshot RefreshWorldInteractionFocus();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="OwningPlayer"))
    bool InteractWithFocusedWorldTarget(FHuwamWorldInteractionResult& OutResult, APlayerController* OwningPlayer = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="OwningPlayer"))
    bool ShowNpcSupplyQuestInteractionScreen(AHuwamNpcSupplyQuestOfferActor* QuestRequester, APlayerController* OwningPlayer = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="bRestoreGameInput"))
    void HideNpcSupplyQuestInteractionScreen(bool bRestoreGameInput = true);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="OwningPlayer"))
    bool ShowWorldInteractionPromptScreen(APlayerController* OwningPlayer = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    void HideWorldInteractionPromptScreen();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="OwningPlayer"))
    bool ShowTutorialLabPromptScreen(APlayerController* OwningPlayer = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    void HideTutorialLabPromptScreen();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="OwningPlayer"))
    bool ShowPrototypeHudScreen(APlayerController* OwningPlayer = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    void HidePrototypeHudScreen();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="OwningPlayer"))
    bool ShowPrototypeMenuScreen(EHuwamPrototypeMenuPage Page, APlayerController* OwningPlayer = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="bRestoreGameInput"))
    void HidePrototypeMenuScreen(bool bRestoreGameInput = true);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="SlotName,UserIndex"))
    bool SavePrototypeProgress(const FString& SlotName = TEXT("HuwamPrototypeSlice"), int32 UserIndex = 0);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="SlotName,UserIndex"))
    bool LoadPrototypeProgress(const FString& SlotName = TEXT("HuwamPrototypeSlice"), int32 UserIndex = 0);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="OwningPlayer"))
    bool EnablePrototypeWorldInteractionInput(APlayerController* OwningPlayer = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player", meta=(AdvancedDisplay="OwningPlayer"))
    void DisablePrototypeWorldInteractionInput(APlayerController* OwningPlayer = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Player")
    void SetPrototypeAvatarActor(AActor* InAvatarActor);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    AActor* GetPrototypeAvatarActor() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamInventoryComponent* GetInventoryComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamSurvivalVitalsComponent* GetSurvivalVitalsComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamEquipmentComponent* GetEquipmentComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamCharacterStatsComponent* GetCharacterStatsComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamCharacterCreationComponent* GetCharacterCreationComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamCharacterCreationMenuDataComponent* GetCharacterCreationMenuDataComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamCharacterCreationScreenWidget* GetCharacterCreationScreen() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamCombatComponent* GetCombatComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamRewardComponent* GetRewardComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamQuestComponent* GetQuestComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamLiveContentComponent* GetLiveContentComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamMapAwarenessComponent* GetMapAwarenessComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamTutorialLabComponent* GetTutorialLabComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamWorldInteractionComponent* GetWorldInteractionComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamNpcSupplyQuestInteractionWidget* GetNpcSupplyQuestInteractionScreen() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamWorldInteractionPromptWidget* GetWorldInteractionPromptScreen() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamTutorialLabPromptWidget* GetTutorialLabPromptScreen() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamPrototypeHudWidget* GetPrototypeHudScreen() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamPrototypeMenuWidget* GetPrototypeMenuScreen() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Player")
    UHuwamHudDataComponent* GetHudDataComponent() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamInventoryComponent> Inventory;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamSurvivalVitalsComponent> SurvivalVitals;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamEquipmentComponent> Equipment;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamCharacterStatsComponent> CharacterStats;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamCharacterCreationComponent> CharacterCreation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamCharacterCreationMenuDataComponent> CharacterCreationMenuData;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamCombatComponent> Combat;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamRewardComponent> Rewards;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamQuestComponent> Quests;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamLiveContentComponent> LiveContent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamMapAwarenessComponent> MapAwareness;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamTutorialLabComponent> TutorialLab;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamWorldInteractionComponent> WorldInteraction;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamHudDataComponent> HudData;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<AActor> PrototypeAvatarActor;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    bool bAutoInitializeOnBeginPlay = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    bool bShowCharacterCreationScreenOnBeginPlay = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    bool bGrantStarterLoadoutOnInitialize = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    bool bStartSlimeQuestOnInitialize = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    bool bBeginTutorialLabAfterCharacterCreation = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    bool bShowWorldInteractionPromptOnInitialize = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    bool bShowPrototypeHudOnInitialize = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    bool bEnterEldoriaAfterTutorial = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    bool bEnablePrototypeWorldInteractionInputOnBeginPlay = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    bool bConfiguredForThirdPersonBootstrap = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    FHuwamCombatRollConfig StarterMeleeAttackConfig;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TSubclassOf<UHuwamCharacterCreationScreenWidget> CharacterCreationScreenClass;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamCharacterCreationScreenWidget> CharacterCreationScreen;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TSubclassOf<UHuwamNpcSupplyQuestInteractionWidget> NpcSupplyQuestInteractionScreenClass;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamNpcSupplyQuestInteractionWidget> NpcSupplyQuestInteractionScreen;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TSubclassOf<UHuwamWorldInteractionPromptWidget> WorldInteractionPromptScreenClass;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamWorldInteractionPromptWidget> WorldInteractionPromptScreen;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TSubclassOf<UHuwamTutorialLabPromptWidget> TutorialLabPromptScreenClass;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamTutorialLabPromptWidget> TutorialLabPromptScreen;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TSubclassOf<UHuwamPrototypeHudWidget> PrototypeHudScreenClass;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamPrototypeHudWidget> PrototypeHudScreen;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TSubclassOf<UHuwamPrototypeMenuWidget> PrototypeMenuScreenClass;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    TObjectPtr<UHuwamPrototypeMenuWidget> PrototypeMenuScreen;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Prototype Player")
    FKey PrototypeWorldInteractKey;

private:
    void InitializePrototypeSystemsAfterCharacterCreation(bool bStartFirstSlimeQuest);
    bool AddInventoryItemIfMissing(const FString& ItemId, int32 Quantity);
    bool EquipItemIfAvailable(const FString& ItemId, const FString& SlotId);

    UFUNCTION()
    void HandlePrototypeWorldInteractInput();

    UFUNCTION()
    void HandleTutorialLabChanged(FHuwamTutorialLabSnapshot Snapshot);

    AHuwamEldoriaStarterEntryActor* FindEldoriaStarterEntry() const;

    bool bPrototypeWorldInteractionInputBound = false;
    bool bHasEnteredEldoriaStarterArea = false;
};
