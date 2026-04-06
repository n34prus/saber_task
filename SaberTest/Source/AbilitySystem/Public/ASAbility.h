#pragma once

#include "CoreMinimal.h"
#include "PTGameplayTags.h"
#include "ASAbility.generated.h"

UCLASS(Abstract, Blueprintable)
class ABILITYSYSTEMMODULE_API UASAbility : public UObject
{
	GENERATED_BODY()

public:
	void Init(AActor* InOwner);

	UFUNCTION(BlueprintPure, Category="Combat|Ability")
	bool CanActivate() const;

	UFUNCTION(BlueprintCallable, Category="Combat|Ability")
	void TryActivate();

	UFUNCTION(BlueprintPure, Category="Combat|Ability")
	float GetCooldownRemaining() const;

	UFUNCTION(BlueprintPure, Category="Combat|Ability")
	const TArray<FGameplayTag>& GetTags() const;

protected:
	virtual void Activate();

protected:
	UPROPERTY(Transient)
	TObjectPtr<AActor> OwnerActor = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Ability", meta=(ClampMin="0.0"))
	TArray<FGameplayTag> Tags;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat|Ability", meta=(ClampMin="0.0"))
	float Cooldown = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Ability", meta=(ClampMin="0.0"))
	TSoftObjectPtr<UAnimMontage> Animation;

	float LastUseTime = -FLT_MAX;
};