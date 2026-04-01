#include "CustomAbility.h"

UCustomAbility::UCustomAbility()
{
	bIsActive = false;
	Cooldown = 0.f;
}

void UCustomAbility::Activate()
{
	if (bIsActive)
		return;

	InternalStart();
}

void UCustomAbility::Deactivate()
{
	if (!bIsActive)
		return;

	InternalEnd();
}

void UCustomAbility::ApplyModifier(const FCustomModifier& Modifier)
{
	Modifiers.Add(Modifier);
	OnModifierApplied.Broadcast(Modifier);
}

void UCustomAbility::InternalStart()
{
	bIsActive = true;
	OnAbilityStarted.Broadcast(this);
	
	for (const FCustomModifier& Mod : Modifiers)
	{
		OnModifierApplied.Broadcast(Mod);
	}
}

void UCustomAbility::InternalEnd()
{
	bIsActive = false;
	OnAbilityEnded.Broadcast(this);
	Modifiers.Empty();
}