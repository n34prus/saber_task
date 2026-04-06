#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
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
	void BindToPlayerModifiers(AActor* NewPlayerActor);
	void UnbindFromCombatEvents();
	void UnbindFromPlayerModifiers(AActor* OldPlayerActor);
	

	UFUNCTION()
	void Scan();
	
	UFUNCTION()
	void HandleDamage(AActor* DamageCauser, AActor* Target, float Value);

	UFUNCTION()
	void HandleDeath(AActor* DeadActor);

	UFUNCTION()
	void HandleCombatStateChanged(ECpCombatState NewCombatState);

	UFUNCTION()
	void HandleModifier(const FName Message, const UObject* Source);

	UFUNCTION(BlueprintCallable)
	void Load();
	
	UFUNCTION(BlueprintCallable)
	void Save();
};

