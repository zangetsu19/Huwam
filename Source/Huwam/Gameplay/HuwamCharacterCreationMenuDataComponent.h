#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/HuwamCharacterCreationComponent.h"
#include "HuwamCharacterCreationMenuDataComponent.generated.h"

class UHuwamDataRegistry;
class UHuwamDataSubsystem;

UENUM(BlueprintType)
enum class EHuwamCharacterCreationOptionType : uint8
{
    Race UMETA(DisplayName = "Race"),
    SubRace UMETA(DisplayName = "Sub-Race"),
    Class UMETA(DisplayName = "Class"),
    Job UMETA(DisplayName = "Job"),
    Skill UMETA(DisplayName = "Skill"),
    Ability UMETA(DisplayName = "Ability")
};

USTRUCT(BlueprintType)
struct FHuwamCharacterCreationOption
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    EHuwamCharacterCreationOptionType OptionType = EHuwamCharacterCreationOptionType::Race;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    FString OptionId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    FString ParentId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    FText DetailText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    bool bSelected = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    bool bAvailable = true;
};

USTRUCT(BlueprintType)
struct FHuwamCharacterCreationMenuSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    FHuwamCharacterCreationRequest Request;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    TArray<FHuwamCharacterCreationOption> RaceOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    TArray<FHuwamCharacterCreationOption> SubRaceOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    TArray<FHuwamCharacterCreationOption> ClassOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    TArray<FHuwamCharacterCreationOption> JobOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    TArray<FHuwamCharacterCreationOption> SkillOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    TArray<FHuwamCharacterCreationOption> AbilityOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    int32 ClassSlotsUsed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    int32 ClassSlotLimit = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    int32 JobSlotsUsed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    int32 JobSlotLimit = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    int32 SkillSlotsUsed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    int32 SkillSlotLimit = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    int32 AbilitySlotsUsed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    int32 AbilitySlotLimit = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    FHuwamCharacterCreationResult ValidationResult;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    bool bCanConfirm = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation Menu")
    bool bUsingFallbackOptions = false;
};

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamCharacterCreationMenuDataComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamCharacterCreationMenuDataComponent();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Character Creation Menu")
    void SetCharacterCreationComponent(UHuwamCharacterCreationComponent* InCharacterCreation);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Character Creation Menu")
    UHuwamCharacterCreationComponent* GetCharacterCreationComponent() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Character Creation Menu")
    FHuwamCharacterCreationMenuSnapshot BuildDefaultMenuSnapshot() const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Character Creation Menu")
    FHuwamCharacterCreationMenuSnapshot BuildMenuSnapshot(const FHuwamCharacterCreationRequest& Request) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Character Creation Menu")
    bool BuildRequestWithSingleSelection(const FHuwamCharacterCreationRequest& Request, EHuwamCharacterCreationOptionType OptionType, const FString& OptionId, FHuwamCharacterCreationRequest& OutRequest) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Character Creation Menu")
    bool BuildRequestWithToggledSelection(const FHuwamCharacterCreationRequest& Request, EHuwamCharacterCreationOptionType OptionType, const FString& OptionId, FHuwamCharacterCreationRequest& OutRequest) const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Character Creation Menu")
    TObjectPtr<UHuwamCharacterCreationComponent> CharacterCreation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Character Creation Menu")
    bool bUseFallbackOptionsWhenRegistryMissing = true;

private:
    void BuildRaceOptions(const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions, bool& bOutUsedFallback) const;
    void BuildSubRaceOptions(const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions, bool& bOutUsedFallback) const;
    void BuildClassOptions(const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions, bool& bOutUsedFallback) const;
    void BuildJobOptions(const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions, bool& bOutUsedFallback) const;
    void BuildSkillOptions(const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions, bool& bOutUsedFallback) const;
    void BuildAbilityOptions(const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions, bool& bOutUsedFallback) const;
    void AddFallbackOptions(EHuwamCharacterCreationOptionType OptionType, const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions) const;
    bool ToggleIdInArray(TArray<FString>& Values, const FString& OptionId, int32 MaxCount) const;
    int32 GetSkillLimitForRequest(const FHuwamCharacterCreationRequest& Request) const;
    int32 GetAbilityLimitForRequest(const FHuwamCharacterCreationRequest& Request) const;
    UHuwamCharacterCreationComponent* ResolveCharacterCreationComponent() const;
    UHuwamDataSubsystem* GetDataSubsystem() const;
    UHuwamDataRegistry* GetDataRegistry() const;
    FText MakeDisplayTextFromId(const FString& Id) const;
    FText MakeSubRaceDetailText(const FString& SubRaceId, EHuwamManaWellDepth ManaWellDepth) const;
};
