#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Gameplay/HuwamCharacterCreationMenuDataComponent.h"
#include "HuwamCharacterCreationScreenWidget.generated.h"

class AHuwamPrototypePlayerActor;
class UEditableTextBox;
class UTextBlock;
class UVerticalBox;
class UWidgetTree;
class UWrapBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHuwamCharacterCreationScreenSnapshotSignature, const FHuwamCharacterCreationMenuSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHuwamCharacterCreationScreenResultSignature, const FHuwamCharacterCreationResult&, Result);

UCLASS()
class HUWAM_API UHuwamCharacterCreationOptionButton : public UButton
{
    GENERATED_BODY()

public:
    void ConfigureOption(class UHuwamCharacterCreationScreenWidget* InOwnerScreen, const FHuwamCharacterCreationOption& InOption);

protected:
    UFUNCTION()
    void HandleOptionClicked();

private:
    UPROPERTY(Transient)
    TObjectPtr<class UHuwamCharacterCreationScreenWidget> OwnerScreen;

    UPROPERTY(Transient)
    EHuwamCharacterCreationOptionType OptionType = EHuwamCharacterCreationOptionType::Race;

    UPROPERTY(Transient)
    FString OptionId;
};

UCLASS(Blueprintable)
class HUWAM_API UHuwamCharacterCreationScreenWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Huwam|Character Creation Screen")
    FHuwamCharacterCreationScreenSnapshotSignature OnMenuSnapshotChanged;

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Character Creation Screen")
    FHuwamCharacterCreationScreenResultSignature OnCharacterCreationConfirmed;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Character Creation Screen")
    void SetPrototypePlayer(AHuwamPrototypePlayerActor* InPrototypePlayer);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Character Creation Screen")
    AHuwamPrototypePlayerActor* GetPrototypePlayer() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Character Creation Screen")
    FHuwamCharacterCreationMenuSnapshot GetCurrentMenuSnapshot() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Character Creation Screen")
    FHuwamCharacterCreationResult GetLastCreationResult() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Character Creation Screen")
    bool RefreshMenu();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Character Creation Screen")
    bool SelectOption(EHuwamCharacterCreationOptionType OptionType, const FString& OptionId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Character Creation Screen")
    void SetCharacterName(const FText& CharacterName);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Character Creation Screen")
    bool ConfirmCharacter();

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void NativeConstruct() override;

private:
    void BuildNativeWidgetTree();
    void RefreshNativeScreen();
    void RebuildNativeOptionSections();
    void AddNativeOptionSection(const FText& Heading, const FText& CountText, const TArray<FHuwamCharacterCreationOption>& Options);
    UHuwamCharacterCreationOptionButton* BuildNativeOptionButton(const FHuwamCharacterCreationOption& Option);
    UTextBlock* BuildNativeTextBlock(UWidgetTree* Tree, const FText& Text, int32 FontSize, const FLinearColor& Color) const;
    AHuwamPrototypePlayerActor* FindPrototypePlayerInWorld() const;
    FText BuildValidationText() const;
    FText BuildCreationResultText(const FHuwamCharacterCreationResult& Result) const;

    UFUNCTION()
    void HandleCharacterNameChanged(const FText& CharacterName);

    UFUNCTION()
    void HandleConfirmClicked();

    UPROPERTY(Transient)
    TObjectPtr<AHuwamPrototypePlayerActor> PrototypePlayer;

    UPROPERTY(Transient)
    FHuwamCharacterCreationRequest CurrentRequest;

    UPROPERTY(Transient)
    FHuwamCharacterCreationMenuSnapshot CurrentSnapshot;

    UPROPERTY(Transient)
    FHuwamCharacterCreationResult LastCreationResult;

    UPROPERTY(Transient)
    TObjectPtr<UEditableTextBox> CharacterNameEntry;

    UPROPERTY(Transient)
    TObjectPtr<UVerticalBox> NativeOptionSections;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> NativeValidationText;

    UPROPERTY(Transient)
    TObjectPtr<UButton> NativeConfirmButton;

    bool bHasCurrentRequest = false;
};
