#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HuwamInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHuwamInventoryChangedSignature);

UENUM(BlueprintType)
enum class EHuwamInventoryFreshness : uint8
{
    Stable UMETA(DisplayName = "Stable"),
    Fresh UMETA(DisplayName = "Fresh"),
    Stale UMETA(DisplayName = "Stale"),
    Spoiled UMETA(DisplayName = "Spoiled")
};

UENUM(BlueprintType)
enum class EHuwamInventoryStorageQuality : uint8
{
    Loose UMETA(DisplayName = "Loose"),
    Pouch UMETA(DisplayName = "Pouch"),
    Chest UMETA(DisplayName = "Chest"),
    Cooled UMETA(DisplayName = "Cooled"),
    Magical UMETA(DisplayName = "Magical")
};

USTRUCT(BlueprintType)
struct FHuwamInventoryStack
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    FString ItemId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory", meta = (ClampMin = "0"))
    int32 Quantity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    bool bTracksFreshness = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    int32 AcquiredInGameDay = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory", meta = (ClampMin = "0"))
    int32 StaleAfterInGameDays = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory", meta = (ClampMin = "0"))
    int32 SpoilsAfterInGameDays = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    EHuwamInventoryFreshness Freshness = EHuwamInventoryFreshness::Stable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    EHuwamInventoryStorageQuality StorageQuality = EHuwamInventoryStorageQuality::Loose;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    FString StorageRouteId = TEXT("storage.loose");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory", meta = (ClampMin = "0.0"))
    float SpoilageDurationMultiplier = 1.0f;

    bool IsValidStack() const
    {
        return !ItemId.IsEmpty() && Quantity > 0;
    }
};

USTRUCT(BlueprintType)
struct FHuwamInventoryStorageRoute
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    FString RouteId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    FString ContainerItemId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    EHuwamInventoryStorageQuality StorageQuality = EHuwamInventoryStorageQuality::Loose;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    int32 UsedStacks = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    int32 MaxStacks = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    int32 RemainingStacks = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    float SpoilageDurationMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    bool bAvailable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Inventory")
    bool bCanPreserveFreshness = false;
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
    bool AddItemByIdToStorage(const FString& ItemId, int32 Quantity, EHuwamInventoryStorageQuality StorageQuality, int32& OutRemainder);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Inventory")
    bool AssignFirstMatchingStackToStorage(const FString& ItemId, EHuwamInventoryStorageQuality TargetStorageQuality);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Inventory")
    bool RemoveItemById(const FString& ItemId, int32 Quantity);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    bool HasItemQuantity(const FString& ItemId, int32 Quantity) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    int32 GetItemQuantity(const FString& ItemId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    TArray<FHuwamInventoryStack> GetInventoryStacks() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Inventory")
    int32 RefreshSpoilageState();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    EHuwamInventoryFreshness PreviewItemFreshnessAfterInGameDays(const FString& ItemId, int32 ElapsedInGameDays) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    EHuwamInventoryFreshness PreviewItemFreshnessAfterInGameDaysWithStorage(const FString& ItemId, int32 ElapsedInGameDays, EHuwamInventoryStorageQuality StorageQuality) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    EHuwamInventoryStorageQuality GetBestStorageQuality() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    TArray<FHuwamInventoryStorageRoute> GetStorageRoutes() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    int32 GetUsedStorageRouteStacks(EHuwamInventoryStorageQuality StorageQuality) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    int32 GetMaxStorageRouteStacks(EHuwamInventoryStorageQuality StorageQuality) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    int32 GetRemainingStorageRouteStacks(EHuwamInventoryStorageQuality StorageQuality) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    float GetSpoilageDurationMultiplierForStorage(EHuwamInventoryStorageQuality StorageQuality) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    int32 GetUsedSlots() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    int32 GetMaxSlots() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Inventory")
    int32 GetRemainingSlots() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Inventory")
    void ClearInventory();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Inventory")
    bool RestoreInventoryStacksForPrototypeSave(const TArray<FHuwamInventoryStack>& SavedStacks);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Inventory", meta = (ClampMin = "1"))
    int32 MaxSlots = 20;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Inventory", meta = (ClampMin = "1"))
    int32 FallbackStackLimit = 99;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Inventory")
    TArray<FHuwamInventoryStack> InventoryStacks;

private:
    struct FHuwamSpoilageProfile
    {
        bool bTracksFreshness = false;
        int32 StaleAfterInGameDays = 0;
        int32 SpoilsAfterInGameDays = 0;
    };

    int32 GetStackLimitForItem(const FString& ItemId) const;
    int32 FindFirstPartialStackIndex(const FHuwamInventoryStack& IncomingStack, int32 StackLimit) const;
    FHuwamInventoryStack MakeInventoryStack(const FString& ItemId, int32 Quantity) const;
    FHuwamInventoryStack MakeInventoryStack(const FString& ItemId, int32 Quantity, EHuwamInventoryStorageQuality StorageQuality) const;
    FHuwamSpoilageProfile GetSpoilageProfileForItem(const FString& ItemId) const;
    FHuwamSpoilageProfile ApplyStorageToSpoilageProfile(const FHuwamSpoilageProfile& SpoilageProfile, EHuwamInventoryStorageQuality StorageQuality) const;
    EHuwamInventoryFreshness CalculateFreshness(const FHuwamSpoilageProfile& SpoilageProfile, int32 ElapsedInGameDays) const;
    EHuwamInventoryFreshness GetWorseFreshness(EHuwamInventoryFreshness FirstFreshness, EHuwamInventoryFreshness SecondFreshness) const;
    EHuwamInventoryStorageQuality GetPreferredStorageQualityForItem(const FString& ItemId) const;
    FString GetStorageRouteIdForQuality(EHuwamInventoryStorageQuality StorageQuality) const;
    FString GetContainerItemIdForStorageQuality(EHuwamInventoryStorageQuality StorageQuality) const;
    FText GetStorageRouteDisplayName(EHuwamInventoryStorageQuality StorageQuality) const;
    int32 GetCurrentInGameDay() const;
    bool CanStacksMerge(const FHuwamInventoryStack& ExistingStack, const FHuwamInventoryStack& IncomingStack) const;
    void RemoveEmptyStacks();
};
