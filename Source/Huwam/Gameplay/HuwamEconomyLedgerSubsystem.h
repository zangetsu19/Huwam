#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HuwamEconomyLedgerSubsystem.generated.h"

UENUM(BlueprintType)
enum class EHuwamEconomyAccountType : uint8
{
    NpcWallet UMETA(DisplayName = "NPC Wallet"),
    FactionFund UMETA(DisplayName = "Faction Fund"),
    SettlementTreasury UMETA(DisplayName = "Settlement Treasury")
};

USTRUCT(BlueprintType)
struct FHuwamEconomyAccountBalance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    FString AccountId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    EHuwamEconomyAccountType AccountType = EHuwamEconomyAccountType::NpcWallet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    int64 BalanceCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    int64 TotalIncomeCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    int64 TotalSpentCopper = 0;
};

USTRUCT(BlueprintType)
struct FHuwamEconomyLedgerEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    bool bRecorded = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    int32 TransactionId = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    FString SourceTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    FString OwnerNpcId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    FString OwningFactionId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    FString SettlementId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    int64 AmountCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    int64 OwnerShareCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    int64 FactionShareCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    int64 SettlementShareCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    int32 InGameDay = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    FText Note;
};

USTRUCT(BlueprintType)
struct FHuwamEconomyLedgerSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    int32 TransactionCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    int64 TotalRevenueCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    TArray<FHuwamEconomyLedgerEntry> Entries;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    TArray<FHuwamEconomyAccountBalance> AccountBalances;
};

USTRUCT(BlueprintType)
struct FHuwamEconomyServiceUpkeepResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    int64 RequestedCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    int64 PaidCopper = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    FString PayerAccountId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    EHuwamEconomyAccountType PayerAccountType = EHuwamEconomyAccountType::NpcWallet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    FString OwnerNpcId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    FString OwningFactionId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Huwam|Economy Ledger")
    FString SettlementId;
};

UCLASS(BlueprintType)
class HUWAM_API UHuwamEconomyLedgerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Huwam|Economy Ledger")
    FHuwamEconomyLedgerEntry RecordServiceRevenue(
        const FString& SourceTag,
        const FString& OwnerNpcId,
        const FString& OwningFactionId,
        const FString& SettlementId,
        int64 AmountCopper,
        int32 InGameDay,
        const FText& Note
    );

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Economy Ledger")
    FHuwamEconomyLedgerSnapshot GetLedgerSnapshot() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Economy Ledger")
    TArray<FHuwamEconomyLedgerEntry> GetLedgerEntries() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Economy Ledger")
    TArray<FHuwamEconomyAccountBalance> GetAccountBalances() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Economy Ledger")
    int64 GetRevenueCopperForNpc(const FString& OwnerNpcId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Economy Ledger")
    int64 GetRevenueCopperForFaction(const FString& OwningFactionId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Economy Ledger")
    int64 GetRevenueCopperForSettlement(const FString& SettlementId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Economy Ledger")
    int64 GetNpcWalletCopper(const FString& OwnerNpcId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Economy Ledger")
    int64 GetFactionFundCopper(const FString& OwningFactionId) const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Huwam|Economy Ledger")
    int64 GetSettlementTreasuryCopper(const FString& SettlementId) const;

    UFUNCTION(BlueprintCallable, Category = "Huwam|Economy Ledger")
    bool SpendFromNpcWallet(const FString& OwnerNpcId, int64 AmountCopper);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Economy Ledger")
    bool SpendFromFactionFund(const FString& OwningFactionId, int64 AmountCopper);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Economy Ledger")
    bool SpendFromSettlementTreasury(const FString& SettlementId, int64 AmountCopper);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Economy Ledger")
    FHuwamEconomyServiceUpkeepResult PayServiceUpkeep(const FString& OwnerNpcId, const FString& OwningFactionId, const FString& SettlementId, int64 AmountCopper);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Economy Ledger")
    void ResetLedgerForPrototypeValidation();

    UFUNCTION(BlueprintCallable, Category = "Huwam|Economy Ledger")
    void RestoreLedgerEntriesForPrototypeValidation(const TArray<FHuwamEconomyLedgerEntry>& SavedEntries);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Economy Ledger")
    void RestoreAccountBalancesForPrototypeValidation(const TArray<FHuwamEconomyAccountBalance>& SavedBalances);

    UFUNCTION(BlueprintCallable, Category = "Huwam|Economy Ledger")
    void RestoreLedgerStateForPrototypeSave(const TArray<FHuwamEconomyLedgerEntry>& SavedEntries, const TArray<FHuwamEconomyAccountBalance>& SavedBalances);

private:
    UPROPERTY()
    TArray<FHuwamEconomyLedgerEntry> LedgerEntries;

    UPROPERTY()
    int32 NextTransactionId = 1;

    UPROPERTY()
    TMap<FString, FHuwamEconomyAccountBalance> NpcWallets;

    UPROPERTY()
    TMap<FString, FHuwamEconomyAccountBalance> FactionFunds;

    UPROPERTY()
    TMap<FString, FHuwamEconomyAccountBalance> SettlementTreasuries;

    void SplitRevenueShares(FHuwamEconomyLedgerEntry& Entry) const;
    void CreditAccount(TMap<FString, FHuwamEconomyAccountBalance>& Accounts, const FString& AccountId, EHuwamEconomyAccountType AccountType, int64 AmountCopper);
    bool SpendFromAccount(TMap<FString, FHuwamEconomyAccountBalance>& Accounts, const FString& AccountId, int64 AmountCopper);
    int64 GetAccountBalanceCopper(const TMap<FString, FHuwamEconomyAccountBalance>& Accounts, const FString& AccountId) const;
    void RestoreAccountMapFromBalances(const TArray<FHuwamEconomyAccountBalance>& SavedBalances, EHuwamEconomyAccountType AccountType, TMap<FString, FHuwamEconomyAccountBalance>& OutAccounts);
};
