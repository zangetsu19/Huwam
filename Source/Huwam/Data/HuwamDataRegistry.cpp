#include "Data/HuwamDataRegistry.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY_STATIC(LogHuwamDataRegistry, Log, All);

namespace
{
    struct FHuwamCsvTableSpec
    {
        const TCHAR* Label = TEXT("");
        const TCHAR* FileName = TEXT("");
        UScriptStruct* RowStruct = nullptr;
        TObjectPtr<UDataTable> UHuwamDataRegistry::* TableMember = nullptr;
    };

    TArray<FHuwamCsvTableSpec> BuildHuwamCsvTableSpecs()
    {
        return {
            { TEXT("ContentPacks"), TEXT("DT_ContentPacks.csv"), FHuwamContentPackRow::StaticStruct(), &UHuwamDataRegistry::ContentPacksTable },
            { TEXT("Races"), TEXT("DT_Races.csv"), FHuwamRaceRow::StaticStruct(), &UHuwamDataRegistry::RacesTable },
            { TEXT("SubRaces"), TEXT("DT_SubRaces.csv"), FHuwamSubRaceRow::StaticStruct(), &UHuwamDataRegistry::SubRacesTable },
            { TEXT("Classes"), TEXT("DT_Classes.csv"), FHuwamClassRow::StaticStruct(), &UHuwamDataRegistry::ClassesTable },
            { TEXT("Jobs"), TEXT("DT_Jobs.csv"), FHuwamJobRow::StaticStruct(), &UHuwamDataRegistry::JobsTable },
            { TEXT("Stats"), TEXT("DT_Stats.csv"), FHuwamStatRow::StaticStruct(), &UHuwamDataRegistry::StatsTable },
            { TEXT("Skills"), TEXT("DT_Skills.csv"), FHuwamSkillRow::StaticStruct(), &UHuwamDataRegistry::SkillsTable },
            { TEXT("Abilities"), TEXT("DT_Abilities.csv"), FHuwamAbilityRow::StaticStruct(), &UHuwamDataRegistry::AbilitiesTable },
            { TEXT("Items"), TEXT("DT_Items.csv"), FHuwamItemRow::StaticStruct(), &UHuwamDataRegistry::ItemsTable },
            { TEXT("Materials"), TEXT("DT_Materials.csv"), FHuwamMaterialRow::StaticStruct(), &UHuwamDataRegistry::MaterialsTable },
            { TEXT("Recipes"), TEXT("DT_Recipes.csv"), FHuwamRecipeRow::StaticStruct(), &UHuwamDataRegistry::RecipesTable },
            { TEXT("Spells"), TEXT("DT_Spells.csv"), FHuwamSpellRow::StaticStruct(), &UHuwamDataRegistry::SpellsTable },
            { TEXT("Monsters"), TEXT("DT_Monsters.csv"), FHuwamMonsterRow::StaticStruct(), &UHuwamDataRegistry::MonstersTable },
            { TEXT("NPCs"), TEXT("DT_NPCs.csv"), FHuwamNpcRow::StaticStruct(), &UHuwamDataRegistry::NpcsTable },
            { TEXT("Quests"), TEXT("DT_Quests.csv"), FHuwamQuestRow::StaticStruct(), &UHuwamDataRegistry::QuestsTable }
        };
    }

    int32 GetHuwamRowCount(const UDataTable* Table)
    {
        return Table ? Table->GetRowNames().Num() : 0;
    }
}

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

bool UHuwamDataRegistry::LoadMissingTablesFromProjectCsv()
{
    const TArray<FHuwamCsvTableSpec> Specs = BuildHuwamCsvTableSpecs();
    TArray<FString> SummaryLines;
    int32 ExistingTableCount = 0;
    int32 LoadedTableCount = 0;
    int32 FailedTableCount = 0;
    int32 WarningCount = 0;
    int32 DefaultedColumnCount = 0;

    for (const FHuwamCsvTableSpec& Spec : Specs)
    {
        if (!Spec.RowStruct || !Spec.TableMember)
        {
            ++FailedTableCount;
            SummaryLines.Add(FString::Printf(TEXT("%s: missing row struct setup."), Spec.Label));
            continue;
        }

        TObjectPtr<UDataTable>& TargetTable = this->*(Spec.TableMember);
        if (TargetTable)
        {
            ++ExistingTableCount;
            SummaryLines.Add(FString::Printf(TEXT("%s: using assigned table with %d rows."), Spec.Label, GetHuwamRowCount(TargetTable)));
            continue;
        }

        const FString CsvPath = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Data"), Spec.FileName);
        FString CsvText;
        if (!FFileHelper::LoadFileToString(CsvText, *CsvPath))
        {
            ++FailedTableCount;
            SummaryLines.Add(FString::Printf(TEXT("%s: missing CSV file at %s."), Spec.Label, *CsvPath));
            continue;
        }

        const FString ObjectBaseName = FString::Printf(TEXT("%s_RuntimeCsv"), Spec.Label);
        UDataTable* RuntimeTable = NewObject<UDataTable>(
            this,
            MakeUniqueObjectName(this, UDataTable::StaticClass(), FName(*ObjectBaseName))
        );
        if (!RuntimeTable)
        {
            ++FailedTableCount;
            SummaryLines.Add(FString::Printf(TEXT("%s: could not create runtime DataTable."), Spec.Label));
            continue;
        }

        RuntimeTable->RowStruct = Spec.RowStruct;
        const TArray<FString> ImportProblems = RuntimeTable->CreateTableFromCSVString(CsvText);
        for (const FString& Problem : ImportProblems)
        {
            if (Problem.StartsWith(TEXT("Expected column")) && Problem.Contains(TEXT("not found in input")))
            {
                ++DefaultedColumnCount;
                continue;
            }

            ++WarningCount;
            SummaryLines.Add(FString::Printf(TEXT("%s: CSV import note: %s"), Spec.Label, *Problem));
        }

        const int32 RowCount = GetHuwamRowCount(RuntimeTable);
        if (RowCount <= 0)
        {
            ++FailedTableCount;
            SummaryLines.Add(FString::Printf(TEXT("%s: CSV loaded zero rows from %s."), Spec.Label, Spec.FileName));
            continue;
        }

        TargetTable = RuntimeTable;
        ++LoadedTableCount;
        SummaryLines.Add(FString::Printf(TEXT("%s: loaded %d rows from %s."), Spec.Label, RowCount, Spec.FileName));
    }

    LastCsvBootstrapSummary = FString::Printf(
        TEXT("Huwam data bootstrap: %d loaded, %d assigned, %d failed, %d notes, %d defaulted columns, %d total rows."),
        LoadedTableCount,
        ExistingTableCount,
        FailedTableCount,
        WarningCount,
        DefaultedColumnCount,
        GetTotalKnownRowCount()
    );

    if (DefaultedColumnCount > 0)
    {
        SummaryLines.Insert(FString::Printf(TEXT("%d omitted optional CSV columns used default values."), DefaultedColumnCount), 0);
    }

    if (!SummaryLines.IsEmpty())
    {
        LastCsvBootstrapSummary += LINE_TERMINATOR;
        LastCsvBootstrapSummary += FString::Join(SummaryLines, LINE_TERMINATOR);
    }

    const FString SingleLineSummary = LastCsvBootstrapSummary.Replace(TEXT("\r"), TEXT(" ")).Replace(TEXT("\n"), TEXT(" | "));
    if (FailedTableCount > 0)
    {
        UE_LOG(LogHuwamDataRegistry, Warning, TEXT("%s"), *SingleLineSummary);
        return false;
    }

    UE_LOG(LogHuwamDataRegistry, Display, TEXT("%s"), *SingleLineSummary);
    return true;
}

int32 UHuwamDataRegistry::GetTotalKnownRowCount() const
{
    int32 TotalRows = 0;
    for (const FHuwamCsvTableSpec& Spec : BuildHuwamCsvTableSpecs())
    {
        const TObjectPtr<UDataTable>& Table = this->*(Spec.TableMember);
        TotalRows += GetHuwamRowCount(Table);
    }

    return TotalRows;
}

FString UHuwamDataRegistry::GetLastCsvBootstrapSummary() const
{
    return LastCsvBootstrapSummary;
}
