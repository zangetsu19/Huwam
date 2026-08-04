#include "UI/HuwamCharacterCreationScreenWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"

namespace
{
    const FLinearColor HuwamScreenBackground(0.035f, 0.045f, 0.065f, 0.98f);
    const FLinearColor HuwamSectionBackground(0.08f, 0.10f, 0.13f, 0.96f);
    const FLinearColor HuwamBodyText(0.90f, 0.92f, 0.95f, 1.0f);
    const FLinearColor HuwamMutedText(0.64f, 0.70f, 0.76f, 1.0f);
    const FLinearColor HuwamSelectedButton(0.18f, 0.43f, 0.30f, 1.0f);
    const FLinearColor HuwamDefaultButton(0.14f, 0.17f, 0.22f, 1.0f);

    FText BuildSlotCountText(int32 Used, int32 Limit)
    {
        return FText::Format(
            FText::FromString(TEXT("{0} / {1} selected")),
            FText::AsNumber(Used),
            FText::AsNumber(Limit)
        );
    }

    FString JoinMessages(const TArray<FText>& Messages)
    {
        TArray<FString> Lines;
        for (const FText& Message : Messages)
        {
            if (!Message.IsEmpty())
            {
                Lines.Add(Message.ToString());
            }
        }

        return FString::Join(Lines, TEXT("\n"));
    }
}

void UHuwamCharacterCreationOptionButton::ConfigureOption(UHuwamCharacterCreationScreenWidget* InOwnerScreen, const FHuwamCharacterCreationOption& InOption)
{
    OwnerScreen = InOwnerScreen;
    OptionType = InOption.OptionType;
    OptionId = InOption.OptionId;
    SetIsEnabled(InOption.bAvailable);
    SetBackgroundColor(InOption.bSelected ? HuwamSelectedButton : HuwamDefaultButton);
    SetToolTipText(InOption.Description);
    OnClicked.AddDynamic(this, &UHuwamCharacterCreationOptionButton::HandleOptionClicked);
}

void UHuwamCharacterCreationOptionButton::HandleOptionClicked()
{
    if (OwnerScreen)
    {
        OwnerScreen->SelectOption(OptionType, OptionId);
    }
}

TSharedRef<SWidget> UHuwamCharacterCreationScreenWidget::RebuildWidget()
{
    if (WidgetTree && !WidgetTree->RootWidget)
    {
        BuildNativeWidgetTree();
    }

    return Super::RebuildWidget();
}

void UHuwamCharacterCreationScreenWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (WidgetTree && !WidgetTree->RootWidget)
    {
        BuildNativeWidgetTree();
    }

    if (!PrototypePlayer)
    {
        SetPrototypePlayer(FindPrototypePlayerInWorld());
    }
    else
    {
        RefreshMenu();
    }
}

void UHuwamCharacterCreationScreenWidget::SetPrototypePlayer(AHuwamPrototypePlayerActor* InPrototypePlayer)
{
    PrototypePlayer = InPrototypePlayer;
    bHasCurrentRequest = false;
    RefreshMenu();
}

AHuwamPrototypePlayerActor* UHuwamCharacterCreationScreenWidget::GetPrototypePlayer() const
{
    return PrototypePlayer;
}

FHuwamCharacterCreationMenuSnapshot UHuwamCharacterCreationScreenWidget::GetCurrentMenuSnapshot() const
{
    return CurrentSnapshot;
}

FHuwamCharacterCreationResult UHuwamCharacterCreationScreenWidget::GetLastCreationResult() const
{
    return LastCreationResult;
}

bool UHuwamCharacterCreationScreenWidget::RefreshMenu()
{
    UHuwamCharacterCreationMenuDataComponent* MenuData = PrototypePlayer ? PrototypePlayer->GetCharacterCreationMenuDataComponent() : nullptr;
    if (!MenuData)
    {
        CurrentSnapshot = FHuwamCharacterCreationMenuSnapshot();
        CurrentSnapshot.ValidationResult.Errors.Add(FText::FromString(TEXT("No prototype player is linked to this character creation screen.")));
        CurrentSnapshot.bCanConfirm = false;
        RefreshNativeScreen();
        OnMenuSnapshotChanged.Broadcast(CurrentSnapshot);
        return false;
    }

    if (!bHasCurrentRequest)
    {
        CurrentSnapshot = MenuData->BuildDefaultMenuSnapshot();
        CurrentRequest = CurrentSnapshot.Request;
        bHasCurrentRequest = true;
    }
    else
    {
        CurrentSnapshot = MenuData->BuildMenuSnapshot(CurrentRequest);
        CurrentRequest = CurrentSnapshot.Request;
    }

    RefreshNativeScreen();
    OnMenuSnapshotChanged.Broadcast(CurrentSnapshot);
    return true;
}

bool UHuwamCharacterCreationScreenWidget::SelectOption(EHuwamCharacterCreationOptionType OptionType, const FString& OptionId)
{
    UHuwamCharacterCreationMenuDataComponent* MenuData = PrototypePlayer ? PrototypePlayer->GetCharacterCreationMenuDataComponent() : nullptr;
    if (!MenuData || OptionId.IsEmpty())
    {
        return false;
    }

    FHuwamCharacterCreationRequest UpdatedRequest;
    bool bChanged = false;
    if (OptionType == EHuwamCharacterCreationOptionType::Race || OptionType == EHuwamCharacterCreationOptionType::SubRace)
    {
        bChanged = MenuData->BuildRequestWithSingleSelection(CurrentRequest, OptionType, OptionId, UpdatedRequest);
    }
    else
    {
        bChanged = MenuData->BuildRequestWithToggledSelection(CurrentRequest, OptionType, OptionId, UpdatedRequest);
    }

    if (!bChanged)
    {
        return false;
    }

    CurrentRequest = UpdatedRequest;
    LastCreationResult = FHuwamCharacterCreationResult();
    bHasCurrentRequest = true;
    return RefreshMenu();
}

void UHuwamCharacterCreationScreenWidget::SetCharacterName(const FText& CharacterName)
{
    CurrentRequest.CharacterName = CharacterName;
    LastCreationResult = FHuwamCharacterCreationResult();
    bHasCurrentRequest = true;
    RefreshMenu();
}

bool UHuwamCharacterCreationScreenWidget::ConfirmCharacter()
{
    if (!PrototypePlayer || !CurrentSnapshot.bCanConfirm)
    {
        return false;
    }

    LastCreationResult = FHuwamCharacterCreationResult();
    const bool bPrototypeReady = PrototypePlayer->InitializePrototypePlayerFromCharacterCreation(CurrentRequest, LastCreationResult);
    if (!bPrototypeReady)
    {
        RefreshNativeScreen();
        return false;
    }

    OnCharacterCreationConfirmed.Broadcast(LastCreationResult);

    if (APlayerController* OwningPlayer = GetOwningPlayer())
    {
        OwningPlayer->SetInputMode(FInputModeGameOnly());
        OwningPlayer->bShowMouseCursor = false;
    }

    RemoveFromParent();
    return true;
}

void UHuwamCharacterCreationScreenWidget::BuildNativeWidgetTree()
{
    if (!WidgetTree)
    {
        return;
    }

    UBorder* ScreenBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("CharacterCreationScreenBorder"));
    ScreenBorder->SetBrushColor(HuwamScreenBackground);
    ScreenBorder->SetPadding(FMargin(40.0f, 28.0f));
    WidgetTree->RootWidget = ScreenBorder;

    UScrollBox* PageScroll = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("CharacterCreationPageScroll"));
    ScreenBorder->SetContent(PageScroll);

    UVerticalBox* Page = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("CharacterCreationPage"));
    PageScroll->AddChild(Page);

    UTextBlock* Title = BuildNativeTextBlock(WidgetTree, FText::FromString(TEXT("Create A Character")), 30, HuwamBodyText);
    Page->AddChildToVerticalBox(Title)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));

    UTextBlock* Intro = BuildNativeTextBlock(
        WidgetTree,
        FText::FromString(TEXT("Choose the identity that will enter Laucian's tutorial lab.")),
        15,
        HuwamMutedText
    );
    Page->AddChildToVerticalBox(Intro)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 14.0f));

    CharacterNameEntry = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("CharacterNameEntry"));
    CharacterNameEntry->SetHintText(FText::FromString(TEXT("Character name")));
    CharacterNameEntry->OnTextChanged.AddDynamic(this, &UHuwamCharacterCreationScreenWidget::HandleCharacterNameChanged);
    Page->AddChildToVerticalBox(CharacterNameEntry)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 18.0f));

    NativeOptionSections = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("CharacterCreationOptionSections"));
    Page->AddChildToVerticalBox(NativeOptionSections)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 12.0f));

    NativeValidationText = BuildNativeTextBlock(WidgetTree, FText::GetEmpty(), 14, HuwamMutedText);
    Page->AddChildToVerticalBox(NativeValidationText)->SetPadding(FMargin(0.0f, 4.0f, 0.0f, 12.0f));

    NativeConfirmButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ConfirmCharacterButton"));
    NativeConfirmButton->SetBackgroundColor(FLinearColor(0.18f, 0.34f, 0.47f, 1.0f));
    NativeConfirmButton->OnClicked.AddDynamic(this, &UHuwamCharacterCreationScreenWidget::HandleConfirmClicked);
    NativeConfirmButton->AddChild(BuildNativeTextBlock(WidgetTree, FText::FromString(TEXT("Begin In Huwan")), 17, HuwamBodyText));
    Page->AddChildToVerticalBox(NativeConfirmButton)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 24.0f));
}

void UHuwamCharacterCreationScreenWidget::RefreshNativeScreen()
{
    if (CharacterNameEntry && CharacterNameEntry->GetText().ToString() != CurrentRequest.CharacterName.ToString())
    {
        CharacterNameEntry->SetText(CurrentRequest.CharacterName);
    }

    RebuildNativeOptionSections();

    if (NativeValidationText)
    {
        NativeValidationText->SetText(LastCreationResult.Errors.IsEmpty() ? BuildValidationText() : BuildCreationResultText(LastCreationResult));
        NativeValidationText->SetColorAndOpacity(FSlateColor(CurrentSnapshot.bCanConfirm ? HuwamMutedText : FLinearColor(0.96f, 0.58f, 0.50f, 1.0f)));
    }

    if (NativeConfirmButton)
    {
        NativeConfirmButton->SetIsEnabled(CurrentSnapshot.bCanConfirm);
    }
}

void UHuwamCharacterCreationScreenWidget::RebuildNativeOptionSections()
{
    if (!NativeOptionSections)
    {
        return;
    }

    NativeOptionSections->ClearChildren();
    AddNativeOptionSection(FText::FromString(TEXT("Race")), FText::FromString(TEXT("Choose one")), CurrentSnapshot.RaceOptions);
    AddNativeOptionSection(FText::FromString(TEXT("Sub-Race")), FText::FromString(TEXT("Optional")), CurrentSnapshot.SubRaceOptions);
    AddNativeOptionSection(FText::FromString(TEXT("Class Paths")), BuildSlotCountText(CurrentSnapshot.ClassSlotsUsed, CurrentSnapshot.ClassSlotLimit), CurrentSnapshot.ClassOptions);
    AddNativeOptionSection(FText::FromString(TEXT("Jobs")), BuildSlotCountText(CurrentSnapshot.JobSlotsUsed, CurrentSnapshot.JobSlotLimit), CurrentSnapshot.JobOptions);
    AddNativeOptionSection(FText::FromString(TEXT("Permanent Skills")), BuildSlotCountText(CurrentSnapshot.SkillSlotsUsed, CurrentSnapshot.SkillSlotLimit), CurrentSnapshot.SkillOptions);
    AddNativeOptionSection(FText::FromString(TEXT("Permanent Abilities")), BuildSlotCountText(CurrentSnapshot.AbilitySlotsUsed, CurrentSnapshot.AbilitySlotLimit), CurrentSnapshot.AbilityOptions);
}

void UHuwamCharacterCreationScreenWidget::AddNativeOptionSection(const FText& Heading, const FText& CountText, const TArray<FHuwamCharacterCreationOption>& Options)
{
    if (!WidgetTree || !NativeOptionSections)
    {
        return;
    }

    UBorder* SectionBorder = WidgetTree->ConstructWidget<UBorder>();
    SectionBorder->SetBrushColor(HuwamSectionBackground);
    SectionBorder->SetPadding(FMargin(14.0f));
    NativeOptionSections->AddChildToVerticalBox(SectionBorder)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 10.0f));

    UVerticalBox* Section = WidgetTree->ConstructWidget<UVerticalBox>();
    SectionBorder->SetContent(Section);

    Section->AddChildToVerticalBox(BuildNativeTextBlock(WidgetTree, Heading, 18, HuwamBodyText))->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 2.0f));
    Section->AddChildToVerticalBox(BuildNativeTextBlock(WidgetTree, CountText, 13, HuwamMutedText))->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 8.0f));

    UWrapBox* OptionWrap = WidgetTree->ConstructWidget<UWrapBox>();
    Section->AddChildToVerticalBox(OptionWrap);

    if (Options.IsEmpty())
    {
        OptionWrap->AddChildToWrapBox(BuildNativeTextBlock(WidgetTree, FText::FromString(TEXT("No options for this selection yet.")), 13, HuwamMutedText));
        return;
    }

    for (const FHuwamCharacterCreationOption& Option : Options)
    {
        UHuwamCharacterCreationOptionButton* OptionButton = BuildNativeOptionButton(Option);
        if (OptionButton)
        {
            OptionWrap->AddChildToWrapBox(OptionButton)->SetPadding(FMargin(0.0f, 0.0f, 8.0f, 8.0f));
        }
    }
}

UHuwamCharacterCreationOptionButton* UHuwamCharacterCreationScreenWidget::BuildNativeOptionButton(const FHuwamCharacterCreationOption& Option)
{
    if (!WidgetTree)
    {
        return nullptr;
    }

    UHuwamCharacterCreationOptionButton* OptionButton = WidgetTree->ConstructWidget<UHuwamCharacterCreationOptionButton>();
    OptionButton->ConfigureOption(this, Option);

    UVerticalBox* Copy = WidgetTree->ConstructWidget<UVerticalBox>();
    Copy->AddChildToVerticalBox(BuildNativeTextBlock(WidgetTree, Option.DisplayName, 15, HuwamBodyText))->SetPadding(FMargin(10.0f, 8.0f, 10.0f, 2.0f));
    if (!Option.DetailText.IsEmpty())
    {
        Copy->AddChildToVerticalBox(BuildNativeTextBlock(WidgetTree, Option.DetailText, 12, HuwamMutedText))->SetPadding(FMargin(10.0f, 0.0f, 10.0f, 8.0f));
    }

    OptionButton->AddChild(Copy);
    return OptionButton;
}

UTextBlock* UHuwamCharacterCreationScreenWidget::BuildNativeTextBlock(UWidgetTree* Tree, const FText& Text, int32 FontSize, const FLinearColor& Color) const
{
    UTextBlock* TextBlock = Tree ? Tree->ConstructWidget<UTextBlock>() : nullptr;
    if (!TextBlock)
    {
        return nullptr;
    }

    FSlateFontInfo Font = TextBlock->GetFont();
    Font.Size = FontSize;
    TextBlock->SetFont(Font);
    TextBlock->SetText(Text);
    TextBlock->SetColorAndOpacity(FSlateColor(Color));
    TextBlock->SetAutoWrapText(true);
    return TextBlock;
}

AHuwamPrototypePlayerActor* UHuwamCharacterCreationScreenWidget::FindPrototypePlayerInWorld() const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    for (TActorIterator<AHuwamPrototypePlayerActor> It(GetWorld()); It; ++It)
    {
        return *It;
    }

    return nullptr;
}

FText UHuwamCharacterCreationScreenWidget::BuildValidationText() const
{
    if (CurrentSnapshot.bCanConfirm)
    {
        return CurrentSnapshot.bUsingFallbackOptions
            ? FText::FromString(TEXT("Ready. Using fallback creation options until the registry tables are linked."))
            : FText::FromString(TEXT("Ready. Confirm to begin."));
    }

    const FString Errors = JoinMessages(CurrentSnapshot.ValidationResult.Errors);
    if (!Errors.IsEmpty())
    {
        return FText::FromString(Errors);
    }

    const FString Warnings = JoinMessages(CurrentSnapshot.ValidationResult.Warnings);
    return Warnings.IsEmpty()
        ? FText::FromString(TEXT("Choose a valid identity before beginning."))
        : FText::FromString(Warnings);
}

FText UHuwamCharacterCreationScreenWidget::BuildCreationResultText(const FHuwamCharacterCreationResult& Result) const
{
    const FString Errors = JoinMessages(Result.Errors);
    if (!Errors.IsEmpty())
    {
        return FText::FromString(Errors);
    }

    const FString Warnings = JoinMessages(Result.Warnings);
    return Warnings.IsEmpty()
        ? BuildValidationText()
        : FText::FromString(Warnings);
}

void UHuwamCharacterCreationScreenWidget::HandleCharacterNameChanged(const FText& CharacterName)
{
    SetCharacterName(CharacterName);
}

void UHuwamCharacterCreationScreenWidget::HandleConfirmClicked()
{
    ConfirmCharacter();
}
