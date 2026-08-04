#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gameplay/HuwamHudDataComponent.h"
#include "HuwamPrototypeMenuWidget.generated.h"

class AHuwamPrototypePlayerActor;
class UButton;
class UTextBlock;
class UWidgetTree;

UENUM(BlueprintType)
enum class EHuwamPrototypeMenuPage : uint8
{
    InventoryEquipment UMETA(DisplayName = "Inventory And Equipment"),
    Map UMETA(DisplayName = "Map"),
    Quests UMETA(DisplayName = "Quests")
};

UCLASS(Blueprintable)
class HUWAM_API UHuwamPrototypeMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Menu")
    void SetPrototypePlayer(AHuwamPrototypePlayerActor* InPrototypePlayer);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Menu")
    bool OpenPage(EHuwamPrototypeMenuPage InPage);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Prototype Menu")
    bool RefreshMenu();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Menu")
    EHuwamPrototypeMenuPage GetCurrentPage() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Prototype Menu")
    FHuwamHudSnapshot GetCurrentHudSnapshot() const;

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void NativeConstruct() override;

private:
    void BuildNativeWidgetTree();
    void RefreshNativeMenu();
    UTextBlock* BuildNativeTextBlock(UWidgetTree* InWidgetTree, const FText& Text, int32 FontSize, const FLinearColor& Color) const;
    UButton* BuildNativeButton(UWidgetTree* InWidgetTree, const FText& Text, const FLinearColor& Color) const;
    FText BuildPageTitle() const;
    FText BuildPageBody() const;
    FText BuildInventoryEquipmentBody() const;
    FText BuildMapBody() const;
    FText BuildQuestBody() const;
    AHuwamPrototypePlayerActor* FindPrototypePlayerInWorld() const;

    UFUNCTION()
    void HandleInventoryPageClicked();

    UFUNCTION()
    void HandleMapPageClicked();

    UFUNCTION()
    void HandleQuestsPageClicked();

    UFUNCTION()
    void HandleInspectInventoryClicked();

    UFUNCTION()
    void HandleInspectEquipmentClicked();

    UFUNCTION()
    void HandleMapLessonClicked();

    UFUNCTION()
    void HandleQuestLessonClicked();

    UFUNCTION()
    void HandleCloseClicked();

    UPROPERTY(Transient)
    TObjectPtr<AHuwamPrototypePlayerActor> PrototypePlayer;

    UPROPERTY(Transient)
    EHuwamPrototypeMenuPage CurrentPage = EHuwamPrototypeMenuPage::InventoryEquipment;

    UPROPERTY(Transient)
    FHuwamHudSnapshot CurrentSnapshot;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeTitleText;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeBodyText;

    UPROPERTY(Transient)
    TObjectPtr<UButton> NativeInspectInventoryButton;

    UPROPERTY(Transient)
    TObjectPtr<UButton> NativeInspectEquipmentButton;

    UPROPERTY(Transient)
    TObjectPtr<UButton> NativeMapLessonButton;

    UPROPERTY(Transient)
    TObjectPtr<UButton> NativeQuestLessonButton;
};
