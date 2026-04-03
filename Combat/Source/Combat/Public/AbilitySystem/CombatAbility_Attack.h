#pragma once

#include "CoreMinimal.h"
#include "CombatAbility.h"
#include "CombatAbility_Attack.generated.h"

UCLASS(Blueprintable)
class COMBAT_API UCombatAbility_Attack : public UCombatAbility
{
	GENERATED_BODY()
public:
	UCombatAbility_Attack()
	{
		Cooldown = 0.3f;
		Tags.Add("Attack");
	}
	
protected:
	virtual void Activate() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat|Attack", meta=(ClampMin="1.0"))
	float BaseAttackDamage = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat|Attack", meta=(ClampMin="10.0"))
	float ProbeRadius = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat|Attack", meta=(ClampMin="10.0"))
	float AttackRange = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat|Attack", meta=(ClampMin="0.0", ClampMax="1.0"))
	float AttackCriticalChance = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat|Attack", meta=(ClampMin="1.0"))
	float AttackCriticalMultiplier = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat|Attack", meta=(ClampMin="0.0"))
	float AttackArmorPenetration = 0.f;
};