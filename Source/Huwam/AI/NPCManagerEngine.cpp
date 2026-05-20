#include "NPCManagerEngine.h"

void FNPCManagerEngine::Initialize()
{
    // Initialize NPC management systems.
}

void FNPCManagerEngine::Update(float DeltaTime)
{
    // NPC manager updates are handled by the overseer.
}

void FNPCManagerEngine::GenerateNPC(FNPCAttributes& NPC)
{
    AssignRaceAttributes(NPC);
    AssignJobAndAmbition(NPC);
    // Add initial inventory or quests if needed.
}

void FNPCManagerEngine::AssignRaceAttributes(FNPCAttributes& NPC)
{
    // Based on race, set lifespan, alignment tendencies, etc.
    switch (NPC.Race)
    {
    case ENPCRace::Human:
        NPC.Lifespan = 70 + FMath::RandRange(-10, 10);
        NPC.Alignment = EAlignment::TrueNeutral; // Common human
        break;
    case ENPCRace::Elf:
        NPC.Lifespan = 300 + FMath::RandRange(-50, 50);
        NPC.Alignment = EAlignment::ChaoticGood; // Forest dwellers
        break;
    case ENPCRace::Dwarf:
        NPC.Lifespan = 200 + FMath::RandRange(-30, 30);
        NPC.Alignment = EAlignment::LawfulNeutral; // Mountain folk
        break;
    case ENPCRace::Halfling:
        NPC.Lifespan = 100 + FMath::RandRange(-20, 20);
        NPC.Alignment = EAlignment::NeutralGood; // Cheerful
        break;
    case ENPCRace::Orc:
        NPC.Lifespan = 60 + FMath::RandRange(-10, 10);
        NPC.Alignment = EAlignment::ChaoticNeutral; // Tribal
        break;
    case ENPCRace::Ogre:
        NPC.Lifespan = 80 + FMath::RandRange(-15, 15);
        NPC.Alignment = EAlignment::NeutralEvil; // Brute
        break;
    case ENPCRace::Beastfolk:
        NPC.Lifespan = 90 + FMath::RandRange(-20, 20);
        NPC.Alignment = EAlignment::TrueNeutral; // Animalistic
        break;
    case ENPCRace::Demon:
        NPC.Lifespan = 500 + FMath::RandRange(-100, 100);
        NPC.Alignment = EAlignment::ChaoticEvil; // Evil
        if (FMath::FRand() < 0.1f) // 10% chance for rare neutral demons
        {
            NPC.Alignment = EAlignment::NeutralEvil;
        }
        break;
    case ENPCRace::Angelic:
        NPC.Lifespan = 1000 + FMath::RandRange(-200, 200);
        NPC.Alignment = EAlignment::LawfulGood; // Divine
        break;
    case ENPCRace::Spirit:
        NPC.Lifespan = 200 + FMath::RandRange(-50, 50);
        NPC.Alignment = EAlignment::NeutralGood; // Ethereal
        break;
    case ENPCRace::Fae:
        NPC.Lifespan = 150 + FMath::RandRange(-30, 30);
        NPC.Alignment = EAlignment::ChaoticNeutral; // Mischievous
        break;
    case ENPCRace::Virus:
        NPC.Lifespan = 200 + FMath::RandRange(-50, 100); // Variable due to curse
        NPC.Alignment = EAlignment::NeutralEvil; // Cursed beings
        break;
    default:
        break;
    }

    // Assign sub-race
    switch (NPC.Race)
    {
    case ENPCRace::Human:
        NPC.SubRace = FMath::RandBool() ? ENPCSubRace::Noble : ENPCSubRace::Peasant;
        break;
    case ENPCRace::Elf:
        {
            int32 Rand = FMath::RandRange(0, 2);
            if (Rand == 0) NPC.SubRace = ENPCSubRace::HighElf;
            else if (Rand == 1) NPC.SubRace = ENPCSubRace::WoodElf;
            else NPC.SubRace = ENPCSubRace::DarkElf;
        }
        break;
    case ENPCRace::Dwarf:
        NPC.SubRace = FMath::RandBool() ? ENPCSubRace::MountainDwarf : ENPCSubRace::HillDwarf;
        break;
    case ENPCRace::Halfling:
        NPC.SubRace = FMath::RandBool() ? ENPCSubRace::Lightfoot : ENPCSubRace::Stout;
        break;
    case ENPCRace::Orc:
        NPC.SubRace = FMath::RandBool() ? ENPCSubRace::CommonOrc : ENPCSubRace::HalfOrc;
        break;
    case ENPCRace::Ogre:
        NPC.SubRace = FMath::RandBool() ? ENPCSubRace::HillOgre : ENPCSubRace::SwampOgre;
        break;
    case ENPCRace::Beastfolk:
        {
            int32 Rand = FMath::RandRange(0, 4);
            if (Rand == 0) NPC.SubRace = ENPCSubRace::Catfolk;
            else if (Rand == 1) NPC.SubRace = ENPCSubRace::Wolfkin;
            else if (Rand == 2) NPC.SubRace = ENPCSubRace::Tigerfolk;
            else if (Rand == 3) NPC.SubRace = ENPCSubRace::Lionkin;
            else NPC.SubRace = ENPCSubRace::Pantherkin;
        }
        break;
    case ENPCRace::Demon:
        NPC.SubRace = FMath::RandBool() ? ENPCSubRace::Imp : ENPCSubRace::Fiend;
        break;
    case ENPCRace::Angelic:
        NPC.SubRace = FMath::RandBool() ? ENPCSubRace::Seraph : ENPCSubRace::Cherub;
        break;
    case ENPCRace::Spirit:
        NPC.SubRace = FMath::RandBool() ? ENPCSubRace::Elemental : ENPCSubRace::Ghost;
        break;
    case ENPCRace::Fae:
        {
            int32 Rand = FMath::RandRange(0, 3);
            if (Rand == 0) NPC.SubRace = ENPCSubRace::Pixie;
            else if (Rand == 1) NPC.SubRace = ENPCSubRace::Sprite;
            else if (Rand == 2) NPC.SubRace = ENPCSubRace::Faun;
            else NPC.SubRace = ENPCSubRace::ArchFey;
        }
        break;
    case ENPCRace::Virus:
        {
            int32 Rand = FMath::RandRange(0, 3);
            if (Rand == 0) NPC.SubRace = ENPCSubRace::Vampire;
            else if (Rand == 1) NPC.SubRace = ENPCSubRace::Werewolf;
            else if (Rand == 2) NPC.SubRace = ENPCSubRace::Hag;
            else NPC.SubRace = ENPCSubRace::Warlock;
        }
        break;
    default:
        NPC.SubRace = ENPCSubRace::None;
        break;
    }

    // Adjust alignment for sub-races (outliers)
    if (NPC.SubRace == ENPCSubRace::DarkElf && FMath::FRand() < 0.1f)
    {
        NPC.Alignment = EAlignment::LawfulEvil; // Outlier
    }
}

void FNPCManagerEngine::AssignJobAndAmbition(FNPCAttributes& NPC)
{
    // Assign job based on race and sub-race
    TArray<ENPCJob> PossibleJobs;
    switch (NPC.Race)
    {
    case ENPCRace::Human:
        PossibleJobs = {ENPCJob::Farmer, ENPCJob::Blacksmith, ENPCJob::Construction, ENPCJob::Thief, ENPCJob::Fighter, ENPCJob::Adventurer, ENPCJob::Nobility, ENPCJob::ShopKeeper, ENPCJob::Merchant, ENPCJob::Guard, ENPCJob::Spy, ENPCJob::Waitress, ENPCJob::Pirate, ENPCJob::SeaCaptain};
        break;
    case ENPCRace::Elf:
        PossibleJobs = {ENPCJob::Wizard, ENPCJob::Sorcerer, ENPCJob::Archer, ENPCJob::Cleric, ENPCJob::Tamer, ENPCJob::Mage, ENPCJob::Priest, ENPCJob::Guard, ENPCJob::Spy, ENPCJob::ShopKeeper};
        break;
    case ENPCRace::Dwarf:
        PossibleJobs = {ENPCJob::Blacksmith, ENPCJob::Construction, ENPCJob::Fighter, ENPCJob::Barbarian, ENPCJob::Inventor, ENPCJob::Guard, ENPCJob::Knight, ENPCJob::Merchant};
        break;
    case ENPCRace::Halfling:
        PossibleJobs = {ENPCJob::Farmer, ENPCJob::Thief, ENPCJob::SlowLife, ENPCJob::Criminal, ENPCJob::ShopKeeper, ENPCJob::Waitress, ENPCJob::Merchant, ENPCJob::Breeder};
        break;
    case ENPCRace::Orc:
        PossibleJobs = {ENPCJob::Barbarian, ENPCJob::Fighter, ENPCJob::Thief, ENPCJob::Guard};
        break;
    case ENPCRace::Ogre:
        PossibleJobs = {ENPCJob::Barbarian, ENPCJob::Fighter, ENPCJob::Guard};
        break;
    case ENPCRace::Beastfolk:
        PossibleJobs = {ENPCJob::Thief, ENPCJob::Tamer, ENPCJob::Barbarian, ENPCJob::Merchant, ENPCJob::Pirate};
        break;
    case ENPCRace::Demon:
        PossibleJobs = {ENPCJob::Warlock, ENPCJob::Sorcerer, ENPCJob::Demonlord, ENPCJob::Villain, ENPCJob::Spy};
        break;
    case ENPCRace::Angelic:
        PossibleJobs = {ENPCJob::Cleric, ENPCJob::Paladin, ENPCJob::Hero, ENPCJob::Priest, ENPCJob::Knight};
        break;
    case ENPCRace::Spirit:
        PossibleJobs = {ENPCJob::Wizard, ENPCJob::Cleric, ENPCJob::Sorcerer, ENPCJob::Mage, ENPCJob::Priest};
        break;
    case ENPCRace::Fae:
        PossibleJobs = {ENPCJob::Wizard, ENPCJob::Thief, ENPCJob::Sorcerer, ENPCJob::Mage, ENPCJob::Spy};
        break;
    case ENPCRace::Virus:
        PossibleJobs = {ENPCJob::Assassin, ENPCJob::Warlock, ENPCJob::Villain, ENPCJob::AntiHero, ENPCJob::Spy};
        break;
    default:
        PossibleJobs = {ENPCJob::Unemployed};
        break;
    }

    if (PossibleJobs.Num() > 0)
    {
        NPC.Job = PossibleJobs[FMath::RandRange(0, PossibleJobs.Num() - 1)];
    }

    // Assign ambition
    TArray<ENPCAmbition> PossibleAmbitions = {ENPCAmbition::Wealth, ENPCAmbition::Power, ENPCAmbition::Knowledge, ENPCAmbition::Peace, ENPCAmbition::Adventure, ENPCAmbition::Family};
    NPC.Ambition = PossibleAmbitions[FMath::RandRange(0, PossibleAmbitions.Num() - 1)];

    // Set job data
    NPC.JobData = GetJobData(NPC.Job);

    // Initial wealth based on job data
    NPC.Wealth = NPC.JobData.StartingWealth + FMath::RandRange(-10, 10);
    NPC.Inventory = NPC.JobData.StartingInventory;
}

void FNPCManagerEngine::UpdateNPCJobs(FWorldState& WorldState)
{
    for (FNPCAttributes& NPC : WorldState.NPCPopulation)
    {
        if (!NPC.bIsAlive)
        {
            continue;
        }

        // Update job based on ambitions or world events (simplified)
        if (NPC.Ambition == ENPCAmbition::Wealth && NPC.Wealth < 100)
        {
            NPC.Job = ENPCJob::Criminal;
        }
    }
}
FJobData FNPCManagerEngine::GetJobData(ENPCJob Job)
{
    FJobData Data;
    Data.Job = Job;

    switch (Job)
    {
    case ENPCJob::Newbie:
        Data.Strength = 10; Data.Dexterity = 10; Data.Constitution = 10; Data.Intelligence = 10; Data.Wisdom = 10; Data.Charisma = 10;
        Data.Skills = {};
        Data.StartingInventory = {{EMaterialType::Food, 5}};
        Data.StartingWealth = 10;
        Data.TypicalAlignment = EAlignment::TrueNeutral;
        Data.Gear = TEXT("Starter clothes");
        Data.Difficulty = 1;
        break;
    case ENPCJob::Thief:
        Data.Strength = 10; Data.Dexterity = 16; Data.Constitution = 12; Data.Intelligence = 12; Data.Wisdom = 10; Data.Charisma = 14;
        Data.Skills = {ESkillType::Combat, ESkillType::Survival};
        Data.StartingInventory = {{EMaterialType::Gold, 20}};
        Data.StartingWealth = 50;
        Data.TypicalAlignment = EAlignment::ChaoticNeutral;
        Data.Gear = TEXT("Daggers, lockpicks");
        Data.Difficulty = 3;
        break;
    case ENPCJob::Sorcerer:
        Data.Strength = 8; Data.Dexterity = 12; Data.Constitution = 14; Data.Intelligence = 10; Data.Wisdom = 12; Data.Charisma = 16;
        Data.Skills = {ESkillType::Magic};
        Data.StartingInventory = {{EMaterialType::Herbs, 5}};
        Data.StartingWealth = 30;
        Data.TypicalAlignment = EAlignment::ChaoticNeutral;
        Data.Gear = TEXT("Spellbook, wand");
        Data.Difficulty = 4;
        break;
    case ENPCJob::Wizard:
        Data.Strength = 8; Data.Dexterity = 12; Data.Constitution = 12; Data.Intelligence = 18; Data.Wisdom = 14; Data.Charisma = 10;
        Data.Skills = {ESkillType::Magic, ESkillType::Knowledge};
        Data.StartingInventory = {{EMaterialType::Herbs, 10}};
        Data.StartingWealth = 40;
        Data.TypicalAlignment = EAlignment::TrueNeutral;
        Data.Gear = TEXT("Spellbook, staff");
        Data.Difficulty = 5;
        break;
    case ENPCJob::Warlock:
        Data.Strength = 10; Data.Dexterity = 12; Data.Constitution = 14; Data.Intelligence = 12; Data.Wisdom = 10; Data.Charisma = 16;
        Data.Skills = {ESkillType::Magic};
        Data.StartingInventory = {{EMaterialType::Gold, 10}};
        Data.StartingWealth = 25;
        Data.TypicalAlignment = EAlignment::NeutralEvil;
        Data.Gear = TEXT("Pact weapon, tome");
        Data.Difficulty = 4;
        break;
    case ENPCJob::Barbarian:
        Data.Strength = 16; Data.Dexterity = 12; Data.Constitution = 16; Data.Intelligence = 8; Data.Wisdom = 10; Data.Charisma = 8;
        Data.Skills = {ESkillType::Combat, ESkillType::Survival};
        Data.StartingInventory = {{EMaterialType::Food, 20}};
        Data.StartingWealth = 20;
        Data.TypicalAlignment = EAlignment::ChaoticNeutral;
        Data.Gear = TEXT("Axe, hide armor");
        Data.Difficulty = 3;
        break;
    case ENPCJob::Fighter:
        Data.Strength = 16; Data.Dexterity = 14; Data.Constitution = 14; Data.Intelligence = 10; Data.Wisdom = 12; Data.Charisma = 10;
        Data.Skills = {ESkillType::Combat};
        Data.StartingInventory = {{EMaterialType::Iron, 5}};
        Data.StartingWealth = 50;
        Data.TypicalAlignment = EAlignment::TrueNeutral;
        Data.Gear = TEXT("Sword, shield, armor");
        Data.Difficulty = 3;
        break;
    case ENPCJob::Inventor:
        Data.Strength = 12; Data.Dexterity = 14; Data.Constitution = 12; Data.Intelligence = 16; Data.Wisdom = 10; Data.Charisma = 10;
        Data.Skills = {ESkillType::Crafting};
        Data.StartingInventory = {{EMaterialType::Iron, 10}, {EMaterialType::Wood, 10}};
        Data.StartingWealth = 60;
        Data.TypicalAlignment = EAlignment::TrueNeutral;
        Data.Gear = TEXT("Tools, gadgets");
        Data.Difficulty = 4;
        break;
    case ENPCJob::Cleric:
        Data.Strength = 12; Data.Dexterity = 10; Data.Constitution = 14; Data.Intelligence = 10; Data.Wisdom = 16; Data.Charisma = 12;
        Data.Skills = {ESkillType::Magic};
        Data.StartingInventory = {{EMaterialType::Herbs, 5}};
        Data.StartingWealth = 30;
        Data.TypicalAlignment = EAlignment::LawfulGood;
        Data.Gear = TEXT("Holy symbol, mace");
        Data.Difficulty = 3;
        break;
    case ENPCJob::Paladin:
        Data.Strength = 16; Data.Dexterity = 10; Data.Constitution = 14; Data.Intelligence = 8; Data.Wisdom = 12; Data.Charisma = 14;
        Data.Skills = {ESkillType::Combat, ESkillType::Leadership};
        Data.StartingInventory = {{EMaterialType::Gold, 20}};
        Data.StartingWealth = 100;
        Data.TypicalAlignment = EAlignment::LawfulGood;
        Data.Gear = TEXT("Sword, shield, armor");
        Data.Difficulty = 5;
        break;
    case ENPCJob::Assassin:
        Data.Strength = 12; Data.Dexterity = 18; Data.Constitution = 12; Data.Intelligence = 12; Data.Wisdom = 10; Data.Charisma = 10;
        Data.Skills = {ESkillType::Combat, ESkillType::Survival};
        Data.StartingInventory = {{EMaterialType::Gold, 30}};
        Data.StartingWealth = 40;
        Data.TypicalAlignment = EAlignment::NeutralEvil;
        Data.Gear = TEXT("Daggers, poison");
        Data.Difficulty = 4;
        break;
    case ENPCJob::Farmer:
        Data.Strength = 14; Data.Dexterity = 10; Data.Constitution = 14; Data.Intelligence = 8; Data.Wisdom = 12; Data.Charisma = 10;
        Data.Skills = {ESkillType::Farming};
        Data.StartingInventory = {{EMaterialType::Food, 50}, {EMaterialType::Wood, 10}};
        Data.StartingWealth = 15;
        Data.TypicalAlignment = EAlignment::NeutralGood;
        Data.Gear = TEXT("Hoe, seeds");
        Data.Difficulty = 1;
        break;
    case ENPCJob::Blacksmith:
        Data.Strength = 16; Data.Dexterity = 12; Data.Constitution = 14; Data.Intelligence = 10; Data.Wisdom = 10; Data.Charisma = 8;
        Data.Skills = {ESkillType::Crafting};
        Data.StartingInventory = {{EMaterialType::Iron, 20}, {EMaterialType::Wood, 5}};
        Data.StartingWealth = 70;
        Data.TypicalAlignment = EAlignment::TrueNeutral;
        Data.Gear = TEXT("Hammer, anvil");
        Data.Difficulty = 2;
        break;
    case ENPCJob::Demonlord:
        Data.Strength = 18; Data.Dexterity = 12; Data.Constitution = 16; Data.Intelligence = 14; Data.Wisdom = 10; Data.Charisma = 16;
        Data.Skills = {ESkillType::Magic, ESkillType::Leadership};
        Data.StartingInventory = {{EMaterialType::Gold, 100}};
        Data.StartingWealth = 500;
        Data.TypicalAlignment = EAlignment::LawfulEvil;
        Data.Gear = TEXT("Infernal weapon, crown");
        Data.Difficulty = 8;
        break;
    case ENPCJob::Hero:
        Data.Strength = 16; Data.Dexterity = 14; Data.Constitution = 14; Data.Intelligence = 12; Data.Wisdom = 12; Data.Charisma = 16;
        Data.Skills = {ESkillType::Combat, ESkillType::Leadership};
        Data.StartingInventory = {{EMaterialType::Gold, 50}};
        Data.StartingWealth = 200;
        Data.TypicalAlignment = EAlignment::LawfulGood;
        Data.Gear = TEXT("Legendary sword, armor");
        Data.Difficulty = 6;
        break;
    case ENPCJob::AntiHero:
        Data.Strength = 14; Data.Dexterity = 16; Data.Constitution = 12; Data.Intelligence = 12; Data.Wisdom = 10; Data.Charisma = 14;
        Data.Skills = {ESkillType::Combat, ESkillType::Survival};
        Data.StartingInventory = {{EMaterialType::Gold, 30}};
        Data.StartingWealth = 80;
        Data.TypicalAlignment = EAlignment::ChaoticGood;
        Data.Gear = TEXT("Dual weapons, cloak");
        Data.Difficulty = 5;
        break;
    case ENPCJob::Villain:
        Data.Strength = 14; Data.Dexterity = 12; Data.Constitution = 14; Data.Intelligence = 16; Data.Wisdom = 10; Data.Charisma = 14;
        Data.Skills = {ESkillType::Magic, ESkillType::Leadership};
        Data.StartingInventory = {{EMaterialType::Gold, 40}};
        Data.StartingWealth = 150;
        Data.TypicalAlignment = EAlignment::LawfulEvil;
        Data.Gear = TEXT("Dark robes, artifact");
        Data.Difficulty = 7;
        break;
    case ENPCJob::Criminal:
        Data.Strength = 12; Data.Dexterity = 16; Data.Constitution = 12; Data.Intelligence = 12; Data.Wisdom = 10; Data.Charisma = 12;
        Data.Skills = {ESkillType::Survival};
        Data.StartingInventory = {{EMaterialType::Gold, 15}};
        Data.StartingWealth = 25;
        Data.TypicalAlignment = EAlignment::ChaoticNeutral;
        Data.Gear = TEXT("Lockpicks, disguise");
        Data.Difficulty = 2;
        break;
    case ENPCJob::Adventurer:
        Data.Strength = 14; Data.Dexterity = 14; Data.Constitution = 12; Data.Intelligence = 12; Data.Wisdom = 10; Data.Charisma = 12;
        Data.Skills = {ESkillType::Combat, ESkillType::Survival};
        Data.StartingInventory = {{EMaterialType::Food, 10}, {EMaterialType::Gold, 20}};
        Data.StartingWealth = 50;
        Data.TypicalAlignment = EAlignment::TrueNeutral;
        Data.Gear = TEXT("Sword, backpack, map");
        Data.Difficulty = 3;
        break;
    case ENPCJob::SlowLife:
        Data.Strength = 10; Data.Dexterity = 10; Data.Constitution = 12; Data.Intelligence = 10; Data.Wisdom = 14; Data.Charisma = 12;
        Data.Skills = {ESkillType::Farming};
        Data.StartingInventory = {{EMaterialType::Food, 30}, {EMaterialType::Wood, 20}};
        Data.StartingWealth = 10;
        Data.TypicalAlignment = EAlignment::NeutralGood;
        Data.Gear = TEXT("Fishing rod, garden tools");
        Data.Difficulty = 1;
        break;
    case ENPCJob::Nobility:
        Data.Strength = 12; Data.Dexterity = 10; Data.Constitution = 12; Data.Intelligence = 12; Data.Wisdom = 12; Data.Charisma = 16;
        Data.Skills = {ESkillType::Leadership, ESkillType::Diplomacy};
        Data.StartingInventory = {{EMaterialType::Gold, 100}};
        Data.StartingWealth = 1000;
        Data.TypicalAlignment = EAlignment::LawfulNeutral;
        Data.Gear = TEXT("Fine clothes, jewelry");
        Data.Difficulty = 4;
        break;
    case ENPCJob::Tamer:
        Data.Strength = 12; Data.Dexterity = 14; Data.Constitution = 12; Data.Intelligence = 10; Data.Wisdom = 12; Data.Charisma = 14;
        Data.Skills = {ESkillType::Survival};
        Data.StartingInventory = {{EMaterialType::Food, 15}};
        Data.StartingWealth = 40;
        Data.TypicalAlignment = EAlignment::TrueNeutral;
        Data.Gear = TEXT("Whip, animal treats");
        Data.Difficulty = 3;
        break;
    case ENPCJob::Archer:
        Data.Strength = 12; Data.Dexterity = 16; Data.Constitution = 12; Data.Intelligence = 10; Data.Wisdom = 12; Data.Charisma = 10;
        Data.Skills = {ESkillType::Combat};
        Data.StartingInventory = {{EMaterialType::Wood, 20}};
        Data.StartingWealth = 35;
        Data.TypicalAlignment = EAlignment::TrueNeutral;
        Data.Gear = TEXT("Bow, arrows, quiver");
        Data.Difficulty = 3;
        break;
    case ENPCJob::Construction:
        Data.Strength = 14; Data.Dexterity = 10; Data.Constitution = 14; Data.Intelligence = 12; Data.Wisdom = 12; Data.Charisma = 10;
        Data.Skills = {ESkillType::Crafting};
        Data.StartingInventory = {{EMaterialType::Wood, 20}, {EMaterialType::Iron, 10}};
        Data.StartingWealth = 45;
        Data.TypicalAlignment = EAlignment::LawfulNeutral;
        Data.Gear = TEXT("Hammer, blueprint, construction tools");
        Data.Difficulty = 2;
        break;
    case ENPCJob::Pirate:
        Data.Strength = 14; Data.Dexterity = 14; Data.Constitution = 14; Data.Intelligence = 10; Data.Wisdom = 10; Data.Charisma = 12;
        Data.Skills = {ESkillType::Combat, ESkillType::Survival};
        Data.StartingInventory = {{EMaterialType::Gold, 20}, {EMaterialType::Food, 15}};
        Data.StartingWealth = 80;
        Data.TypicalAlignment = EAlignment::ChaoticNeutral;
        Data.Gear = TEXT("Cutlass, flintlock, tricorn hat");
        Data.Difficulty = 4;
        break;
    case ENPCJob::SeaCaptain:
        Data.Strength = 14; Data.Dexterity = 12; Data.Constitution = 14; Data.Intelligence = 14; Data.Wisdom = 12; Data.Charisma = 16;
        Data.Skills = {ESkillType::Leadership, ESkillType::Survival};
        Data.StartingInventory = {{EMaterialType::Gold, 50}, {EMaterialType::Food, 20}};
        Data.StartingWealth = 150;
        Data.TypicalAlignment = EAlignment::TrueNeutral;
        Data.Gear = TEXT("Captain's compass, sea charts, pistol");
        Data.Difficulty = 5;
        break;
    case ENPCJob::Breeder:
        Data.Strength = 10; Data.Dexterity = 10; Data.Constitution = 12; Data.Intelligence = 12; Data.Wisdom = 14; Data.Charisma = 12;
        Data.Skills = {ESkillType::Farming, ESkillType::Crafting};
        Data.StartingInventory = {{EMaterialType::Food, 30}, {EMaterialType::Herbs, 10}};
        Data.StartingWealth = 35;
        Data.TypicalAlignment = EAlignment::NeutralGood;
        Data.Gear = TEXT("Breeding ledger, animal tools");
        Data.Difficulty = 2;
        break;
    case ENPCJob::ShopKeeper:
        Data.Strength = 8; Data.Dexterity = 12; Data.Constitution = 10; Data.Intelligence = 14; Data.Wisdom = 12; Data.Charisma = 14;
        Data.Skills = {ESkillType::Diplomacy};
        Data.StartingInventory = {{EMaterialType::Gold, 40}};
        Data.StartingWealth = 80;
        Data.TypicalAlignment = EAlignment::TrueNeutral;
        Data.Gear = TEXT("Ledger, scales, shop keys");
        Data.Difficulty = 2;
        break;
    case ENPCJob::Merchant:
        Data.Strength = 10; Data.Dexterity = 12; Data.Constitution = 12; Data.Intelligence = 14; Data.Wisdom = 12; Data.Charisma = 16;
        Data.Skills = {ESkillType::Diplomacy, ESkillType::Leadership};
        Data.StartingInventory = {{EMaterialType::Gold, 70}};
        Data.StartingWealth = 120;
        Data.TypicalAlignment = EAlignment::NeutralGood;
        Data.Gear = TEXT("Trading goods, ledger, coin purse");
        Data.Difficulty = 3;
        break;
    case ENPCJob::Waitress:
        Data.Strength = 8; Data.Dexterity = 14; Data.Constitution = 12; Data.Intelligence = 10; Data.Wisdom = 12; Data.Charisma = 14;
        Data.Skills = {ESkillType::Diplomacy};
        Data.StartingInventory = {{EMaterialType::Food, 20}};
        Data.StartingWealth = 30;
        Data.TypicalAlignment = EAlignment::NeutralGood;
        Data.Gear = TEXT("Tray, apron, service bell");
        Data.Difficulty = 1;
        break;
    case ENPCJob::Guard:
        Data.Strength = 14; Data.Dexterity = 12; Data.Constitution = 14; Data.Intelligence = 10; Data.Wisdom = 12; Data.Charisma = 10;
        Data.Skills = {ESkillType::Combat};
        Data.StartingInventory = {{EMaterialType::Iron, 5}};
        Data.StartingWealth = 45;
        Data.TypicalAlignment = EAlignment::LawfulNeutral;
        Data.Gear = TEXT("Spear, shield, armor");
        Data.Difficulty = 2;
        break;
    case ENPCJob::Knight:
        Data.Strength = 16; Data.Dexterity = 12; Data.Constitution = 14; Data.Intelligence = 10; Data.Wisdom = 12; Data.Charisma = 14;
        Data.Skills = {ESkillType::Combat, ESkillType::Leadership};
        Data.StartingInventory = {{EMaterialType::Gold, 50}};
        Data.StartingWealth = 150;
        Data.TypicalAlignment = EAlignment::LawfulGood;
        Data.Gear = TEXT("Longsword, heavy armor, shield");
        Data.Difficulty = 5;
        break;
    case ENPCJob::Mage:
        Data.Strength = 8; Data.Dexterity = 12; Data.Constitution = 12; Data.Intelligence = 16; Data.Wisdom = 14; Data.Charisma = 12;
        Data.Skills = {ESkillType::Magic, ESkillType::Knowledge};
        Data.StartingInventory = {{EMaterialType::Herbs, 10}};
        Data.StartingWealth = 50;
        Data.TypicalAlignment = EAlignment::TrueNeutral;
        Data.Gear = TEXT("Staff, arcane robe");
        Data.Difficulty = 5;
        break;
    case ENPCJob::Priest:
        Data.Strength = 10; Data.Dexterity = 10; Data.Constitution = 12; Data.Intelligence = 10; Data.Wisdom = 16; Data.Charisma = 14;
        Data.Skills = {ESkillType::Magic, ESkillType::Diplomacy};
        Data.StartingInventory = {{EMaterialType::Herbs, 10}};
        Data.StartingWealth = 40;
        Data.TypicalAlignment = EAlignment::LawfulGood;
        Data.Gear = TEXT("Holy symbol, priest robes");
        Data.Difficulty = 3;
        break;
    case ENPCJob::Spy:
        Data.Strength = 10; Data.Dexterity = 16; Data.Constitution = 12; Data.Intelligence = 14; Data.Wisdom = 12; Data.Charisma = 14;
        Data.Skills = {ESkillType::Survival, ESkillType::Diplomacy};
        Data.StartingInventory = {{EMaterialType::Gold, 30}};
        Data.StartingWealth = 60;
        Data.TypicalAlignment = EAlignment::ChaoticNeutral;
        Data.Gear = TEXT("Cloak, daggers, false papers");
        Data.Difficulty = 5;
        break;
    default:
        // Unemployed
        Data.StartingWealth = 5;
        Data.TypicalAlignment = EAlignment::TrueNeutral;
        Data.Difficulty = 1;
        break;
    }

    return Data;
}