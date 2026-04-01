#include "CustomTargetSelectorAnimNotify.h"
#include "CustomAbilityComponent.h"
#include "CustomEffect.h"
#include "GameFramework/Actor.h"

void UCustomTargetSelectorAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) return;

	UCustomAbilityComponent* AbilityComp = OwnerActor->FindComponentByClass<UCustomAbilityComponent>();
	if (!AbilityComp) return;
	
	for (UCustomAbility* ActiveAbility : AbilityComp->ActiveAbilities)
	{
		if (!ActiveAbility) continue;
		
		if (!NotifyTag.IsNone() && !ActiveAbility->RequiredTags.Contains(NotifyTag))
			continue;
		
		if (!TargetSelectorClass) continue;

		UCustomTargetSelector* Selector = NewObject<UCustomTargetSelector>(OwnerActor, TargetSelectorClass);
		if (!Selector) continue;

		FCustomAbilityExecutionContext* Context = AbilityComp->GetExecutionContext(ActiveAbility);
		if (!Context) return;
		
		Selector->SelectTargets(*Context);

		// Execute effects on all targets
		for (const FCustomTargetInfo& TargetInfo : Context->Targets)
		{
			if (!TargetInfo.IsValid()) continue;
			
			for (UCustomEffect* Effect : ActiveAbility->Effects)
			{
				if (Effect)
				{
					Effect->Execute(*Context, TargetInfo.Target);
				}
			}
		}
	}
}