#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/HuwamDataTypes.h"
#include "HuwamNpcSupplyComponent.generated.h"

class UHuwamInventoryComponent;
class UHuwamRewardComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHuwamNpcSupplyChangedSignature);

UENUM(BlueprintType)
enum class EHuwamNpcSupplyNeedUrgency : uint8
{
    Stable UMETA(DisplayName = "Stable"),
    Low UMETA(DisplayName = "Low"),
    Urgent UMETA(DisplayName = "Urgent"),
    Critical UMETA(DisplayName = "Critical")
};

UENUM(BlueprintType)
enum class EHuwamNpcSupplyNeedNextAction : uint8
{
    ContinueRoutine UMETA(DisplayName = "Continue Routine"),
    AcceptSurplus UMETA(DisplayName = "Accept Surplus"),
    RequestPlayerHelp UMETA(DisplayName = "Request Player Help")
};

USTRUCT(BlueprintType)
struct FHuwamNpcSupplyNeedState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    FString NpcId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    FString NeedId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    FText Reason;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    EHuwamNpcSupplyNeedUrgency Urgency = EHuwamNpcSupplyNeedUrgency::Stable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    EHuwamNpcSupplyNeedNextAction NextAction = EHuwamNpcSupplyNeedNextAction::ContinueRoutine;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    int32 TotalStockQuantity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    int32 LowStockThreshold = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    int32 TargetStockQuantity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    int32 NeededQuantity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    bool bNeedSatisfied = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    bool bShouldRequestSupplyHelp = false;
};

USTRUCT(BlueprintType)
struct FHuwamNpcSupplyDeliveryResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    FString NpcId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    TArray<FHuwamIdQuantity> DeliveredItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    int32 TotalQuantityDelivered = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    int32 StockQuantityAfter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    bool bNeedSatisfied = false;
};

USTRUCT(BlueprintType)
struct FHuwamNpcSupplyPurchaseResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    FString NpcId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    FHuwamIdQuantity PurchasedItem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    int64 UnitValueCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    int64 TotalCopperPaid = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    int32 TotalStockQuantityAfter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    bool bNeedSatisfied = false;
};

USTRUCT(BlueprintType)
struct FHuwamNpcSupplyUseResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    FString NpcId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    FHuwamIdQuantity UsedItem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    int32 ItemStockQuantityAfter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    int32 TotalStockQuantityAfter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply")
    bool bShouldRequestSupplyHelp = false;
};

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamNpcSupplyComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamNpcSupplyComponent();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(BlueprintAssignable, Category = "Huwam|NPC Supply")
    FHuwamNpcSupplyChangedSignature OnNpcSupplyChanged;

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply")
    bool DeliverAcceptedItemsFromInventory(UHuwamInventoryComponent* SourceInventory, int32 Quantity, FHuwamNpcSupplyDeliveryResult& OutDeliveryResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply")
    bool PurchaseAcceptedItemFromInventory(UHuwamInventoryComponent* SellerInventory, UHuwamRewardComponent* SellerRewards, const FString& ItemId, int32 Quantity, FHuwamNpcSupplyPurchaseResult& OutPurchaseResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply")
    bool UseStockItemById(const FString& ItemId, int32 Quantity, FHuwamNpcSupplyUseResult& OutUseResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply")
    bool StartRoutineSupplyUse();

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply")
    void StopRoutineSupplyUse();

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply")
    bool UseRoutineSupply(FHuwamNpcSupplyUseResult& OutUseResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply")
    bool AddStockItemById(const FString& ItemId, int32 Quantity);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    bool AcceptsSupplyItem(const FString& ItemId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    int32 GetAcceptedInventoryQuantity(UHuwamInventoryComponent* SourceInventory) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    int32 GetStockQuantity(const FString& ItemId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    int32 GetTotalStockQuantity() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    int32 GetRemainingStockCapacity() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    int64 GetPurchaseValueCopper(const FString& ItemId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    bool IsSupplyLow() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    bool IsSupplyNeedSatisfied() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    bool ShouldRequestSupplyHelp() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    FHuwamNpcSupplyNeedState GetSupplyNeedState() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    bool IsRoutineSupplyUseActive() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    float GetRoutineUseDelaySeconds() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    FString GetRoutineUseCandidateItemId() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    FString GetNpcId() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply")
    TArray<FHuwamIdQuantity> GetStockItems() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply")
    FString NpcId = TEXT("npc.eldoria.poor_resident_01");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply")
    TArray<FString> AcceptedSupplyItemIds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply")
    TArray<FHuwamIdQuantity> StockItems;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply|Need")
    FString SupplyNeedId = TEXT("need.eldoria.field_supplies");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply|Need")
    FText SupplyNeedDisplayName = NSLOCTEXT("HuwamNpcSupply", "FieldSuppliesNeedName", "Field Supplies");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply|Need")
    FText SupplyNeedReason = NSLOCTEXT("HuwamNpcSupply", "FieldSuppliesNeedReason", "Daily materials are being used for local repairs, care, and ordinary work.");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply", meta = (ClampMin = "0"))
    int32 LowSupplyThreshold = 4;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply", meta = (ClampMin = "1"))
    int32 SupplyNeedTargetQuantity = 10;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply", meta = (ClampMin = "1"))
    int32 StockCapacity = 50;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply", meta = (ClampMin = "0"))
    int64 FallbackPurchaseValueCopper = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply|Routine")
    bool bAutoStartRoutineSupplyUse = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply|Routine")
    FString RoutineUseItemId = TEXT("material.wood.basic");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply|Routine")
    bool bRoutineCanUseAnyAcceptedStock = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply|Routine", meta = (ClampMin = "1"))
    int32 RoutineUseQuantity = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply|Routine", meta = (ClampMin = "1"))
    int32 RoutineUseIntervalInGameDays = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply|Routine")
    bool bUseSharedHuwamTime = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply|Routine", meta = (ClampMin = "1.0"))
    float SecondsPerInGameDay = 86400.0f / 7.0f;

private:
    FTimerHandle RoutineUseTimerHandle;

    int32 FindStockItemIndex(const FString& ItemId) const;
    bool AddStockItemByIdInternal(const FString& ItemId, int32 Quantity);
    bool RemoveStockItemByIdInternal(const FString& ItemId, int32 Quantity);
    bool CanAddStockQuantity(int32 Quantity) const;
    bool LoadItemRow(const FString& ItemId, FHuwamItemRow& OutItemRow) const;
    float ResolveSecondsPerInGameDay() const;
    void HandleRoutineSupplyUse();
};
