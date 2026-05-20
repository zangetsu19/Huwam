#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/HuwamDataTypes.h"
#include "HuwamCharacterCreationComponent.generated.h"

class UHuwamCharacterStatsComponent;
class UHuwamDataSubsystem;

UENUM(BlueprintType)
enum class EHuwamCharacterCreationPath : uint8
{
    Normal UMETA(DisplayName = "Normal"),
    Reincarnated UMETA(DisplayName = "Reincarnated")
};

USTRUCT(BlueprintType)
struct FHuwamCharacterCreationRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FText CharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    EHuwamCharacterCreationPath CreationPath = EHuwamCharacterCreationPath::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FString RaceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FString SubRaceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    TArray<FString> ClassIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    TArray<FString> JobIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    TArray<FString> SkillIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    TArray<FString> AbilityIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    bool bAllowHiddenOptions = false;
};

USTRUCT(BlueprintType)
struct FHuwamCreatedClassState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FString ClassId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    int32 Level = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    int32 MaxLevel = 20;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FString GuildId;
};

USTRUCT(BlueprintType)
struct FHuwamCreatedJobState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FString JobId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FText RankName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    int32 MinSalaryGold = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    int32 MaxSalaryGold = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FString AssociatedGuildId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    TArray<FString> DiscountResourceIds;
};

USTRUCT(BlueprintType)
struct FHuwamCreatedSkillState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FString SkillId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FText Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    bool bPermanentAtCreation = true;
};

USTRUCT(BlueprintType)
struct FHuwamCreatedAbilityState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FString AbilityId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FText AbilityType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    bool bPermanentAtCreation = true;
};

USTRUCT(BlueprintType)
struct FHuwamCreatedCharacterState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FText CharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    EHuwamCharacterCreationPath CreationPath = EHuwamCharacterCreationPath::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FString RaceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FText RaceName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FString SubRaceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FText SubRaceName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    EHuwamManaWellDepth ManaWellDepth = EHuwamManaWellDepth::Average;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    TArray<FHuwamCreatedClassState> Classes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    TArray<FHuwamCreatedJobState> Jobs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    TArray<FHuwamCreatedSkillState> PermanentSkills;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    TArray<FHuwamCreatedAbilityState> PermanentAbilities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    TMap<FString, int32> StartingStatBonuses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FString StartingTownId = TEXT("town.eldoria");
};

USTRUCT(BlueprintType)
struct FHuwamCharacterCreationResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    TArray<FText> Errors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    TArray<FText> Warnings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Character Creation")
    FHuwamCreatedCharacterState CharacterState;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHuwamCharacterCreatedSignature, const FHuwamCharacterCreationResult&, Result);

UCLASS(ClassGroup=(Huwam), meta=(BlueprintSpawnableComponent))
class HUWAM_API UHuwamCharacterCreationComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHuwamCharacterCreationComponent();

    UPROPERTY(BlueprintAssignable, Category = "Huwam|Character Creation")
    FHuwamCharacterCreatedSignature OnCharacterCreated;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Character Creation")
    FHuwamCharacterCreationRequest BuildDefaultCharacterCreationRequest() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Character Creation")
    int32 GetSkillLimitForPath(EHuwamCharacterCreationPath CreationPath) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Character Creation")
    int32 GetAbilityLimitForPath(EHuwamCharacterCreationPath CreationPath) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Character Creation")
    bool ValidateCharacterCreation(const FHuwamCharacterCreationRequest& Request, FHuwamCharacterCreationResult& OutResult) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Character Creation", meta=(AdvancedDisplay="TargetStats"))
    bool ApplyCharacterCreation(const FHuwamCharacterCreationRequest& Request, UHuwamCharacterStatsComponent* TargetStats, FHuwamCharacterCreationResult& OutResult);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Character Creation")
    void ResetCreatedCharacter();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Character Creation")
    bool HasCreatedCharacter() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Character Creation")
    FHuwamCreatedCharacterState GetCreatedCharacterState() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Character Creation", meta=(ClampMin="1"))
    int32 MaxClassSlots = 3;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Character Creation", meta=(ClampMin="1"))
    int32 MaxJobSlots = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Character Creation", meta=(ClampMin="1"))
    int32 NormalSkillLimit = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Character Creation", meta=(ClampMin="1"))
    int32 NormalAbilityLimit = 5;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Character Creation", meta=(ClampMin="1"))
    int32 ReincarnatedSkillLimit = 15;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Character Creation", meta=(ClampMin="1"))
    int32 ReincarnatedAbilityLimit = 15;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Character Creation")
    FHuwamCreatedCharacterState CreatedCharacterState;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Character Creation")
    bool bHasCreatedCharacter = false;

private:
    bool BuildCreatedCharacterState(const FHuwamCharacterCreationRequest& Request, FHuwamCreatedCharacterState& OutState, TArray<FText>& OutWarnings) const;
    void ApplyStartingStatBonuses(const FHuwamCreatedCharacterState& State, UHuwamCharacterStatsComponent* TargetStats) const;
    void BuildStartingStatBonuses(const FHuwamCharacterCreationRequest& Request, TMap<FString, int32>& OutBonuses) const;
    void AddRaceStatBonuses(const FString& RaceId, TMap<FString, int32>& OutBonuses) const;
    void AddSubRaceStatBonuses(const FString& SubRaceId, TMap<FString, int32>& OutBonuses) const;
    void AddClassStatBonuses(const FString& ClassId, TMap<FString, int32>& OutBonuses) const;
    void AddJobStatBonuses(const FString& JobId, TMap<FString, int32>& OutBonuses) const;
    void AddSkillStatBonuses(const FString& SkillId, TMap<FString, int32>& OutBonuses) const;
    void AddAbilityStatBonuses(const FString& AbilityId, TMap<FString, int32>& OutBonuses) const;
    void AddStatBonus(TMap<FString, int32>& OutBonuses, const FString& StatId, int32 Delta) const;
    EHuwamManaWellDepth ResolveManaWellDepth(const FHuwamRaceRow* RaceRow, const FHuwamSubRaceRow* SubRaceRow) const;
    UHuwamDataSubsystem* GetDataSubsystem() const;
    bool HasDataRegistry() const;
    bool GetRaceRowById(const FString& RaceId, FHuwamRaceRow& OutRow) const;
    bool GetSubRaceRowById(const FString& SubRaceId, FHuwamSubRaceRow& OutRow) const;
    bool GetClassRowById(const FString& ClassId, FHuwamClassRow& OutRow) const;
    bool GetJobRowById(const FString& JobId, FHuwamJobRow& OutRow) const;
    bool GetSkillRowById(const FString& SkillId, FHuwamSkillRow& OutRow) const;
    bool GetAbilityRowById(const FString& AbilityId, FHuwamAbilityRow& OutRow) const;
    bool HasDuplicates(const TArray<FString>& Values) const;
    FText MakeMissingRowWarning(const FString& Kind, const FString& Id) const;
};
