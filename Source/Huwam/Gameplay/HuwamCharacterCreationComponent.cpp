#include "Gameplay/HuwamCharacterCreationComponent.h"

#include "Data/HuwamDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Gameplay/HuwamCharacterStatsComponent.h"

namespace
{
    const TCHAR* StatStrength = TEXT("stat.strength");
    const TCHAR* StatCharisma = TEXT("stat.charisma");
    const TCHAR* StatIntelligence = TEXT("stat.intelligence");
    const TCHAR* StatWisdom = TEXT("stat.wisdom");
    const TCHAR* StatDexterity = TEXT("stat.dexterity");
    const TCHAR* StatLuck = TEXT("stat.luck");
    const TCHAR* StatHealth = TEXT("stat.health");
    const TCHAR* StatMana = TEXT("stat.mana");
    const TCHAR* StatDefense = TEXT("stat.defense");
    const TCHAR* StatWorldPopularity = TEXT("stat.world_popularity");

    FName GetRowNameFromId(const FString& Id)
    {
        return FName(*Id.Replace(TEXT("."), TEXT("_")));
    }

    FText MakeSimpleDisplayName(const FString& Id)
    {
        FString DisplayName = Id;
        const int32 DotIndex = DisplayName.Find(TEXT("."), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
        if (DotIndex != INDEX_NONE)
        {
            DisplayName = DisplayName.RightChop(DotIndex + 1);
        }

        DisplayName.ReplaceInline(TEXT("_"), TEXT(" "));
        DisplayName.ReplaceInline(TEXT("."), TEXT(" "));
        return FText::FromString(DisplayName);
    }

    FString GetPrimaryRaceId(const FString& RaceId)
    {
        if (RaceId.StartsWith(TEXT("race.elf.")))
        {
            return TEXT("race.elf");
        }

        if (RaceId.StartsWith(TEXT("race.dwarf.")))
        {
            return TEXT("race.dwarf");
        }

        if (RaceId.StartsWith(TEXT("race.orc.")))
        {
            return TEXT("race.orc");
        }

        if (RaceId.StartsWith(TEXT("race.beastfolk.")))
        {
            return TEXT("race.beastfolk");
        }

        if (RaceId.StartsWith(TEXT("race.fae.")))
        {
            return TEXT("race.fae");
        }

        return RaceId;
    }
}

UHuwamCharacterCreationComponent::UHuwamCharacterCreationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

FHuwamCharacterCreationRequest UHuwamCharacterCreationComponent::BuildDefaultCharacterCreationRequest() const
{
    FHuwamCharacterCreationRequest Request;
    Request.CharacterName = FText::FromString(TEXT("New Huwam Hero"));
    Request.CreationPath = EHuwamCharacterCreationPath::Normal;
    Request.RaceId = TEXT("race.human");
    Request.SubRaceId = TEXT("race.human.common");
    Request.ClassIds = { TEXT("class.progression_zero") };
    Request.JobIds = { TEXT("job.adventurer") };
    Request.SkillIds = {
        TEXT("skill.basic_weapon_handling"),
        TEXT("skill.gathering"),
        TEXT("skill.woodworking"),
        TEXT("skill.tanning"),
        TEXT("skill.sewing")
    };
    Request.AbilityIds = {
        TEXT("ability.born_leader"),
        TEXT("ability.indomitable"),
        TEXT("ability.quick_hands"),
        TEXT("ability.natural_gatherer"),
        TEXT("ability.stubborn_survivor")
    };

    return Request;
}

int32 UHuwamCharacterCreationComponent::GetSkillLimitForPath(EHuwamCharacterCreationPath CreationPath) const
{
    return CreationPath == EHuwamCharacterCreationPath::Reincarnated ? ReincarnatedSkillLimit : NormalSkillLimit;
}

int32 UHuwamCharacterCreationComponent::GetAbilityLimitForPath(EHuwamCharacterCreationPath CreationPath) const
{
    return CreationPath == EHuwamCharacterCreationPath::Reincarnated ? ReincarnatedAbilityLimit : NormalAbilityLimit;
}

bool UHuwamCharacterCreationComponent::ValidateCharacterCreation(const FHuwamCharacterCreationRequest& Request, FHuwamCharacterCreationResult& OutResult) const
{
    OutResult = FHuwamCharacterCreationResult();

    if (Request.RaceId.IsEmpty())
    {
        OutResult.Errors.Add(FText::FromString(TEXT("A race must be selected.")));
    }

    if (Request.ClassIds.IsEmpty())
    {
        OutResult.Errors.Add(FText::FromString(TEXT("At least one class must be selected.")));
    }

    if (Request.ClassIds.Num() > MaxClassSlots)
    {
        OutResult.Errors.Add(FText::Format(
            FText::FromString(TEXT("A character can only hold {0} class slots.")),
            FText::AsNumber(MaxClassSlots)
        ));
    }

    if (Request.JobIds.Num() > MaxJobSlots)
    {
        OutResult.Errors.Add(FText::Format(
            FText::FromString(TEXT("A character can only hold {0} job slots.")),
            FText::AsNumber(MaxJobSlots)
        ));
    }

    const int32 SkillLimit = GetSkillLimitForPath(Request.CreationPath);
    if (Request.SkillIds.Num() > SkillLimit)
    {
        OutResult.Errors.Add(FText::Format(
            FText::FromString(TEXT("This creation path only allows {0} permanent creation skills.")),
            FText::AsNumber(SkillLimit)
        ));
    }

    const int32 AbilityLimit = GetAbilityLimitForPath(Request.CreationPath);
    if (Request.AbilityIds.Num() > AbilityLimit)
    {
        OutResult.Errors.Add(FText::Format(
            FText::FromString(TEXT("This creation path only allows {0} permanent creation abilities.")),
            FText::AsNumber(AbilityLimit)
        ));
    }

    if (HasDuplicates(Request.ClassIds))
    {
        OutResult.Errors.Add(FText::FromString(TEXT("Class choices cannot contain duplicates.")));
    }

    if (HasDuplicates(Request.JobIds))
    {
        OutResult.Errors.Add(FText::FromString(TEXT("Job choices cannot contain duplicates.")));
    }

    if (HasDuplicates(Request.SkillIds))
    {
        OutResult.Errors.Add(FText::FromString(TEXT("Skill choices cannot contain duplicates.")));
    }

    if (HasDuplicates(Request.AbilityIds))
    {
        OutResult.Errors.Add(FText::FromString(TEXT("Ability choices cannot contain duplicates.")));
    }

    const bool bCanCheckRows = HasDataRegistry();
    if (!bCanCheckRows)
    {
        OutResult.Warnings.Add(FText::FromString(TEXT("No Huwam data registry is active, so row existence checks were skipped.")));
    }
    else
    {
        FHuwamRaceRow RaceRow;
        if (!Request.RaceId.IsEmpty() && !GetRaceRowById(Request.RaceId, RaceRow))
        {
            OutResult.Errors.Add(FText::Format(
                FText::FromString(TEXT("Unknown race id: {0}")),
                FText::FromString(Request.RaceId)
            ));
        }

        FHuwamSubRaceRow SubRaceRow;
        if (!Request.SubRaceId.IsEmpty())
        {
            if (!GetSubRaceRowById(Request.SubRaceId, SubRaceRow))
            {
                OutResult.Errors.Add(FText::Format(
                    FText::FromString(TEXT("Unknown sub-race id: {0}")),
                    FText::FromString(Request.SubRaceId)
                ));
            }
            else if (!Request.RaceId.IsEmpty() && SubRaceRow.ParentRaceId != Request.RaceId)
            {
                OutResult.Errors.Add(FText::Format(
                    FText::FromString(TEXT("{0} belongs to {1}, not {2}.")),
                    FText::FromString(Request.SubRaceId),
                    FText::FromString(SubRaceRow.ParentRaceId),
                    FText::FromString(Request.RaceId)
                ));
            }
        }

        for (const FString& ClassId : Request.ClassIds)
        {
            FHuwamClassRow ClassRow;
            if (!GetClassRowById(ClassId, ClassRow))
            {
                OutResult.Errors.Add(FText::Format(
                    FText::FromString(TEXT("Unknown class id: {0}")),
                    FText::FromString(ClassId)
                ));
            }
        }

        for (const FString& JobId : Request.JobIds)
        {
            FHuwamJobRow JobRow;
            if (!GetJobRowById(JobId, JobRow))
            {
                OutResult.Errors.Add(FText::Format(
                    FText::FromString(TEXT("Unknown job id: {0}")),
                    FText::FromString(JobId)
                ));
            }
        }

        for (const FString& SkillId : Request.SkillIds)
        {
            FHuwamSkillRow SkillRow;
            if (!GetSkillRowById(SkillId, SkillRow))
            {
                OutResult.Errors.Add(FText::Format(
                    FText::FromString(TEXT("Unknown skill id: {0}")),
                    FText::FromString(SkillId)
                ));
            }
            else if (!SkillRow.bPermanentAtCreation)
            {
                OutResult.Warnings.Add(FText::Format(
                    FText::FromString(TEXT("{0} is not normally marked as a permanent character creation skill.")),
                    SkillRow.DisplayName
                ));
            }
        }

        for (const FString& AbilityId : Request.AbilityIds)
        {
            FHuwamAbilityRow AbilityRow;
            if (!GetAbilityRowById(AbilityId, AbilityRow))
            {
                OutResult.Errors.Add(FText::Format(
                    FText::FromString(TEXT("Unknown ability id: {0}")),
                    FText::FromString(AbilityId)
                ));
            }
            else if (!AbilityRow.bPermanentAtCreation)
            {
                OutResult.Warnings.Add(FText::Format(
                    FText::FromString(TEXT("{0} is not normally marked as a permanent character creation ability.")),
                    AbilityRow.DisplayName
                ));
            }
        }
    }

    if (OutResult.Errors.IsEmpty())
    {
        TArray<FText> StateWarnings;
        OutResult.bSuccess = BuildCreatedCharacterState(Request, OutResult.CharacterState, StateWarnings);
        OutResult.Warnings.Append(StateWarnings);
    }

    return OutResult.Errors.IsEmpty();
}

bool UHuwamCharacterCreationComponent::ApplyCharacterCreation(const FHuwamCharacterCreationRequest& Request, UHuwamCharacterStatsComponent* TargetStats, FHuwamCharacterCreationResult& OutResult)
{
    if (!ValidateCharacterCreation(Request, OutResult))
    {
        return false;
    }

    CreatedCharacterState = OutResult.CharacterState;
    bHasCreatedCharacter = true;
    ApplyStartingStatBonuses(CreatedCharacterState, TargetStats);
    OnCharacterCreated.Broadcast(OutResult);
    return true;
}

void UHuwamCharacterCreationComponent::ResetCreatedCharacter()
{
    CreatedCharacterState = FHuwamCreatedCharacterState();
    bHasCreatedCharacter = false;
}

bool UHuwamCharacterCreationComponent::HasCreatedCharacter() const
{
    return bHasCreatedCharacter;
}

FHuwamCreatedCharacterState UHuwamCharacterCreationComponent::GetCreatedCharacterState() const
{
    return CreatedCharacterState;
}

bool UHuwamCharacterCreationComponent::BuildCreatedCharacterState(const FHuwamCharacterCreationRequest& Request, FHuwamCreatedCharacterState& OutState, TArray<FText>& OutWarnings) const
{
    OutState = FHuwamCreatedCharacterState();
    OutState.CharacterName = Request.CharacterName.IsEmpty() ? FText::FromString(TEXT("New Huwam Hero")) : Request.CharacterName;
    OutState.CreationPath = Request.CreationPath;
    OutState.RaceId = Request.RaceId;
    OutState.SubRaceId = Request.SubRaceId;

    FHuwamRaceRow RaceRow;
    const bool bHasRaceRow = GetRaceRowById(Request.RaceId, RaceRow);
    OutState.RaceName = bHasRaceRow ? RaceRow.DisplayName : MakeSimpleDisplayName(Request.RaceId);

    FHuwamSubRaceRow SubRaceRow;
    const bool bHasSubRaceRow = !Request.SubRaceId.IsEmpty() && GetSubRaceRowById(Request.SubRaceId, SubRaceRow);
    OutState.SubRaceName = bHasSubRaceRow ? SubRaceRow.DisplayName : MakeSimpleDisplayName(Request.SubRaceId);
    OutState.ManaWellDepth = ResolveManaWellDepth(bHasRaceRow ? &RaceRow : nullptr, bHasSubRaceRow ? &SubRaceRow : nullptr);

    for (const FString& ClassId : Request.ClassIds)
    {
        FHuwamCreatedClassState ClassState;
        ClassState.ClassId = ClassId;

        FHuwamClassRow ClassRow;
        if (GetClassRowById(ClassId, ClassRow))
        {
            ClassState.DisplayName = ClassRow.DisplayName;
            ClassState.MaxLevel = ClassRow.MaxLevel;
            ClassState.GuildId = ClassRow.GuildId;
        }
        else
        {
            ClassState.DisplayName = MakeSimpleDisplayName(ClassId);
            OutWarnings.Add(MakeMissingRowWarning(TEXT("class"), ClassId));
        }

        OutState.Classes.Add(ClassState);
    }

    for (const FString& JobId : Request.JobIds)
    {
        FHuwamCreatedJobState JobState;
        JobState.JobId = JobId;
        JobState.RankName = FText::FromString(TEXT("Novice"));

        FHuwamJobRow JobRow;
        if (GetJobRowById(JobId, JobRow))
        {
            JobState.DisplayName = JobRow.DisplayName;
            JobState.MinSalaryGold = JobRow.MinSalaryGold;
            JobState.MaxSalaryGold = JobRow.MaxSalaryGold;
            JobState.AssociatedGuildId = JobRow.AssociatedGuildId;
            JobState.DiscountResourceIds = JobRow.DiscountResourceIds;
        }
        else
        {
            JobState.DisplayName = MakeSimpleDisplayName(JobId);
            OutWarnings.Add(MakeMissingRowWarning(TEXT("job"), JobId));
        }

        OutState.Jobs.Add(JobState);
    }

    for (const FString& SkillId : Request.SkillIds)
    {
        FHuwamCreatedSkillState SkillState;
        SkillState.SkillId = SkillId;

        FHuwamSkillRow SkillRow;
        if (GetSkillRowById(SkillId, SkillRow))
        {
            SkillState.DisplayName = SkillRow.DisplayName;
            SkillState.Category = SkillRow.Category;
            SkillState.bPermanentAtCreation = true;
        }
        else
        {
            SkillState.DisplayName = MakeSimpleDisplayName(SkillId);
            SkillState.Category = FText::FromString(TEXT("Unknown"));
            OutWarnings.Add(MakeMissingRowWarning(TEXT("skill"), SkillId));
        }

        OutState.PermanentSkills.Add(SkillState);
    }

    for (const FString& AbilityId : Request.AbilityIds)
    {
        FHuwamCreatedAbilityState AbilityState;
        AbilityState.AbilityId = AbilityId;

        FHuwamAbilityRow AbilityRow;
        if (GetAbilityRowById(AbilityId, AbilityRow))
        {
            AbilityState.DisplayName = AbilityRow.DisplayName;
            AbilityState.AbilityType = AbilityRow.AbilityType;
            AbilityState.bPermanentAtCreation = true;
        }
        else
        {
            AbilityState.DisplayName = MakeSimpleDisplayName(AbilityId);
            AbilityState.AbilityType = FText::FromString(TEXT("Unknown"));
            OutWarnings.Add(MakeMissingRowWarning(TEXT("ability"), AbilityId));
        }

        OutState.PermanentAbilities.Add(AbilityState);
    }

    BuildStartingStatBonuses(Request, OutState.StartingStatBonuses);
    return true;
}

void UHuwamCharacterCreationComponent::ApplyStartingStatBonuses(const FHuwamCreatedCharacterState& State, UHuwamCharacterStatsComponent* TargetStats) const
{
    if (!TargetStats)
    {
        return;
    }

    TargetStats->ResetToProgressionZeroDefaults();
    for (const TPair<FString, int32>& Pair : State.StartingStatBonuses)
    {
        TargetStats->SetPermanentStatBonus(Pair.Key, Pair.Value);
    }
}

void UHuwamCharacterCreationComponent::BuildStartingStatBonuses(const FHuwamCharacterCreationRequest& Request, TMap<FString, int32>& OutBonuses) const
{
    OutBonuses.Reset();

    AddRaceStatBonuses(Request.RaceId, OutBonuses);
    AddSubRaceStatBonuses(Request.SubRaceId, OutBonuses);

    for (const FString& ClassId : Request.ClassIds)
    {
        AddClassStatBonuses(ClassId, OutBonuses);
    }

    for (const FString& JobId : Request.JobIds)
    {
        AddJobStatBonuses(JobId, OutBonuses);
    }

    for (const FString& SkillId : Request.SkillIds)
    {
        AddSkillStatBonuses(SkillId, OutBonuses);
    }

    for (const FString& AbilityId : Request.AbilityIds)
    {
        AddAbilityStatBonuses(AbilityId, OutBonuses);
    }

    if (Request.CreationPath == EHuwamCharacterCreationPath::Reincarnated)
    {
        AddStatBonus(OutBonuses, StatLuck, 5);
        AddStatBonus(OutBonuses, StatWorldPopularity, 5);
    }
}

void UHuwamCharacterCreationComponent::AddRaceStatBonuses(const FString& RaceId, TMap<FString, int32>& OutBonuses) const
{
    const FString PrimaryRaceId = GetPrimaryRaceId(RaceId);
    if (PrimaryRaceId == TEXT("race.human"))
    {
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatCharisma, 1);
        AddStatBonus(OutBonuses, StatIntelligence, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (PrimaryRaceId == TEXT("race.elf"))
    {
        AddStatBonus(OutBonuses, StatIntelligence, 2);
        AddStatBonus(OutBonuses, StatMana, 2);
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (PrimaryRaceId == TEXT("race.dwarf"))
    {
        AddStatBonus(OutBonuses, StatStrength, 2);
        AddStatBonus(OutBonuses, StatDefense, 3);
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (PrimaryRaceId == TEXT("race.orc"))
    {
        AddStatBonus(OutBonuses, StatStrength, 3);
        AddStatBonus(OutBonuses, StatDefense, 2);
        AddStatBonus(OutBonuses, StatHealth, 1);
        AddStatBonus(OutBonuses, StatIntelligence, -1);
    }
    else if (PrimaryRaceId == TEXT("race.demon"))
    {
        AddStatBonus(OutBonuses, StatMana, 3);
        AddStatBonus(OutBonuses, StatCharisma, 2);
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatWorldPopularity, -2);
    }
    else if (PrimaryRaceId == TEXT("race.beastfolk"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 2);
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatLuck, 1);
        AddStatBonus(OutBonuses, StatHealth, 1);
    }
    else if (PrimaryRaceId == TEXT("race.fae"))
    {
        AddStatBonus(OutBonuses, StatMana, 3);
        AddStatBonus(OutBonuses, StatWisdom, 2);
        AddStatBonus(OutBonuses, StatLuck, 1);
        AddStatBonus(OutBonuses, StatHealth, -1);
    }
    else if (PrimaryRaceId == TEXT("race.halfling"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 2);
        AddStatBonus(OutBonuses, StatLuck, 2);
        AddStatBonus(OutBonuses, StatCharisma, 1);
        AddStatBonus(OutBonuses, StatStrength, -1);
    }
}

void UHuwamCharacterCreationComponent::AddSubRaceStatBonuses(const FString& SubRaceId, TMap<FString, int32>& OutBonuses) const
{
    if (SubRaceId == TEXT("race.human.high"))
    {
        AddStatBonus(OutBonuses, StatCharisma, 2);
        AddStatBonus(OutBonuses, StatIntelligence, 1);
        AddStatBonus(OutBonuses, StatMana, 1);
    }
    else if (SubRaceId == TEXT("race.human.dawn"))
    {
        AddStatBonus(OutBonuses, StatWisdom, 2);
        AddStatBonus(OutBonuses, StatMana, 1);
        AddStatBonus(OutBonuses, StatCharisma, 1);
    }
    else if (SubRaceId == TEXT("race.human.dusk"))
    {
        AddStatBonus(OutBonuses, StatDefense, 2);
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (SubRaceId == TEXT("race.human.frontier"))
    {
        AddStatBonus(OutBonuses, StatWisdom, 1);
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatHealth, 1);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (SubRaceId == TEXT("race.human.urban"))
    {
        AddStatBonus(OutBonuses, StatCharisma, 2);
        AddStatBonus(OutBonuses, StatLuck, 1);
        AddStatBonus(OutBonuses, StatIntelligence, 1);
    }
    else if (SubRaceId == TEXT("race.human.warborn"))
    {
        AddStatBonus(OutBonuses, StatStrength, 2);
        AddStatBonus(OutBonuses, StatDefense, 1);
        AddStatBonus(OutBonuses, StatHealth, 1);
        AddStatBonus(OutBonuses, StatIntelligence, -1);
    }
    else if (SubRaceId == TEXT("race.human.blessed"))
    {
        AddStatBonus(OutBonuses, StatMana, 2);
        AddStatBonus(OutBonuses, StatWisdom, 2);
        AddStatBonus(OutBonuses, StatWorldPopularity, 1);
    }
    else if (SubRaceId == TEXT("race.human.cursed"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 2);
        AddStatBonus(OutBonuses, StatLuck, 1);
        AddStatBonus(OutBonuses, StatMana, 1);
        AddStatBonus(OutBonuses, StatCharisma, -1);
    }
    else if (SubRaceId == TEXT("race.human.half_elf") || SubRaceId == TEXT("race.elf.half"))
    {
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatCharisma, 1);
    }
    else if (SubRaceId == TEXT("race.elf.dark"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatIntelligence, 1);
        AddStatBonus(OutBonuses, StatMana, 1);
        AddStatBonus(OutBonuses, StatCharisma, -1);
    }
    else if (SubRaceId == TEXT("race.elf.high"))
    {
        AddStatBonus(OutBonuses, StatIntelligence, 2);
        AddStatBonus(OutBonuses, StatCharisma, 1);
    }
    else if (SubRaceId == TEXT("race.dwarf.mountain"))
    {
        AddStatBonus(OutBonuses, StatDefense, 1);
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (SubRaceId == TEXT("race.dwarf.forge"))
    {
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatMana, 1);
    }
    else if (SubRaceId == TEXT("race.dwarf.quarry"))
    {
        AddStatBonus(OutBonuses, StatDefense, 1);
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatCharisma, 1);
    }
    else if (SubRaceId == TEXT("race.dwarf.deep"))
    {
        AddStatBonus(OutBonuses, StatDefense, 1);
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (SubRaceId == TEXT("race.dwarf.goldbeard"))
    {
        AddStatBonus(OutBonuses, StatCharisma, 2);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (SubRaceId == TEXT("race.dwarf.iron_oath"))
    {
        AddStatBonus(OutBonuses, StatDefense, 2);
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (SubRaceId == TEXT("race.dwarf.dreamforge"))
    {
        AddStatBonus(OutBonuses, StatMana, 2);
        AddStatBonus(OutBonuses, StatIntelligence, 1);
    }
    else if (SubRaceId == TEXT("race.dwarf.hearth"))
    {
        AddStatBonus(OutBonuses, StatCharisma, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (SubRaceId == TEXT("race.orc.common"))
    {
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatDefense, 1);
        AddStatBonus(OutBonuses, StatHealth, 1);
        AddStatBonus(OutBonuses, StatIntelligence, -1);
    }
    else if (SubRaceId == TEXT("race.orc.half"))
    {
        AddStatBonus(OutBonuses, StatStrength, 2);
        AddStatBonus(OutBonuses, StatDefense, 1);
        AddStatBonus(OutBonuses, StatCharisma, 1);
    }
    else if (SubRaceId == TEXT("race.orc.war"))
    {
        AddStatBonus(OutBonuses, StatStrength, 2);
        AddStatBonus(OutBonuses, StatHealth, 1);
        AddStatBonus(OutBonuses, StatIntelligence, -1);
    }
    else if (SubRaceId == TEXT("race.orc.stonehide"))
    {
        AddStatBonus(OutBonuses, StatDefense, 3);
        AddStatBonus(OutBonuses, StatDexterity, -1);
    }
    else if (SubRaceId == TEXT("race.orc.redtusk"))
    {
        AddStatBonus(OutBonuses, StatStrength, 2);
        AddStatBonus(OutBonuses, StatHealth, 1);
        AddStatBonus(OutBonuses, StatCharisma, -1);
    }
    else if (SubRaceId == TEXT("race.orc.gray"))
    {
        AddStatBonus(OutBonuses, StatIntelligence, 1);
        AddStatBonus(OutBonuses, StatWisdom, 2);
    }
    else if (SubRaceId == TEXT("race.orc.ironbound"))
    {
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatDefense, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (SubRaceId == TEXT("race.orc.wild"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 2);
        AddStatBonus(OutBonuses, StatLuck, 1);
        AddStatBonus(OutBonuses, StatIntelligence, -1);
    }
    else if (SubRaceId == TEXT("race.beastfolk.panthera"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 2);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (SubRaceId == TEXT("race.fae.faun"))
    {
        AddStatBonus(OutBonuses, StatCharisma, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
        AddStatBonus(OutBonuses, StatDexterity, 1);
    }
}

void UHuwamCharacterCreationComponent::AddClassStatBonuses(const FString& ClassId, TMap<FString, int32>& OutBonuses) const
{
    if (ClassId == TEXT("class.progression_rank_1"))
    {
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatCharisma, 1);
        AddStatBonus(OutBonuses, StatIntelligence, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (ClassId == TEXT("class.mage"))
    {
        AddStatBonus(OutBonuses, StatMana, 2);
        AddStatBonus(OutBonuses, StatIntelligence, 2);
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (ClassId == TEXT("class.archer"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 2);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (ClassId == TEXT("class.knight"))
    {
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatDefense, 2);
    }
    else if (ClassId == TEXT("class.priest"))
    {
        AddStatBonus(OutBonuses, StatWisdom, 2);
        AddStatBonus(OutBonuses, StatMana, 1);
    }
    else if (ClassId == TEXT("class.thief"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 2);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (ClassId == TEXT("class.adventurer_rank_f"))
    {
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
}

void UHuwamCharacterCreationComponent::AddJobStatBonuses(const FString& JobId, TMap<FString, int32>& OutBonuses) const
{
    if (JobId == TEXT("job.builder"))
    {
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatDefense, 1);
    }
    else if (JobId == TEXT("job.adventurer"))
    {
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (JobId == TEXT("job.hunter_gatherer"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (JobId == TEXT("job.blacksmith"))
    {
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatDefense, 1);
    }
    else if (JobId == TEXT("job.merchant"))
    {
        AddStatBonus(OutBonuses, StatCharisma, 2);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (JobId == TEXT("job.officer"))
    {
        AddStatBonus(OutBonuses, StatCharisma, 1);
        AddStatBonus(OutBonuses, StatDefense, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (JobId == TEXT("job.tavern_owner"))
    {
        AddStatBonus(OutBonuses, StatCharisma, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (JobId == TEXT("job.farmer"))
    {
        AddStatBonus(OutBonuses, StatHealth, 1);
        AddStatBonus(OutBonuses, StatStrength, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (JobId == TEXT("job.fencer"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 2);
        AddStatBonus(OutBonuses, StatCharisma, 1);
    }
    else if (JobId == TEXT("job.dungeon_explorer"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatDefense, 1);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (JobId == TEXT("job.craftsman"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatIntelligence, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (JobId == TEXT("job.baker"))
    {
        AddStatBonus(OutBonuses, StatWisdom, 1);
        AddStatBonus(OutBonuses, StatCharisma, 1);
    }
    else if (JobId == TEXT("job.contractor"))
    {
        AddStatBonus(OutBonuses, StatCharisma, 1);
        AddStatBonus(OutBonuses, StatIntelligence, 1);
    }
}

void UHuwamCharacterCreationComponent::AddSkillStatBonuses(const FString& SkillId, TMap<FString, int32>& OutBonuses) const
{
    FHuwamSkillRow SkillRow;
    if (GetSkillRowById(SkillId, SkillRow))
    {
        for (const FString& StatId : SkillRow.GoverningStatIds)
        {
            AddStatBonus(OutBonuses, StatId, 1);
        }
    }

    if (SkillId == TEXT("skill.smithing"))
    {
        AddStatBonus(OutBonuses, StatStrength, 1);
    }
    else if (SkillId == TEXT("skill.tanning") || SkillId == TEXT("skill.sewing") || SkillId == TEXT("skill.woodworking") || SkillId == TEXT("skill.bowyer") || SkillId == TEXT("skill.fletcher"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 1);
    }
    else if (SkillId == TEXT("skill.alchemy") || SkillId == TEXT("skill.runecrafting"))
    {
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (SkillId == TEXT("skill.basic_weapon_handling"))
    {
        AddStatBonus(OutBonuses, StatStrength, 1);
    }
    else if (SkillId == TEXT("skill.gathering"))
    {
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
}

void UHuwamCharacterCreationComponent::AddAbilityStatBonuses(const FString& AbilityId, TMap<FString, int32>& OutBonuses) const
{
    if (AbilityId == TEXT("ability.born_leader"))
    {
        AddStatBonus(OutBonuses, StatCharisma, 2);
    }
    else if (AbilityId == TEXT("ability.indomitable"))
    {
        AddStatBonus(OutBonuses, StatDefense, 2);
    }
    else if (AbilityId == TEXT("ability.quick_hands"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (AbilityId == TEXT("ability.mana_sense"))
    {
        AddStatBonus(OutBonuses, StatMana, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (AbilityId == TEXT("ability.natural_gatherer"))
    {
        AddStatBonus(OutBonuses, StatLuck, 1);
        AddStatBonus(OutBonuses, StatWisdom, 1);
    }
    else if (AbilityId == TEXT("ability.steady_aim"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 2);
    }
    else if (AbilityId == TEXT("ability.stubborn_survivor"))
    {
        AddStatBonus(OutBonuses, StatHealth, 2);
    }
    else if (AbilityId == TEXT("ability.trade_instinct"))
    {
        AddStatBonus(OutBonuses, StatCharisma, 1);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (AbilityId == TEXT("ability.soft_step"))
    {
        AddStatBonus(OutBonuses, StatDexterity, 1);
        AddStatBonus(OutBonuses, StatLuck, 1);
    }
    else if (AbilityId == TEXT("ability.world_touched"))
    {
        AddStatBonus(OutBonuses, StatWorldPopularity, 2);
    }
}

void UHuwamCharacterCreationComponent::AddStatBonus(TMap<FString, int32>& OutBonuses, const FString& StatId, int32 Delta) const
{
    if (StatId.IsEmpty() || Delta == 0)
    {
        return;
    }

    OutBonuses.FindOrAdd(StatId) += Delta;
}

EHuwamManaWellDepth UHuwamCharacterCreationComponent::ResolveManaWellDepth(const FHuwamRaceRow* RaceRow, const FHuwamSubRaceRow* SubRaceRow) const
{
    if (SubRaceRow && SubRaceRow->ManaWellModifier != EHuwamManaWellDepth::Average)
    {
        return SubRaceRow->ManaWellModifier;
    }

    return RaceRow ? RaceRow->ManaWellBaseline : EHuwamManaWellDepth::Average;
}

UHuwamDataSubsystem* UHuwamCharacterCreationComponent::GetDataSubsystem() const
{
    if (const UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            return GameInstance->GetSubsystem<UHuwamDataSubsystem>();
        }
    }

    return nullptr;
}

bool UHuwamCharacterCreationComponent::HasDataRegistry() const
{
    if (const UHuwamDataSubsystem* DataSubsystem = GetDataSubsystem())
    {
        return DataSubsystem->HasDataRegistry();
    }

    return false;
}

bool UHuwamCharacterCreationComponent::GetRaceRowById(const FString& RaceId, FHuwamRaceRow& OutRow) const
{
    if (RaceId.IsEmpty())
    {
        return false;
    }

    if (const UHuwamDataSubsystem* DataSubsystem = GetDataSubsystem())
    {
        return DataSubsystem->GetRaceRow(GetRowNameFromId(RaceId), OutRow);
    }

    return false;
}

bool UHuwamCharacterCreationComponent::GetSubRaceRowById(const FString& SubRaceId, FHuwamSubRaceRow& OutRow) const
{
    if (SubRaceId.IsEmpty())
    {
        return false;
    }

    if (const UHuwamDataSubsystem* DataSubsystem = GetDataSubsystem())
    {
        if (SubRaceId.StartsWith(TEXT("race.")))
        {
            FString SubRaceRowName = SubRaceId;
            SubRaceRowName.RemoveFromStart(TEXT("race."));
            SubRaceRowName.ReplaceInline(TEXT("."), TEXT("_"));
            SubRaceRowName = FString::Printf(TEXT("subrace_%s"), *SubRaceRowName);
            if (DataSubsystem->GetSubRaceRow(FName(*SubRaceRowName), OutRow))
            {
                return true;
            }
        }

        return DataSubsystem->GetSubRaceRow(GetRowNameFromId(SubRaceId), OutRow);
    }

    return false;
}

bool UHuwamCharacterCreationComponent::GetClassRowById(const FString& ClassId, FHuwamClassRow& OutRow) const
{
    if (ClassId.IsEmpty())
    {
        return false;
    }

    if (const UHuwamDataSubsystem* DataSubsystem = GetDataSubsystem())
    {
        return DataSubsystem->GetClassRow(GetRowNameFromId(ClassId), OutRow);
    }

    return false;
}

bool UHuwamCharacterCreationComponent::GetJobRowById(const FString& JobId, FHuwamJobRow& OutRow) const
{
    if (JobId.IsEmpty())
    {
        return false;
    }

    if (const UHuwamDataSubsystem* DataSubsystem = GetDataSubsystem())
    {
        return DataSubsystem->GetJobRow(GetRowNameFromId(JobId), OutRow);
    }

    return false;
}

bool UHuwamCharacterCreationComponent::GetSkillRowById(const FString& SkillId, FHuwamSkillRow& OutRow) const
{
    if (SkillId.IsEmpty())
    {
        return false;
    }

    if (const UHuwamDataSubsystem* DataSubsystem = GetDataSubsystem())
    {
        return DataSubsystem->GetSkillRow(GetRowNameFromId(SkillId), OutRow);
    }

    return false;
}

bool UHuwamCharacterCreationComponent::GetAbilityRowById(const FString& AbilityId, FHuwamAbilityRow& OutRow) const
{
    if (AbilityId.IsEmpty())
    {
        return false;
    }

    if (const UHuwamDataSubsystem* DataSubsystem = GetDataSubsystem())
    {
        return DataSubsystem->GetAbilityRow(GetRowNameFromId(AbilityId), OutRow);
    }

    return false;
}

bool UHuwamCharacterCreationComponent::HasDuplicates(const TArray<FString>& Values) const
{
    TSet<FString> SeenValues;
    for (const FString& Value : Values)
    {
        if (Value.IsEmpty())
        {
            continue;
        }

        if (SeenValues.Contains(Value))
        {
            return true;
        }

        SeenValues.Add(Value);
    }

    return false;
}

FText UHuwamCharacterCreationComponent::MakeMissingRowWarning(const FString& Kind, const FString& Id) const
{
    return FText::Format(
        FText::FromString(TEXT("No {0} row was found for {1}; fallback display text was used.")),
        FText::FromString(Kind),
        FText::FromString(Id)
    );
}
