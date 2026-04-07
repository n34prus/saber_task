#pragma once

#include "CoreMinimal.h"
#include "EBEventBusSubsystem.h"
#include "PTGameplayTags.h"
#include "GameFramework/SaveGame.h"
#include "SSStatisticSubsystem.generated.h"

UCLASS()
class STATISTICSYSTEMMODULE_API UCpStatisticSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FGameplayTag, float> TotalStats;
};

/**
 * Game instance subsystem for tracking and persisting player statistics.
 * Monitors game events and automatically updates relevant statistics.
 * 
 * Key Features:
 * - Session-based and total (persistent) statistics tracking
 * - Automatic event monitoring via event bus integration
 * - Save/load functionality for persistent statistics
 * - Player-specific statistic filtering
 * - Combat event integration (damage, death, critical hits)
 */

UCLASS()
class STATISTICSYSTEMMODULE_API USSStatisticSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void AddValueToSessionStat(FGameplayTag StatTag, float Value);
	UFUNCTION(BlueprintCallable)
	void AddValueToTotalStat(FGameplayTag StatTag, float Value);
	UFUNCTION(BlueprintPure)
	float GetSessionStat(FGameplayTag StatTag) const;
	UFUNCTION(BlueprintPure)
	float GetTotalStat(FGameplayTag StatTag) const;
	UFUNCTION(BlueprintCallable)
	void ClearStats();

private:
	
	UPROPERTY()
	TMap<FGameplayTag, float> SessionStats;

	UPROPERTY()
	TMap<FGameplayTag, float> TotalStats;

	UPROPERTY()
	TObjectPtr<UCpStatisticSaveGame> SaveObject;

	FString SlotName = TEXT("PlayerStats");
	int32 UserIndex = 0;
	FTimerHandle ScanTimerHandle;
	AActor* PlayerActor;

	void UpdateBindings();
	
	void BindToBusEvents();
	void UnbindFromBusEvents();

	UFUNCTION()
	void Scan();
	
	void HandleDamage(const FEBEventData& Event);
	void HandleDeath(const FEBEventData& Event);
	void HandleCombatStateChanged(const FEBEventData& Event);
	void HandleCrit(const FEBEventData& Event);

	UFUNCTION(BlueprintCallable)
	void Load();
	
	UFUNCTION(BlueprintCallable)
	void Save();
};

