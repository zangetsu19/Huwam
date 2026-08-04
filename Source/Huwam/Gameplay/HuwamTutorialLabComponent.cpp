#include "Gameplay/HuwamTutorialLabComponent.h"

#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Gameplay/HuwamQuestComponent.h"
#include "Gameplay/HuwamRewardComponent.h"
#include "Gameplay/HuwamTutorialLabEntryActor.h"

namespace
{
    const TCHAR* TutorialQuestId = TEXT("quest.tutorial.progression_zero");
    const TCHAR* TutorialObjectiveId = TEXT("objective.complete_tutorial");
}

UHuwamTutorialLabComponent::UHuwamTutorialLabComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UHuwamTutorialLabComponent::BeginLaucianTutorialLab(
    UHuwamQuestComponent* Quests,
    UHuwamRewardComponent* Rewards,
    AHuwamTutorialLabEntryActor* LabEntry,
    AActor* ArrivalActor
)
{
    UHuwamQuestComponent* ResolvedQuests = ResolveQuestComponent(Quests);
    UHuwamRewardComponent* ResolvedRewards = ResolveRewardComponent(Rewards);
    if (!ResolvedQuests || !ResolvedRewards || !ResolvedQuests->StartProgressionZeroTutorialQuest(ResolvedRewards))
    {
        return false;
    }

    TutorialLabSnapshot = FHuwamTutorialLabSnapshot();
    TutorialLabSnapshot.bActive = true;

    AHuwamTutorialLabEntryActor* ResolvedLabEntry = LabEntry ? LabEntry : FindPrimaryLabEntry();
    if (ResolvedLabEntry)
    {
        AActor* ActorToMove = ArrivalActor ? ArrivalActor : GetOwner();
        if (ActorToMove)
        {
            ActorToMove->SetActorTransform(ResolvedLabEntry->GetLabEntryTransform());
            TutorialLabSnapshot.bMovedToLabEntry = true;
        }
    }

    SetBeat(EHuwamTutorialLabBeat::Movement);
    BroadcastSnapshot();
    return true;
}

bool UHuwamTutorialLabComponent::MarkMovementLessonComplete()
{
    if (!TutorialLabSnapshot.bActive || TutorialLabSnapshot.bMovementLessonComplete)
    {
        return false;
    }

    TutorialLabSnapshot.bMovementLessonComplete = true;
    SetBeat(EHuwamTutorialLabBeat::HudBasics);
    BroadcastSnapshot();
    return true;
}

bool UHuwamTutorialLabComponent::MarkHudLessonComplete()
{
    if (!TutorialLabSnapshot.bActive || !TutorialLabSnapshot.bMovementLessonComplete || TutorialLabSnapshot.bHudLessonComplete)
    {
        return false;
    }

    TutorialLabSnapshot.bHudLessonComplete = true;
    SetBeat(EHuwamTutorialLabBeat::InteractionReady);
    BroadcastSnapshot();
    return true;
}

bool UHuwamTutorialLabComponent::MarkInteractionLessonComplete(const FString& InteractionId)
{
    if (!TutorialLabSnapshot.bActive
        || !TutorialLabSnapshot.bMovementLessonComplete
        || !TutorialLabSnapshot.bHudLessonComplete
        || TutorialLabSnapshot.bInteractionLessonComplete
        || TutorialLabSnapshot.CurrentBeat != EHuwamTutorialLabBeat::InteractionReady)
    {
        return false;
    }

    TutorialLabSnapshot.bInteractionLessonComplete = true;
    TutorialLabSnapshot.LastInteractionId = InteractionId;
    SetBeat(EHuwamTutorialLabBeat::InventoryBasics);
    BroadcastSnapshot();
    return true;
}

bool UHuwamTutorialLabComponent::MarkInventoryLessonComplete(const FString& InspectedItemId)
{
    if (!TutorialLabSnapshot.bActive
        || !TutorialLabSnapshot.bInteractionLessonComplete
        || TutorialLabSnapshot.bInventoryLessonComplete
        || TutorialLabSnapshot.CurrentBeat != EHuwamTutorialLabBeat::InventoryBasics
        || InspectedItemId.IsEmpty())
    {
        return false;
    }

    if (!TutorialLabSnapshot.InventoryLessonItemId.IsEmpty() && InspectedItemId != TutorialLabSnapshot.InventoryLessonItemId)
    {
        return false;
    }

    TutorialLabSnapshot.bInventoryLessonComplete = true;
    TutorialLabSnapshot.LastInspectedInventoryItemId = InspectedItemId;
    SetBeat(EHuwamTutorialLabBeat::EquipmentBasics);
    BroadcastSnapshot();
    return true;
}

bool UHuwamTutorialLabComponent::MarkEquipmentLessonComplete(const FString& InspectedSlotId, const FString& InspectedItemId)
{
    if (!TutorialLabSnapshot.bActive
        || !TutorialLabSnapshot.bInventoryLessonComplete
        || TutorialLabSnapshot.bEquipmentLessonComplete
        || TutorialLabSnapshot.CurrentBeat != EHuwamTutorialLabBeat::EquipmentBasics
        || InspectedSlotId.IsEmpty()
        || InspectedItemId.IsEmpty())
    {
        return false;
    }

    if ((!TutorialLabSnapshot.EquipmentLessonSlotId.IsEmpty() && InspectedSlotId != TutorialLabSnapshot.EquipmentLessonSlotId)
        || (!TutorialLabSnapshot.EquipmentLessonItemId.IsEmpty() && InspectedItemId != TutorialLabSnapshot.EquipmentLessonItemId))
    {
        return false;
    }

    TutorialLabSnapshot.bEquipmentLessonComplete = true;
    TutorialLabSnapshot.LastInspectedEquipmentSlotId = InspectedSlotId;
    TutorialLabSnapshot.LastInspectedEquipmentItemId = InspectedItemId;
    SetBeat(EHuwamTutorialLabBeat::CombatBasics);
    BroadcastSnapshot();
    return true;
}

bool UHuwamTutorialLabComponent::MarkCombatLessonComplete(const FString& TargetId)
{
    if (!TutorialLabSnapshot.bActive
        || !TutorialLabSnapshot.bEquipmentLessonComplete
        || TutorialLabSnapshot.bCombatLessonComplete
        || TutorialLabSnapshot.CurrentBeat != EHuwamTutorialLabBeat::CombatBasics
        || TargetId.IsEmpty())
    {
        return false;
    }

    if (!TutorialLabSnapshot.CombatLessonTargetId.IsEmpty() && TargetId != TutorialLabSnapshot.CombatLessonTargetId)
    {
        return false;
    }

    TutorialLabSnapshot.bCombatLessonComplete = true;
    TutorialLabSnapshot.LastCombatTargetId = TargetId;
    SetBeat(EHuwamTutorialLabBeat::MapBasics);
    BroadcastSnapshot();
    return true;
}

bool UHuwamTutorialLabComponent::MarkMapLessonComplete(const FString& MapId)
{
    if (!TutorialLabSnapshot.bActive
        || !TutorialLabSnapshot.bCombatLessonComplete
        || TutorialLabSnapshot.bMapLessonComplete
        || TutorialLabSnapshot.CurrentBeat != EHuwamTutorialLabBeat::MapBasics
        || MapId.IsEmpty())
    {
        return false;
    }

    if (!TutorialLabSnapshot.MapLessonMapId.IsEmpty() && MapId != TutorialLabSnapshot.MapLessonMapId)
    {
        return false;
    }

    TutorialLabSnapshot.bMapLessonComplete = true;
    TutorialLabSnapshot.LastOpenedMapId = MapId;
    SetBeat(EHuwamTutorialLabBeat::QuestBasics);
    BroadcastSnapshot();
    return true;
}

bool UHuwamTutorialLabComponent::MarkQuestLessonComplete(const FString& QuestId, const FString& ObjectiveId)
{
    if (!TutorialLabSnapshot.bActive
        || !TutorialLabSnapshot.bMapLessonComplete
        || TutorialLabSnapshot.bQuestLessonComplete
        || TutorialLabSnapshot.CurrentBeat != EHuwamTutorialLabBeat::QuestBasics
        || QuestId.IsEmpty()
        || ObjectiveId.IsEmpty())
    {
        return false;
    }

    if ((!TutorialLabSnapshot.QuestLessonQuestId.IsEmpty() && QuestId != TutorialLabSnapshot.QuestLessonQuestId)
        || (!TutorialLabSnapshot.QuestLessonObjectiveId.IsEmpty() && ObjectiveId != TutorialLabSnapshot.QuestLessonObjectiveId))
    {
        return false;
    }

    TutorialLabSnapshot.bQuestLessonComplete = true;
    TutorialLabSnapshot.LastTrackedQuestId = QuestId;
    TutorialLabSnapshot.LastTrackedObjectiveId = ObjectiveId;
    SetBeat(EHuwamTutorialLabBeat::GatheringBasics);
    BroadcastSnapshot();
    return true;
}

bool UHuwamTutorialLabComponent::MarkGatheringLessonComplete(const FString& SourceId, const FString& GatheredItemId, int32 GatheredQuantity)
{
    if (!TutorialLabSnapshot.bActive
        || !TutorialLabSnapshot.bQuestLessonComplete
        || TutorialLabSnapshot.bGatheringLessonComplete
        || TutorialLabSnapshot.CurrentBeat != EHuwamTutorialLabBeat::GatheringBasics
        || SourceId.IsEmpty()
        || GatheredItemId.IsEmpty()
        || GatheredQuantity <= 0)
    {
        return false;
    }

    if ((!TutorialLabSnapshot.GatheringLessonSourceId.IsEmpty() && SourceId != TutorialLabSnapshot.GatheringLessonSourceId)
        || (!TutorialLabSnapshot.GatheringLessonItemId.IsEmpty() && GatheredItemId != TutorialLabSnapshot.GatheringLessonItemId)
        || GatheredQuantity < TutorialLabSnapshot.GatheringLessonQuantity)
    {
        return false;
    }

    TutorialLabSnapshot.bGatheringLessonComplete = true;
    TutorialLabSnapshot.LastGatheringSourceId = SourceId;
    TutorialLabSnapshot.LastGatheredItemId = GatheredItemId;
    TutorialLabSnapshot.LastGatheredItemQuantity = GatheredQuantity;
    return CompleteLaucianTutorialLab();
}

bool UHuwamTutorialLabComponent::CompleteLaucianTutorialLab(UHuwamQuestComponent* Quests, UHuwamRewardComponent* Rewards)
{
    UHuwamQuestComponent* ResolvedQuests = ResolveQuestComponent(Quests);
    UHuwamRewardComponent* ResolvedRewards = ResolveRewardComponent(Rewards);
    if (!TutorialLabSnapshot.bActive || !ResolvedQuests || !ResolvedRewards)
    {
        return false;
    }

    int32 ProgressAfter = 0;
    bool bCompleted = false;
    if (!ResolvedRewards->AddQuestProgress(TutorialQuestId, TutorialObjectiveId, 1, 1, ProgressAfter, bCompleted))
    {
        return false;
    }

    ResolvedQuests->RefreshQuestProgressFromRewards(TutorialQuestId, ResolvedRewards);
    TutorialLabSnapshot.bActive = false;
    SetBeat(bCompleted ? EHuwamTutorialLabBeat::Complete : TutorialLabSnapshot.CurrentBeat);
    BroadcastSnapshot();
    return bCompleted;
}

FHuwamTutorialLabSnapshot UHuwamTutorialLabComponent::GetTutorialLabSnapshot() const
{
    return TutorialLabSnapshot;
}

bool UHuwamTutorialLabComponent::IsTutorialLabActive() const
{
    return TutorialLabSnapshot.bActive;
}

void UHuwamTutorialLabComponent::RestoreTutorialLabForPrototypeSave(const FHuwamTutorialLabSnapshot& SavedSnapshot)
{
    TutorialLabSnapshot = SavedSnapshot;
    BroadcastSnapshot();
}

AHuwamTutorialLabEntryActor* UHuwamTutorialLabComponent::FindPrimaryLabEntry() const
{
    if (!GetWorld())
    {
        return nullptr;
    }

    AHuwamTutorialLabEntryActor* FirstEntry = nullptr;
    for (TActorIterator<AHuwamTutorialLabEntryActor> It(GetWorld()); It; ++It)
    {
        if (!FirstEntry)
        {
            FirstEntry = *It;
        }

        if (It->IsPrimaryLabEntry())
        {
            return *It;
        }
    }

    return FirstEntry;
}

UHuwamQuestComponent* UHuwamTutorialLabComponent::ResolveQuestComponent(UHuwamQuestComponent* Quests) const
{
    if (Quests)
    {
        return Quests;
    }

    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamQuestComponent>();
    }

    return nullptr;
}

UHuwamRewardComponent* UHuwamTutorialLabComponent::ResolveRewardComponent(UHuwamRewardComponent* Rewards) const
{
    if (Rewards)
    {
        return Rewards;
    }

    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamRewardComponent>();
    }

    return nullptr;
}

void UHuwamTutorialLabComponent::SetBeat(EHuwamTutorialLabBeat NewBeat)
{
    TutorialLabSnapshot.CurrentBeat = NewBeat;

    switch (NewBeat)
    {
    case EHuwamTutorialLabBeat::Movement:
        TutorialLabSnapshot.BeatTitle = NSLOCTEXT("HuwamTutorialLab", "MovementBeatTitle", "Find Your Feet");
        TutorialLabSnapshot.InstructionText = NSLOCTEXT("HuwamTutorialLab", "MovementInstruction", "Move and look around Laucian's white lab.");
        TutorialLabSnapshot.LaucianLine = NSLOCTEXT("HuwamTutorialLab", "MovementLaucianLine", "Welcome to the white lab, young one. Walk first. Destiny can wait a breath.");
        break;
    case EHuwamTutorialLabBeat::HudBasics:
        TutorialLabSnapshot.BeatTitle = NSLOCTEXT("HuwamTutorialLab", "HudBeatTitle", "Read The HUD");
        TutorialLabSnapshot.InstructionText = NSLOCTEXT("HuwamTutorialLab", "HudInstruction", "Inspect the health, mana, quest, and inventory signals prepared for you, then press Tab.");
        TutorialLabSnapshot.LaucianLine = NSLOCTEXT("HuwamTutorialLab", "HudLaucianLine", "Those little truths around you matter. Ignore them later and Huwan will be educational.");
        break;
    case EHuwamTutorialLabBeat::InteractionReady:
        TutorialLabSnapshot.BeatTitle = NSLOCTEXT("HuwamTutorialLab", "InteractionReadyBeatTitle", "First Object Ready");
        TutorialLabSnapshot.InstructionText = NSLOCTEXT("HuwamTutorialLab", "InteractionReadyInstruction", "Use the interact button on Laucian's training pedestal.");
        TutorialLabSnapshot.LaucianLine = NSLOCTEXT("HuwamTutorialLab", "InteractionReadyLaucianLine", "Good. Now touch the one thing I prepared for touching.");
        break;
    case EHuwamTutorialLabBeat::InventoryBasics:
        TutorialLabSnapshot.BeatTitle = NSLOCTEXT("HuwamTutorialLab", "InventoryBeatTitle", "Open Your Inventory");
        TutorialLabSnapshot.InstructionText = NSLOCTEXT("HuwamTutorialLab", "InventoryInstruction", "Open your inventory and inspect the gathering knife Laucian left loose in your pack.");
        TutorialLabSnapshot.LaucianLine = NSLOCTEXT("HuwamTutorialLab", "InventoryLaucianLine", "There. Interaction. Now find what you are carrying before Huwan finds a way to make you drop it.");
        break;
    case EHuwamTutorialLabBeat::EquipmentBasics:
        TutorialLabSnapshot.BeatTitle = NSLOCTEXT("HuwamTutorialLab", "EquipmentBeatTitle", "What Is Equipped");
        TutorialLabSnapshot.InstructionText = NSLOCTEXT("HuwamTutorialLab", "EquipmentInstruction", "Inspect the Basic Sword already equipped in your main hand slot.");
        TutorialLabSnapshot.LaucianLine = NSLOCTEXT("HuwamTutorialLab", "EquipmentLaucianLine", "A pack is not a hand, and a blade in a slot is not a blade in a fight. Let us sort that out next.");
        break;
    case EHuwamTutorialLabBeat::CombatBasics:
        TutorialLabSnapshot.BeatTitle = NSLOCTEXT("HuwamTutorialLab", "CombatBeatTitle", "Ready For Combat");
        TutorialLabSnapshot.InstructionText = NSLOCTEXT("HuwamTutorialLab", "CombatInstruction", "Strike Laucian's training focus with your equipped weapon.");
        TutorialLabSnapshot.LaucianLine = NSLOCTEXT("HuwamTutorialLab", "CombatLaucianLine", "You know what is in your hands now. Next we learn what happens when something bites back.");
        break;
    case EHuwamTutorialLabBeat::MapBasics:
        TutorialLabSnapshot.BeatTitle = NSLOCTEXT("HuwamTutorialLab", "MapBeatTitle", "Read The World");
        TutorialLabSnapshot.InstructionText = NSLOCTEXT("HuwamTutorialLab", "MapInstruction", "Open the map and read the North, South, East, and West markers.");
        TutorialLabSnapshot.LaucianLine = NSLOCTEXT("HuwamTutorialLab", "MapLaucianLine", "A sword solves what is close. A map warns you what is coming.");
        break;
    case EHuwamTutorialLabBeat::QuestBasics:
        TutorialLabSnapshot.BeatTitle = NSLOCTEXT("HuwamTutorialLab", "QuestBeatTitle", "Follow A Quest");
        TutorialLabSnapshot.InstructionText = NSLOCTEXT("HuwamTutorialLab", "QuestInstruction", "Track Welcome to Progression Zero and read its objective marker.");
        TutorialLabSnapshot.LaucianLine = NSLOCTEXT("HuwamTutorialLab", "QuestLaucianLine", "Direction is useful. Purpose is where quests begin to get dramatic.");
        break;
    case EHuwamTutorialLabBeat::GatheringBasics:
        TutorialLabSnapshot.BeatTitle = NSLOCTEXT("HuwamTutorialLab", "GatheringBeatTitle", "Take What Helps");
        TutorialLabSnapshot.InstructionText = NSLOCTEXT("HuwamTutorialLab", "GatheringInstruction", "Gather the lavender Laucian prepared with the gathering knife in your pack.");
        TutorialLabSnapshot.LaucianLine = NSLOCTEXT("HuwamTutorialLab", "GatheringLaucianLine", "Now that the quest has your attention, let us see whether your hands can finish one.");
        break;
    case EHuwamTutorialLabBeat::Complete:
        TutorialLabSnapshot.BeatTitle = NSLOCTEXT("HuwamTutorialLab", "CompleteBeatTitle", "Tutorial Objective Complete");
        TutorialLabSnapshot.InstructionText = NSLOCTEXT("HuwamTutorialLab", "CompleteInstruction", "Laucian's first tutorial objective is complete.");
        TutorialLabSnapshot.LaucianLine = NSLOCTEXT("HuwamTutorialLab", "CompleteLaucianLine", "You have survived the first lesson. The rest of the world is less polite.");
        break;
    default:
        TutorialLabSnapshot.BeatTitle = FText::GetEmpty();
        TutorialLabSnapshot.InstructionText = FText::GetEmpty();
        TutorialLabSnapshot.LaucianLine = FText::GetEmpty();
        break;
    }
}

void UHuwamTutorialLabComponent::BroadcastSnapshot()
{
    OnTutorialLabChanged.Broadcast(TutorialLabSnapshot);
}
