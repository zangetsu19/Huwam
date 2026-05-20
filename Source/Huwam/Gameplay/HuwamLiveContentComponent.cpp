#include "Gameplay/HuwamLiveContentComponent.h"

#include "Data/HuwamDataRegistry.h"
#include "Data/HuwamDataSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

namespace
{
    FName GetRowNameFromId(const FString& Id)
    {
        return FName(*Id.Replace(TEXT("."), TEXT("_")));
    }

    TArray<int32> ParseVersion(const FString& Version)
    {
        TArray<FString> Segments;
        Version.ParseIntoArray(Segments, TEXT("."), true);

        TArray<int32> ParsedSegments;
        for (const FString& Segment : Segments)
        {
            ParsedSegments.Add(FCString::Atoi(*Segment));
        }

        return ParsedSegments;
    }

    int32 CompareVersions(const FString& Left, const FString& Right)
    {
        const TArray<int32> LeftSegments = ParseVersion(Left);
        const TArray<int32> RightSegments = ParseVersion(Right);
        const int32 MaxSegments = FMath::Max(LeftSegments.Num(), RightSegments.Num());

        for (int32 Index = 0; Index < MaxSegments; ++Index)
        {
            const int32 LeftValue = LeftSegments.IsValidIndex(Index) ? LeftSegments[Index] : 0;
            const int32 RightValue = RightSegments.IsValidIndex(Index) ? RightSegments[Index] : 0;

            if (LeftValue > RightValue)
            {
                return 1;
            }

            if (LeftValue < RightValue)
            {
                return -1;
            }
        }

        return 0;
    }
}

UHuwamLiveContentComponent::UHuwamLiveContentComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHuwamLiveContentComponent::SetCurrentGameVersion(const FString& InCurrentGameVersion)
{
    if (InCurrentGameVersion.IsEmpty())
    {
        return;
    }

    CurrentGameVersion = InCurrentGameVersion;
    OnLiveContentChanged.Broadcast();
}

bool UHuwamLiveContentComponent::EnableContentPack(const FString& ContentPackId)
{
    if (ContentPackId.IsEmpty())
    {
        return false;
    }

    DisabledContentPackIds.Remove(ContentPackId);
    EnabledContentPackIds.AddUnique(ContentPackId);
    OnLiveContentChanged.Broadcast();
    return true;
}

bool UHuwamLiveContentComponent::DisableContentPack(const FString& ContentPackId)
{
    if (ContentPackId.IsEmpty())
    {
        return false;
    }

    EnabledContentPackIds.Remove(ContentPackId);
    DisabledContentPackIds.AddUnique(ContentPackId);
    OnLiveContentChanged.Broadcast();
    return true;
}

bool UHuwamLiveContentComponent::EnableFeatureFlag(const FString& FeatureFlagId)
{
    if (FeatureFlagId.IsEmpty())
    {
        return false;
    }

    EnabledFeatureFlagIds.AddUnique(FeatureFlagId);
    OnLiveContentChanged.Broadcast();
    return true;
}

bool UHuwamLiveContentComponent::DisableFeatureFlag(const FString& FeatureFlagId)
{
    if (FeatureFlagId.IsEmpty())
    {
        return false;
    }

    const int32 RemovedCount = EnabledFeatureFlagIds.Remove(FeatureFlagId);
    if (RemovedCount > 0)
    {
        OnLiveContentChanged.Broadcast();
    }

    return RemovedCount > 0;
}

bool UHuwamLiveContentComponent::GrantEntitlement(const FString& EntitlementId)
{
    if (EntitlementId.IsEmpty())
    {
        return false;
    }

    OwnedEntitlementIds.AddUnique(EntitlementId);
    OnLiveContentChanged.Broadcast();
    return true;
}

bool UHuwamLiveContentComponent::RevokeEntitlement(const FString& EntitlementId)
{
    if (EntitlementId.IsEmpty())
    {
        return false;
    }

    const int32 RemovedCount = OwnedEntitlementIds.Remove(EntitlementId);
    if (RemovedCount > 0)
    {
        OnLiveContentChanged.Broadcast();
    }

    return RemovedCount > 0;
}

bool UHuwamLiveContentComponent::HasEntitlement(const FString& EntitlementId) const
{
    return EntitlementId.IsEmpty() || OwnedEntitlementIds.Contains(EntitlementId);
}

bool UHuwamLiveContentComponent::IsContentPackEnabled(const FString& ContentPackId) const
{
    if (ContentPackId.IsEmpty() || DisabledContentPackIds.Contains(ContentPackId))
    {
        return false;
    }

    FHuwamContentPackRow ContentPackRow;
    if (!LoadContentPackRow(ContentPackId, ContentPackRow))
    {
        return EnabledContentPackIds.Contains(ContentPackId);
    }

    return ContentPackRow.bEnabledByDefault || EnabledContentPackIds.Contains(ContentPackId);
}

bool UHuwamLiveContentComponent::IsFeatureFlagEnabled(const FString& FeatureFlagId) const
{
    return FeatureFlagId.IsEmpty() || EnabledFeatureFlagIds.Contains(FeatureFlagId);
}

FHuwamContentAccessResult UHuwamLiveContentComponent::CanUseContentPack(const FString& ContentPackId) const
{
    FHuwamContentAccessResult Result;
    Result.ContentPackId = ContentPackId;

    FHuwamContentPackRow ContentPackRow;
    if (!LoadContentPackRow(ContentPackId, ContentPackRow))
    {
        Result.FailureReason = NSLOCTEXT("HuwamLiveContent", "ContentPackUnknown", "Content pack is not known by the active data registry.");
        return Result;
    }

    Result.bPackKnown = true;
    Result.EntitlementId = ContentPackRow.EntitlementId;
    Result.PackType = ContentPackRow.PackType;
    Result.PackState = ContentPackRow.PackState;
    Result.bServerEnabled = IsContentPackEnabled(ContentPackId);
    Result.bOwned = !ContentPackRow.bRequiresOwnership || HasEntitlement(ContentPackRow.EntitlementId);
    Result.bVersionCompatible = IsVersionCompatible(ContentPackRow.RequiredGameVersion);
    Result.bFeatureFlagsEnabled = AreFeatureFlagsEnabled(ContentPackRow.FeatureFlagIds);

    if (ContentPackRow.PackState != EHuwamContentPackState::Active)
    {
        Result.FailureReason = NSLOCTEXT("HuwamLiveContent", "ContentPackNotActive", "Content pack is not active.");
        return Result;
    }

    if (!Result.bServerEnabled)
    {
        Result.FailureReason = NSLOCTEXT("HuwamLiveContent", "ContentPackServerDisabled", "Content pack is disabled by the server.");
        return Result;
    }

    if (!Result.bVersionCompatible)
    {
        Result.FailureReason = NSLOCTEXT("HuwamLiveContent", "ContentPackVersionMismatch", "Content pack requires a newer game version.");
        return Result;
    }

    if (!Result.bFeatureFlagsEnabled)
    {
        Result.FailureReason = NSLOCTEXT("HuwamLiveContent", "ContentPackFeatureFlagDisabled", "Content pack requires a disabled feature flag.");
        return Result;
    }

    if (!Result.bOwned)
    {
        Result.FailureReason = NSLOCTEXT("HuwamLiveContent", "ContentPackNotOwned", "Content pack entitlement is not owned.");
        return Result;
    }

    Result.bAllowed = true;
    Result.FailureReason = FText::GetEmpty();
    return Result;
}

TArray<FHuwamContentPackRow> UHuwamLiveContentComponent::GetKnownContentPacks() const
{
    TArray<FHuwamContentPackRow> KnownContentPacks;

    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (const UHuwamDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UHuwamDataSubsystem>())
            {
                if (const UHuwamDataRegistry* DataRegistry = DataSubsystem->GetDataRegistry())
                {
                    for (const FName RowName : DataRegistry->GetRowNames(DataRegistry->ContentPacksTable))
                    {
                        FHuwamContentPackRow ContentPackRow;
                        if (DataRegistry->GetContentPackRow(RowName, ContentPackRow))
                        {
                            KnownContentPacks.Add(ContentPackRow);
                        }
                    }
                }
            }
        }
    }

    return KnownContentPacks;
}

TArray<FString> UHuwamLiveContentComponent::GetAccessibleContentPackIds() const
{
    TArray<FString> AccessibleContentPackIds;
    for (const FHuwamContentPackRow& ContentPackRow : GetKnownContentPacks())
    {
        if (CanUseContentPack(ContentPackRow.ContentPackId).bAllowed)
        {
            AccessibleContentPackIds.Add(ContentPackRow.ContentPackId);
        }
    }

    return AccessibleContentPackIds;
}

TArray<FString> UHuwamLiveContentComponent::GetOwnedEntitlementIds() const
{
    return OwnedEntitlementIds;
}

TArray<FString> UHuwamLiveContentComponent::GetEnabledContentPackIds() const
{
    return EnabledContentPackIds;
}

TArray<FString> UHuwamLiveContentComponent::GetEnabledFeatureFlagIds() const
{
    return EnabledFeatureFlagIds;
}

bool UHuwamLiveContentComponent::LoadContentPackRow(const FString& ContentPackId, FHuwamContentPackRow& OutContentPackRow) const
{
    if (ContentPackId.IsEmpty())
    {
        return false;
    }

    if (UWorld* World = GetWorld())
    {
        if (UGameInstance* GameInstance = World->GetGameInstance())
        {
            if (const UHuwamDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UHuwamDataSubsystem>())
            {
                return DataSubsystem->GetContentPackRow(GetRowNameFromId(ContentPackId), OutContentPackRow);
            }
        }
    }

    return false;
}

bool UHuwamLiveContentComponent::IsVersionCompatible(const FString& RequiredGameVersion) const
{
    if (RequiredGameVersion.IsEmpty())
    {
        return true;
    }

    return CompareVersions(CurrentGameVersion, RequiredGameVersion) >= 0;
}

bool UHuwamLiveContentComponent::AreFeatureFlagsEnabled(const TArray<FString>& FeatureFlagIds) const
{
    for (const FString& FeatureFlagId : FeatureFlagIds)
    {
        if (!IsFeatureFlagEnabled(FeatureFlagId))
        {
            return false;
        }
    }

    return true;
}
