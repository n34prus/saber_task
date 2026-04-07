#pragma once

#include "CoreMinimal.h"
#include "EBEventBusSubsystem.h"
#include "PTGameplayTags.h"
#include "CSCombatSubsystem.generated.h"

class UHSHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultyChangedSignature, float, NewDifficulty);

// Combat state flow:
// None -> (player cross trigger) ->
// Init -> (play sfx/ prepare anims/ smth else) ->
// Active -> (broadcast "start" event to combat members -> combat -> some team is dead) -> 
// Finished -> (play sfx/ unequip anims / save statistics) ->
// Resulting -> (show statistic screen, invite to new game) -> None

UENUM(BlueprintType)
enum class ECpCombatState : uint8
{
	CpCombat_None       UMETA(DisplayName = "None"),
	CpCombat_Init        UMETA(DisplayName = "Init"),
	CpCombat_Active      UMETA(DisplayName = "Active"),
	CpCombat_Finished    UMETA(DisplayName = "Finished"),
	CpCombat_Resulting   UMETA(DisplayName = "Resulting")
};

/**
 * Game instance subsystem that manages the overall combat system.
 * Handles combat state transitions, participant registration, damage processing, and difficulty scaling.
 * 
 * Key Responsibilities:
 * - Combat session lifecycle management
 * - Participant registration and tracking
 * - Centralized damage calculation with modifier support
 * - Combat state machine with event broadcasting
 * - Difficulty adjustment system
 */

UCLASS()
class COMBATSYSTEMMODULE_API UCSCombatSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	static inline UCSCombatSubsystem* Get(UObject* WorldContextObject)
	{
		if (!WorldContextObject) return nullptr;

		UGameInstance* GI = WorldContextObject->GetWorld()->GetGameInstance();
		if (!GI) return nullptr;

		return GI->GetSubsystem<UCSCombatSubsystem>();
	}
	
	FOnDifficultyChangedSignature OnDifficultyChanged;
	
	UFUNCTION(BlueprintCallable, Category="Combat|Damage")
	float ApplyCombatDamage(AActor* DamageCauser, AActor* Target, const FASCombatDamagePacket& DamagePacket);
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	static const FGameplayTag GetTagFromState(ECpCombatState State);

	UFUNCTION(BlueprintCallable, Category="Combat")
	FORCEINLINE ECpCombatState GetCombatState() const { return CombatState; }

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool StartCombat();

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool StopCombat();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void ResetCombat();

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool RegisterMember(AActor* Member);

	UFUNCTION(BlueprintCallable, Category="Combat")
    bool UnregisterMember(AActor* Member);

	UFUNCTION(BlueprintPure, Category="Combat")
	bool IsMemberRegistered(AActor* Member);

	UFUNCTION(BlueprintCallable, Category="Combat")
	void SetDifficulty(float NewDifficulty);
	
	UFUNCTION(BlueprintPure, Category="Combat")
	float GetDifficulty();
	
protected:
	
	void SetCombatState(ECpCombatState NewState);

	UFUNCTION()
	void OnCombatStateChanged_Implementation(ECpCombatState NewState);
	
	float Difficulty = 0.0f;
	
	void CheckFinishCondition();	// hardcode now, but can be reworked as functor

	void BindToBusEvents();
	void UnbindFromBusEvents();

	void HandleDeath(const FEBEventData& Event);
	
	ECpCombatState CombatState = ECpCombatState::CpCombat_None;

	TSet<AActor*> CombatMembers;
	AActor* PlayerActor = nullptr;
};