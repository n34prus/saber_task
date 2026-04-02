#pragma once

#include "CustomTargetSelector.h"
#include "CustomTargetSelector_Impl.generated.h"

UCLASS()
class UCustomTargetSelector_Closest : public UCustomTargetSelector
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRange = 2000.f;

	virtual void SelectTargets_Implementation(FCustomAbilityExecutionContext& Context) override;
};