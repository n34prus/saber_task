#pragma once

#include "CoreMinimal.h"
#include "ASAbility.h"
#include "GIAbility_HitReaction.generated.h"

UCLASS(Blueprintable)
class GAMEPLAYIMPLEMENTATIONMODULE_API UGIAbility_HitReaction : public UASAbility
{
	GENERATED_BODY()

protected:
	UGIAbility_HitReaction()
	{
		Tags.Add("HitReaction");
	}
	virtual void Activate() override;
};