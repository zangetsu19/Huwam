#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/HuwamQuestComponent.h"
#include "HuwamGuildContractBoardActor.generated.h"

class AHuwamPrototypePlayerActor;
class USceneComponent;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EHuwamGuildContractBoardState : uint8
{
    Unavailable UMETA(DisplayName = "Unavailable"),
    OfferingContract UMETA(DisplayName = "Offering Contract"),
    TrackingContract UMETA(DisplayName = "Tracking Contract"),
    ReadyForTurnIn UMETA(DisplayName = "Ready For Turn-In"),
    ContractCleared UMETA(DisplayName = "Contract Cleared")
};

USTRUCT(BlueprintType)
struct FHuwamGuildContractBoardEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    FString QuestId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    EHuwamGuildContractBoardState State = EHuwamGuildContractBoardState::Unavailable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    FHuwamQuestOffer QuestOffer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    FHuwamQuestRuntimeState QuestState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    bool bCanInteract = false;
};

USTRUCT(BlueprintType)
struct FHuwamGuildContractBoardSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    EHuwamGuildContractBoardState State = EHuwamGuildContractBoardState::Unavailable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    FText PromptText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    FHuwamQuestOffer QuestOffer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    FHuwamQuestRuntimeState ActiveQuest;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    FString SelectedQuestId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    TArray<FHuwamGuildContractBoardEntry> Entries;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    bool bCanInteract = false;
};

USTRUCT(BlueprintType)
struct FHuwamGuildContractBoardResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    FHuwamGuildContractBoardSnapshot SnapshotBefore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    FHuwamGuildContractBoardSnapshot SnapshotAfter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Guild Contract Board")
    FHuwamQuestRuntimeState TurnedInQuest;
};

UCLASS(Blueprintable)
class HUWAM_API AHuwamGuildContractBoardActor : public AActor
{
    GENERATED_BODY()

public:
    AHuwamGuildContractBoardActor();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Guild Contract Board")
    FHuwamGuildContractBoardSnapshot GetBoardSnapshotForPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Guild Contract Board")
    bool CanInteractWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Guild Contract Board")
    bool InteractWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, FHuwamGuildContractBoardResult& OutResult);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Guild Contract Board")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Guild Contract Board")
    TObjectPtr<UStaticMeshComponent> BoardMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Guild Contract Board")
    TObjectPtr<UStaticMeshComponent> LeftPostMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Guild Contract Board")
    TObjectPtr<UStaticMeshComponent> RightPostMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|Guild Contract Board")
    TObjectPtr<UStaticMeshComponent> NoticeMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Guild Contract Board")
    TArray<FString> ContractQuestIds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Guild Contract Board")
    FText OfferPrompt = NSLOCTEXT("HuwamGuildBoard", "OfferPrompt", "Accept Adventurers Guild slime contract");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Guild Contract Board")
    FText TrackPrompt = NSLOCTEXT("HuwamGuildBoard", "TrackPrompt", "Track active slime contract");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Guild Contract Board")
    FText TurnInPrompt = NSLOCTEXT("HuwamGuildBoard", "TurnInPrompt", "Turn in slime contract");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Guild Contract Board")
    FText ClearedPrompt = NSLOCTEXT("HuwamGuildBoard", "ClearedPrompt", "The first slime contract is cleared");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Guild Contract Board")
    FText UnavailablePrompt = NSLOCTEXT("HuwamGuildBoard", "UnavailablePrompt", "No guild contract is ready here");

private:
    FHuwamGuildContractBoardEntry BuildContractEntry(AHuwamPrototypePlayerActor* PrototypePlayer, const FString& QuestId) const;
    bool TrackQuestForPlayer(AHuwamPrototypePlayerActor* PrototypePlayer, const FString& QuestId) const;
    FText BuildPromptForEntry(const FHuwamGuildContractBoardEntry& Entry) const;
};
