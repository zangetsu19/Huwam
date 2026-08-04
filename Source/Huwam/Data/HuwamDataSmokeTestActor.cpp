#include "Data/HuwamDataSmokeTestActor.h"

#include "Data/HuwamDataSubsystem.h"
#include "Engine/Engine.h"

AHuwamDataSmokeTestActor::AHuwamDataSmokeTestActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AHuwamDataSmokeTestActor::BeginPlay()
{
    Super::BeginPlay();

    UHuwamDataRegistry* RegistryToTest = DataRegistry;
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UHuwamDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UHuwamDataSubsystem>())
        {
            if (DataRegistry)
            {
                DataSubsystem->SetDataRegistry(DataRegistry);
            }
            else
            {
                DataSubsystem->EnsureDataRegistryLoaded();
                RegistryToTest = DataSubsystem->GetDataRegistry();
            }
        }
    }

    if (!RegistryToTest)
    {
        UE_LOG(LogTemp, Warning, TEXT("Huwam data smoke test skipped: no active DataRegistry was available."));
        return;
    }

    UE_LOG(LogTemp, Display, TEXT("Huwam data smoke test registry rows: %d"), RegistryToTest->GetTotalKnownRowCount());

    FHuwamRaceRow RaceRow;
    FHuwamSubRaceRow SubRaceRow;
    FHuwamContentPackRow ContentPackRow;
    FHuwamClassRow ClassRow;
    FHuwamItemRow ItemRow;
    FHuwamSpellRow SpellRow;
    FHuwamMonsterRow MonsterRow;
    FHuwamNpcRow NpcRow;
    FHuwamQuestRow QuestRow;

    LogSmokeTestResult(TEXT("Race"), RegistryToTest->GetRaceRow(FName(TEXT("race_human")), RaceRow), RaceRow.DisplayName);
    LogSmokeTestResult(TEXT("Sub-Race"), RegistryToTest->GetSubRaceRow(FName(TEXT("subrace_dwarf_dreamforge")), SubRaceRow), SubRaceRow.DisplayName);
    LogSmokeTestResult(TEXT("Sub-Race"), RegistryToTest->GetSubRaceRow(FName(TEXT("subrace_orc_redtusk")), SubRaceRow), SubRaceRow.DisplayName);
    LogSmokeTestResult(TEXT("Content Pack"), RegistryToTest->GetContentPackRow(FName(TEXT("content_pack_starter_core")), ContentPackRow), ContentPackRow.DisplayName);
    LogSmokeTestResult(TEXT("Class"), RegistryToTest->GetClassRow(FName(TEXT("class_progression_zero")), ClassRow), ClassRow.DisplayName);
    LogSmokeTestResult(TEXT("Item"), RegistryToTest->GetItemRow(FName(TEXT("item_weapon_basic_sword")), ItemRow), ItemRow.DisplayName);
    LogSmokeTestResult(TEXT("Spell"), RegistryToTest->GetSpellRow(FName(TEXT("spell_fire_spark")), SpellRow), SpellRow.DisplayName);
    LogSmokeTestResult(TEXT("Monster"), RegistryToTest->GetMonsterRow(FName(TEXT("monster_slime_basic")), MonsterRow), MonsterRow.DisplayName);
    LogSmokeTestResult(TEXT("NPC"), RegistryToTest->GetNpcRow(FName(TEXT("npc_eldoria_gorth_runkis")), NpcRow), NpcRow.DisplayName);
    LogSmokeTestResult(TEXT("Quest"), RegistryToTest->GetQuestRow(FName(TEXT("quest_eldoria_slimes_in_tall_grass")), QuestRow), QuestRow.DisplayName);
}

void AHuwamDataSmokeTestActor::LogSmokeTestResult(const TCHAR* Label, bool bFound, const FText& DisplayName) const
{
    if (bFound)
    {
        UE_LOG(LogTemp, Display, TEXT("Huwam data smoke test found %s row: %s"), Label, *DisplayName.ToString());
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Huwam data smoke test missing %s row."), Label);
}
