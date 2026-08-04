#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/HuwamNpcSupplyComponent.h"
#include "Gameplay/HuwamQuestComponent.h"
#include "HuwamNpcSupplyQuestOfferActor.generated.h"

class AHuwamPrototypePlayerActor;
class USceneComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHuwamNpcSupplyQuestOfferSignature, AHuwamPrototypePlayerActor*, PrototypePlayer, FHuwamQuestOffer, QuestOffer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHuwamNpcSupplyQuestTurnInSignature, AHuwamPrototypePlayerActor*, PrototypePlayer, FHuwamQuestRuntimeState, TurnedInQuest, FHuwamNpcSupplyDeliveryResult, DeliveryResult);

UENUM(BlueprintType)
enum class EHuwamNpcSupplyQuestInteractionState : uint8
{
    Unavailable UMETA(DisplayName = "Unavailable"),
    OfferingWork UMETA(DisplayName = "Offering Work"),
    WaitingForDelivery UMETA(DisplayName = "Waiting For Delivery"),
    ReadyForTurnIn UMETA(DisplayName = "Ready For Turn-In"),
    Satisfied UMETA(DisplayName = "Satisfied")
};

USTRUCT(BlueprintType)
struct FHuwamNpcSupplyQuestInteractionSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    EHuwamNpcSupplyQuestInteractionState State = EHuwamNpcSupplyQuestInteractionState::Unavailable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    FText PromptText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    FHuwamNpcSupplyNeedState SupplyNeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    FHuwamQuestOffer QuestOffer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    FHuwamQuestRuntimeState ActiveQuest;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    bool bCanPreviewOffer = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    bool bCanAcceptOffer = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    bool bCanTurnInQuest = false;
};

UENUM(BlueprintType)
enum class EHuwamNpcSupplyQuestInteractionOutcome : uint8
{
    None UMETA(DisplayName = "None"),
    OfferPreviewed UMETA(DisplayName = "Offer Previewed"),
    OfferAccepted UMETA(DisplayName = "Offer Accepted"),
    WaitingForDelivery UMETA(DisplayName = "Waiting For Delivery"),
    QuestTurnedIn UMETA(DisplayName = "Quest Turned In"),
    Satisfied UMETA(DisplayName = "Satisfied"),
    Unavailable UMETA(DisplayName = "Unavailable")
};

USTRUCT(BlueprintType)
struct FHuwamNpcSupplyQuestInteractionResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    EHuwamNpcSupplyQuestInteractionOutcome Outcome = EHuwamNpcSupplyQuestInteractionOutcome::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    FHuwamNpcSupplyQuestInteractionSnapshot SnapshotBefore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    FHuwamNpcSupplyQuestInteractionSnapshot SnapshotAfter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    FHuwamQuestOffer QuestOffer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    FHuwamQuestRuntimeState TurnedInQuest;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|NPC Supply Quest Offer")
    FHuwamNpcSupplyDeliveryResult DeliveryResult;
};

UCLASS(Blueprintable)
class HUWAM_API AHuwamNpcSupplyQuestOfferActor : public AActor
{
    GENERATED_BODY()

public:
    AHuwamNpcSupplyQuestOfferActor();

    UPROPERTY(BlueprintAssignable, Category = "Huwam|NPC Supply Quest Offer")
    FHuwamNpcSupplyQuestOfferSignature OnQuestOfferPreviewed;

    UPROPERTY(BlueprintAssignable, Category = "Huwam|NPC Supply Quest Offer")
    FHuwamNpcSupplyQuestOfferSignature OnQuestOfferAccepted;

    UPROPERTY(BlueprintAssignable, Category = "Huwam|NPC Supply Quest Offer")
    FHuwamNpcSupplyQuestTurnInSignature OnQuestTurnedIn;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply Quest Offer")
    bool CanInteractWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply Quest Offer")
    FHuwamNpcSupplyQuestInteractionSnapshot GetInteractionSnapshotForPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply Quest Offer")
    bool InteractByStateWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, bool bAcceptOfferWhenAvailable, FHuwamNpcSupplyQuestInteractionResult& OutInteractionResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply Quest Offer")
    bool InteractWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, FHuwamQuestOffer& OutQuestOffer);

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply Quest Offer")
    bool AcceptQuestOfferWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, FHuwamQuestOffer& OutAcceptedQuestOffer);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply Quest Offer")
    bool CanTurnInQuestWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|NPC Supply Quest Offer")
    bool TurnInQuestWithPrototypePlayer(AHuwamPrototypePlayerActor* PrototypePlayer, FHuwamQuestRuntimeState& OutTurnedInQuest, FHuwamNpcSupplyDeliveryResult& OutDeliveryResult);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply Quest Offer")
    FText GetInteractionPrompt() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|NPC Supply Quest Offer")
    UHuwamNpcSupplyComponent* GetNpcSupplyComponent() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply Quest Offer")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply Quest Offer")
    TObjectPtr<UHuwamNpcSupplyComponent> NpcSupply;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply Quest Offer")
    TObjectPtr<UStaticMeshComponent> PrototypeBodyMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply Quest Offer")
    TObjectPtr<UStaticMeshComponent> PrototypeHeadMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply Quest Offer")
    FText InteractionPrompt = NSLOCTEXT("HuwamNpcSupplyQuestOffer", "FieldSuppliesOfferPrompt", "Ask about field supplies");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply Quest Offer")
    FText WaitingForDeliveryPrompt = NSLOCTEXT("HuwamNpcSupplyQuestOffer", "FieldSuppliesWaitingPrompt", "Gather field supplies");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply Quest Offer")
    FText ReadyForTurnInPrompt = NSLOCTEXT("HuwamNpcSupplyQuestOffer", "FieldSuppliesTurnInPrompt", "Deliver field supplies");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply Quest Offer")
    FText SatisfiedPrompt = NSLOCTEXT("HuwamNpcSupplyQuestOffer", "FieldSuppliesSatisfiedPrompt", "Field supplies are stocked");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|NPC Supply Quest Offer")
    FText UnavailablePrompt = NSLOCTEXT("HuwamNpcSupplyQuestOffer", "FieldSuppliesUnavailablePrompt", "No field supply request available");
};
