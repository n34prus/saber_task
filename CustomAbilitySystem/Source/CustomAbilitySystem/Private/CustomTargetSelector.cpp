#include "CustomTargetSelector.h"

void UCustomTargetSelector::SelectTargets_Implementation(FCustomAbilityExecutionContext& Context)
{
	// Override it in BP/native code

	for (const FName& Tag : RequiredTags)
	{
		if (!Context.AggregatedTags.Contains(Tag))
			return;
	}

	for (const FName& Tag : BlockingTags)
	{
		if (Context.AggregatedTags.Contains(Tag))
			return;
	}
	
}