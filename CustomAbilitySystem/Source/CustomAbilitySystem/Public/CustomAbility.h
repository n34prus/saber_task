#pragma once

#include "CoreMinimal.h"
#include "CustomModifier.h"
#include "CustomAbility.generated.h"

class UCustomEffect;
class UCustomTargetSelector;
struct FCustomAbilityExecutionContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityStarted, UObject*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityEnded, UObject*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModifierApplied, const FCustomModifier&, Modifier);

UCLASS(Blueprintable, BlueprintType)
class CUSTOMABILITYSYSTEM_API UCustomAbility : public UObject
{
	GENERATED_BODY()

public:
	UCustomAbility();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	FName AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	float Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	TMap<FName, float> Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	TArray<UCustomEffect*> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	TSubclassOf<UCustomTargetSelector> TargetSelectorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	bool bSelectTargetsOnNotify = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	TArray<FCustomModifier> Modifiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	UAnimMontage* MontageToPlay;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ability")
	bool bIsActive;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability|Tags")
	TSet<FName> RequiredTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability|Tags")
	TSet<FName> BlockingTags;

	ECustomAbilityState State;
	
	UPROPERTY(BlueprintAssignable, Category="Ability|Events")
	FOnAbilityStarted OnAbilityStarted;

	UPROPERTY(BlueprintAssignable, Category="Ability|Events")
	FOnAbilityEnded OnAbilityEnded;

	UPROPERTY(BlueprintAssignable, Category="Ability|Events")
	FOnModifierApplied OnModifierApplied;

public:

	UFUNCTION(BlueprintCallable, Category="Ability")
	void Activate();

	UFUNCTION(BlueprintCallable, Category="Ability")
	void Deactivate();

	UFUNCTION(BlueprintCallable, Category="Ability")
	void ApplyModifier(const FCustomModifier& Modifier);
	
	UFUNCTION(BlueprintCallable)
	void StartExecution(FCustomAbilityExecutionContext& Context)
	{
		State = ECustomAbilityState::CaState_Casting;
		OnAbilityStarted.Broadcast(this);

		// notifies etc
	}

	UFUNCTION(BlueprintCallable)
	void Interrupt(FCustomAbilityExecutionContext& Context)
	{
		State = ECustomAbilityState::CaState_Interrupted;
		OnAbilityEnded.Broadcast(this);

		// clear context etc
	}

	UFUNCTION(BlueprintCallable)
	ECustomAbilityState GetState() const
	{
		return State;
	}

private:
	void InternalStart();
	void InternalEnd();
};