#pragma once

#include "CoreMinimal.h"
#include "ASAbilityComponent.generated.h"

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
	bool ActivateAbilityByTag(FName Tag);

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