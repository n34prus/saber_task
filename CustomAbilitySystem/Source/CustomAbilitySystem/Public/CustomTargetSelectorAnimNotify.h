#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CustomTargetSelector.h"
//#include "CustomAbilityExecutionContext.h"
#include "CustomTargetSelectorAnimNotify.generated.h"

UCLASS()
class CUSTOMABILITYSYSTEM_API UCustomTargetSelectorAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetSelector")
	TSubclassOf<UCustomTargetSelector> TargetSelectorClass;

	// only abils with this tag will be triggered
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	FName NotifyTag;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};