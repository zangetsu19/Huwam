#include "UI/HuwamWorldInteractionPromptWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "EngineUtils.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"

namespace
{
    const FLinearColor HuwamPromptBackground(0.03f, 0.05f, 0.07f, 0.88f);
    const FLinearColor HuwamPromptText(0.95f, 0.97f, 0.98f, 1.0f);
}

TSharedRef<SWidget> UHuwamWorldInteractionPromptWidget::RebuildWidget()
{
    if (WidgetTree && !WidgetTree->RootWidget)
    {
        BuildNativeWidgetTree();
    }

    return Super::RebuildWidget();
}

void UHuwamWorldInteractionPromptWidget::NativeConstruct()
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

    BindToWorldInteraction();
    RefreshPrompt();
}

void UHuwamWorldInteractionPromptWidget::NativeDestruct()
{
    UnbindFromWorldInteraction();
    Super::NativeDestruct();
}

void UHuwamWorldInteractionPromptWidget::SetPrototypePlayer(AHuwamPrototypePlayerActor* InPrototypePlayer)
{
    UnbindFromWorldInteraction();
    PrototypePlayer = InPrototypePlayer;
    BindToWorldInteraction();
    RefreshPrompt();
}

bool UHuwamWorldInteractionPromptWidget::RefreshPrompt()
{
    if (!PrototypePlayer)
    {
        CurrentSnapshot = FHuwamWorldInteractionSnapshot();
        RefreshNativePrompt();
        return false;
    }

    CurrentSnapshot = PrototypePlayer->RefreshWorldInteractionFocus();
    RefreshNativePrompt();
    return CurrentSnapshot.bHasTarget;
}

AHuwamPrototypePlayerActor* UHuwamWorldInteractionPromptWidget::GetPrototypePlayer() const
{
    return PrototypePlayer;
}

FHuwamWorldInteractionSnapshot UHuwamWorldInteractionPromptWidget::GetCurrentInteractionSnapshot() const
{
    return CurrentSnapshot;
}

void UHuwamWorldInteractionPromptWidget::BindToWorldInteraction()
{
    UHuwamWorldInteractionComponent* WorldInteraction = PrototypePlayer
        ? PrototypePlayer->GetWorldInteractionComponent()
        : nullptr;
    if (WorldInteraction)
    {
        WorldInteraction->OnInteractionFocusChanged.AddUniqueDynamic(this, &UHuwamWorldInteractionPromptWidget::HandleInteractionFocusChanged);
    }
}

void UHuwamWorldInteractionPromptWidget::UnbindFromWorldInteraction()
{
    UHuwamWorldInteractionComponent* WorldInteraction = PrototypePlayer
        ? PrototypePlayer->GetWorldInteractionComponent()
        : nullptr;
    if (WorldInteraction)
    {
        WorldInteraction->OnInteractionFocusChanged.RemoveDynamic(this, &UHuwamWorldInteractionPromptWidget::HandleInteractionFocusChanged);
    }
}

void UHuwamWorldInteractionPromptWidget::BuildNativeWidgetTree()
{
    if (!WidgetTree)
    {
        return;
    }

    NativePromptPanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("WorldInteractionPromptPanel"));
    NativePromptPanel->SetBrushColor(HuwamPromptBackground);
    NativePromptPanel->SetPadding(FMargin(14.0f, 8.0f));
    WidgetTree->RootWidget = NativePromptPanel;

    NativePromptText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("WorldInteractionPromptText"));
    FSlateFontInfo Font = NativePromptText->GetFont();
    Font.Size = 16;
    NativePromptText->SetFont(Font);
    NativePromptText->SetColorAndOpacity(FSlateColor(HuwamPromptText));
    NativePromptText->SetAutoWrapText(true);
    NativePromptPanel->SetContent(NativePromptText);
}

void UHuwamWorldInteractionPromptWidget::RefreshNativePrompt()
{
    const bool bShowPrompt = CurrentSnapshot.bHasTarget
        && CurrentSnapshot.bCanInteract
        && !CurrentSnapshot.PromptText.IsEmpty();

    if (NativePromptText)
    {
        NativePromptText->SetText(CurrentSnapshot.PromptText);
    }

    SetVisibility(bShowPrompt ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

AHuwamPrototypePlayerActor* UHuwamWorldInteractionPromptWidget::FindPrototypePlayerInWorld() const
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

void UHuwamWorldInteractionPromptWidget::HandleInteractionFocusChanged(FHuwamWorldInteractionSnapshot Snapshot)
{
    CurrentSnapshot = Snapshot;
    RefreshNativePrompt();
}
