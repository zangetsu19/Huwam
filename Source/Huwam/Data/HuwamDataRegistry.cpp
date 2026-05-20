#include "Data/HuwamDataRegistry.h"

template <typename RowType>
static bool GetHuwamTableRow(const UDataTable* Table, FName RowName, RowType& OutRow)
{
    if (!Table || RowName.IsNone())
    {
        return false;
    }

    const RowType* FoundRow = Table->FindRow<RowType>(RowName, TEXT("HuwamDataRegistry"));
    if (!FoundRow)
    {
        return false;
    }

    OutRow = *FoundRow;
    return true;
}

bool UHuwamDataRegistry::GetRaceRow(FName RowName, FHuwamRaceRow& OutRow) const
{
    return GetHuwamTableRow(RacesTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetContentPackRow(FName RowName, FHuwamContentPackRow& OutRow) const
{
    return GetHuwamTableRow(ContentPacksTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetSubRaceRow(FName RowName, FHuwamSubRaceRow& OutRow) const
{
    return GetHuwamTableRow(SubRacesTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetClassRow(FName RowName, FHuwamClassRow& OutRow) const
{
    return GetHuwamTableRow(ClassesTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetJobRow(FName RowName, FHuwamJobRow& OutRow) const
{
    return GetHuwamTableRow(JobsTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetStatRow(FName RowName, FHuwamStatRow& OutRow) const
{
    return GetHuwamTableRow(StatsTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetSkillRow(FName RowName, FHuwamSkillRow& OutRow) const
{
    return GetHuwamTableRow(SkillsTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetAbilityRow(FName RowName, FHuwamAbilityRow& OutRow) const
{
    return GetHuwamTableRow(AbilitiesTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetItemRow(FName RowName, FHuwamItemRow& OutRow) const
{
    return GetHuwamTableRow(ItemsTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetMaterialRow(FName RowName, FHuwamMaterialRow& OutRow) const
{
    return GetHuwamTableRow(MaterialsTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetRecipeRow(FName RowName, FHuwamRecipeRow& OutRow) const
{
    return GetHuwamTableRow(RecipesTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetSpellRow(FName RowName, FHuwamSpellRow& OutRow) const
{
    return GetHuwamTableRow(SpellsTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetMonsterRow(FName RowName, FHuwamMonsterRow& OutRow) const
{
    return GetHuwamTableRow(MonstersTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetNpcRow(FName RowName, FHuwamNpcRow& OutRow) const
{
    return GetHuwamTableRow(NpcsTable, RowName, OutRow);
}

bool UHuwamDataRegistry::GetQuestRow(FName RowName, FHuwamQuestRow& OutRow) const
{
    return GetHuwamTableRow(QuestsTable, RowName, OutRow);
}

TArray<FName> UHuwamDataRegistry::GetRowNames(const UDataTable* Table) const
{
    if (!Table)
    {
        return {};
    }

    return Table->GetRowNames();
}
