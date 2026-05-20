#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HuwamInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHuwamInventoryChangedSignature);

USTRUCT(BlueprintType)
struct FHuwamInventoryStack
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    FString ItemId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory", meta = (ClampMin = "0"))
    int32 Quantity = 0;

    bool IsValidStack() const
    {
        return !ItemId.IsEmpty() && Quantity > 0;
    }
};

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamInventoryComponent();

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Inventory")
    FHuwamInventoryChangedSignature OnInventoryChanged;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Inventory")
    bool AddItemById(const FString& ItemId, int32 Quantity, int32& OutRemainder);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Inventory")
    bool RemoveItemById(const FString& ItemId, int32 Quantity);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    bool HasItemQuantity(const FString& ItemId, int32 Quantity) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    int32 GetItemQuantity(const FString& ItemId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    TArray<FHuwamInventoryStack> GetInventoryStacks() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    int32 GetUsedSlots() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    int32 GetMaxSlots() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    int32 GetRemainingSlots() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Inventory")
    void ClearInventory();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Inventory", meta = (ClampMin = "1"))
    int32 MaxSlots = 20;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Inventory", meta = (ClampMin = "1"))
    int32 FallbackStackLimit = 99;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Inventory")
    TArray<FHuwamInventoryStack> InventoryStacks;

private:
    int32 GetStackLimitForItem(const FString& ItemId) const;
    int32 FindFirstPartialStackIndex(const FString& ItemId, int32 StackLimit) const;
    void RemoveEmptyStacks();
};
