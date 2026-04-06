#pragma once

#include "CoreMinimal.h"
#include "ASAbility.h"
#include "GIAbility_Dash.generated.h"

UCLASS(Blueprintable)
class GAMEPLAYIMPLEMENTATIONMODULE_API UGIAbility_Dash : public UASAbility
{
	GENERATED_BODY()

protected:
	UGIAbility_Dash()
	{
		Tags.Add(PluginTags::TAG_Ability_Dash);
	}
	virtual void Activate() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Combat|Dash", meta=(ClampMin="0.0"))
	float DashStrength = 1200.0f;
};