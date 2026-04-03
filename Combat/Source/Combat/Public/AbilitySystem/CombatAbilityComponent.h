#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatAbilityComponent.generated.h"

class UCombatAbility;

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class COMBAT_API UCombatAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatAbilityComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
	bool ActivateAbilityByClass(TSubclassOf<UCombatAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
	bool ActivateAbilityByTag(FName Tag);

	UFUNCTION(BlueprintPure, Category="Combat|Abilities")
	UCombatAbility* GetAbilityByClass(TSubclassOf<UCombatAbility> AbilityClass) const;

	UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
	void AddAbilityByClass(TSubclassOf<UCombatAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
	void RemoveAbilityByClass(TSubclassOf<UCombatAbility> AbilityClass);

protected:
	UPROPERTY(EditAnywhere, Category="Combat|Abilities")
	TArray<TSubclassOf<UCombatAbility>> StartingAbilities;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UCombatAbility>> Abilities;
};