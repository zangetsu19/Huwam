#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/HuwamTutorialLabComponent.h"
#include "HuwamTutorialLabPromptWidget.generated.h"

class AHuwamPrototypePlayerActor;
class UBorder;
class UTextBlock;
class UWidgetTree;

UCLASS(Blueprintable)
class HUWAM_API UHuwamTutorialLabPromptWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab Prompt")
    void SetPrototypePlayer(AHuwamPrototypePlayerActor* InPrototypePlayer);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Tutorial Lab Prompt")
    bool RefreshPrompt();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab Prompt")
    AHuwamPrototypePlayerActor* GetPrototypePlayer() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Tutorial Lab Prompt")
    FHuwamTutorialLabSnapshot GetCurrentTutorialSnapshot() const;

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
    void BindToTutorialLab();
    void UnbindFromTutorialLab();
    void BuildNativeWidgetTree();
    void RefreshNativePrompt();
    UTextBlock* BuildNativeTextBlock(UWidgetTree* InWidgetTree, int32 FontSize, const FLinearColor& Color) const;
    AHuwamPrototypePlayerActor* FindPrototypePlayerInWorld() const;

    UFUNCTION()
    void HandleTutorialLabChanged(FHuwamTutorialLabSnapshot Snapshot);

    UPROPERTY(Transient)
    TObjectPtr<AHuwamPrototypePlayerActor> PrototypePlayer;

    UPROPERTY(Transient)
    FHuwamTutorialLabSnapshot CurrentSnapshot;

    UPROPERTY(Transient)
    TObjectPtr<UBorder> NativePromptPanel;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeBeatTitleText;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeInstructionText;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeLaucianLineText;
};
