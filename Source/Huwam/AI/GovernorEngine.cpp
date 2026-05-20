#include "GovernorEngine.h"

static ESkillType GetSkillForQuestType(EQuestType QuestType)
{
    switch (QuestType)
    {
    case EQuestType::Gather:
        return ESkillType::Farming;
    case EQuestType::Deliver:
        return ESkillType::Trade;
    case EQuestType::Kill:
        return ESkillType::Combat;
    case EQuestType::Explore:
        return ESkillType::Survival;
    case EQuestType::Craft:
        return ESkillType::Crafting;
    case EQuestType::Escort:
        return ESkillType::Leadership;
    default:
        return ESkillType::Survival;
    }
}

static ESkillType GetJobPrimarySkill(ENPCJob Job)
{
    switch (Job)
    {
    case ENPCJob::Farmer:
    case ENPCJob::Breeder:
        return ESkillType::Farming;
    case ENPCJob::Blacksmith:
    case ENPCJob::Construction:
    case ENPCJob::Inventor:
    case ENPCJob::ShopKeeper:
        return ESkillType::Crafting;
    case ENPCJob::Merchant:
    case ENPCJob::SeaCaptain:
    case ENPCJob::Spy:
        return ESkillType::Diplomacy;
    case ENPCJob::Wizard:
    case ENPCJob::Sorcerer:
    case ENPCJob::Warlock:
    case ENPCJob::Mage:
    case ENPCJob::Cleric:
    case ENPCJob::Priest:
    case ENPCJob::Demonlord:
    case ENPCJob::Hero:
    case ENPCJob::Villain:
    case ENPCJob::AntiHero:
        return ESkillType::Magic;
    case ENPCJob::Fighter:
    case ENPCJob::Barbarian:
    case ENPCJob::Knight:
    case ENPCJob::Guard:
    case ENPCJob::Assassin:
    case ENPCJob::Archer:
    case ENPCJob::Thief:
    case ENPCJob::Pirate:
        return ESkillType::Combat;
    case ENPCJob::Tamer:
    case ENPCJob::Adventurer:
    case ENPCJob::SlowLife:
        return ESkillType::Survival;
    case ENPCJob::Nobility:
        return ESkillType::Leadership;
    default:
        return ESkillType::Survival;
    }
}

static void IncreasePlayerJobProgress(FHUDState& PlayerHUD, ENPCJob Job, int32 ProgressValue)
{
    FPlayerJobProgress* JobState = PlayerHUD.JobProgress.FindByPredicate([Job](const FPlayerJobProgress& Item) {
        return Item.Job == Job;
    });

    if (!JobState)
    {
        FPlayerJobProgress NewJob;
        NewJob.Job = Job;
        NewJob.Progress = 0;
        NewJob.RequiredProgress = 5;
        NewJob.bUnlocked = false;
        PlayerHUD.JobProgress.Add(NewJob);
        JobState = &PlayerHUD.JobProgress.Last();
    }

    JobState->Progress += ProgressValue;
    if (JobState->Progress >= JobState->RequiredProgress)
    {
        JobState->bUnlocked = true;
    }
}

static void ApplyQuestProgressToPlayerJobs(FHUDState& PlayerHUD, ESkillType SkillType, int32 ProgressGain)
{
    for (ENPCJob Job : PlayerHUD.SelectedJobs)
    {
        if (GetJobPrimarySkill(Job) == SkillType)
        {
            IncreasePlayerJobProgress(PlayerHUD, Job, ProgressGain);
        }
    }

    for (FSkillDistribution& Skill : PlayerHUD.Skills)
    {
        if (Skill.Skill == SkillType)
        {
            Skill.PlayerSkillLevel = FMath::Min(Skill.PlayerSkillLevel + ProgressGain, 10);
            Skill.bUnlocked = true;
        }
    }

    if (PlayerHUD.CurrentJob == ENPCJob::Newbie)
    {
        for (FPlayerJobProgress& JobState : PlayerHUD.JobProgress)
        {
            if (JobState.bUnlocked && PlayerHUD.SelectedJobs.Contains(JobState.Job))
            {
                PlayerHUD.CurrentJob = JobState.Job;
                break;
            }
        }
    }
}

void FGovernorEngine::Initialize()
{
    // Initialize governor management systems.
}

void FGovernorEngine::Update(float DeltaTime)
{
    // Governor engine updates are handled by the overseer.
}

void FGovernorEngine::ManageQuests(FWorldState& WorldState)
{
    // Generate new quests based on world state.
    if (WorldState.CurrentTick % 50 == 0 && WorldState.ActiveQuests.Num() < 5)
    {
        FQuest NewQuest;
        NewQuest.Description = TEXT("Gather resources for the village.");
        NewQuest.Type = EQuestType::Gather;
        NewQuest.RewardExperience = 100;
        NewQuest.RewardGold = 50;
        WorldState.ActiveQuests.Add(MoveTemp(NewQuest));
    }

    // Check for quest completions (simplified).
    for (FQuest& Quest : WorldState.ActiveQuests)
    {
        if (!Quest.bIsCompleted && FMath::FRand() < 0.1f) // Random completion for demo
        {
            Quest.bIsCompleted = true;
            for (FHUDState& HUD : WorldState.PlayerHUDs)
            {
                if (HUD.PlayerId == Quest.AssignedPlayerId)
                {
                    HandleQuestCompletion(Quest, HUD);
                    break;
                }
            }
        }
    }
}

void FGovernorEngine::HandleQuestCompletion(FQuest& Quest, FHUDState& PlayerHUD)
{
    PlayerHUD.Experience += Quest.RewardExperience;
    PlayerHUD.Gold += Quest.RewardGold;

    ESkillType RewardSkill = GetSkillForQuestType(Quest.Type);
    int32 SkillProgressGain = FMath::Clamp(Quest.RewardExperience / 25, 1, 3);
    ApplyQuestProgressToPlayerJobs(PlayerHUD, RewardSkill, SkillProgressGain);
}

void FGovernorEngine::ManageInventories(FWorldState& WorldState)
{
    // Simulate inventory changes.
    for (FInventory& Inv : WorldState.NPCInventories)
    {
        for (FInventoryItem& Item : Inv.Items)
        {
            Item.Quantity = FMath::Clamp(Item.Quantity + FMath::RandRange(-5, 5), 0, 100);
        }
    }
    for (FInventory& Inv : WorldState.ChestInventories)
    {
        for (FInventoryItem& Item : Inv.Items)
        {
            Item.Quantity = FMath::Clamp(Item.Quantity + FMath::RandRange(-2, 2), 0, 50);
        }
    }
}

void FGovernorEngine::UpdateFarmableAreas(FWorldState& WorldState)
{
    for (FFarmableArea& Area : WorldState.FarmableAreas)
    {
        if (!Area.bIsFarmable)
        {
            Area.CooldownTicks--;
            if (Area.CooldownTicks <= 0)
            {
                Area.bIsFarmable = true;
            }
        }
    }
}

void FGovernorEngine::UpdateHUDs(FWorldState& WorldState)
{
    for (FHUDState& HUD : WorldState.PlayerHUDs)
    {
        // Update HUD based on player state (simplified).
        HUD.Health = FMath::Clamp(HUD.Health + FMath::RandRange(-5, 5), 0, 100);
    }
}

void FGovernorEngine::UpdateLeaderboards(FWorldState& WorldState)
{
    for (FLeaderboard& Board : WorldState.Leaderboards)
    {
        // Sort entries by score.
        Board.Entries.Sort([](const FLeaderboardEntry& A, const FLeaderboardEntry& B) {
            return A.Score > B.Score;
        });
        // Keep top 10.
        if (Board.Entries.Num() > 10)
        {
            Board.Entries.RemoveAt(10, Board.Entries.Num() - 10);
        }
    }
}