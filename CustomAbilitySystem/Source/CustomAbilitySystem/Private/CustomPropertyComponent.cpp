#include "CustomPropertyComponent.h"

UCustomPropertyComponent::UCustomPropertyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UCustomPropertyComponent::GetFinalStat(FName StatName) const
{
	float BaseValue = BaseStats.Contains(StatName) ? BaseStats[StatName] : 0.f;
	float AddSum = 0.f;
	float MulProd = 1.f;

	for (const FCustomModifier& Mod : ActiveModifiers)
	{
		if (Mod.Stat != StatName) continue;

		if (Mod.Op == ECustomModifierOp::CmOp_Add)
			AddSum += Mod.Value;
		else if (Mod.Op == ECustomModifierOp::CmOp_Multiply)
			MulProd *= Mod.Value;
	}

	return (BaseValue + AddSum) * MulProd;
}

void UCustomPropertyComponent::ApplyModifiers(const TArray<FCustomModifier>& Modifiers)
{
	for (const FCustomModifier& Mod : Modifiers)
	{
		ActiveModifiers.Add(Mod);
		
		if (Mod.Stat == FName("Health"))
		{
			float NewHealth = GetFinalStat(FName("Health"));
			OnHealthChanged.Broadcast(NewHealth);

			if (Mod.Op == ECustomModifierOp::CmOp_Add && Mod.Value < 0.f)
				OnDamageTaken.Broadcast(-Mod.Value);
		}
	}
}