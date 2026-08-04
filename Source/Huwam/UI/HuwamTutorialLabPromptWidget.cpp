#include "UI/HuwamTutorialLabPromptWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "EngineUtils.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"

namespace
{
    const FLinearColor HuwamTutorialPanelBackground(0.06f, 0.07f, 0.10f, 0.92f);
    const FLinearColor HuwamTutorialTitleText(0.98f, 0.94f, 0.80f, 1.0f);
    const FLinearColor HuwamTutorialBodyText(0.94f, 0.96f, 0.98f, 1.0f);
    const FLinearColor HuwamTutorialLaucianText(0.72f, 0.83f, 0.88f, 1.0f);
}

TSharedRef<SWidget> UHuwamTutorialLabPromptWidget::RebuildWidget()
{
    if (WidgetTree && !WidgetTree->RootWidget)
    {
        BuildNativeWidgetTree();
    }

    return Super::RebuildWidget();
}

void UHuwamTutorialLabPromptWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (WidgetTree && !WidgetTree->RootWidget)
    {
        BuildNativeWidgetTree();
    }

    if (!PrototypePlayer)
    {
        PrototypePlayer = FindPrototypePlayerInWorld();
    }

    BindToTutorialLab();
    RefreshPrompt();
}

void UHuwamTutorialLabPromptWidget::NativeDestruct()
{
    UnbindFromTutorialLab();
    Super::NativeDestruct();
}

void UHuwamTutorialLabPromptWidget::SetPrototypePlayer(AHuwamPrototypePlayerActor* InPrototypePlayer)
{
    UnbindFromTutorialLab();
    PrototypePlayer = InPrototypePlayer;
    BindToTutorialLab();
    RefreshPrompt();
}

bool UHuwamTutorialLabPromptWidget::RefreshPrompt()
{
    UHuwamTutorialLabComponent* TutorialLab = PrototypePlayer
        ? PrototypePlayer->GetTutorialLabComponent()
        : nullptr;
    CurrentSnapshot = TutorialLab
        ? TutorialLab->GetTutorialLabSnapshot()
        : FHuwamTutorialLabSnapshot();

    RefreshNativePrompt();
    return CurrentSnapshot.CurrentBeat != EHuwamTutorialLabBeat::Inactive;
}

AHuwamPrototypePlayerActor* UHuwamTutorialLabPromptWidget::GetPrototypePlayer() const
{
    return PrototypePlayer;
}

FHuwamTutorialLabSnapshot UHuwamTutorialLabPromptWidget::GetCurrentTutorialSnapshot() const
{
    return CurrentSnapshot;
}

void UHuwamTutorialLabPromptWidget::BindToTutorialLab()
{
    UHuwamTutorialLabComponent* TutorialLab = PrototypePlayer
        ? PrototypePlayer->GetTutorialLabComponent()
        : nullptr;
    if (TutorialLab)
    {
        TutorialLab->OnTutorialLabChanged.AddUniqueDynamic(this, &UHuwamTutorialLabPromptWidget::HandleTutorialLabChanged);
    }
}

void UHuwamTutorialLabPromptWidget::UnbindFromTutorialLab()
{
    UHuwamTutorialLabComponent* TutorialLab = PrototypePlayer
        ? PrototypePlayer->GetTutorialLabComponent()
        : nullptr;
    if (TutorialLab)
    {
        TutorialLab->OnTutorialLabChanged.RemoveDynamic(this, &UHuwamTutorialLabPromptWidget::HandleTutorialLabChanged);
    }
}

void UHuwamTutorialLabPromptWidget::BuildNativeWidgetTree()
{
    if (!WidgetTree)
    {
        return;
    }

    NativePromptPanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("TutorialLabPromptPanel"));
    NativePromptPanel->SetBrushColor(HuwamTutorialPanelBackground);
    NativePromptPanel->SetPadding(FMargin(18.0f, 12.0f));
    WidgetTree->RootWidget = NativePromptPanel;

    UVerticalBox* PromptContent = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("TutorialLabPromptContent"));
    NativePromptPanel->SetContent(PromptContent);

    NativeBeatTitleText = BuildNativeTextBlock(WidgetTree, 18, HuwamTutorialTitleText);
    PromptContent->AddChildToVerticalBox(NativeBeatTitleText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 5.0f));

    NativeInstructionText = BuildNativeTextBlock(WidgetTree, 15, HuwamTutorialBodyText);
    PromptContent->AddChildToVerticalBox(NativeInstructionText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 5.0f));

    NativeLaucianLineText = BuildNativeTextBlock(WidgetTree, 13, HuwamTutorialLaucianText);
    PromptContent->AddChildToVerticalBox(NativeLaucianLineText);
}

void UHuwamTutorialLabPromptWidget::RefreshNativePrompt()
{
    const bool bShowPrompt = CurrentSnapshot.bActive
        && CurrentSnapshot.CurrentBeat != EHuwamTutorialLabBeat::Inactive
        && (!CurrentSnapshot.BeatTitle.IsEmpty()
            || !CurrentSnapshot.InstructionText.IsEmpty()
            || !CurrentSnapshot.LaucianLine.IsEmpty());

    if (NativeBeatTitleText)
    {
        NativeBeatTitleText->SetText(CurrentSnapshot.BeatTitle);
    }

    if (NativeInstructionText)
    {
        NativeInstructionText->SetText(CurrentSnapshot.InstructionText);
    }

    if (NativeLaucianLineText)
    {
        NativeLaucianLineText->SetText(CurrentSnapshot.LaucianLine);
    }

    SetVisibility(bShowPrompt ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

UTextBlock* UHuwamTutorialLabPromptWidget::BuildNativeTextBlock(UWidgetTree* InWidgetTree, int32 FontSize, const FLinearColor& Color) const
{
    if (!InWidgetTree)
    {
        return nullptr;
    }

    UTextBlock* TextBlock = InWidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    FSlateFontInfo Font = TextBlock->GetFont();
    Font.Size = FontSize;
    TextBlock->SetFont(Font);
    TextBlock->SetColorAndOpacity(FSlateColor(Color));
    TextBlock->SetAutoWrapText(true);
    TextBlock->SetWrapTextAt(480.0f);
    return TextBlock;
}

AHuwamPrototypePlayerActor* UHuwamTutorialLabPromptWidget::FindPrototypePlayerInWorld() const
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

void UHuwamTutorialLabPromptWidget::HandleTutorialLabChanged(FHuwamTutorialLabSnapshot Snapshot)
{
    CurrentSnapshot = Snapshot;
    RefreshNativePrompt();
}
