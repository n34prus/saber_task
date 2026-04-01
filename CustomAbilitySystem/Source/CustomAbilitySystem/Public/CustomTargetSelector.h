#pragma once

#include "CoreMinimal.h"
//#include "FCustomTargetInfo.h"
//#include "CustomAbilityExecutionContext.h"
//#include "UObject/NoExportTypes.h"
#include "CustomAbilityExecutionContext.h"
#include "CustomTargetSelector.generated.h"

UCLASS(Blueprintable, BlueprintType, Abstract)
class CUSTOMABILITYSYSTEM_API UCustomTargetSelector : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetSelector")
	TSet<FName> RequiredTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetSelector")
	TSet<FName> BlockingTags;

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="TargetSelector")
	void SelectTargets(FCustomAbilityExecutionContext& Context);
	// selection logic must be realized in child
	virtual void SelectTargets_Implementation(FCustomAbilityExecutionContext& Context);
};