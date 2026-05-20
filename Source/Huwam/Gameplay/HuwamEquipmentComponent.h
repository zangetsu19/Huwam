#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HuwamEquipmentComponent.generated.h"

class UHuwamInventoryComponent;
struct FHuwamItemRow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHuwamEquipmentChangedSignature);

USTRUCT(BlueprintType)
struct FHuwamEquipmentSlotState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Equipment")
    FString SlotId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Equipment")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Equipment")
    FString EquippedItemId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Equipment")
    bool bUnlocked = true;

    bool IsOccupied() const
    {
        return !EquippedItemId.IsEmpty();
    }
};

USTRUCT(BlueprintType)
struct FHuwamEquipmentStatSummary
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Equipment")
    int32 AttackPower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Equipment")
    int32 DefensePower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Equipment")
    int32 ManaPower = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Equipment")
    int32 UtilityPower = 0;
};

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamEquipmentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamEquipmentComponent();

    virtual void BeginPlay() override;

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Equipment")
    FHuwamEquipmentChangedSignature OnEquipmentChanged;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Equipment")
    bool EquipItemFromInventory(const FString& ItemId, const FString& PreferredSlotId, UHuwamInventoryComponent* Inventory, FString& OutUnequippedItemId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Equipment")
    bool UnequipSlotToInventory(const FString& SlotId, UHuwamInventoryComponent* Inventory, FString& OutUnequippedItemId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Equipment")
    bool UnlockSlot(const FString& SlotId);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Equipment")
    bool CanEquipItemToSlot(const FString& ItemId, const FString& SlotId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Equipment")
    bool IsSlotUnlocked(const FString& SlotId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Equipment")
    bool IsSlotOccupied(const FString& SlotId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Equipment")
    FString GetEquippedItemInSlot(const FString& SlotId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Equipment")
    TArray<FHuwamEquipmentSlotState> GetEquipmentSlots() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Equipment")
    FHuwamEquipmentStatSummary GetEquipmentStatSummary() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Equipment")
    int32 GetEquipmentAttackPower() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Equipment")
    int32 GetEquipmentDefensePower() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Equipment")
    int32 GetEquipmentManaPower() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Equipment")
    int32 GetEquipmentUtilityPower() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Equipment")
    void ResetToDefaultSlots();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Equipment")
    TArray<FHuwamEquipmentSlotState> EquipmentSlots;

private:
    int32 FindSlotIndex(const FString& SlotId) const;
    FString ResolveTargetSlotForItem(const FString& ItemId, const FString& PreferredSlotId) const;
    bool GetItemRowForEquippedItem(const FString& ItemId, FHuwamItemRow& OutItemRow) const;
    FString GetPrimaryEquipSlotForItem(const FString& ItemId) const;
    bool IsPouchSlot(const FString& SlotId) const;
    bool IsWeaponSlot(const FString& SlotId) const;
    void AddDefaultSlot(const FString& SlotId, const FText& DisplayName, bool bUnlocked);
};
