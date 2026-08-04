#pragma once

#include "CoreMinimal.h"
#include "Data/HuwamDataTypes.h"
#include "GameFramework/Actor.h"
#include "HuwamGatheringResourceActor.generated.h"

class AHuwamPrototypePlayerActor;
class USceneComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHuwamGatheringResourceGatheredSignature, AHuwamPrototypePlayerActor*, PrototypePlayer, FString, GatheredItemId, int32, GatheredQuantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHuwamGatheringResourceRespawnedSignature);

UCLASS(Blueprintable)
class HUWAM_API AHuwamGatheringResourceActor : public AActor
{
    GENERATED_BODY()

public:
    AHuwamGatheringResourceActor();

    virtual void BeginPlay() override;

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Gathering")
    FHuwamGatheringResourceGatheredSignature OnResourceGathered;

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Gathering")
    FHuwamGatheringResourceRespawnedSignature OnResourceRespawned;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Gathering")
    bool InitializeFromMaterialData();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Gathering")
    bool CanGatherWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Gathering")
    bool GatherWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Gathering")
    void RespawnResource();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Gathering")
    FString GetMaterialId() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Gathering")
    FString GetGatheredItemId() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Gathering")
    FText GetResourceDisplayName() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Gathering")
    FText GetGatherPrompt() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Gathering")
    bool IsAvailable() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Gathering")
    float GetRespawnDelaySeconds() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering")
    TObjectPtr<UStaticMeshComponent> ResourceMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering")
    FString MaterialId = TEXT("material.herb.lavender");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering")
    FString GatheredItemId = TEXT("material.herb.lavender");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering")
    FText ResourceDisplayName = NSLOCTEXT("HuwamGathering", "FallbackLavenderName", "Lavender");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering")
    EHuwamRank Rank = EHuwamRank::F;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering", meta = (ClampMin = "1"))
    int32 GatheredQuantity = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering")
    TArray<FString> RequiredToolItemIds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering")
    TArray<FString> QuestProgressSourceIds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering", meta = (ClampMin = "0"))
    int32 QuestProgressPerGather = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering")
    FText GatherPrompt = NSLOCTEXT("HuwamGathering", "GatherResourcePrompt", "Gather resource");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering")
    bool bAutoInitializeFromData = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering")
    bool bRespawns = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering", meta = (ClampMin = "0"))
    int32 RespawnInGameDays = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering")
    bool bUseSharedHuwamTime = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering", meta = (ClampMin = "1.0"))
    float SecondsPerInGameDay = 86400.0f / 7.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Gathering")
    bool bAvailable = true;

private:
    FTimerHandle RespawnTimerHandle;

    bool LoadMaterialRow(FHuwamMaterialRow& OutMaterialRow) const;
    float ResolveSecondsPerInGameDay() const;
    bool HasRequiredTool(const AHuwamPrototypePlayerActor* PrototypePlayer) const;
    void AdvanceMatchingQuestProgress(AHuwamPrototypePlayerActor* PrototypePlayer) const;
    void DepleteResource();
    void RefreshAvailabilityVisualState();
};
