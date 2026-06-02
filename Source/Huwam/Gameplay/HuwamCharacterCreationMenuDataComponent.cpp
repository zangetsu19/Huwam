#include "Gameplay/HuwamCharacterCreationMenuDataComponent.h"

#include "Data/HuwamDataRegistry.h"
#include "Data/HuwamDataSubsystem.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

namespace
{
    bool ContainsId(const TArray<FString>& Values, const FString& Id)
    {
        return Values.Contains(Id);
    }

    FHuwamCharacterCreationOption MakeOption(
        EHuwamCharacterCreationOptionType OptionType,
        const FString& OptionId,
        const FString& ParentId,
        const FText& DisplayName,
        const FText& Description,
        const FText& DetailText,
        bool bSelected,
        bool bAvailable = true
    )
    {
        FHuwamCharacterCreationOption Option;
        Option.OptionType = OptionType;
        Option.OptionId = OptionId;
        Option.ParentId = ParentId;
        Option.DisplayName = DisplayName;
        Option.Description = Description;
        Option.DetailText = DetailText;
        Option.bSelected = bSelected;
        Option.bAvailable = bAvailable;
        return Option;
    }

    template <typename RowType>
    void AddRowsFromTable(const UDataTable* Table, TFunctionRef<void(const RowType&)> AddRow)
    {
        if (!Table)
        {
            return;
        }

        for (const FName& RowName : Table->GetRowNames())
        {
            if (const RowType* Row = Table->FindRow<RowType>(RowName, TEXT("HuwamCharacterCreationMenuData")))
            {
                AddRow(*Row);
            }
        }
    }

    FString ManaWellDepthToString(EHuwamManaWellDepth ManaWellDepth)
    {
        switch (ManaWellDepth)
        {
        case EHuwamManaWellDepth::None:
            return TEXT("No mana well");
        case EHuwamManaWellDepth::Shallow:
            return TEXT("Shallow mana");
        case EHuwamManaWellDepth::Low:
            return TEXT("Low mana");
        case EHuwamManaWellDepth::Average:
            return TEXT("Average mana");
        case EHuwamManaWellDepth::Deep:
            return TEXT("Deep mana");
        case EHuwamManaWellDepth::VeryDeep:
            return TEXT("Very deep mana");
        case EHuwamManaWellDepth::Extreme:
            return TEXT("Extreme mana");
        default:
            return TEXT("Mana unknown");
        }
    }
}

UHuwamCharacterCreationMenuDataComponent::UHuwamCharacterCreationMenuDataComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHuwamCharacterCreationMenuDataComponent::SetCharacterCreationComponent(UHuwamCharacterCreationComponent* InCharacterCreation)
{
    CharacterCreation = InCharacterCreation;
}

UHuwamCharacterCreationComponent* UHuwamCharacterCreationMenuDataComponent::GetCharacterCreationComponent() const
{
    return ResolveCharacterCreationComponent();
}

FHuwamCharacterCreationMenuSnapshot UHuwamCharacterCreationMenuDataComponent::BuildDefaultMenuSnapshot() const
{
    if (const UHuwamCharacterCreationComponent* CreationComponent = ResolveCharacterCreationComponent())
    {
        return BuildMenuSnapshot(CreationComponent->BuildDefaultCharacterCreationRequest());
    }

    FHuwamCharacterCreationRequest Request;
    Request.CharacterName = FText::FromString(TEXT("New Huwam Hero"));
    Request.CreationPath = EHuwamCharacterCreationPath::Normal;
    Request.RaceId = TEXT("race.human");
    Request.SubRaceId = TEXT("race.human.common");
    Request.ClassIds = { TEXT("class.progression_zero") };
    Request.JobIds = { TEXT("job.adventurer") };
    return BuildMenuSnapshot(Request);
}

FHuwamCharacterCreationMenuSnapshot UHuwamCharacterCreationMenuDataComponent::BuildMenuSnapshot(const FHuwamCharacterCreationRequest& Request) const
{
    FHuwamCharacterCreationMenuSnapshot Snapshot;
    Snapshot.Request = Request;
    Snapshot.ClassSlotsUsed = Request.ClassIds.Num();
    Snapshot.JobSlotsUsed = Request.JobIds.Num();
    Snapshot.SkillSlotsUsed = Request.SkillIds.Num();
    Snapshot.AbilitySlotsUsed = Request.AbilityIds.Num();
    Snapshot.SkillSlotLimit = GetSkillLimitForRequest(Request);
    Snapshot.AbilitySlotLimit = GetAbilityLimitForRequest(Request);

    bool bUsedFallback = false;
    BuildRaceOptions(Request, Snapshot.RaceOptions, bUsedFallback);
    BuildSubRaceOptions(Request, Snapshot.SubRaceOptions, bUsedFallback);
    BuildClassOptions(Request, Snapshot.ClassOptions, bUsedFallback);
    BuildJobOptions(Request, Snapshot.JobOptions, bUsedFallback);
    BuildSkillOptions(Request, Snapshot.SkillOptions, bUsedFallback);
    BuildAbilityOptions(Request, Snapshot.AbilityOptions, bUsedFallback);
    Snapshot.bUsingFallbackOptions = bUsedFallback;

    if (const UHuwamCharacterCreationComponent* CreationComponent = ResolveCharacterCreationComponent())
    {
        CreationComponent->ValidateCharacterCreation(Request, Snapshot.ValidationResult);
        Snapshot.bCanConfirm = Snapshot.ValidationResult.bSuccess;
    }
    else
    {
        Snapshot.ValidationResult.Errors.Add(FText::FromString(TEXT("No character creation component is available to validate this request.")));
        Snapshot.bCanConfirm = false;
    }

    return Snapshot;
}

bool UHuwamCharacterCreationMenuDataComponent::BuildRequestWithSingleSelection(const FHuwamCharacterCreationRequest& Request, EHuwamCharacterCreationOptionType OptionType, const FString& OptionId, FHuwamCharacterCreationRequest& OutRequest) const
{
    OutRequest = Request;
    if (OptionId.IsEmpty())
    {
        return false;
    }

    if (OptionType == EHuwamCharacterCreationOptionType::Race)
    {
        OutRequest.RaceId = OptionId;
        OutRequest.SubRaceId.Empty();
        return true;
    }

    if (OptionType == EHuwamCharacterCreationOptionType::SubRace)
    {
        OutRequest.SubRaceId = OptionId;
        return true;
    }

    return false;
}

bool UHuwamCharacterCreationMenuDataComponent::BuildRequestWithToggledSelection(const FHuwamCharacterCreationRequest& Request, EHuwamCharacterCreationOptionType OptionType, const FString& OptionId, FHuwamCharacterCreationRequest& OutRequest) const
{
    OutRequest = Request;
    if (OptionId.IsEmpty())
    {
        return false;
    }

    switch (OptionType)
    {
    case EHuwamCharacterCreationOptionType::Class:
        return ToggleIdInArray(OutRequest.ClassIds, OptionId, 3);
    case EHuwamCharacterCreationOptionType::Job:
        return ToggleIdInArray(OutRequest.JobIds, OptionId, 5);
    case EHuwamCharacterCreationOptionType::Skill:
        return ToggleIdInArray(OutRequest.SkillIds, OptionId, GetSkillLimitForRequest(Request));
    case EHuwamCharacterCreationOptionType::Ability:
        return ToggleIdInArray(OutRequest.AbilityIds, OptionId, GetAbilityLimitForRequest(Request));
    default:
        return false;
    }
}

void UHuwamCharacterCreationMenuDataComponent::BuildRaceOptions(const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions, bool& bOutUsedFallback) const
{
    OutOptions.Reset();

    if (const UHuwamDataRegistry* Registry = GetDataRegistry())
    {
        AddRowsFromTable<FHuwamRaceRow>(Registry->RacesTable, [&](const FHuwamRaceRow& Row)
        {
            OutOptions.Add(MakeOption(
                EHuwamCharacterCreationOptionType::Race,
                Row.RaceId,
                TEXT(""),
                Row.DisplayName,
                Row.Description,
                Row.CommonReputation,
                Request.RaceId == Row.RaceId
            ));
        });
    }

    if (OutOptions.IsEmpty() && bUseFallbackOptionsWhenRegistryMissing)
    {
        AddFallbackOptions(EHuwamCharacterCreationOptionType::Race, Request, OutOptions);
        bOutUsedFallback = true;
    }
}

void UHuwamCharacterCreationMenuDataComponent::BuildSubRaceOptions(const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions, bool& bOutUsedFallback) const
{
    OutOptions.Reset();

    if (const UHuwamDataRegistry* Registry = GetDataRegistry())
    {
        AddRowsFromTable<FHuwamSubRaceRow>(Registry->SubRacesTable, [&](const FHuwamSubRaceRow& Row)
        {
            const bool bMatchesRace = Request.RaceId.IsEmpty() || Row.ParentRaceId == Request.RaceId;
            if (bMatchesRace)
            {
                OutOptions.Add(MakeOption(
                    EHuwamCharacterCreationOptionType::SubRace,
                    Row.SubRaceId,
                    Row.ParentRaceId,
                    Row.DisplayName,
                    Row.Description,
                    MakeSubRaceDetailText(Row.SubRaceId, Row.ManaWellModifier),
                    Request.SubRaceId == Row.SubRaceId
                ));
            }
        });
    }

    if (OutOptions.IsEmpty() && bUseFallbackOptionsWhenRegistryMissing)
    {
        AddFallbackOptions(EHuwamCharacterCreationOptionType::SubRace, Request, OutOptions);
        bOutUsedFallback = true;
    }
}

void UHuwamCharacterCreationMenuDataComponent::BuildClassOptions(const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions, bool& bOutUsedFallback) const
{
    OutOptions.Reset();

    if (const UHuwamDataRegistry* Registry = GetDataRegistry())
    {
        AddRowsFromTable<FHuwamClassRow>(Registry->ClassesTable, [&](const FHuwamClassRow& Row)
        {
            OutOptions.Add(MakeOption(
                EHuwamCharacterCreationOptionType::Class,
                Row.ClassId,
                Row.GuildId,
                Row.DisplayName,
                Row.Description,
                FText::Format(FText::FromString(TEXT("Max level {0}")), FText::AsNumber(Row.MaxLevel)),
                ContainsId(Request.ClassIds, Row.ClassId)
            ));
        });
    }

    if (OutOptions.IsEmpty() && bUseFallbackOptionsWhenRegistryMissing)
    {
        AddFallbackOptions(EHuwamCharacterCreationOptionType::Class, Request, OutOptions);
        bOutUsedFallback = true;
    }
}

void UHuwamCharacterCreationMenuDataComponent::BuildJobOptions(const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions, bool& bOutUsedFallback) const
{
    OutOptions.Reset();

    if (const UHuwamDataRegistry* Registry = GetDataRegistry())
    {
        AddRowsFromTable<FHuwamJobRow>(Registry->JobsTable, [&](const FHuwamJobRow& Row)
        {
            OutOptions.Add(MakeOption(
                EHuwamCharacterCreationOptionType::Job,
                Row.JobId,
                Row.AssociatedGuildId,
                Row.DisplayName,
                Row.Description,
                FText::Format(FText::FromString(TEXT("{0}-{1} gold salary")), FText::AsNumber(Row.MinSalaryGold), FText::AsNumber(Row.MaxSalaryGold)),
                ContainsId(Request.JobIds, Row.JobId)
            ));
        });
    }

    if (OutOptions.IsEmpty() && bUseFallbackOptionsWhenRegistryMissing)
    {
        AddFallbackOptions(EHuwamCharacterCreationOptionType::Job, Request, OutOptions);
        bOutUsedFallback = true;
    }
}

void UHuwamCharacterCreationMenuDataComponent::BuildSkillOptions(const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions, bool& bOutUsedFallback) const
{
    OutOptions.Reset();

    if (const UHuwamDataRegistry* Registry = GetDataRegistry())
    {
        AddRowsFromTable<FHuwamSkillRow>(Registry->SkillsTable, [&](const FHuwamSkillRow& Row)
        {
            OutOptions.Add(MakeOption(
                EHuwamCharacterCreationOptionType::Skill,
                Row.SkillId,
                TEXT(""),
                Row.DisplayName,
                Row.Description,
                Row.Category,
                ContainsId(Request.SkillIds, Row.SkillId),
                Row.bPermanentAtCreation
            ));
        });
    }

    if (OutOptions.IsEmpty() && bUseFallbackOptionsWhenRegistryMissing)
    {
        AddFallbackOptions(EHuwamCharacterCreationOptionType::Skill, Request, OutOptions);
        bOutUsedFallback = true;
    }
}

void UHuwamCharacterCreationMenuDataComponent::BuildAbilityOptions(const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions, bool& bOutUsedFallback) const
{
    OutOptions.Reset();

    if (const UHuwamDataRegistry* Registry = GetDataRegistry())
    {
        AddRowsFromTable<FHuwamAbilityRow>(Registry->AbilitiesTable, [&](const FHuwamAbilityRow& Row)
        {
            OutOptions.Add(MakeOption(
                EHuwamCharacterCreationOptionType::Ability,
                Row.AbilityId,
                TEXT(""),
                Row.DisplayName,
                Row.Description,
                Row.AbilityType,
                ContainsId(Request.AbilityIds, Row.AbilityId),
                Row.bPermanentAtCreation
            ));
        });
    }

    if (OutOptions.IsEmpty() && bUseFallbackOptionsWhenRegistryMissing)
    {
        AddFallbackOptions(EHuwamCharacterCreationOptionType::Ability, Request, OutOptions);
        bOutUsedFallback = true;
    }
}

void UHuwamCharacterCreationMenuDataComponent::AddFallbackOptions(EHuwamCharacterCreationOptionType OptionType, const FHuwamCharacterCreationRequest& Request, TArray<FHuwamCharacterCreationOption>& OutOptions) const
{
    auto AddFallback = [&](const FString& Id, const FString& ParentId, const FString& Description, const FString& Detail, bool bSelected)
    {
        OutOptions.Add(MakeOption(
            OptionType,
            Id,
            ParentId,
            MakeDisplayTextFromId(Id),
            FText::FromString(Description),
            FText::FromString(Detail),
            bSelected
        ));
    };

    switch (OptionType)
    {
    case EHuwamCharacterCreationOptionType::Race:
        AddFallback(TEXT("race.human"), TEXT(""), TEXT("All-rounder citizens of Nanira."), TEXT("Common"), Request.RaceId == TEXT("race.human"));
        AddFallback(TEXT("race.elf"), TEXT(""), TEXT("Long-lived knowledge experts."), TEXT("Deep mana"), Request.RaceId == TEXT("race.elf"));
        AddFallback(TEXT("race.dwarf"), TEXT(""), TEXT("Bearded builders and master crafters."), TEXT("Respected"), Request.RaceId == TEXT("race.dwarf"));
        AddFallback(TEXT("race.orc"), TEXT(""), TEXT("Tough guards, hunters, and hard fighters."), TEXT("Feared and respected"), Request.RaceId == TEXT("race.orc"));
        AddFallback(TEXT("race.demon"), TEXT(""), TEXT("Misunderstood people with deep mana wells."), TEXT("Very deep mana"), Request.RaceId == TEXT("race.demon"));
        AddFallback(TEXT("race.beastfolk"), TEXT(""), TEXT("Animal-featured survivors with strong senses."), TEXT("Uncommon"), Request.RaceId == TEXT("race.beastfolk"));
        AddFallback(TEXT("race.fae"), TEXT(""), TEXT("Dreamlike people who reject normality."), TEXT("Rare"), Request.RaceId == TEXT("race.fae"));
        break;
    case EHuwamCharacterCreationOptionType::SubRace:
        if (Request.RaceId == TEXT("race.human") || Request.RaceId.IsEmpty())
        {
            AddFallback(TEXT("race.human.common"), TEXT("race.human"), TEXT("Grounded level one all-rounder with worn leather, soft cloth, and a used short sword."), TEXT("All-rounder adventurer"), Request.SubRaceId == TEXT("race.human.common"));
            AddFallback(TEXT("race.human.high"), TEXT("race.human"), TEXT("Alabaster smug noble magic user in fine fabrics and controlled spellwork."), TEXT("Noble magic user"), Request.SubRaceId == TEXT("race.human.high"));
            AddFallback(TEXT("race.human.dawn"), TEXT("race.human"), TEXT("Yellow-warmed cheerful holy Human with bright sacred garments and a gentle aura."), TEXT("Holy optimist"), Request.SubRaceId == TEXT("race.human.dawn"));
            AddFallback(TEXT("race.human.dusk"), TEXT("race.human"), TEXT("Dark skinned stalwart tank guard with knight armor, greatsword, kite shield, and void aura."), TEXT("Void-touched tank"), Request.SubRaceId == TEXT("race.human.dusk"));
            AddFallback(TEXT("race.human.frontier"), TEXT("race.human"), TEXT("Reddish sunburnt farmer ranger with wild predator-watchful eyes and rugged work gear."), TEXT("Predator-alert survivor"), Request.SubRaceId == TEXT("race.human.frontier"));
            AddFallback(TEXT("race.human.urban"), TEXT("race.human"), TEXT("Slightly dirty city merchant with softer build, trade clothes, ledgers, and coin pouches."), TEXT("Streetwise merchant"), Request.SubRaceId == TEXT("race.human.urban"));
            AddFallback(TEXT("race.human.warborn"), TEXT("race.human"), TEXT("Dark tanned scarred fierce battle-happy Human for barbarians, guards, and war knights."), TEXT("Battle-happy frontliner"), Request.SubRaceId == TEXT("race.human.warborn"));
            AddFallback(TEXT("race.human.half_elf"), TEXT("race.human"), TEXT("Human-Elf hybrid with very short elf ears, sword gear, charisma, and mixed-culture flexibility."), TEXT("Hybrid adventurer"), Request.SubRaceId == TEXT("race.human.half_elf"));
            AddFallback(TEXT("race.human.blessed"), TEXT("race.human"), TEXT("Ebony skin, white hair, white eyes, divine wings, deity tattoos, and radiant holy aura."), TEXT("Rare divine-touched"), Request.SubRaceId == TEXT("race.human.blessed"));
            AddFallback(TEXT("race.human.cursed"), TEXT("race.human"), TEXT("Sickly purple-tinted dark tan rogue with black eyes, curse tattoos, and purple aura."), TEXT("Rogue curse-bearer"), Request.SubRaceId == TEXT("race.human.cursed"));
        }
        if (Request.RaceId == TEXT("race.elf") || Request.RaceId.IsEmpty())
        {
            AddFallback(TEXT("race.elf.dark"), TEXT("race.elf"), TEXT("Mysterious purple-skinned elf outliers."), TEXT("Stealth and mana"), Request.SubRaceId == TEXT("race.elf.dark"));
            AddFallback(TEXT("race.elf.high"), TEXT("race.elf"), TEXT("High-status elf lineage."), TEXT("Knowledge and court life"), Request.SubRaceId == TEXT("race.elf.high"));
            AddFallback(TEXT("race.elf.wood"), TEXT("race.elf"), TEXT("Forest-dwelling Elves tied to archery."), TEXT("Herbs and natural mana"), Request.SubRaceId == TEXT("race.elf.wood"));
            AddFallback(TEXT("race.elf.half"), TEXT("race.elf"), TEXT("Human-Elf hybrid with very short elf ears, sword gear, charisma, and mixed-culture flexibility."), TEXT("Hybrid adventurer"), Request.SubRaceId == TEXT("race.elf.half"));
        }
        if (Request.RaceId == TEXT("race.dwarf") || Request.RaceId.IsEmpty())
        {
            AddFallback(TEXT("race.dwarf.mountain"), TEXT("race.dwarf"), TEXT("Fortress builders and mountain smiths."), TEXT("Stone and forge"), Request.SubRaceId == TEXT("race.dwarf.mountain"));
            AddFallback(TEXT("race.dwarf.forge"), TEXT("race.dwarf"), TEXT("Fire, metal, furnace, and smithing-focused Dwarves."), TEXT("Furnace smith"), Request.SubRaceId == TEXT("race.dwarf.forge"));
            AddFallback(TEXT("race.dwarf.quarry"), TEXT("race.dwarf"), TEXT("Stonecutters, masons, road builders, and civic construction masters."), TEXT("Mason builder"), Request.SubRaceId == TEXT("race.dwarf.quarry"));
            AddFallback(TEXT("race.dwarf.deep"), TEXT("race.dwarf"), TEXT("Underground Dwarves adapted to caverns, mines, and dark sealed routes."), TEXT("Cave delver"), Request.SubRaceId == TEXT("race.dwarf.deep"));
            AddFallback(TEXT("race.dwarf.goldbeard"), TEXT("race.dwarf"), TEXT("Trade, appraisal, banking, and high-value gear Dwarves."), TEXT("Contract appraiser"), Request.SubRaceId == TEXT("race.dwarf.goldbeard"));
            AddFallback(TEXT("race.dwarf.iron_oath"), TEXT("race.dwarf"), TEXT("Duty-bound Dwarves who treat work contracts as sacred oaths."), TEXT("Sacred contract guard"), Request.SubRaceId == TEXT("race.dwarf.iron_oath"));
            AddFallback(TEXT("race.dwarf.dreamforge"), TEXT("race.dwarf"), TEXT("Ione-touched crafters who dream designs before building them."), TEXT("Dream smith"), Request.SubRaceId == TEXT("race.dwarf.dreamforge"));
            AddFallback(TEXT("race.dwarf.hearth"), TEXT("race.dwarf"), TEXT("Domestic, tavern, food, comfort, and community-focused Dwarves."), TEXT("Hearthkeeper"), Request.SubRaceId == TEXT("race.dwarf.hearth"));
        }
        if (Request.RaceId == TEXT("race.orc") || Request.RaceId.IsEmpty())
        {
            AddFallback(TEXT("race.orc.common"), TEXT("race.orc"), TEXT("Classic green clanned Orc with tusks, ugly brute features, and battle-ready presence."), TEXT("Clanned brute"), Request.SubRaceId == TEXT("race.orc.common"));
            AddFallback(TEXT("race.orc.half"), TEXT("race.orc"), TEXT("Half-Human Orc branch with green skin, smaller tusks, leaner build, and more Human features."), TEXT("Human-Orc bridge"), Request.SubRaceId == TEXT("race.orc.half"));
            AddFallback(TEXT("race.orc.war"), TEXT("race.orc"), TEXT("Brown-red battle Orcs with dense chiseled muscle and mandatory war paint."), TEXT("War-painted fighter"), Request.SubRaceId == TEXT("race.orc.war"));
            AddFallback(TEXT("race.orc.stonehide"), TEXT("race.orc"), TEXT("Stone-colored Orcs with rocky hide protrusions and metal armor."), TEXT("Stone-armored tank"), Request.SubRaceId == TEXT("race.orc.stonehide"));
            AddFallback(TEXT("race.orc.redtusk"), TEXT("race.orc"), TEXT("Blood-red aggressive Orcs with bloodshot eyes and constant fight-ready posture."), TEXT("Bloodthirsty duelist"), Request.SubRaceId == TEXT("race.orc.redtusk"));
            AddFallback(TEXT("race.orc.gray"), TEXT("race.orc"), TEXT("Gray-skinned intelligent Orcs with studious, neutral, passive body language."), TEXT("Studious neutral"), Request.SubRaceId == TEXT("race.orc.gray"));
            AddFallback(TEXT("race.orc.ironbound"), TEXT("race.orc"), TEXT("Pink pig-like hard labor Orcs with ironbound history, work gear, and resilient dignity."), TEXT("Iron laborer"), Request.SubRaceId == TEXT("race.orc.ironbound"));
            AddFallback(TEXT("race.orc.wild"), TEXT("race.orc"), TEXT("Very short green cartoonish ambush Orcs with armored helmets, small axes, and tall-grass hiding habits."), TEXT("Tall-grass ambusher"), Request.SubRaceId == TEXT("race.orc.wild"));
        }
        if (Request.RaceId == TEXT("race.demon") || Request.RaceId.IsEmpty())
        {
            AddFallback(TEXT("race.demon.common"), TEXT("race.demon"), TEXT("Baseline Demon citizen with deep mana."), TEXT("Horns and suspicion"), Request.SubRaceId == TEXT("race.demon.common"));
            AddFallback(TEXT("race.demon.peacebound"), TEXT("race.demon"), TEXT("Reformist Demon seeking coexistence."), TEXT("Controlled mana"), Request.SubRaceId == TEXT("race.demon.peacebound"));
        }
        if (Request.RaceId == TEXT("race.beastfolk") || Request.RaceId.IsEmpty())
        {
            AddFallback(TEXT("race.beastfolk.rabbitoid"), TEXT("race.beastfolk"), TEXT("Rabbit-lineage Beastfolk with fast senses."), TEXT("Speed and hearing"), Request.SubRaceId == TEXT("race.beastfolk.rabbitoid"));
            AddFallback(TEXT("race.beastfolk.panthera"), TEXT("race.beastfolk"), TEXT("Panther-like beastfolk known for stealth."), TEXT("Dexterity and senses"), Request.SubRaceId == TEXT("race.beastfolk.panthera"));
        }
        if (Request.RaceId == TEXT("race.fae") || Request.RaceId.IsEmpty())
        {
            AddFallback(TEXT("race.fae.fairy"), TEXT("race.fae"), TEXT("Small winged Fae tied to glamours."), TEXT("Flight and mischief"), Request.SubRaceId == TEXT("race.fae.fairy"));
            AddFallback(TEXT("race.fae.arch_fae"), TEXT("race.fae"), TEXT("High Fae with old pact authority."), TEXT("Very deep mana"), Request.SubRaceId == TEXT("race.fae.arch_fae"));
            AddFallback(TEXT("race.fae.faun"), TEXT("race.fae"), TEXT("Forest revel Fae with music and instinct."), TEXT("Wild movement"), Request.SubRaceId == TEXT("race.fae.faun"));
        }
        break;
    case EHuwamCharacterCreationOptionType::Class:
        AddFallback(TEXT("class.progression_zero"), TEXT(""), TEXT("The universal starter state."), TEXT("Starter"), ContainsId(Request.ClassIds, TEXT("class.progression_zero")));
        AddFallback(TEXT("class.progression_rank_1"), TEXT(""), TEXT("The hard generalist path."), TEXT("Progression"), ContainsId(Request.ClassIds, TEXT("class.progression_rank_1")));
        AddFallback(TEXT("class.archer"), TEXT("guild.archers"), TEXT("Bow use, scouting, and field pressure."), TEXT("Martial"), ContainsId(Request.ClassIds, TEXT("class.archer")));
        AddFallback(TEXT("class.priest"), TEXT("guild.church"), TEXT("Faith, rites, and divine support."), TEXT("Divine"), ContainsId(Request.ClassIds, TEXT("class.priest")));
        AddFallback(TEXT("class.knight"), TEXT("guild.knights"), TEXT("Protection, escort duty, and heavy gear."), TEXT("Martial"), ContainsId(Request.ClassIds, TEXT("class.knight")));
        AddFallback(TEXT("class.adventurer_rank_f"), TEXT("guild.adventurers"), TEXT("Entry quests and F-rank danger."), TEXT("Starter"), ContainsId(Request.ClassIds, TEXT("class.adventurer_rank_f")));
        AddFallback(TEXT("class.thief"), TEXT("guild.thieves"), TEXT("Locks, contracts, and shadowed work."), TEXT("Martial"), ContainsId(Request.ClassIds, TEXT("class.thief")));
        AddFallback(TEXT("class.mage"), TEXT("guild.mages"), TEXT("Rune and component caster."), TEXT("Hidden guild"), ContainsId(Request.ClassIds, TEXT("class.mage")));
        break;
    case EHuwamCharacterCreationOptionType::Job:
        AddFallback(TEXT("job.builder"), TEXT(""), TEXT("Constructs repairs and expands towns."), TEXT("12-80 gold"), ContainsId(Request.JobIds, TEXT("job.builder")));
        AddFallback(TEXT("job.adventurer"), TEXT("guild.adventurers"), TEXT("Takes quests and proves rank through danger."), TEXT("10-60 gold"), ContainsId(Request.JobIds, TEXT("job.adventurer")));
        AddFallback(TEXT("job.hunter_gatherer"), TEXT("guild.archers"), TEXT("Collects field resources and harvests monsters."), TEXT("8-45 gold"), ContainsId(Request.JobIds, TEXT("job.hunter_gatherer")));
        AddFallback(TEXT("job.officer"), TEXT("guild.knights"), TEXT("Maintains patrols law and public order."), TEXT("18-110 gold"), ContainsId(Request.JobIds, TEXT("job.officer")));
        AddFallback(TEXT("job.blacksmith"), TEXT(""), TEXT("Smelts and crafts gear."), TEXT("20-120 gold"), ContainsId(Request.JobIds, TEXT("job.blacksmith")));
        AddFallback(TEXT("job.merchant"), TEXT("guild.merchants"), TEXT("Buys, sells, transports, and reads the market."), TEXT("15-150 gold"), ContainsId(Request.JobIds, TEXT("job.merchant")));
        AddFallback(TEXT("job.tavern_owner"), TEXT(""), TEXT("Runs food rooms rumors and staff."), TEXT("16-130 gold"), ContainsId(Request.JobIds, TEXT("job.tavern_owner")));
        AddFallback(TEXT("job.farmer"), TEXT(""), TEXT("Grows food herbs and field resources."), TEXT("6-40 gold"), ContainsId(Request.JobIds, TEXT("job.farmer")));
        AddFallback(TEXT("job.fencer"), TEXT(""), TEXT("Trains precision dueling and refined weapon play."), TEXT("12-90 gold"), ContainsId(Request.JobIds, TEXT("job.fencer")));
        AddFallback(TEXT("job.dungeon_explorer"), TEXT("guild.adventurers"), TEXT("Maps delves and survives dungeon routes."), TEXT("14-100 gold"), ContainsId(Request.JobIds, TEXT("job.dungeon_explorer")));
        AddFallback(TEXT("job.craftsman"), TEXT(""), TEXT("Makes reliable everyday goods."), TEXT("12-90 gold"), ContainsId(Request.JobIds, TEXT("job.craftsman")));
        AddFallback(TEXT("job.baker"), TEXT(""), TEXT("Bakes town food sweets and rations."), TEXT("8-55 gold"), ContainsId(Request.JobIds, TEXT("job.baker")));
        AddFallback(TEXT("job.contractor"), TEXT(""), TEXT("Organizes crews permits and build orders."), TEXT("20-140 gold"), ContainsId(Request.JobIds, TEXT("job.contractor")));
        break;
    case EHuwamCharacterCreationOptionType::Skill:
        AddFallback(TEXT("skill.basic_weapon_handling"), TEXT(""), TEXT("Use starter weapons safely."), TEXT("Combat"), ContainsId(Request.SkillIds, TEXT("skill.basic_weapon_handling")));
        AddFallback(TEXT("skill.gathering"), TEXT(""), TEXT("Collect plants, wood, stone, and field goods."), TEXT("Gathering"), ContainsId(Request.SkillIds, TEXT("skill.gathering")));
        AddFallback(TEXT("skill.woodworking"), TEXT(""), TEXT("Carve wood handles, arrows, chests, and tools."), TEXT("Crafting"), ContainsId(Request.SkillIds, TEXT("skill.woodworking")));
        AddFallback(TEXT("skill.tanning"), TEXT(""), TEXT("Turn hides into usable leather."), TEXT("Crafting"), ContainsId(Request.SkillIds, TEXT("skill.tanning")));
        AddFallback(TEXT("skill.sewing"), TEXT(""), TEXT("Make cloth armor, pouches, robes, and repairs."), TEXT("Crafting"), ContainsId(Request.SkillIds, TEXT("skill.sewing")));
        break;
    case EHuwamCharacterCreationOptionType::Ability:
        AddFallback(TEXT("ability.born_leader"), TEXT(""), TEXT("Builds Charisma faster with allies."), TEXT("Passive"), ContainsId(Request.AbilityIds, TEXT("ability.born_leader")));
        AddFallback(TEXT("ability.indomitable"), TEXT(""), TEXT("Builds Defense through hardship."), TEXT("Passive"), ContainsId(Request.AbilityIds, TEXT("ability.indomitable")));
        AddFallback(TEXT("ability.quick_hands"), TEXT(""), TEXT("Improves early crafting and item handling."), TEXT("Passive"), ContainsId(Request.AbilityIds, TEXT("ability.quick_hands")));
        AddFallback(TEXT("ability.natural_gatherer"), TEXT(""), TEXT("Improves F-rank gathering yield."), TEXT("Passive"), ContainsId(Request.AbilityIds, TEXT("ability.natural_gatherer")));
        AddFallback(TEXT("ability.stubborn_survivor"), TEXT(""), TEXT("Improves survival under pressure."), TEXT("Passive"), ContainsId(Request.AbilityIds, TEXT("ability.stubborn_survivor")));
        break;
    }
}

bool UHuwamCharacterCreationMenuDataComponent::ToggleIdInArray(TArray<FString>& Values, const FString& OptionId, int32 MaxCount) const
{
    if (OptionId.IsEmpty())
    {
        return false;
    }

    if (Values.RemoveSingle(OptionId) > 0)
    {
        return true;
    }

    if (MaxCount > 0 && Values.Num() >= MaxCount)
    {
        return false;
    }

    Values.Add(OptionId);
    return true;
}

int32 UHuwamCharacterCreationMenuDataComponent::GetSkillLimitForRequest(const FHuwamCharacterCreationRequest& Request) const
{
    if (const UHuwamCharacterCreationComponent* CreationComponent = ResolveCharacterCreationComponent())
    {
        return CreationComponent->GetSkillLimitForPath(Request.CreationPath);
    }

    return Request.CreationPath == EHuwamCharacterCreationPath::Reincarnated ? 15 : 5;
}

int32 UHuwamCharacterCreationMenuDataComponent::GetAbilityLimitForRequest(const FHuwamCharacterCreationRequest& Request) const
{
    if (const UHuwamCharacterCreationComponent* CreationComponent = ResolveCharacterCreationComponent())
    {
        return CreationComponent->GetAbilityLimitForPath(Request.CreationPath);
    }

    return Request.CreationPath == EHuwamCharacterCreationPath::Reincarnated ? 15 : 5;
}

UHuwamCharacterCreationComponent* UHuwamCharacterCreationMenuDataComponent::ResolveCharacterCreationComponent() const
{
    if (CharacterCreation)
    {
        return CharacterCreation;
    }

    if (const AActor* Owner = GetOwner())
    {
        return Owner->FindComponentByClass<UHuwamCharacterCreationComponent>();
    }

    return nullptr;
}

UHuwamDataSubsystem* UHuwamCharacterCreationMenuDataComponent::GetDataSubsystem() const
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

UHuwamDataRegistry* UHuwamCharacterCreationMenuDataComponent::GetDataRegistry() const
{
    if (const UHuwamDataSubsystem* DataSubsystem = GetDataSubsystem())
    {
        return DataSubsystem->GetDataRegistry();
    }

    return nullptr;
}

FText UHuwamCharacterCreationMenuDataComponent::MakeDisplayTextFromId(const FString& Id) const
{
    FString Display = Id;
    const int32 DotIndex = Display.Find(TEXT("."), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
    if (DotIndex != INDEX_NONE)
    {
        Display = Display.RightChop(DotIndex + 1);
    }

    Display.ReplaceInline(TEXT("_"), TEXT(" "));
    Display.ReplaceInline(TEXT("."), TEXT(" "));
    return FText::FromString(Display);
}

FText UHuwamCharacterCreationMenuDataComponent::MakeSubRaceDetailText(const FString& SubRaceId, EHuwamManaWellDepth ManaWellDepth) const
{
    FString VisualHook;

    if (SubRaceId == TEXT("race.human.common"))
    {
        VisualHook = TEXT("All-rounder adventurer");
    }
    else if (SubRaceId == TEXT("race.human.high"))
    {
        VisualHook = TEXT("Noble magic user");
    }
    else if (SubRaceId == TEXT("race.human.dawn"))
    {
        VisualHook = TEXT("Holy optimist");
    }
    else if (SubRaceId == TEXT("race.human.dusk"))
    {
        VisualHook = TEXT("Void-touched tank");
    }
    else if (SubRaceId == TEXT("race.human.frontier"))
    {
        VisualHook = TEXT("Predator-alert survivor");
    }
    else if (SubRaceId == TEXT("race.human.urban"))
    {
        VisualHook = TEXT("Streetwise merchant");
    }
    else if (SubRaceId == TEXT("race.human.warborn"))
    {
        VisualHook = TEXT("Battle-happy frontliner");
    }
    else if (SubRaceId == TEXT("race.human.blessed"))
    {
        VisualHook = TEXT("Rare divine-touched");
    }
    else if (SubRaceId == TEXT("race.human.cursed"))
    {
        VisualHook = TEXT("Rogue curse-bearer");
    }
    else if (SubRaceId == TEXT("race.human.half_elf") || SubRaceId == TEXT("race.elf.half"))
    {
        VisualHook = TEXT("Hybrid adventurer");
    }
    else if (SubRaceId == TEXT("race.dwarf.mountain"))
    {
        VisualHook = TEXT("Stone and forge");
    }
    else if (SubRaceId == TEXT("race.dwarf.forge"))
    {
        VisualHook = TEXT("Furnace smith");
    }
    else if (SubRaceId == TEXT("race.dwarf.quarry"))
    {
        VisualHook = TEXT("Mason builder");
    }
    else if (SubRaceId == TEXT("race.dwarf.deep"))
    {
        VisualHook = TEXT("Cave delver");
    }
    else if (SubRaceId == TEXT("race.dwarf.goldbeard"))
    {
        VisualHook = TEXT("Contract appraiser");
    }
    else if (SubRaceId == TEXT("race.dwarf.iron_oath"))
    {
        VisualHook = TEXT("Sacred contract guard");
    }
    else if (SubRaceId == TEXT("race.dwarf.dreamforge"))
    {
        VisualHook = TEXT("Dream smith");
    }
    else if (SubRaceId == TEXT("race.dwarf.hearth"))
    {
        VisualHook = TEXT("Hearthkeeper");
    }
    else if (SubRaceId == TEXT("race.orc.common"))
    {
        VisualHook = TEXT("Clanned brute");
    }
    else if (SubRaceId == TEXT("race.orc.half"))
    {
        VisualHook = TEXT("Human-Orc bridge");
    }
    else if (SubRaceId == TEXT("race.orc.war"))
    {
        VisualHook = TEXT("War-painted fighter");
    }
    else if (SubRaceId == TEXT("race.orc.stonehide"))
    {
        VisualHook = TEXT("Stone-armored tank");
    }
    else if (SubRaceId == TEXT("race.orc.redtusk"))
    {
        VisualHook = TEXT("Bloodthirsty duelist");
    }
    else if (SubRaceId == TEXT("race.orc.gray"))
    {
        VisualHook = TEXT("Studious neutral");
    }
    else if (SubRaceId == TEXT("race.orc.ironbound"))
    {
        VisualHook = TEXT("Iron laborer");
    }
    else if (SubRaceId == TEXT("race.orc.wild"))
    {
        VisualHook = TEXT("Tall-grass ambusher");
    }
    else
    {
        VisualHook = ManaWellDepthToString(ManaWellDepth);
    }

    if (VisualHook == ManaWellDepthToString(ManaWellDepth))
    {
        return FText::FromString(VisualHook);
    }

    return FText::FromString(FString::Printf(TEXT("%s | %s"), *VisualHook, *ManaWellDepthToString(ManaWellDepth)));
}
