#include "Gameplay/HuwamNpcSupplyComponent.h"

#include "Data/HuwamDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Gameplay/HuwamInventoryComponent.h"
#include "Gameplay/HuwamRewardComponent.h"
#include "Gameplay/HuwamTimeSubsystem.h"
#include "TimerManager.h"

namespace
{
    FName GetRowNameFromId(const FString& Id)
    {
        return FName(*Id.Replace(TEXT("."), TEXT("_")));
    }
}

UHuwamNpcSupplyComponent::UHuwamNpcSupplyComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    AcceptedSupplyItemIds =
    {
        TEXT("material.wood.basic"),
        TEXT("material.stone.basic"),
        TEXT("material.flax"),
        TEXT("material.fiber.grass"),
        TEXT("material.herb.lavender"),
        TEXT("material.herb.mint"),
        TEXT("material.herb.rosemary")
    };
}

void UHuwamNpcSupplyComponent::BeginPlay()
{
    Super::BeginPlay();

    if (bAutoStartRoutineSupplyUse)
    {
        StartRoutineSupplyUse();
    }
}

void UHuwamNpcSupplyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    StopRoutineSupplyUse();
    Super::EndPlay(EndPlayReason);
}

bool UHuwamNpcSupplyComponent::DeliverAcceptedItemsFromInventory(UHuwamInventoryComponent* SourceInventory, int32 Quantity, FHuwamNpcSupplyDeliveryResult& OutDeliveryResult)
{
    OutDeliveryResult = FHuwamNpcSupplyDeliveryResult();
    OutDeliveryResult.NpcId = NpcId;

    if (!SourceInventory || Quantity <= 0 || GetAcceptedInventoryQuantity(SourceInventory) < Quantity || !CanAddStockQuantity(Quantity))
    {
        return false;
    }

    TArray<FHuwamIdQuantity> DeliveryPlan;
    int32 RemainingToDeliver = Quantity;

    const TArray<FHuwamInventoryStack> SourceStacks = SourceInventory->GetInventoryStacks();
    for (const FHuwamInventoryStack& Stack : SourceStacks)
    {
        if (RemainingToDeliver <= 0)
        {
            break;
        }

        if (!Stack.IsValidStack() || !AcceptsSupplyItem(Stack.ItemId))
        {
            continue;
        }

        FHuwamIdQuantity PlannedItem;
        PlannedItem.Id = Stack.ItemId;
        PlannedItem.Quantity = FMath::Min(Stack.Quantity, RemainingToDeliver);
        DeliveryPlan.Add(PlannedItem);
        RemainingToDeliver -= PlannedItem.Quantity;
    }

    if (RemainingToDeliver > 0)
    {
        return false;
    }

    for (const FHuwamIdQuantity& PlannedItem : DeliveryPlan)
    {
        if (PlannedItem.Id.IsEmpty() || PlannedItem.Quantity <= 0 || !SourceInventory->RemoveItemById(PlannedItem.Id, PlannedItem.Quantity))
        {
            return false;
        }

        AddStockItemByIdInternal(PlannedItem.Id, PlannedItem.Quantity);
    }

    OutDeliveryResult.bSuccess = true;
    OutDeliveryResult.DeliveredItems = DeliveryPlan;
    OutDeliveryResult.TotalQuantityDelivered = Quantity;
    OutDeliveryResult.StockQuantityAfter = GetTotalStockQuantity();
    OutDeliveryResult.bNeedSatisfied = IsSupplyNeedSatisfied();
    OnNpcSupplyChanged.Broadcast();
    return true;
}

bool UHuwamNpcSupplyComponent::PurchaseAcceptedItemFromInventory(UHuwamInventoryComponent* SellerInventory, UHuwamRewardComponent* SellerRewards, const FString& ItemId, int32 Quantity, FHuwamNpcSupplyPurchaseResult& OutPurchaseResult)
{
    OutPurchaseResult = FHuwamNpcSupplyPurchaseResult();
    OutPurchaseResult.NpcId = NpcId;

    const int64 UnitValueCopper = GetPurchaseValueCopper(ItemId);
    if (!SellerInventory
        || !SellerRewards
        || Quantity <= 0
        || UnitValueCopper <= 0
        || !AcceptsSupplyItem(ItemId)
        || !SellerInventory->HasItemQuantity(ItemId, Quantity)
        || !CanAddStockQuantity(Quantity))
    {
        return false;
    }

    if (!SellerInventory->RemoveItemById(ItemId, Quantity) || !AddStockItemByIdInternal(ItemId, Quantity))
    {
        return false;
    }

    OutPurchaseResult.bSuccess = true;
    OutPurchaseResult.PurchasedItem.Id = ItemId;
    OutPurchaseResult.PurchasedItem.Quantity = Quantity;
    OutPurchaseResult.UnitValueCopper = UnitValueCopper;
    OutPurchaseResult.TotalCopperPaid = SellerRewards->AddCopper(UnitValueCopper * Quantity);
    OutPurchaseResult.TotalStockQuantityAfter = GetTotalStockQuantity();
    OutPurchaseResult.bNeedSatisfied = IsSupplyNeedSatisfied();
    OnNpcSupplyChanged.Broadcast();
    return true;
}

bool UHuwamNpcSupplyComponent::UseStockItemById(const FString& ItemId, int32 Quantity, FHuwamNpcSupplyUseResult& OutUseResult)
{
    OutUseResult = FHuwamNpcSupplyUseResult();
    OutUseResult.NpcId = NpcId;

    if (!RemoveStockItemByIdInternal(ItemId, Quantity))
    {
        return false;
    }

    OutUseResult.bSuccess = true;
    OutUseResult.UsedItem.Id = ItemId;
    OutUseResult.UsedItem.Quantity = Quantity;
    OutUseResult.ItemStockQuantityAfter = GetStockQuantity(ItemId);
    OutUseResult.TotalStockQuantityAfter = GetTotalStockQuantity();
    OutUseResult.bShouldRequestSupplyHelp = ShouldRequestSupplyHelp();
    OnNpcSupplyChanged.Broadcast();
    return true;
}

bool UHuwamNpcSupplyComponent::StartRoutineSupplyUse()
{
    const float RoutineUseDelaySeconds = GetRoutineUseDelaySeconds();
    if (RoutineUseDelaySeconds <= 0.0f
        || RoutineUseQuantity <= 0
        || (!RoutineUseItemId.IsEmpty() && !AcceptsSupplyItem(RoutineUseItemId))
        || (RoutineUseItemId.IsEmpty() && !bRoutineCanUseAnyAcceptedStock))
    {
        return false;
    }

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            RoutineUseTimerHandle,
            this,
            &UHuwamNpcSupplyComponent::HandleRoutineSupplyUse,
            RoutineUseDelaySeconds,
            true
        );
        return true;
    }

    return false;
}

void UHuwamNpcSupplyComponent::StopRoutineSupplyUse()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(RoutineUseTimerHandle);
    }
}

bool UHuwamNpcSupplyComponent::UseRoutineSupply(FHuwamNpcSupplyUseResult& OutUseResult)
{
    const FString CandidateItemId = GetRoutineUseCandidateItemId();
    return !CandidateItemId.IsEmpty() && UseStockItemById(CandidateItemId, RoutineUseQuantity, OutUseResult);
}

bool UHuwamNpcSupplyComponent::AddStockItemById(const FString& ItemId, int32 Quantity)
{
    if (!AddStockItemByIdInternal(ItemId, Quantity))
    {
        return false;
    }

    OnNpcSupplyChanged.Broadcast();
    return true;
}

bool UHuwamNpcSupplyComponent::AcceptsSupplyItem(const FString& ItemId) const
{
    return !ItemId.IsEmpty() && (AcceptedSupplyItemIds.IsEmpty() || AcceptedSupplyItemIds.Contains(ItemId));
}

int32 UHuwamNpcSupplyComponent::GetAcceptedInventoryQuantity(UHuwamInventoryComponent* SourceInventory) const
{
    if (!SourceInventory)
    {
        return 0;
    }

    int32 Quantity = 0;
    for (const FHuwamInventoryStack& Stack : SourceInventory->GetInventoryStacks())
    {
        if (Stack.IsValidStack() && AcceptsSupplyItem(Stack.ItemId))
        {
            Quantity += Stack.Quantity;
        }
    }

    return Quantity;
}

int32 UHuwamNpcSupplyComponent::GetStockQuantity(const FString& ItemId) const
{
    const int32 StockIndex = FindStockItemIndex(ItemId);
    return StockIndex == INDEX_NONE ? 0 : StockItems[StockIndex].Quantity;
}

int32 UHuwamNpcSupplyComponent::GetTotalStockQuantity() const
{
    int32 Quantity = 0;
    for (const FHuwamIdQuantity& StockItem : StockItems)
    {
        if (!StockItem.Id.IsEmpty() && StockItem.Quantity > 0)
        {
            Quantity += StockItem.Quantity;
        }
    }

    return Quantity;
}

int32 UHuwamNpcSupplyComponent::GetRemainingStockCapacity() const
{
    return FMath::Max(0, StockCapacity - GetTotalStockQuantity());
}

int64 UHuwamNpcSupplyComponent::GetPurchaseValueCopper(const FString& ItemId) const
{
    FHuwamItemRow ItemRow;
    if (LoadItemRow(ItemId, ItemRow))
    {
        if (ItemRow.BaseValueCopper > 0)
        {
            return ItemRow.BaseValueCopper;
        }

        if (ItemRow.BaseValueGold > 0)
        {
            return static_cast<int64>(ItemRow.BaseValueGold) * 1000000;
        }
    }

    return FallbackPurchaseValueCopper;
}

bool UHuwamNpcSupplyComponent::IsSupplyLow() const
{
    return GetTotalStockQuantity() <= LowSupplyThreshold;
}

bool UHuwamNpcSupplyComponent::IsSupplyNeedSatisfied() const
{
    return GetTotalStockQuantity() >= SupplyNeedTargetQuantity;
}

bool UHuwamNpcSupplyComponent::ShouldRequestSupplyHelp() const
{
    return IsSupplyLow() && !IsSupplyNeedSatisfied();
}

FHuwamNpcSupplyNeedState UHuwamNpcSupplyComponent::GetSupplyNeedState() const
{
    FHuwamNpcSupplyNeedState NeedState;
    NeedState.NpcId = NpcId;
    NeedState.NeedId = SupplyNeedId;
    NeedState.DisplayName = SupplyNeedDisplayName;
    NeedState.Reason = SupplyNeedReason;
    NeedState.TotalStockQuantity = GetTotalStockQuantity();
    NeedState.LowStockThreshold = LowSupplyThreshold;
    NeedState.TargetStockQuantity = SupplyNeedTargetQuantity;
    NeedState.NeededQuantity = FMath::Max(0, SupplyNeedTargetQuantity - NeedState.TotalStockQuantity);
    NeedState.bNeedSatisfied = IsSupplyNeedSatisfied();
    NeedState.bShouldRequestSupplyHelp = ShouldRequestSupplyHelp();

    if (NeedState.TotalStockQuantity <= 0)
    {
        NeedState.Urgency = EHuwamNpcSupplyNeedUrgency::Critical;
        NeedState.NextAction = EHuwamNpcSupplyNeedNextAction::RequestPlayerHelp;
    }
    else if (NeedState.bShouldRequestSupplyHelp)
    {
        NeedState.Urgency = EHuwamNpcSupplyNeedUrgency::Urgent;
        NeedState.NextAction = EHuwamNpcSupplyNeedNextAction::RequestPlayerHelp;
    }
    else if (!NeedState.bNeedSatisfied)
    {
        NeedState.Urgency = EHuwamNpcSupplyNeedUrgency::Low;
        NeedState.NextAction = EHuwamNpcSupplyNeedNextAction::AcceptSurplus;
    }

    return NeedState;
}

bool UHuwamNpcSupplyComponent::IsRoutineSupplyUseActive() const
{
    if (const UWorld* World = GetWorld())
    {
        return World->GetTimerManager().IsTimerActive(RoutineUseTimerHandle);
    }

    return false;
}

float UHuwamNpcSupplyComponent::GetRoutineUseDelaySeconds() const
{
    const float ResolvedSecondsPerDay = ResolveSecondsPerInGameDay();
    if (RoutineUseIntervalInGameDays <= 0 || ResolvedSecondsPerDay <= 0.0f)
    {
        return 0.0f;
    }

    return static_cast<float>(RoutineUseIntervalInGameDays) * ResolvedSecondsPerDay;
}

FString UHuwamNpcSupplyComponent::GetRoutineUseCandidateItemId() const
{
    if (!RoutineUseItemId.IsEmpty()
        && AcceptsSupplyItem(RoutineUseItemId)
        && GetStockQuantity(RoutineUseItemId) >= RoutineUseQuantity)
    {
        return RoutineUseItemId;
    }

    if (!bRoutineCanUseAnyAcceptedStock)
    {
        return FString();
    }

    for (const FString& AcceptedItemId : AcceptedSupplyItemIds)
    {
        if (!AcceptedItemId.IsEmpty() && GetStockQuantity(AcceptedItemId) >= RoutineUseQuantity)
        {
            return AcceptedItemId;
        }
    }

    for (const FHuwamIdQuantity& StockItem : StockItems)
    {
        if (!StockItem.Id.IsEmpty()
            && StockItem.Quantity >= RoutineUseQuantity
            && AcceptsSupplyItem(StockItem.Id))
        {
            return StockItem.Id;
        }
    }

    return FString();
}

FString UHuwamNpcSupplyComponent::GetNpcId() const
{
    return NpcId;
}

TArray<FHuwamIdQuantity> UHuwamNpcSupplyComponent::GetStockItems() const
{
    return StockItems;
}

int32 UHuwamNpcSupplyComponent::FindStockItemIndex(const FString& ItemId) const
{
    if (ItemId.IsEmpty())
    {
        return INDEX_NONE;
    }

    for (int32 Index = 0; Index < StockItems.Num(); ++Index)
    {
        if (StockItems[Index].Id == ItemId)
        {
            return Index;
        }
    }

    return INDEX_NONE;
}

bool UHuwamNpcSupplyComponent::AddStockItemByIdInternal(const FString& ItemId, int32 Quantity)
{
    if (!AcceptsSupplyItem(ItemId) || Quantity <= 0 || !CanAddStockQuantity(Quantity))
    {
        return false;
    }

    const int32 StockIndex = FindStockItemIndex(ItemId);
    if (StockIndex != INDEX_NONE)
    {
        StockItems[StockIndex].Quantity += Quantity;
        return true;
    }

    FHuwamIdQuantity NewStockItem;
    NewStockItem.Id = ItemId;
    NewStockItem.Quantity = Quantity;
    StockItems.Add(NewStockItem);
    return true;
}

bool UHuwamNpcSupplyComponent::RemoveStockItemByIdInternal(const FString& ItemId, int32 Quantity)
{
    const int32 StockIndex = FindStockItemIndex(ItemId);
    if (StockIndex == INDEX_NONE || Quantity <= 0 || StockItems[StockIndex].Quantity < Quantity)
    {
        return false;
    }

    StockItems[StockIndex].Quantity -= Quantity;
    if (StockItems[StockIndex].Quantity <= 0)
    {
        StockItems.RemoveAt(StockIndex);
    }

    return true;
}

bool UHuwamNpcSupplyComponent::CanAddStockQuantity(int32 Quantity) const
{
    return Quantity > 0 && GetRemainingStockCapacity() >= Quantity;
}

bool UHuwamNpcSupplyComponent::LoadItemRow(const FString& ItemId, FHuwamItemRow& OutItemRow) const
{
    if (ItemId.IsEmpty())
    {
        return false;
    }

    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (const UHuwamDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UHuwamDataSubsystem>())
            {
                return DataSubsystem->GetItemRow(GetRowNameFromId(ItemId), OutItemRow);
            }
        }
    }

    return false;
}

float UHuwamNpcSupplyComponent::ResolveSecondsPerInGameDay() const
{
    if (bUseSharedHuwamTime)
    {
        if (const UWorld* World = GetWorld())
        {
            if (UGameInstance* GameInstance = World->GetGameInstance())
            {
                if (const UHuwamTimeSubsystem* TimeSubsystem = GameInstance->GetSubsystem<UHuwamTimeSubsystem>())
                {
                    return TimeSubsystem->GetSecondsPerInGameDay();
                }
            }
        }
    }

    return SecondsPerInGameDay;
}

void UHuwamNpcSupplyComponent::HandleRoutineSupplyUse()
{
    FHuwamNpcSupplyUseResult RoutineUseResult;
    UseRoutineSupply(RoutineUseResult);
}
