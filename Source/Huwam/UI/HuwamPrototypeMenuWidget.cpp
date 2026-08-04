#include "UI/HuwamPrototypeMenuWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "EngineUtils.h"
#include "Gameplay/HuwamPrototypePlayerActor.h"

namespace
{
    const FLinearColor HuwamMenuBackground(0.035f, 0.045f, 0.060f, 0.97f);
    const FLinearColor HuwamMenuText(0.93f, 0.95f, 0.97f, 1.0f);
    const FLinearColor HuwamMenuMutedText(0.68f, 0.75f, 0.81f, 1.0f);
    const FLinearColor HuwamMenuTabButton(0.12f, 0.18f, 0.23f, 1.0f);
    const FLinearColor HuwamMenuActionButton(0.18f, 0.38f, 0.31f, 1.0f);

    FString FreshnessToString(EHuwamInventoryFreshness Freshness)
    {
        switch (Freshness)
        {
        case EHuwamInventoryFreshness::Fresh:
            return TEXT("Fresh");
        case EHuwamInventoryFreshness::Stale:
            return TEXT("Stale");
        case EHuwamInventoryFreshness::Spoiled:
            return TEXT("Spoiled");
        case EHuwamInventoryFreshness::Stable:
        default:
            return TEXT("Stable");
        }
    }

    FString StorageToString(EHuwamInventoryStorageQuality StorageQuality)
    {
        switch (StorageQuality)
        {
        case EHuwamInventoryStorageQuality::Pouch:
            return TEXT("Pouch");
        case EHuwamInventoryStorageQuality::Chest:
            return TEXT("Chest");
        case EHuwamInventoryStorageQuality::Cooled:
            return TEXT("Cooled");
        case EHuwamInventoryStorageQuality::Magical:
            return TEXT("Magical");
        case EHuwamInventoryStorageQuality::Loose:
        default:
            return TEXT("Loose");
        }
    }

    FString JoinInventoryLines(const TArray<FHuwamInventoryStack>& Stacks)
    {
        TArray<FString> Lines;
        for (const FHuwamInventoryStack& Stack : Stacks)
        {
            if (Stack.IsValidStack())
            {
                Lines.Add(FString::Printf(
                    TEXT("%s x%d [%s, %s]"),
                    *Stack.ItemId,
                    Stack.Quantity,
                    *StorageToString(Stack.StorageQuality),
                    *FreshnessToString(Stack.Freshness)
                ));
            }
        }

        return Lines.IsEmpty() ? TEXT("Empty") : FString::Join(Lines, TEXT("\n"));
    }

    FString JoinStorageRouteLines(const TArray<FHuwamInventoryStorageRoute>& Routes)
    {
        TArray<FString> Lines;
        for (const FHuwamInventoryStorageRoute& Route : Routes)
        {
            Lines.Add(FString::Printf(
                TEXT("%s: %d / %d stacks%s"),
                *Route.DisplayName.ToString(),
                Route.UsedStacks,
                Route.MaxStacks,
                Route.bAvailable ? TEXT("") : TEXT(" (locked)")
            ));
        }

        return Lines.IsEmpty() ? TEXT("No storage routes") : FString::Join(Lines, TEXT("\n"));
    }

    FString JoinEquipmentLines(const TArray<FHuwamEquipmentSlotState>& Slots)
    {
        TArray<FString> Lines;
        for (const FHuwamEquipmentSlotState& Slot : Slots)
        {
            if (Slot.bUnlocked && Slot.IsOccupied())
            {
                Lines.Add(FString::Printf(TEXT("%s: %s"), *Slot.DisplayName.ToString(), *Slot.EquippedItemId));
            }
        }

        return Lines.IsEmpty() ? TEXT("Nothing equipped") : FString::Join(Lines, TEXT("\n"));
    }
}

TSharedRef<SWidget> UHuwamPrototypeMenuWidget::RebuildWidget()
{
    if (WidgetTree && !WidgetTree->RootWidget)
    {
        BuildNativeWidgetTree();
    }

    return Super::RebuildWidget();
}

void UHuwamPrototypeMenuWidget::NativeConstruct()
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

    RefreshMenu();
}

void UHuwamPrototypeMenuWidget::SetPrototypePlayer(AHuwamPrototypePlayerActor* InPrototypePlayer)
{
    PrototypePlayer = InPrototypePlayer;
    RefreshMenu();
}

bool UHuwamPrototypeMenuWidget::OpenPage(EHuwamPrototypeMenuPage InPage)
{
    CurrentPage = InPage;
    return RefreshMenu();
}

bool UHuwamPrototypeMenuWidget::RefreshMenu()
{
    CurrentSnapshot = PrototypePlayer
        ? PrototypePlayer->RefreshPrototypeHud()
        : FHuwamHudSnapshot();
    RefreshNativeMenu();
    return PrototypePlayer != nullptr;
}

EHuwamPrototypeMenuPage UHuwamPrototypeMenuWidget::GetCurrentPage() const
{
    return CurrentPage;
}

FHuwamHudSnapshot UHuwamPrototypeMenuWidget::GetCurrentHudSnapshot() const
{
    return CurrentSnapshot;
}

void UHuwamPrototypeMenuWidget::BuildNativeWidgetTree()
{
    if (!WidgetTree)
    {
        return;
    }

    UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("PrototypeMenuPanel"));
    Panel->SetBrushColor(HuwamMenuBackground);
    Panel->SetPadding(FMargin(24.0f, 18.0f));
    WidgetTree->RootWidget = Panel;

    UVerticalBox* Content = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("PrototypeMenuContent"));
    Panel->SetContent(Content);

    UHorizontalBox* PageActions = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("PrototypeMenuPages"));
    Content->AddChildToVerticalBox(PageActions)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 10.0f));

    UButton* InventoryPageButton = BuildNativeButton(WidgetTree, FText::FromString(TEXT("Pack")), HuwamMenuTabButton);
    InventoryPageButton->OnClicked.AddDynamic(this, &UHuwamPrototypeMenuWidget::HandleInventoryPageClicked);
    PageActions->AddChildToHorizontalBox(InventoryPageButton)->SetPadding(FMargin(0.0f, 0.0f, 8.0f, 0.0f));

    UButton* MapPageButton = BuildNativeButton(WidgetTree, FText::FromString(TEXT("Map")), HuwamMenuTabButton);
    MapPageButton->OnClicked.AddDynamic(this, &UHuwamPrototypeMenuWidget::HandleMapPageClicked);
    PageActions->AddChildToHorizontalBox(MapPageButton)->SetPadding(FMargin(0.0f, 0.0f, 8.0f, 0.0f));

    UButton* QuestsPageButton = BuildNativeButton(WidgetTree, FText::FromString(TEXT("Quests")), HuwamMenuTabButton);
    QuestsPageButton->OnClicked.AddDynamic(this, &UHuwamPrototypeMenuWidget::HandleQuestsPageClicked);
    PageActions->AddChildToHorizontalBox(QuestsPageButton);

    NativeTitleText = BuildNativeTextBlock(WidgetTree, FText::GetEmpty(), 24, HuwamMenuText);
    Content->AddChildToVerticalBox(NativeTitleText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 8.0f));

    NativeBodyText = BuildNativeTextBlock(WidgetTree, FText::GetEmpty(), 15, HuwamMenuMutedText);
    Content->AddChildToVerticalBox(NativeBodyText)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 14.0f));

    UHorizontalBox* LessonActions = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("PrototypeMenuLessonActions"));
    Content->AddChildToVerticalBox(LessonActions)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 12.0f));

    NativeInspectInventoryButton = BuildNativeButton(WidgetTree, FText::FromString(TEXT("Inspect Gathering Knife")), HuwamMenuActionButton);
    NativeInspectInventoryButton->OnClicked.AddDynamic(this, &UHuwamPrototypeMenuWidget::HandleInspectInventoryClicked);
    LessonActions->AddChildToHorizontalBox(NativeInspectInventoryButton)->SetPadding(FMargin(0.0f, 0.0f, 8.0f, 0.0f));

    NativeInspectEquipmentButton = BuildNativeButton(WidgetTree, FText::FromString(TEXT("Inspect Main Hand")), HuwamMenuActionButton);
    NativeInspectEquipmentButton->OnClicked.AddDynamic(this, &UHuwamPrototypeMenuWidget::HandleInspectEquipmentClicked);
    LessonActions->AddChildToHorizontalBox(NativeInspectEquipmentButton)->SetPadding(FMargin(0.0f, 0.0f, 8.0f, 0.0f));

    NativeMapLessonButton = BuildNativeButton(WidgetTree, FText::FromString(TEXT("Read Full Map")), HuwamMenuActionButton);
    NativeMapLessonButton->OnClicked.AddDynamic(this, &UHuwamPrototypeMenuWidget::HandleMapLessonClicked);
    LessonActions->AddChildToHorizontalBox(NativeMapLessonButton)->SetPadding(FMargin(0.0f, 0.0f, 8.0f, 0.0f));

    NativeQuestLessonButton = BuildNativeButton(WidgetTree, FText::FromString(TEXT("Track Tutorial Quest")), HuwamMenuActionButton);
    NativeQuestLessonButton->OnClicked.AddDynamic(this, &UHuwamPrototypeMenuWidget::HandleQuestLessonClicked);
    LessonActions->AddChildToHorizontalBox(NativeQuestLessonButton);

    UButton* CloseButton = BuildNativeButton(WidgetTree, FText::FromString(TEXT("Close")), HuwamMenuTabButton);
    CloseButton->OnClicked.AddDynamic(this, &UHuwamPrototypeMenuWidget::HandleCloseClicked);
    Content->AddChildToVerticalBox(CloseButton);
}

void UHuwamPrototypeMenuWidget::RefreshNativeMenu()
{
    if (NativeTitleText)
    {
        NativeTitleText->SetText(BuildPageTitle());
    }

    if (NativeBodyText)
    {
        NativeBodyText->SetText(BuildPageBody());
    }

    const EHuwamTutorialLabBeat TutorialBeat = CurrentSnapshot.TutorialLab.CurrentBeat;
    if (NativeInspectInventoryButton)
    {
        NativeInspectInventoryButton->SetVisibility(CurrentPage == EHuwamPrototypeMenuPage::InventoryEquipment ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        NativeInspectInventoryButton->SetIsEnabled(TutorialBeat == EHuwamTutorialLabBeat::InventoryBasics);
    }

    if (NativeInspectEquipmentButton)
    {
        NativeInspectEquipmentButton->SetVisibility(CurrentPage == EHuwamPrototypeMenuPage::InventoryEquipment ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        NativeInspectEquipmentButton->SetIsEnabled(TutorialBeat == EHuwamTutorialLabBeat::EquipmentBasics);
    }

    if (NativeMapLessonButton)
    {
        NativeMapLessonButton->SetVisibility(CurrentPage == EHuwamPrototypeMenuPage::Map ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        NativeMapLessonButton->SetIsEnabled(TutorialBeat == EHuwamTutorialLabBeat::MapBasics);
    }

    if (NativeQuestLessonButton)
    {
        NativeQuestLessonButton->SetVisibility(CurrentPage == EHuwamPrototypeMenuPage::Quests ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        NativeQuestLessonButton->SetIsEnabled(TutorialBeat == EHuwamTutorialLabBeat::QuestBasics);
    }
}

UTextBlock* UHuwamPrototypeMenuWidget::BuildNativeTextBlock(UWidgetTree* InWidgetTree, const FText& Text, int32 FontSize, const FLinearColor& Color) const
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
    TextBlock->SetText(Text);
    return TextBlock;
}

UButton* UHuwamPrototypeMenuWidget::BuildNativeButton(UWidgetTree* InWidgetTree, const FText& Text, const FLinearColor& Color) const
{
    UButton* Button = InWidgetTree ? InWidgetTree->ConstructWidget<UButton>(UButton::StaticClass()) : nullptr;
    if (!Button)
    {
        return nullptr;
    }

    Button->SetBackgroundColor(Color);
    Button->AddChild(BuildNativeTextBlock(InWidgetTree, Text, 15, HuwamMenuText));
    return Button;
}

FText UHuwamPrototypeMenuWidget::BuildPageTitle() const
{
    switch (CurrentPage)
    {
    case EHuwamPrototypeMenuPage::Map:
        return FText::FromString(TEXT("Map"));
    case EHuwamPrototypeMenuPage::Quests:
        return FText::FromString(TEXT("Quests"));
    case EHuwamPrototypeMenuPage::InventoryEquipment:
    default:
        return FText::FromString(TEXT("Inventory And Equipment"));
    }
}

FText UHuwamPrototypeMenuWidget::BuildPageBody() const
{
    switch (CurrentPage)
    {
    case EHuwamPrototypeMenuPage::Map:
        return BuildMapBody();
    case EHuwamPrototypeMenuPage::Quests:
        return BuildQuestBody();
    case EHuwamPrototypeMenuPage::InventoryEquipment:
    default:
        return BuildInventoryEquipmentBody();
    }
}

FText UHuwamPrototypeMenuWidget::BuildInventoryEquipmentBody() const
{
    return FText::FromString(FString::Printf(
        TEXT("Inventory slots: %d / %d\n\nStorage\n%s\n\nItems\n%s\n\nEquipped\n%s"),
        CurrentSnapshot.Inventory.UsedSlots,
        CurrentSnapshot.Inventory.MaxSlots,
        *JoinStorageRouteLines(CurrentSnapshot.Inventory.StorageRoutes),
        *JoinInventoryLines(CurrentSnapshot.Inventory.Stacks),
        *JoinEquipmentLines(CurrentSnapshot.Equipment.Slots)
    ));
}

FText UHuwamPrototypeMenuWidget::BuildMapBody() const
{
    const FHuwamMapAwarenessSnapshot& Map = CurrentSnapshot.MapAwareness;
    return FText::FromString(FString::Printf(
        TEXT("%s\nRegion: %s\nBattle fog: %s\nCardinal markers: %s\nTracked marker: %s"),
        *Map.DisplayName.ToString(),
        *Map.RegionId,
        Map.bBattleFogEnabled ? TEXT("Enabled") : TEXT("Disabled"),
        Map.bCardinalMarkersVisible ? TEXT("Visible") : TEXT("Hidden"),
        Map.bTrackedQuestMarkerVisible ? TEXT("Visible") : TEXT("Not visible")
    ));
}

FText UHuwamPrototypeMenuWidget::BuildQuestBody() const
{
    TArray<FString> QuestLines;
    for (const FHuwamHudQuestSnapshot& Quest : CurrentSnapshot.ActiveQuestSummaries)
    {
        QuestLines.Add(FString::Printf(
            TEXT("%s\n%s: %d / %d"),
            *Quest.DisplayName.ToString(),
            *Quest.ObjectiveText.ToString(),
            Quest.CurrentValue,
            Quest.TargetValue
        ));
    }

    return FText::FromString(QuestLines.IsEmpty()
        ? TEXT("No active quests.")
        : FString::Join(QuestLines, TEXT("\n\n")));
}

AHuwamPrototypePlayerActor* UHuwamPrototypeMenuWidget::FindPrototypePlayerInWorld() const
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

void UHuwamPrototypeMenuWidget::HandleInventoryPageClicked()
{
    OpenPage(EHuwamPrototypeMenuPage::InventoryEquipment);
}

void UHuwamPrototypeMenuWidget::HandleMapPageClicked()
{
    OpenPage(EHuwamPrototypeMenuPage::Map);
}

void UHuwamPrototypeMenuWidget::HandleQuestsPageClicked()
{
    OpenPage(EHuwamPrototypeMenuPage::Quests);
}

void UHuwamPrototypeMenuWidget::HandleInspectInventoryClicked()
{
    if (PrototypePlayer)
    {
        PrototypePlayer->MarkTutorialInventoryItemInspected(TEXT("item.tool.gathering_knife"));
    }

    RefreshMenu();
}

void UHuwamPrototypeMenuWidget::HandleInspectEquipmentClicked()
{
    if (PrototypePlayer)
    {
        PrototypePlayer->MarkTutorialEquipmentSlotInspected(TEXT("slot.weapon.main_hand"));
    }

    RefreshMenu();
}

void UHuwamPrototypeMenuWidget::HandleMapLessonClicked()
{
    if (PrototypePlayer)
    {
        PrototypePlayer->OpenTutorialMapLesson();
    }

    RefreshMenu();
}

void UHuwamPrototypeMenuWidget::HandleQuestLessonClicked()
{
    if (PrototypePlayer)
    {
        PrototypePlayer->TrackTutorialQuestLesson();
    }

    RefreshMenu();
}

void UHuwamPrototypeMenuWidget::HandleCloseClicked()
{
    if (PrototypePlayer)
    {
        PrototypePlayer->HidePrototypeMenuScreen();
    }
}
