#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HuwamDataRegistry.h"
#include "HuwamDataSubsystem.generated.h"

UCLASS(BlueprintType, Config=Game)
class HUWAM_API UHuwamDataSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    void SetDataRegistry(UHuwamDataRegistry* InDataRegistry);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Data")
    bool EnsureDataRegistryLoaded();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Data")
    UHuwamDataRegistry* GetDataRegistry() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Data")
    bool HasDataRegistry() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Data")
    FString GetDataBootstrapSummary() const;

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

private:
    UPROPERTY(Config, EditAnywhere, Category = "Huwam|Data")
    TSoftObjectPtr<UHuwamDataRegistry> DefaultDataRegistry;

    UPROPERTY(Transient)
    TObjectPtr<UHuwamDataRegistry> ActiveDataRegistry;
};
