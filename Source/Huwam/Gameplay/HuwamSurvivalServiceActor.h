#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/HuwamEconomyLedgerSubsystem.h"
#include "Gameplay/HuwamNpcSupplyComponent.h"
#include "Gameplay/HuwamQuestComponent.h"
#include "Gameplay/HuwamSurvivalVitalsComponent.h"
#include "HuwamSurvivalServiceActor.generated.h"

class AHuwamPrototypePlayerActor;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EHuwamSurvivalServiceAccessPolicy : uint8
{
    Public UMETA(DisplayName = "Public"),
    GuildOrFactionMember UMETA(DisplayName = "Guild Or Faction Member"),
    OwnerOnly UMETA(DisplayName = "Owner Only"),
    Locked UMETA(DisplayName = "Locked")
};

USTRUCT(BlueprintType)
struct FHuwamSurvivalServiceResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    EHuwamSurvivalServiceType ServiceType = EHuwamSurvivalServiceType::TavernMeal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    EHuwamSurvivalServiceAccessPolicy AccessPolicy = EHuwamSurvivalServiceAccessPolicy::Public;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FString OwnerNpcId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FString OwningFactionId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FString SettlementId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    int64 CostCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    bool bCharged = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    bool bRevenueRecorded = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    int64 CurrencyBefore = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    int64 CurrencyAfter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FHuwamSurvivalVitalsSnapshot SnapshotBefore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FHuwamSurvivalVitalsSnapshot SnapshotAfter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FText Message;
};

USTRUCT(BlueprintType)
struct FHuwamSurvivalServiceUpkeepResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    EHuwamSurvivalServiceType ServiceType = EHuwamSurvivalServiceType::TavernMeal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    int64 UpkeepCostCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    bool bServiceEnabledAfter = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    bool bUnderfundedAfter = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FHuwamEconomyServiceUpkeepResult EconomyResult;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FText Message;
};

USTRUCT(BlueprintType)
struct FHuwamSurvivalServiceRestorationResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    EHuwamSurvivalServiceType ServiceType = EHuwamSurvivalServiceType::TavernMeal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FString QuestId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FString NeedId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    bool bServiceEnabledAfter = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    bool bUnderfundedAfter = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FText Message;
};

USTRUCT(BlueprintType)
struct FHuwamSurvivalServiceSaveState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FString ServiceIdentityId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FString ServiceInstanceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    EHuwamSurvivalServiceType ServiceType = EHuwamSurvivalServiceType::TavernMeal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    EHuwamSurvivalServiceAccessPolicy AccessPolicy = EHuwamSurvivalServiceAccessPolicy::Public;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FString OwnerNpcId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FString OwningFactionId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    FString SettlementId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    int64 CostCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    int64 UpkeepCostCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    bool bServiceEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Survival Service")
    bool bUnderfunded = false;
};

UCLASS(Blueprintable)
class HUWAM_API AHuwamSurvivalServiceActor : public AActor
{
    GENERATED_BODY()

public:
    AHuwamSurvivalServiceActor();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival Service")
    void SetServiceType(EHuwamSurvivalServiceType InServiceType);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    EHuwamSurvivalServiceType GetServiceType() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    FString GetServiceIdentityId() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    FString GetServiceInstanceId() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival Service")
    void SetServiceInstanceId(const FString& InServiceInstanceId);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    int64 GetCostCopper() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival Service")
    void SetCostCopper(int64 InCostCopper);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    int64 GetUpkeepCostCopper() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival Service")
    void SetUpkeepCostCopper(int64 InUpkeepCostCopper);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    bool IsUnderfunded() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    FString GetOwnerNpcId() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    FString GetOwningFactionId() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival Service")
    void SetOwnership(const FString& InOwnerNpcId, const FString& InOwningFactionId);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    FString GetSettlementId() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival Service")
    void SetSettlementId(const FString& InSettlementId);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    EHuwamSurvivalServiceAccessPolicy GetAccessPolicy() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival Service")
    void SetAccessPolicy(EHuwamSurvivalServiceAccessPolicy InAccessPolicy);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    FText GetServicePrompt() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    bool CanUseServiceWithPrototypePlayer(const AHuwamPrototypePlayerActor* PrototypePlayer) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival Service")
    bool UseServiceWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, FHuwamSurvivalServiceResult& OutResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival Service")
    bool PayPrototypeUpkeepFromLedger(FHuwamSurvivalServiceUpkeepResult& OutResult);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    FHuwamNpcSupplyNeedState GetServicePressureNeedState() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival Service")
    bool ApplyRestorationQuestTurnIn(const FHuwamQuestRuntimeState& TurnedInQuest, FHuwamSurvivalServiceRestorationResult& OutResult);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Survival Service")
    FHuwamSurvivalServiceSaveState GetServiceSaveState() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Survival Service")
    bool RestoreServiceStateForPrototypeSave(const FHuwamSurvivalServiceSaveState& SavedState);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Survival Service")
    TObjectPtr<UStaticMeshComponent> VisualMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival Service")
    EHuwamSurvivalServiceType ServiceType = EHuwamSurvivalServiceType::TavernMeal;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival Service")
    FString ServiceInstanceId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival Service")
    bool bServiceEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival Service", meta=(ClampMin="0"))
    int64 CostCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival Service", meta=(ClampMin="0"))
    int64 UpkeepCostCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival Service")
    bool bDisableWhenUpkeepUnpaid = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival Service")
    bool bUnderfunded = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival Service")
    FString OwnerNpcId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival Service")
    FString OwningFactionId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival Service")
    FString SettlementId = TEXT("town.eldoria");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Survival Service")
    EHuwamSurvivalServiceAccessPolicy AccessPolicy = EHuwamSurvivalServiceAccessPolicy::Public;

private:
    void ApplyDefaultEconomyForServiceType();
    bool DoesPrototypePlayerHaveAccess(const AHuwamPrototypePlayerActor* PrototypePlayer) const;
    bool CanPrototypePlayerAffordService(const AHuwamPrototypePlayerActor* PrototypePlayer) const;
    FText BuildBasePrompt() const;
    FText BuildFailureMessage(const AHuwamPrototypePlayerActor* PrototypePlayer) const;
    bool DoesCreatedCharacterMatchOwningFaction(const AHuwamPrototypePlayerActor* PrototypePlayer) const;
    FString BuildServiceRevenueSourceTag() const;
};
