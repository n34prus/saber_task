#include "AbilitySystem/CombatAbilityComponent.h"
#include "AbilitySystem/CombatAbility.h"

UCombatAbilityComponent::UCombatAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<UCombatAbility>& AbilityClass : StartingAbilities)
	{
		AddAbilityByClass(AbilityClass);
	}
}

UCombatAbility* UCombatAbilityComponent::GetAbilityByClass(TSubclassOf<UCombatAbility> AbilityClass) const
{
	if (!AbilityClass)
	{
		return nullptr;
	}

	for (UCombatAbility* Ability : Abilities)
	{
		if (Ability && Ability->GetClass() == AbilityClass)
		{
			return Ability;
		}
	}

	return nullptr;
}

void UCombatAbilityComponent::AddAbilityByClass(TSubclassOf<UCombatAbility> AbilityClass)
{
	if (!AbilityClass) return;

	UCombatAbility* Ability = NewObject<UCombatAbility>(this, AbilityClass);
	if (!Ability) return;

	Ability->Init(GetOwner());
	Abilities.Add(Ability);
}

void UCombatAbilityComponent::RemoveAbilityByClass(TSubclassOf<UCombatAbility> AbilityClass)
{
	for (int32 i = 0; i < Abilities.Num(); ++i)
	{
		if (Abilities[i] && Abilities[i]->GetClass() == AbilityClass)
		{
			Abilities.RemoveAtSwap(i);
			break;
		}
	}
}

bool UCombatAbilityComponent::ActivateAbilityByClass(TSubclassOf<UCombatAbility> AbilityClass)
{
	UCombatAbility* Ability = GetAbilityByClass(AbilityClass);
	if (!Ability)
	{
		return false;
	}

	const bool bCanActivate = Ability->CanActivate();
	if (bCanActivate)
	{
		Ability->TryActivate();
	}

	return bCanActivate;
}

bool UCombatAbilityComponent::ActivateAbilityByTag(FName Tag)
{
	for (UCombatAbility* Ability : Abilities)
	{
		if (Ability && Ability->GetTags().Contains(Tag))
		{
			const bool bCanActivate = Ability->CanActivate();
			if (bCanActivate)
			{
				Ability->TryActivate();
			}

			return bCanActivate;
		}
	}

	return false;
}
