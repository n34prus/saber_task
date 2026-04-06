#pragma once

#include "CoreMinimal.h"
#include "EBEventBusSubsystem.h"
#include "GameFramework/SaveGame.h"
#include "SSStatisticSubsystem.generated.h"

UCLASS()
class STATISTICSYSTEMMODULE_API UCpStatisticSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FName, float> TotalStats;
};

UCLASS()
class STATISTICSYSTEMMODULE_API USSStatisticSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintPure)
	float GetSessionStat(FName StatName) const;
	UFUNCTION(BlueprintPure)
	float GetTotalStat(FName StatName) const;
	UFUNCTION(BlueprintCallable)
	void ClearTotalStat();

private:

	// todo: use gameplay tags instead
	static constexpr const TCHAR* StatNames[4] = {
		TEXT("DamageProduced"),
		TEXT("DamageRecieved"),
		TEXT("CritCount"),
		TEXT("ExperiencePoints")
	};

	static constexpr bool IsValidStatName(const TCHAR* Name)
	{
		auto StrEq = [](const TCHAR* A, const TCHAR* B)
		{
			while (*A && *B)
			{
				if (*A != *B) return false;
				++A;
				++B;
			}
			return *A == *B;
		};
		for (auto N : StatNames)
		{
			if (StrEq(N, Name)) return true;
		}
		return false;
	}

	UPROPERTY()
	TMap<FName, float> SessionStats;

	UPROPERTY()
	TMap<FName, float> TotalStats;

	UPROPERTY()
	TObjectPtr<UCpStatisticSaveGame> SaveObject;

	FString SlotName = TEXT("PlayerStats");
	int32 UserIndex = 0;
	FTimerHandle ScanTimerHandle;
	AActor* PlayerActor;

	void BindToNewPlayer(AActor* OldPlayerActor, AActor* NewPlayerActor);
	
	void BindToCombatEvents();
	void UnbindFromCombatEvents();

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

