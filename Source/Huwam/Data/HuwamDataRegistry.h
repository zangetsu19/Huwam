#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HuwamDataTypes.h"
#include "HuwamDataRegistry.generated.h"

UCLASS(BlueprintType)
class HUWAM_API UHuwamDataRegistry : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> ContentPacksTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> RacesTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> SubRacesTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> ClassesTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> JobsTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> StatsTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> SkillsTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> AbilitiesTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> ItemsTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> MaterialsTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> RecipesTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> SpellsTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> MonstersTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> NpcsTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Huwam|Data Tables")
    TObjectPtr<UDataTable> QuestsTable;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetContentPackRow(FName RowName, FHuwamContentPackRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetRaceRow(FName RowName, FHuwamRaceRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetSubRaceRow(FName RowName, FHuwamSubRaceRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetClassRow(FName RowName, FHuwamClassRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetJobRow(FName RowName, FHuwamJobRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetStatRow(FName RowName, FHuwamStatRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetSkillRow(FName RowName, FHuwamSkillRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetAbilityRow(FName RowName, FHuwamAbilityRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetItemRow(FName RowName, FHuwamItemRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetMaterialRow(FName RowName, FHuwamMaterialRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetRecipeRow(FName RowName, FHuwamRecipeRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetSpellRow(FName RowName, FHuwamSpellRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetMonsterRow(FName RowName, FHuwamMonsterRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetNpcRow(FName RowName, FHuwamNpcRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool GetQuestRow(FName RowName, FHuwamQuestRow& OutRow) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    TArray<FName> GetRowNames(const UDataTable* Table) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool LoadMissingTablesFromProjectCsv();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Data")
    int32 GetTotalKnownRowCount() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Data")
    FString GetLastCsvBootstrapSummary() const;

private:
    UPROPERTY(Transient)
    FString LastCsvBootstrapSummary;
};
