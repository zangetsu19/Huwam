#include "Gameplay/HuwamEconomyLedgerSubsystem.h"

FHuwamEconomyLedgerEntry UHuwamEconomyLedgerSubsystem::RecordServiceRevenue(
    const FString& SourceTag,
    const FString& OwnerNpcId,
    const FString& OwningFactionId,
    const FString& SettlementId,
    int64 AmountCopper,
    int32 InGameDay,
    const FText& Note
)
{
    FHuwamEconomyLedgerEntry Entry;
    if (AmountCopper <= 0)
    {
        return Entry;
    }

    Entry.bRecorded = true;
    Entry.TransactionId = NextTransactionId++;
    Entry.SourceTag = SourceTag;
    Entry.OwnerNpcId = OwnerNpcId;
    Entry.OwningFactionId = OwningFactionId;
    Entry.SettlementId = SettlementId;
    Entry.AmountCopper = AmountCopper;
    Entry.InGameDay = FMath::Max(0, InGameDay);
    Entry.Note = Note;
    SplitRevenueShares(Entry);

    LedgerEntries.Add(Entry);
    CreditAccount(NpcWallets, Entry.OwnerNpcId, EHuwamEconomyAccountType::NpcWallet, Entry.OwnerShareCopper);
    CreditAccount(FactionFunds, Entry.OwningFactionId, EHuwamEconomyAccountType::FactionFund, Entry.FactionShareCopper);
    CreditAccount(SettlementTreasuries, Entry.SettlementId, EHuwamEconomyAccountType::SettlementTreasury, Entry.SettlementShareCopper);
    return Entry;
}

FHuwamEconomyLedgerSnapshot UHuwamEconomyLedgerSubsystem::GetLedgerSnapshot() const
{
    FHuwamEconomyLedgerSnapshot Snapshot;
    Snapshot.TransactionCount = LedgerEntries.Num();
    Snapshot.Entries = LedgerEntries;
    Snapshot.AccountBalances = GetAccountBalances();

    for (const FHuwamEconomyLedgerEntry& Entry : LedgerEntries)
    {
        Snapshot.TotalRevenueCopper += FMath::Max<int64>(0, Entry.AmountCopper);
    }

    return Snapshot;
}

TArray<FHuwamEconomyLedgerEntry> UHuwamEconomyLedgerSubsystem::GetLedgerEntries() const
{
    return LedgerEntries;
}

TArray<FHuwamEconomyAccountBalance> UHuwamEconomyLedgerSubsystem::GetAccountBalances() const
{
    TArray<FHuwamEconomyAccountBalance> Balances;
    NpcWallets.GenerateValueArray(Balances);

    TArray<FHuwamEconomyAccountBalance> FactionBalances;
    FactionFunds.GenerateValueArray(FactionBalances);
    Balances.Append(FactionBalances);

    TArray<FHuwamEconomyAccountBalance> SettlementBalances;
    SettlementTreasuries.GenerateValueArray(SettlementBalances);
    Balances.Append(SettlementBalances);

    return Balances;
}

int64 UHuwamEconomyLedgerSubsystem::GetRevenueCopperForNpc(const FString& OwnerNpcId) const
{
    int64 Total = 0;
    if (OwnerNpcId.IsEmpty())
    {
        return Total;
    }

    for (const FHuwamEconomyLedgerEntry& Entry : LedgerEntries)
    {
        if (Entry.OwnerNpcId == OwnerNpcId)
        {
            Total += FMath::Max<int64>(0, Entry.AmountCopper);
        }
    }

    return Total;
}

int64 UHuwamEconomyLedgerSubsystem::GetRevenueCopperForFaction(const FString& OwningFactionId) const
{
    int64 Total = 0;
    if (OwningFactionId.IsEmpty())
    {
        return Total;
    }

    for (const FHuwamEconomyLedgerEntry& Entry : LedgerEntries)
    {
        if (Entry.OwningFactionId == OwningFactionId)
        {
            Total += FMath::Max<int64>(0, Entry.AmountCopper);
        }
    }

    return Total;
}

int64 UHuwamEconomyLedgerSubsystem::GetRevenueCopperForSettlement(const FString& SettlementId) const
{
    int64 Total = 0;
    if (SettlementId.IsEmpty())
    {
        return Total;
    }

    for (const FHuwamEconomyLedgerEntry& Entry : LedgerEntries)
    {
        if (Entry.SettlementId == SettlementId)
        {
            Total += FMath::Max<int64>(0, Entry.AmountCopper);
        }
    }

    return Total;
}

int64 UHuwamEconomyLedgerSubsystem::GetNpcWalletCopper(const FString& OwnerNpcId) const
{
    return GetAccountBalanceCopper(NpcWallets, OwnerNpcId);
}

int64 UHuwamEconomyLedgerSubsystem::GetFactionFundCopper(const FString& OwningFactionId) const
{
    return GetAccountBalanceCopper(FactionFunds, OwningFactionId);
}

int64 UHuwamEconomyLedgerSubsystem::GetSettlementTreasuryCopper(const FString& SettlementId) const
{
    return GetAccountBalanceCopper(SettlementTreasuries, SettlementId);
}

bool UHuwamEconomyLedgerSubsystem::SpendFromNpcWallet(const FString& OwnerNpcId, int64 AmountCopper)
{
    return SpendFromAccount(NpcWallets, OwnerNpcId, AmountCopper);
}

bool UHuwamEconomyLedgerSubsystem::SpendFromFactionFund(const FString& OwningFactionId, int64 AmountCopper)
{
    return SpendFromAccount(FactionFunds, OwningFactionId, AmountCopper);
}

bool UHuwamEconomyLedgerSubsystem::SpendFromSettlementTreasury(const FString& SettlementId, int64 AmountCopper)
{
    return SpendFromAccount(SettlementTreasuries, SettlementId, AmountCopper);
}

FHuwamEconomyServiceUpkeepResult UHuwamEconomyLedgerSubsystem::PayServiceUpkeep(const FString& OwnerNpcId, const FString& OwningFactionId, const FString& SettlementId, int64 AmountCopper)
{
    FHuwamEconomyServiceUpkeepResult Result;
    Result.RequestedCopper = FMath::Max<int64>(0, AmountCopper);
    Result.OwnerNpcId = OwnerNpcId;
    Result.OwningFactionId = OwningFactionId;
    Result.SettlementId = SettlementId;

    if (Result.RequestedCopper <= 0)
    {
        Result.bSuccess = true;
        return Result;
    }

    if (SpendFromNpcWallet(OwnerNpcId, Result.RequestedCopper))
    {
        Result.bSuccess = true;
        Result.PaidCopper = Result.RequestedCopper;
        Result.PayerAccountId = OwnerNpcId;
        Result.PayerAccountType = EHuwamEconomyAccountType::NpcWallet;
        return Result;
    }

    if (SpendFromFactionFund(OwningFactionId, Result.RequestedCopper))
    {
        Result.bSuccess = true;
        Result.PaidCopper = Result.RequestedCopper;
        Result.PayerAccountId = OwningFactionId;
        Result.PayerAccountType = EHuwamEconomyAccountType::FactionFund;
        return Result;
    }

    if (SpendFromSettlementTreasury(SettlementId, Result.RequestedCopper))
    {
        Result.bSuccess = true;
        Result.PaidCopper = Result.RequestedCopper;
        Result.PayerAccountId = SettlementId;
        Result.PayerAccountType = EHuwamEconomyAccountType::SettlementTreasury;
        return Result;
    }

    return Result;
}

void UHuwamEconomyLedgerSubsystem::ResetLedgerForPrototypeValidation()
{
    LedgerEntries.Reset();
    NextTransactionId = 1;
    NpcWallets.Reset();
    FactionFunds.Reset();
    SettlementTreasuries.Reset();
}

void UHuwamEconomyLedgerSubsystem::RestoreLedgerEntriesForPrototypeValidation(const TArray<FHuwamEconomyLedgerEntry>& SavedEntries)
{
    LedgerEntries = SavedEntries;
    NextTransactionId = 1;

    for (const FHuwamEconomyLedgerEntry& Entry : LedgerEntries)
    {
        NextTransactionId = FMath::Max(NextTransactionId, Entry.TransactionId + 1);
    }
}

void UHuwamEconomyLedgerSubsystem::RestoreAccountBalancesForPrototypeValidation(const TArray<FHuwamEconomyAccountBalance>& SavedBalances)
{
    RestoreAccountMapFromBalances(SavedBalances, EHuwamEconomyAccountType::NpcWallet, NpcWallets);
    RestoreAccountMapFromBalances(SavedBalances, EHuwamEconomyAccountType::FactionFund, FactionFunds);
    RestoreAccountMapFromBalances(SavedBalances, EHuwamEconomyAccountType::SettlementTreasury, SettlementTreasuries);
}

void UHuwamEconomyLedgerSubsystem::RestoreLedgerStateForPrototypeSave(const TArray<FHuwamEconomyLedgerEntry>& SavedEntries, const TArray<FHuwamEconomyAccountBalance>& SavedBalances)
{
    RestoreLedgerEntriesForPrototypeValidation(SavedEntries);
    RestoreAccountBalancesForPrototypeValidation(SavedBalances);
}

void UHuwamEconomyLedgerSubsystem::SplitRevenueShares(FHuwamEconomyLedgerEntry& Entry) const
{
    if (Entry.AmountCopper <= 0)
    {
        return;
    }

    const bool bHasFaction = !Entry.OwningFactionId.IsEmpty();
    const bool bHasSettlement = !Entry.SettlementId.IsEmpty();
    const bool bHasOwner = !Entry.OwnerNpcId.IsEmpty();

    Entry.SettlementShareCopper = bHasSettlement
        ? (Entry.AmountCopper >= 10 ? Entry.AmountCopper / 10 : 1)
        : 0;
    Entry.FactionShareCopper = bHasFaction
        ? (Entry.AmountCopper >= 5 ? FMath::Max<int64>(1, Entry.AmountCopper / 5) : 0)
        : 0;

    const int64 NonOwnerShare = FMath::Clamp<int64>(
        Entry.SettlementShareCopper + Entry.FactionShareCopper,
        0,
        Entry.AmountCopper
    );
    const int64 RemainingShare = Entry.AmountCopper - NonOwnerShare;
    Entry.OwnerShareCopper = bHasOwner ? RemainingShare : 0;

    if (!bHasOwner && RemainingShare > 0)
    {
        if (bHasSettlement)
        {
            Entry.SettlementShareCopper += RemainingShare;
        }
        else if (bHasFaction)
        {
            Entry.FactionShareCopper += RemainingShare;
        }
    }
}

void UHuwamEconomyLedgerSubsystem::CreditAccount(TMap<FString, FHuwamEconomyAccountBalance>& Accounts, const FString& AccountId, EHuwamEconomyAccountType AccountType, int64 AmountCopper)
{
    if (AccountId.IsEmpty() || AmountCopper <= 0)
    {
        return;
    }

    FHuwamEconomyAccountBalance& Account = Accounts.FindOrAdd(AccountId);
    Account.AccountId = AccountId;
    Account.AccountType = AccountType;
    Account.BalanceCopper += AmountCopper;
    Account.TotalIncomeCopper += AmountCopper;
}

bool UHuwamEconomyLedgerSubsystem::SpendFromAccount(TMap<FString, FHuwamEconomyAccountBalance>& Accounts, const FString& AccountId, int64 AmountCopper)
{
    if (AccountId.IsEmpty() || AmountCopper <= 0)
    {
        return false;
    }

    FHuwamEconomyAccountBalance* Account = Accounts.Find(AccountId);
    if (!Account || Account->BalanceCopper < AmountCopper)
    {
        return false;
    }

    Account->BalanceCopper -= AmountCopper;
    Account->TotalSpentCopper += AmountCopper;
    return true;
}

int64 UHuwamEconomyLedgerSubsystem::GetAccountBalanceCopper(const TMap<FString, FHuwamEconomyAccountBalance>& Accounts, const FString& AccountId) const
{
    const FHuwamEconomyAccountBalance* Account = Accounts.Find(AccountId);
    return Account ? Account->BalanceCopper : 0;
}

void UHuwamEconomyLedgerSubsystem::RestoreAccountMapFromBalances(const TArray<FHuwamEconomyAccountBalance>& SavedBalances, EHuwamEconomyAccountType AccountType, TMap<FString, FHuwamEconomyAccountBalance>& OutAccounts)
{
    OutAccounts.Reset();

    for (const FHuwamEconomyAccountBalance& Balance : SavedBalances)
    {
        if (Balance.AccountType == AccountType && !Balance.AccountId.IsEmpty())
        {
            OutAccounts.Add(Balance.AccountId, Balance);
        }
    }
}
