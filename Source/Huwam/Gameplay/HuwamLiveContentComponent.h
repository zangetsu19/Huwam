#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/HuwamDataTypes.h"
#include "HuwamLiveContentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHuwamLiveContentChangedSignature);

USTRUCT(BlueprintType)
struct FHuwamContentAccessResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Live Content")
    bool bAllowed = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Live Content")
    bool bPackKnown = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Live Content")
    bool bServerEnabled = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Live Content")
    bool bOwned = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Live Content")
    bool bVersionCompatible = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Live Content")
    bool bFeatureFlagsEnabled = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Live Content")
    FString ContentPackId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Live Content")
    FString EntitlementId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Live Content")
    EHuwamContentPackType PackType = EHuwamContentPackType::Cosmetic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Live Content")
    EHuwamContentPackState PackState = EHuwamContentPackState::Draft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Live Content")
    FText FailureReason;
};

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamLiveContentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamLiveContentComponent();

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Live Content")
    FHuwamLiveContentChangedSignature OnLiveContentChanged;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Live Content")
    void SetCurrentGameVersion(const FString& InCurrentGameVersion);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Live Content")
    bool EnableContentPack(const FString& ContentPackId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Live Content")
    bool DisableContentPack(const FString& ContentPackId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Live Content")
    bool EnableFeatureFlag(const FString& FeatureFlagId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Live Content")
    bool DisableFeatureFlag(const FString& FeatureFlagId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Live Content")
    bool GrantEntitlement(const FString& EntitlementId);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Live Content")
    bool RevokeEntitlement(const FString& EntitlementId);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Live Content")
    bool HasEntitlement(const FString& EntitlementId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Live Content")
    bool IsContentPackEnabled(const FString& ContentPackId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Live Content")
    bool IsFeatureFlagEnabled(const FString& FeatureFlagId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Live Content")
    FHuwamContentAccessResult CanUseContentPack(const FString& ContentPackId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Live Content")
    TArray<FHuwamContentPackRow> GetKnownContentPacks() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Live Content")
    TArray<FString> GetAccessibleContentPackIds() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Live Content")
    TArray<FString> GetOwnedEntitlementIds() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Live Content")
    TArray<FString> GetEnabledContentPackIds() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Live Content")
    TArray<FString> GetEnabledFeatureFlagIds() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Live Content")
    FString CurrentGameVersion = TEXT("0.1.0");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Live Content")
    TArray<FString> EnabledContentPackIds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Live Content")
    TArray<FString> DisabledContentPackIds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Live Content")
    TArray<FString> EnabledFeatureFlagIds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Live Content")
    TArray<FString> OwnedEntitlementIds;

private:
    bool LoadContentPackRow(const FString& ContentPackId, FHuwamContentPackRow& OutContentPackRow) const;
    bool IsVersionCompatible(const FString& RequiredGameVersion) const;
    bool AreFeatureFlagsEnabled(const TArray<FString>& FeatureFlagIds) const;
};
