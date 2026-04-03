#pragma once

#include "CoreMinimal.h"
#include "CombatAbility.h"
#include "CombatAbility_HitReaction.generated.h"

UCLASS(Blueprintable)
class COMBAT_API UCombatAbility_HitReaction : public UCombatAbility
{
	GENERATED_BODY()

protected:
	UCombatAbility_HitReaction()
	{
		Tags.Add("HitReaction");
	}
	virtual void Activate() override;
};