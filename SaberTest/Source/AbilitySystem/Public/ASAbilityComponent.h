#pragma once

#include "CoreMinimal.h"
#include "PTGameplayTags.h"
#include "ASAbilityComponent.generated.h"

/**
 * Component that manages a collection of abilities for an actor.
 * Handles ability lifecycle, activation, and organization.
 * 
 * Features:
 * - Runtime ability addition/removal
 * - Activation by class type or gameplay tag
 * - Automatic initialization of starting abilities
 */

class UASAbility;

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class ABILITYSYSTEMMODULE_API UASAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UASAbilityComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
	bool ActivateAbilityByClass(TSubclassOf<UASAbility> AbilityClass);
	
	UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
	bool ActivateAbilityByTag(FGameplayTag Tag);

	UFUNCTION(BlueprintPure, Category="Combat|Abilities")
	UASAbility* GetAbilityByClass(TSubclassOf<UASAbility> AbilityClass) const;

	UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
	void AddAbilityByClass(TSubclassOf<UASAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category="Combat|Abilities")
	void RemoveAbilityByClass(TSubclassOf<UASAbility> AbilityClass);

protected:
	UPROPERTY(EditAnywhere, Category="Combat|Abilities")
	TArray<TSubclassOf<UASAbility>> StartingAbilities;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UASAbility>> Abilities;
};