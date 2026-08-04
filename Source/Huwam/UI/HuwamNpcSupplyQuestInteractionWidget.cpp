#include "UI/HuwamNpcSupplyQuestInteractionWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "EngineUtils.h"
#include "Gameplay/HuwamInventoryComponent.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"

namespace
{
    const FLinearColor HuwamWidgetBackground(0.04f, 0.06f, 0.08f, 0.96f);
    const FLinearColor HuwamBodyText(0.92f, 0.94f, 0.96f, 1.0f);
    const FLinearColor HuwamMutedText(0.66f, 0.74f, 0.78f, 1.0f);
    const FLinearColor HuwamAccentButton(0.16f, 0.37f, 0.42f, 1.0f);
    const FLinearColor HuwamAcceptButton(0.19f, 0.40f, 0.25f, 1.0f);

    FText GetOutcomeText(EHuwamNpcSupplyQuestInteractionOutcome Outcome)
    {
        switch (Outcome)
        {
            case EHuwamNpcSupplyQuestInteractionOutcome::OfferPreviewed:
                return FText::FromString(TEXT("Request previewed."));
            case EHuwamNpcSupplyQuestInteractionOutcome::OfferAccepted:
                return FText::FromString(TEXT("Request accepted."));
            case EHuwamNpcSupplyQuestInteractionOutcome::WaitingForDelivery:
                return FText::FromString(TEXT("Delivery is still waiting on supplies."));
            case EHuwamNpcSupplyQuestInteractionOutcome::QuestTurnedIn:
                return FText::FromString(TEXT("Delivery complete."));
            case EHuwamNpcSupplyQuestInteractionOutcome::Satisfied:
                return FText::FromString(TEXT("Supply need is currently satisfied."));
            case EHuwamNpcSupplyQuestInteractionOutcome::Unavailable:
                return FText::FromString(TEXT("No request is available right now."));
            case EHuwamNpcSupplyQuestInteractionOutcome::None:
            default:
                return FText::GetEmpty();
        }
    }
}

TSharedRef<SWidget> UHuwamNpcSupplyQuestInteractionWidget::RebuildWidget()
{
    if (WidgetTree && !WidgetTree->RootWidget)
    {
        BuildNativeWidgetTree();
    }

    return Super::RebuildWidget();
}

void UHuwamNpcSupplyQuestInteractionWidget::NativeConstruct()
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

    if (!QuestRequester)
    {
        QuestRequester = FindQuestRequesterInWorld();
    }

    RefreshInteraction();
}

void UHuwamNpcSupplyQuestInteractionWidget::SetInteractionSource(AHuwamPrototypePlayerActor* InPrototypePlayer, AHuwamNpcSupplyQuestOfferActor* InQuestRequester)
{
    PrototypePlayer = InPrototypePlayer;
    QuestRequester = InQuestRequester;
    LastInteractionResult = FHuwamNpcSupplyQuestInteractionResult();
    LastSurplusSaleResult = FHuwamNpcSupplyPurchaseResult();
    RefreshInteraction();
}

bool UHuwamNpcSupplyQuestInteractionWidget::RefreshInteraction()
{
    CurrentSnapshot = QuestRequester
        ? QuestRequester->GetInteractionSnapshotForPrototypePlayer(PrototypePlayer)
        : FHuwamNpcSupplyQuestInteractionSnapshot();

    RefreshNativeWidget();
    OnInteractionSnapshotChanged.Broadcast(CurrentSnapshot);
    return QuestRequester && PrototypePlayer;
}

bool UHuwamNpcSupplyQuestInteractionWidget::RouteInteraction(bool bAcceptOfferWhenAvailable)
{
    LastInteractionResult = FHuwamNpcSupplyQuestInteractionResult();
    LastSurplusSaleResult = FHuwamNpcSupplyPurchaseResult();
    if (!QuestRequester
        || !PrototypePlayer
        || !QuestRequester->InteractByStateWithPrototypePlayer(PrototypePlayer, bAcceptOfferWhenAvailable, LastInteractionResult))
    {
        RefreshInteraction();
        return false;
    }

    CurrentSnapshot = LastInteractionResult.SnapshotAfter;
    RefreshNativeWidget();
    OnInteractionSnapshotChanged.Broadcast(CurrentSnapshot);
    OnInteractionRouted.Broadcast(LastInteractionResult);
    return true;
}

AHuwamPrototypePlayerActor* UHuwamNpcSupplyQuestInteractionWidget::GetPrototypePlayer() const
{
    return PrototypePlayer;
}

AHuwamNpcSupplyQuestOfferActor* UHuwamNpcSupplyQuestInteractionWidget::GetQuestRequester() const
{
    return QuestRequester;
}

FHuwamNpcSupplyQuestInteractionSnapshot UHuwamNpcSupplyQuestInteractionWidget::GetCurrentInteractionSnapshot() const
{
    return CurrentSnapshot;
}

FHuwamNpcSupplyQuestInteractionResult UHuwamNpcSupplyQuestInteractionWidget::GetLastInteractionResult() const
{
    return LastInteractionResult;
}

void UHuwamNpcSupplyQuestInteractionWidget::BuildNativeWidgetTree()
{
    if (!WidgetTree)
    {
        return;
    }

    UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("NpcSupplyQuestInteractionPanel"));
    Panel->SetBrushColor(HuwamWidgetBackground);
    Panel->SetPadding(FMargin(20.0f));
    WidgetTree->RootWidget = Panel;

    UVerticalBox* Content = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("NpcSupplyQuestInteractionContent"));
    Panel->SetContent(Content);

    NativePromptText = BuildNativeTextBlock(WidgetTree, FText::GetEmpty(), 14, HuwamMutedText);
    Content->AddChildToVerticalBox(NativePromptText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));

    NativeQuestTitleText = BuildNativeTextBlock(WidgetTree, FText::GetEmpty(), 22, HuwamBodyText);
    Content->AddChildToVerticalBox(NativeQuestTitleText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 8.0f));

    NativeNeedReasonText = BuildNativeTextBlock(WidgetTree, FText::GetEmpty(), 14, HuwamBodyText);
    Content->AddChildToVerticalBox(NativeNeedReasonText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 8.0f));

    NativeObjectiveText = BuildNativeTextBlock(WidgetTree, FText::GetEmpty(), 14, HuwamMutedText);
    Content->AddChildToVerticalBox(NativeObjectiveText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));

    NativeRewardText = BuildNativeTextBlock(WidgetTree, FText::GetEmpty(), 14, HuwamMutedText);
    Content->AddChildToVerticalBox(NativeRewardText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 10.0f));

    NativeResultText = BuildNativeTextBlock(WidgetTree, FText::GetEmpty(), 13, HuwamMutedText);
    Content->AddChildToVerticalBox(NativeResultText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 10.0f));

    UHorizontalBox* Actions = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("NpcSupplyQuestInteractionActions"));
    Content->AddChildToVerticalBox(Actions);

    NativePrimaryActionButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("NpcSupplyQuestPrimaryActionButton"));
    NativePrimaryActionButton->SetBackgroundColor(HuwamAccentButton);
    NativePrimaryActionButton->OnClicked.AddDynamic(this, &UHuwamNpcSupplyQuestInteractionWidget::HandlePrimaryActionClicked);
    NativePrimaryActionText = BuildNativeTextBlock(WidgetTree, FText::GetEmpty(), 15, HuwamBodyText);
    NativePrimaryActionButton->AddChild(NativePrimaryActionText);
    Actions->AddChildToHorizontalBox(NativePrimaryActionButton)->SetPadding(FMargin(0.0f, 0.0f, 8.0f, 0.0f));

    NativeAcceptOfferButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("NpcSupplyQuestAcceptOfferButton"));
    NativeAcceptOfferButton->SetBackgroundColor(HuwamAcceptButton);
    NativeAcceptOfferButton->OnClicked.AddDynamic(this, &UHuwamNpcSupplyQuestInteractionWidget::HandleAcceptOfferClicked);
    NativeAcceptOfferButton->AddChild(BuildNativeTextBlock(WidgetTree, FText::FromString(TEXT("Accept Request")), 15, HuwamBodyText));
    Actions->AddChildToHorizontalBox(NativeAcceptOfferButton)->SetPadding(FMargin(0.0f, 0.0f, 8.0f, 0.0f));

    NativeSellSurplusButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("NpcSupplyQuestSellSurplusButton"));
    NativeSellSurplusButton->SetBackgroundColor(HuwamAccentButton);
    NativeSellSurplusButton->OnClicked.AddDynamic(this, &UHuwamNpcSupplyQuestInteractionWidget::HandleSellSurplusClicked);
    NativeSellSurplusButton->AddChild(BuildNativeTextBlock(WidgetTree, FText::FromString(TEXT("Sell 1 Surplus")), 15, HuwamBodyText));
    Actions->AddChildToHorizontalBox(NativeSellSurplusButton)->SetPadding(FMargin(0.0f, 0.0f, 8.0f, 0.0f));

    NativeCloseButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("NpcSupplyQuestCloseButton"));
    NativeCloseButton->OnClicked.AddDynamic(this, &UHuwamNpcSupplyQuestInteractionWidget::HandleCloseClicked);
    NativeCloseButton->AddChild(BuildNativeTextBlock(WidgetTree, FText::FromString(TEXT("Close")), 15, HuwamBodyText));
    Actions->AddChildToHorizontalBox(NativeCloseButton);
}

void UHuwamNpcSupplyQuestInteractionWidget::RefreshNativeWidget()
{
    const FHuwamQuestOffer& Offer = CurrentSnapshot.QuestOffer;

    if (NativePromptText)
    {
        NativePromptText->SetText(CurrentSnapshot.PromptText);
    }

    if (NativeQuestTitleText)
    {
        NativeQuestTitleText->SetText(Offer.DisplayName.IsEmpty() ? CurrentSnapshot.SupplyNeed.DisplayName : Offer.DisplayName);
    }

    if (NativeNeedReasonText)
    {
        NativeNeedReasonText->SetText(BuildNeedReasonText());
    }

    if (NativeObjectiveText)
    {
        NativeObjectiveText->SetText(BuildObjectiveText());
    }

    if (NativeRewardText)
    {
        NativeRewardText->SetText(BuildRewardText());
    }

    if (NativeResultText)
    {
        NativeResultText->SetText(BuildResultText());
    }

    if (NativePrimaryActionText)
    {
        NativePrimaryActionText->SetText(BuildPrimaryActionText());
    }

    if (NativePrimaryActionButton)
    {
        NativePrimaryActionButton->SetIsEnabled(
            QuestRequester
            && PrototypePlayer
            && CurrentSnapshot.State != EHuwamNpcSupplyQuestInteractionState::Unavailable
        );
    }

    if (NativeAcceptOfferButton)
    {
        NativeAcceptOfferButton->SetIsEnabled(CurrentSnapshot.bCanAcceptOffer);
        NativeAcceptOfferButton->SetVisibility(CurrentSnapshot.bCanAcceptOffer ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }

    if (NativeSellSurplusButton)
    {
        NativeSellSurplusButton->SetIsEnabled(!FindFirstAcceptedSurplusItemId().IsEmpty());
    }
}

UTextBlock* UHuwamNpcSupplyQuestInteractionWidget::BuildNativeTextBlock(UWidgetTree* Tree, const FText& Text, int32 FontSize, const FLinearColor& Color) const
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

AHuwamPrototypePlayerActor* UHuwamNpcSupplyQuestInteractionWidget::FindPrototypePlayerInWorld() const
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

AHuwamNpcSupplyQuestOfferActor* UHuwamNpcSupplyQuestInteractionWidget::FindQuestRequesterInWorld() const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    for (TActorIterator<AHuwamNpcSupplyQuestOfferActor> It(GetWorld()); It; ++It)
    {
        return *It;
    }

    return nullptr;
}

FText UHuwamNpcSupplyQuestInteractionWidget::BuildNeedReasonText() const
{
    return CurrentSnapshot.SupplyNeed.Reason.IsEmpty()
        ? FText::FromString(TEXT("No current supply reason."))
        : CurrentSnapshot.SupplyNeed.Reason;
}

FText UHuwamNpcSupplyQuestInteractionWidget::BuildObjectiveText() const
{
    const FHuwamQuestObjectiveState& Objective = CurrentSnapshot.ActiveQuest.bActive
        ? CurrentSnapshot.ActiveQuest.PrimaryObjective
        : CurrentSnapshot.QuestOffer.PrimaryObjective;

    if (Objective.Description.IsEmpty())
    {
        return FText::FromString(TEXT("No active field supply objective."));
    }

    if (CurrentSnapshot.ActiveQuest.bActive)
    {
        return FText::Format(
            FText::FromString(TEXT("Objective: {0} ({1} / {2})")),
            Objective.Description,
            FText::AsNumber(Objective.CurrentValue),
            FText::AsNumber(Objective.TargetValue)
        );
    }

    return FText::Format(FText::FromString(TEXT("Objective: {0}")), Objective.Description);
}

FText UHuwamNpcSupplyQuestInteractionWidget::BuildRewardText() const
{
    const FHuwamQuestOffer& Offer = CurrentSnapshot.QuestOffer;
    if (Offer.QuestId.IsEmpty())
    {
        return FText::GetEmpty();
    }

    return FText::Format(
        FText::FromString(TEXT("Reward: {0} copper, {1} experience")),
        FText::AsNumber(Offer.RewardCopper),
        FText::AsNumber(Offer.RewardExperience)
    );
}

FText UHuwamNpcSupplyQuestInteractionWidget::BuildResultText() const
{
    if (LastSurplusSaleResult.bSuccess)
    {
        return FText::Format(
            FText::FromString(TEXT("Sold {0} for {1} copper. Local stock is now {2}.")),
            FText::FromString(LastSurplusSaleResult.PurchasedItem.Id),
            FText::AsNumber(LastSurplusSaleResult.TotalCopperPaid),
            FText::AsNumber(LastSurplusSaleResult.TotalStockQuantityAfter)
        );
    }

    const FText OutcomeText = GetOutcomeText(LastInteractionResult.Outcome);
    return OutcomeText.IsEmpty() ? CurrentSnapshot.SupplyNeed.DisplayName : OutcomeText;
}

FText UHuwamNpcSupplyQuestInteractionWidget::BuildPrimaryActionText() const
{
    switch (CurrentSnapshot.State)
    {
        case EHuwamNpcSupplyQuestInteractionState::OfferingWork:
            return FText::FromString(TEXT("Preview Request"));
        case EHuwamNpcSupplyQuestInteractionState::WaitingForDelivery:
            return FText::FromString(TEXT("Check Request"));
        case EHuwamNpcSupplyQuestInteractionState::ReadyForTurnIn:
            return FText::FromString(TEXT("Deliver Supplies"));
        case EHuwamNpcSupplyQuestInteractionState::Satisfied:
            return FText::FromString(TEXT("Check Supplies"));
        case EHuwamNpcSupplyQuestInteractionState::Unavailable:
        default:
            return FText::FromString(TEXT("Unavailable"));
    }
}

FString UHuwamNpcSupplyQuestInteractionWidget::FindFirstAcceptedSurplusItemId() const
{
    UHuwamNpcSupplyComponent* Supply = QuestRequester ? QuestRequester->GetNpcSupplyComponent() : nullptr;
    const UHuwamInventoryComponent* Inventory = PrototypePlayer ? PrototypePlayer->GetInventoryComponent() : nullptr;
    if (!Supply || !Inventory || Supply->GetRemainingStockCapacity() <= 0)
    {
        return FString();
    }

    for (const FHuwamInventoryStack& Stack : Inventory->GetInventoryStacks())
    {
        if (Stack.IsValidStack() && Supply->AcceptsSupplyItem(Stack.ItemId))
        {
            return Stack.ItemId;
        }
    }

    return FString();
}

void UHuwamNpcSupplyQuestInteractionWidget::HandlePrimaryActionClicked()
{
    RouteInteraction(false);
}

void UHuwamNpcSupplyQuestInteractionWidget::HandleAcceptOfferClicked()
{
    RouteInteraction(true);
}

void UHuwamNpcSupplyQuestInteractionWidget::HandleSellSurplusClicked()
{
    LastSurplusSaleResult = FHuwamNpcSupplyPurchaseResult();

    UHuwamNpcSupplyComponent* Supply = QuestRequester ? QuestRequester->GetNpcSupplyComponent() : nullptr;
    const FString ItemId = FindFirstAcceptedSurplusItemId();
    if (PrototypePlayer && Supply && !ItemId.IsEmpty())
    {
        PrototypePlayer->SellAcceptedSupplyItemToNpc(Supply, ItemId, 1, LastSurplusSaleResult);
    }

    RefreshInteraction();
}

void UHuwamNpcSupplyQuestInteractionWidget::HandleCloseClicked()
{
    if (PrototypePlayer)
    {
        PrototypePlayer->HideNpcSupplyQuestInteractionScreen();
        return;
    }

    APlayerController* OwningPlayer = GetOwningPlayer();
    RemoveFromParent();
    if (OwningPlayer)
    {
        OwningPlayer->SetInputMode(FInputModeGameOnly());
        OwningPlayer->bShowMouseCursor = false;
    }
}
