#include "Data/HuwamDataSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogHuwamDataSubsystem, Log, All);

void UHuwamDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    EnsureDataRegistryLoaded();
}

bool UHuwamDataSubsystem::EnsureDataRegistryLoaded()
{
    if (!ActiveDataRegistry && !DefaultDataRegistry.IsNull())
    {
        ActiveDataRegistry = DefaultDataRegistry.LoadSynchronous();
    }

    if (!ActiveDataRegistry)
    {
        ActiveDataRegistry = NewObject<UHuwamDataRegistry>(this, TEXT("RuntimeCsvHuwamDataRegistry"));
    }

    if (!ActiveDataRegistry)
    {
        UE_LOG(LogHuwamDataSubsystem, Warning, TEXT("Huwam data subsystem could not create an active data registry."));
        return false;
    }

    const bool bLoaded = ActiveDataRegistry->LoadMissingTablesFromProjectCsv();
    UE_LOG(LogHuwamDataSubsystem, Display, TEXT("%s"), *ActiveDataRegistry->GetLastCsvBootstrapSummary().Replace(TEXT("\r"), TEXT(" ")).Replace(TEXT("\n"), TEXT(" | ")));
    return bLoaded;
}

void UHuwamDataSubsystem::SetDataRegistry(UHuwamDataRegistry* InDataRegistry)
{
    ActiveDataRegistry = InDataRegistry;
    if (ActiveDataRegistry)
    {
        ActiveDataRegistry->LoadMissingTablesFromProjectCsv();
    }
}

UHuwamDataRegistry* UHuwamDataSubsystem::GetDataRegistry() const
{
    return ActiveDataRegistry;
}

bool UHuwamDataSubsystem::HasDataRegistry() const
{
    return ActiveDataRegistry != nullptr;
}

FString UHuwamDataSubsystem::GetDataBootstrapSummary() const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetLastCsvBootstrapSummary() : FString();
}

bool UHuwamDataSubsystem::GetContentPackRow(FName RowName, FHuwamContentPackRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetContentPackRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetRaceRow(FName RowName, FHuwamRaceRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetRaceRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetSubRaceRow(FName RowName, FHuwamSubRaceRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetSubRaceRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetClassRow(FName RowName, FHuwamClassRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetClassRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetJobRow(FName RowName, FHuwamJobRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetJobRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetStatRow(FName RowName, FHuwamStatRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetStatRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetSkillRow(FName RowName, FHuwamSkillRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetSkillRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetAbilityRow(FName RowName, FHuwamAbilityRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetAbilityRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetItemRow(FName RowName, FHuwamItemRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetItemRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetMaterialRow(FName RowName, FHuwamMaterialRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetMaterialRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetRecipeRow(FName RowName, FHuwamRecipeRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetRecipeRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetSpellRow(FName RowName, FHuwamSpellRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetSpellRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetMonsterRow(FName RowName, FHuwamMonsterRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetMonsterRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetNpcRow(FName RowName, FHuwamNpcRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetNpcRow(RowName, OutRow) : false;
}

bool UHuwamDataSubsystem::GetQuestRow(FName RowName, FHuwamQuestRow& OutRow) const
{
    return ActiveDataRegistry ? ActiveDataRegistry->GetQuestRow(RowName, OutRow) : false;
}
