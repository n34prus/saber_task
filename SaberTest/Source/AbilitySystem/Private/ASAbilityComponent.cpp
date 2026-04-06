#include "ASAbilityComponent.h"
#include "ASAbility.h"

UASAbilityComponent::UASAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UASAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<UASAbility>& AbilityClass : StartingAbilities)
	{
		AddAbilityByClass(AbilityClass);
	}
}

UASAbility* UASAbilityComponent::GetAbilityByClass(TSubclassOf<UASAbility> AbilityClass) const
{
	if (!AbilityClass)
	{
		return nullptr;
	}

	for (UASAbility* Ability : Abilities)
	{
		if (Ability && Ability->GetClass() == AbilityClass)
		{
			return Ability;
		}
	}

	return nullptr;
}

void UASAbilityComponent::AddAbilityByClass(TSubclassOf<UASAbility> AbilityClass)
{
	if (!AbilityClass) return;

	UASAbility* Ability = NewObject<UASAbility>(this, AbilityClass);
	if (!Ability) return;

	Ability->Init(GetOwner());
	Abilities.Add(Ability);
}

void UASAbilityComponent::RemoveAbilityByClass(TSubclassOf<UASAbility> AbilityClass)
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

bool UASAbilityComponent::ActivateAbilityByClass(TSubclassOf<UASAbility> AbilityClass)
{
	UASAbility* Ability = GetAbilityByClass(AbilityClass);
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

bool UASAbilityComponent::ActivateAbilityByTag(FName Tag)
{
	for (UASAbility* Ability : Abilities)
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
