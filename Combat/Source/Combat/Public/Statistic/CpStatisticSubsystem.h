#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/SaveGame.h"
#include "CpStatisticSubsystem.generated.h"

UCLASS()
class COMBAT_API UCpStatisticSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	float TotalDamageAllTime = 0.f;
};

UCLASS()
class COMBAT_API UCpStatisticSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintPure)
	float GetSessionDamageProduced() const { return SessionDamageProduced; }

	UFUNCTION(BlueprintPure)
	float GetTotalDamageProduced() const { return TotalDamageProduced; }

	UFUNCTION(BlueprintPure)
	float GetSessionDamageRecieved() const { return SessionDamageRecieved; }

	UFUNCTION(BlueprintPure)
	float GetTotalDamageRecieved() const { return TotalDamageRecieved; }

	UFUNCTION(BlueprintPure)
	int GetSessionCritCount() const { return SessionCritCount; }

	UFUNCTION(BlueprintPure)
	int GetTotalCritCount() const { return TotalCritCount; }

private:
	
	float SessionDamageProduced = 0.f;
	float TotalDamageProduced = 0.f;

	float SessionDamageRecieved = 0.f;
	float TotalDamageRecieved = 0.f;

	int SessionCritCount = 0;
	int TotalCritCount = 0;

	UPROPERTY()
	TObjectPtr<UCpStatisticSaveGame> SaveObject;

	FString SlotName = TEXT("PlayerStats");
	int32 UserIndex = 0;
	FTimerHandle ScanTimerHandle;
	AActor* PlayerActor;

	void OnActorSpawned(AActor* Actor);

	void BindToNewPlayer(AActor* OldPlayerActor, AActor* NewPlayerActor);
	
	void BindToPlayerDamage(AActor* NewPlayerActor);
	void BindToPlayerModifiers(AActor* NewPlayerActor);
	void UnbindFromPlayerDamage(AActor* OldPlayerActor);
	void UnbindFromPlayerModifiers(AActor* OldPlayerActor);

	UFUNCTION()
	void Scan();
	
	UFUNCTION()
	void HandleDamage(AActor* DamageCauser, AActor* Target, float Value);

	UFUNCTION()
	void HandleModifier(const FName Message, const UObject* Source);
	
	void Load();

	void Save();
};

