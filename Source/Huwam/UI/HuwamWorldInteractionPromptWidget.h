#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/HuwamWorldInteractionComponent.h"
#include "HuwamWorldInteractionPromptWidget.generated.h"

class AHuwamPrototypePlayerActor;
class UBorder;
class UTextBlock;
class UWidgetTree;

UCLASS(Blueprintable)
class HUWAM_API UHuwamWorldInteractionPromptWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Huwam|World Interaction Prompt")
    void SetPrototypePlayer(AHuwamPrototypePlayerActor* InPrototypePlayer);

    UFUNCTION(BlueprintCallable, Category = "Huwam|World Interaction Prompt")
    bool RefreshPrompt();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|World Interaction Prompt")
    AHuwamPrototypePlayerActor* GetPrototypePlayer() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|World Interaction Prompt")
    FHuwamWorldInteractionSnapshot GetCurrentInteractionSnapshot() const;

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
    void BindToWorldInteraction();
    void UnbindFromWorldInteraction();
    void BuildNativeWidgetTree();
    void RefreshNativePrompt();
    AHuwamPrototypePlayerActor* FindPrototypePlayerInWorld() const;

    UFUNCTION()
    void HandleInteractionFocusChanged(FHuwamWorldInteractionSnapshot Snapshot);

    UPROPERTY(Transient)
    TObjectPtr<AHuwamPrototypePlayerActor> PrototypePlayer;

    UPROPERTY(Transient)
    FHuwamWorldInteractionSnapshot CurrentSnapshot;

    UPROPERTY(Transient)
    TObjectPtr<UBorder> NativePromptPanel;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativePromptText;
};
