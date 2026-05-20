#include "Data/HuwamDataSubsystem.h"

void UHuwamDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (!DefaultDataRegistry.IsNull())
    {
        ActiveDataRegistry = DefaultDataRegistry.LoadSynchronous();
    }
}

void UHuwamDataSubsystem::SetDataRegistry(UHuwamDataRegistry* InDataRegistry)
{
    ActiveDataRegistry = InDataRegistry;
}

UHuwamDataRegistry* UHuwamDataSubsystem::GetDataRegistry() const
{
    return ActiveDataRegistry;
}

bool UHuwamDataSubsystem::HasDataRegistry() const
{
    return ActiveDataRegistry != nullptr;
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
