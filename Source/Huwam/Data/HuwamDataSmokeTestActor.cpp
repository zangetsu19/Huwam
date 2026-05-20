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

    if (!DataRegistry)
    {
        UE_LOG(LogTemp, Warning, TEXT("Huwam data smoke test skipped: no DataRegistry assigned."));
        return;
    }

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UHuwamDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UHuwamDataSubsystem>())
        {
            DataSubsystem->SetDataRegistry(DataRegistry);
        }
    }

    FHuwamRaceRow RaceRow;
    FHuwamContentPackRow ContentPackRow;
    FHuwamClassRow ClassRow;
    FHuwamItemRow ItemRow;
    FHuwamSpellRow SpellRow;
    FHuwamMonsterRow MonsterRow;
    FHuwamNpcRow NpcRow;
    FHuwamQuestRow QuestRow;

    LogSmokeTestResult(TEXT("Race"), DataRegistry->GetRaceRow(FName(TEXT("race_human")), RaceRow), RaceRow.DisplayName);
    LogSmokeTestResult(TEXT("Content Pack"), DataRegistry->GetContentPackRow(FName(TEXT("content_pack_starter_core")), ContentPackRow), ContentPackRow.DisplayName);
    LogSmokeTestResult(TEXT("Class"), DataRegistry->GetClassRow(FName(TEXT("class_progression_zero")), ClassRow), ClassRow.DisplayName);
    LogSmokeTestResult(TEXT("Item"), DataRegistry->GetItemRow(FName(TEXT("item_weapon_basic_sword")), ItemRow), ItemRow.DisplayName);
    LogSmokeTestResult(TEXT("Spell"), DataRegistry->GetSpellRow(FName(TEXT("spell_fire_spark")), SpellRow), SpellRow.DisplayName);
    LogSmokeTestResult(TEXT("Monster"), DataRegistry->GetMonsterRow(FName(TEXT("monster_slime_basic")), MonsterRow), MonsterRow.DisplayName);
    LogSmokeTestResult(TEXT("NPC"), DataRegistry->GetNpcRow(FName(TEXT("npc_eldoria_gorth_runkis")), NpcRow), NpcRow.DisplayName);
    LogSmokeTestResult(TEXT("Quest"), DataRegistry->GetQuestRow(FName(TEXT("quest_eldoria_slimes_in_tall_grass")), QuestRow), QuestRow.DisplayName);
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
