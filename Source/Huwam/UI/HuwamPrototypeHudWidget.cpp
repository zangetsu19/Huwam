#include "UI/HuwamPrototypeHudWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "EngineUtils.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"

namespace
{
    const FLinearColor HuwamHudBackground(0.025f, 0.035f, 0.045f, 0.82f);
    const FLinearColor HuwamHudVitalsText(0.96f, 0.91f, 0.86f, 1.0f);
    const FLinearColor HuwamHudBodyText(0.91f, 0.94f, 0.96f, 1.0f);
    const FLinearColor HuwamHudMutedText(0.68f, 0.76f, 0.83f, 1.0f);
}

TSharedRef<SWidget> UHuwamPrototypeHudWidget::RebuildWidget()
{
    if (WidgetTree && !WidgetTree->RootWidget)
    {
        BuildNativeWidgetTree();
    }

    return Super::RebuildWidget();
}

void UHuwamPrototypeHudWidget::NativeConstruct()
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

    BindToHudData();
    RefreshHud();
}

void UHuwamPrototypeHudWidget::NativeDestruct()
{
    UnbindFromHudData();
    Super::NativeDestruct();
}

void UHuwamPrototypeHudWidget::SetPrototypePlayer(AHuwamPrototypePlayerActor* InPrototypePlayer)
{
    UnbindFromHudData();
    PrototypePlayer = InPrototypePlayer;
    BindToHudData();
    RefreshHud();
}

bool UHuwamPrototypeHudWidget::RefreshHud()
{
    CurrentSnapshot = PrototypePlayer
        ? PrototypePlayer->RefreshPrototypeHud()
        : FHuwamHudSnapshot();
    RefreshNativeHud();
    return PrototypePlayer != nullptr;
}

void UHuwamPrototypeHudWidget::BuildNativeWidgetTree()
{
    if (!WidgetTree)
    {
        return;
    }

    UCanvasPanel* RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("PrototypeHudRoot"));
    WidgetTree->RootWidget = RootCanvas;

    UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("PrototypeHudPanel"));
    Panel->SetBrushColor(HuwamHudBackground);
    Panel->SetPadding(FMargin(14.0f, 10.0f));
    if (UCanvasPanelSlot* PanelSlot = RootCanvas->AddChildToCanvas(Panel))
    {
        PanelSlot->SetAnchors(FAnchors(0.0f, 0.0f));
        PanelSlot->SetAlignment(FVector2D::ZeroVector);
        PanelSlot->SetAutoSize(true);
        PanelSlot->SetPosition(FVector2D(22.0f, 22.0f));
    }

    UVerticalBox* Content = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("PrototypeHudContent"));
    Panel->SetContent(Content);

    NativeVitalsText = BuildNativeTextBlock(WidgetTree, 15, HuwamHudVitalsText);
    Content->AddChildToVerticalBox(NativeVitalsText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));

    NativeWalletText = BuildNativeTextBlock(WidgetTree, 13, HuwamHudBodyText);
    Content->AddChildToVerticalBox(NativeWalletText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));

    NativeMapText = BuildNativeTextBlock(WidgetTree, 13, HuwamHudMutedText);
    Content->AddChildToVerticalBox(NativeMapText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));

    NativeQuestText = BuildNativeTextBlock(WidgetTree, 13, HuwamHudBodyText);
    Content->AddChildToVerticalBox(NativeQuestText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));

    NativeInteractionText = BuildNativeTextBlock(WidgetTree, 13, HuwamHudMutedText);
    Content->AddChildToVerticalBox(NativeInteractionText);
}

void UHuwamPrototypeHudWidget::RefreshNativeHud()
{
    if (NativeVitalsText)
    {
        NativeVitalsText->SetText(FText::FromString(FString::Printf(
            TEXT("Health %d / %d   Mana %d / %d   Hunger %d%%   Thirst %d%%   Social %d%%   Effects %d"),
            CurrentSnapshot.Vitals.CurrentHealth,
            CurrentSnapshot.Vitals.MaxHealth,
            CurrentSnapshot.Vitals.CurrentMana,
            CurrentSnapshot.Vitals.MaxMana,
            FMath::RoundToInt(CurrentSnapshot.Survival.HungerPercent * 100.0f),
            FMath::RoundToInt(CurrentSnapshot.Survival.ThirstPercent * 100.0f),
            FMath::RoundToInt(CurrentSnapshot.Survival.SocialEnergyPercent * 100.0f),
            CurrentSnapshot.Survival.ActiveEffectIds.Num()
        )));
    }

    if (NativeWalletText)
    {
        NativeWalletText->SetText(FText::FromString(FString::Printf(
            TEXT("Wallet %lld MDP  %lld P  %lld G  %lld S  %lld C   XP %d   Pack %d / %d   Ledger %d / %lld C"),
            CurrentSnapshot.Wallet.Currency.MarquisDiamondPrints,
            CurrentSnapshot.Wallet.Currency.Platinum,
            CurrentSnapshot.Wallet.Currency.Gold,
            CurrentSnapshot.Wallet.Currency.Silver,
            CurrentSnapshot.Wallet.Currency.Copper,
            CurrentSnapshot.Wallet.TotalExperience,
            CurrentSnapshot.Inventory.UsedSlots,
            CurrentSnapshot.Inventory.MaxSlots,
            CurrentSnapshot.Economy.TransactionCount,
            CurrentSnapshot.Economy.TotalRevenueCopper
        )));
    }

    if (NativeMapText)
    {
        NativeMapText->SetText(FText::FromString(FString::Printf(
            TEXT("%s   %s"),
            *CurrentSnapshot.MapAwareness.DisplayName.ToString(),
            CurrentSnapshot.MapAwareness.bTrackedQuestMarkerVisible ? TEXT("Quest marker active") : TEXT("No tracked marker")
        )));
    }

    if (NativeQuestText)
    {
        if (CurrentSnapshot.ActiveQuestSummaries.IsEmpty())
        {
            NativeQuestText->SetText(FText::FromString(TEXT("No active quest.")));
        }
        else
        {
            const FHuwamHudQuestSnapshot& Quest = CurrentSnapshot.ActiveQuestSummaries[0];
            const bool bTracked = Quest.QuestId == CurrentSnapshot.TrackedQuestId;
            const TCHAR* QuestState = Quest.bReadyToTurnIn
                ? TEXT("Ready to turn in")
                : (bTracked ? TEXT("Tracked") : TEXT("Active"));
            NativeQuestText->SetText(FText::FromString(FString::Printf(
                TEXT("%s   %d / %d   %s"),
                *Quest.DisplayName.ToString(),
                Quest.CurrentValue,
                Quest.TargetValue,
                QuestState
            )));
        }
    }

    if (NativeInteractionText)
    {
        FText InteractionText = CurrentSnapshot.WorldInteraction.bHasTarget
            ? CurrentSnapshot.WorldInteraction.PromptText
            : FText::FromString(TEXT("Aim at a board, person, lesson object, or gatherable to interact."));

        if (CurrentSnapshot.Economy.bHasFocusedServicePressure && CurrentSnapshot.Economy.bFocusedServiceUnderfunded)
        {
            InteractionText = FText::FromString(FString::Printf(
                TEXT("%s   Need: %s"),
                *InteractionText.ToString(),
                *CurrentSnapshot.Economy.FocusedServicePressure.DisplayName.ToString()
            ));
        }

        NativeInteractionText->SetText(InteractionText);
    }
}

void UHuwamPrototypeHudWidget::BindToHudData()
{
    UHuwamHudDataComponent* HudData = PrototypePlayer ? PrototypePlayer->GetHudDataComponent() : nullptr;
    if (HudData)
    {
        HudData->OnHudSnapshotChanged.AddUniqueDynamic(this, &UHuwamPrototypeHudWidget::HandleHudSnapshotChanged);
    }
}

void UHuwamPrototypeHudWidget::UnbindFromHudData()
{
    UHuwamHudDataComponent* HudData = PrototypePlayer ? PrototypePlayer->GetHudDataComponent() : nullptr;
    if (HudData)
    {
        HudData->OnHudSnapshotChanged.RemoveDynamic(this, &UHuwamPrototypeHudWidget::HandleHudSnapshotChanged);
    }
}

UTextBlock* UHuwamPrototypeHudWidget::BuildNativeTextBlock(UWidgetTree* InWidgetTree, int32 FontSize, const FLinearColor& Color) const
{
    UTextBlock* TextBlock = InWidgetTree ? InWidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass()) : nullptr;
    if (!TextBlock)
    {
        return nullptr;
    }

    FSlateFontInfo Font = TextBlock->GetFont();
    Font.Size = FontSize;
    TextBlock->SetFont(Font);
    TextBlock->SetColorAndOpacity(FSlateColor(Color));
    TextBlock->SetAutoWrapText(true);
    TextBlock->SetWrapTextAt(330.0f);
    return TextBlock;
}

AHuwamPrototypePlayerActor* UHuwamPrototypeHudWidget::FindPrototypePlayerInWorld() const
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

void UHuwamPrototypeHudWidget::HandleHudSnapshotChanged(FHuwamHudSnapshot Snapshot)
{
    CurrentSnapshot = Snapshot;
    RefreshNativeHud();
}
