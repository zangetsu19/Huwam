#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/HuwamHudDataComponent.h"
#include "HuwamPrototypeHudWidget.generated.h"

class AHuwamPrototypePlayerActor;
class UTextBlock;
class UWidgetTree;

UCLASS(Blueprintable)
class HUWAM_API UHuwamPrototypeHudWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype HUD")
    void SetPrototypePlayer(AHuwamPrototypePlayerActor* InPrototypePlayer);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype HUD")
    bool RefreshHud();

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
    void BuildNativeWidgetTree();
    void RefreshNativeHud();
    void BindToHudData();
    void UnbindFromHudData();
    UTextBlock* BuildNativeTextBlock(UWidgetTree* InWidgetTree, int32 FontSize, const FLinearColor& Color) const;
    AHuwamPrototypePlayerActor* FindPrototypePlayerInWorld() const;

    UFUNCTION()
    void HandleHudSnapshotChanged(FHuwamHudSnapshot Snapshot);

    UPROPERTY(Transient)
    TObjectPtr<AHuwamPrototypePlayerActor> PrototypePlayer;

    UPROPERTY(Transient)
    FHuwamHudSnapshot CurrentSnapshot;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeVitalsText;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeWalletText;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeMapText;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeQuestText;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeInteractionText;
};
