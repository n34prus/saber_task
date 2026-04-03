#pragma once

#include "CoreMinimal.h"
#include "CombatAbility.h"
#include "CombatAbility_Dash.generated.h"

UCLASS(Blueprintable)
class COMBAT_API UCombatAbility_Dash : public UCombatAbility
{
	GENERATED_BODY()

protected:
	UCombatAbility_Dash()
	{
		Tags.Add("Dash");
	}
	virtual void Activate() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Combat|Dash", meta=(ClampMin="0.0"))
	float DashStrength = 1200.0f;
};