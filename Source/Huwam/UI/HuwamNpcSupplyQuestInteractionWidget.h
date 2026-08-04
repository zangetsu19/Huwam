#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/HuwamNpcSupplyQuestOfferActor.h"
#include "HuwamNpcSupplyQuestInteractionWidget.generated.h"

class AHuwamPrototypePlayerActor;
class UButton;
class UTextBlock;
class UWidgetTree;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHuwamNpcSupplyQuestWidgetSnapshotSignature, const FHuwamNpcSupplyQuestInteractionSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHuwamNpcSupplyQuestWidgetResultSignature, const FHuwamNpcSupplyQuestInteractionResult&, Result);

UCLASS(Blueprintable)
class HUWAM_API UHuwamNpcSupplyQuestInteractionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Huwam|NPC Supply Quest Widget")
    FHuwamNpcSupplyQuestWidgetSnapshotSignature OnInteractionSnapshotChanged;

    UPROPERTY(BlueprintAssignable, Category = "Huwam|NPC Supply Quest Widget")
    FHuwamNpcSupplyQuestWidgetResultSignature OnInteractionRouted;

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply Quest Widget")
    void SetInteractionSource(AHuwamPrototypePlayerActor* InPrototypePlayer, AHuwamNpcSupplyQuestOfferActor* InQuestRequester);

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply Quest Widget")
    bool RefreshInteraction();

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply Quest Widget")
    bool RouteInteraction(bool bAcceptOfferWhenAvailable);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply Quest Widget")
    AHuwamPrototypePlayerActor* GetPrototypePlayer() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply Quest Widget")
    AHuwamNpcSupplyQuestOfferActor* GetQuestRequester() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply Quest Widget")
    FHuwamNpcSupplyQuestInteractionSnapshot GetCurrentInteractionSnapshot() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply Quest Widget")
    FHuwamNpcSupplyQuestInteractionResult GetLastInteractionResult() const;

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void NativeConstruct() override;

private:
    void BuildNativeWidgetTree();
    void RefreshNativeWidget();
    UTextBlock* BuildNativeTextBlock(UWidgetTree* Tree, const FText& Text, int32 FontSize, const FLinearColor& Color) const;
    AHuwamPrototypePlayerActor* FindPrototypePlayerInWorld() const;
    AHuwamNpcSupplyQuestOfferActor* FindQuestRequesterInWorld() const;
    FText BuildNeedReasonText() const;
    FText BuildObjectiveText() const;
    FText BuildRewardText() const;
    FText BuildResultText() const;
    FText BuildPrimaryActionText() const;
    FString FindFirstAcceptedSurplusItemId() const;

    UFUNCTION()
    void HandlePrimaryActionClicked();

    UFUNCTION()
    void HandleAcceptOfferClicked();

    UFUNCTION()
    void HandleSellSurplusClicked();

    UFUNCTION()
    void HandleCloseClicked();

    UPROPERTY(Transient)
    TObjectPtr<AHuwamPrototypePlayerActor> PrototypePlayer;

    UPROPERTY(Transient)
    TObjectPtr<AHuwamNpcSupplyQuestOfferActor> QuestRequester;

    UPROPERTY(Transient)
    FHuwamNpcSupplyQuestInteractionSnapshot CurrentSnapshot;

    UPROPERTY(Transient)
    FHuwamNpcSupplyQuestInteractionResult LastInteractionResult;

    UPROPERTY(Transient)
    FHuwamNpcSupplyPurchaseResult LastSurplusSaleResult;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativePromptText;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeQuestTitleText;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeNeedReasonText;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeObjectiveText;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeRewardText;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeResultText;

    UPROPERTY(Transient)
    TObjectPtr<UButton> NativePrimaryActionButton;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativePrimaryActionText;

    UPROPERTY(Transient)
    TObjectPtr<UButton> NativeAcceptOfferButton;

    UPROPERTY(Transient)
    TObjectPtr<UButton> NativeSellSurplusButton;

    UPROPERTY(Transient)
    TObjectPtr<UButton> NativeCloseButton;
};
