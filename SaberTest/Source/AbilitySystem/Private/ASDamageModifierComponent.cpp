#include "ASDamageModifierComponent.h"

void UASDamageModifierComponent::BeginPlay()
{
	Super::BeginPlay();
	for (auto Class : DefaultModifiers)
	{
		AddModifierByClass(Class);
	}
}

UASDamageModifier* UASDamageModifierComponent::AddModifierByClass(TSubclassOf<UASDamageModifier> ModifierClass)
{
	UASDamageModifier* NewMod = NewObject<UASDamageModifier>(this, ModifierClass);
	Modifiers.Add(NewMod);
	return NewMod;
}

void UASDamageModifierComponent::RemoveModifierByClass(TSubclassOf<UASDamageModifier> ModifierClass)
{
	for (int32 i = Modifiers.Num() - 1; i >= 0; --i)
	{
		if (Modifiers[i]->GetClass() == ModifierClass)
		{
			Modifiers.RemoveAt(i);
		}
	}
}

void UASDamageModifierComponent::ModifyOutgoingDamage(FASCombatDamagePacket& Packet, AActor* Source, AActor* Target) const
{
	for (auto Modifier : Modifiers)
	{
		if (!Modifier) continue;
		Modifier->ModifyOutgoingDamage(Packet, Source, Target);
	}
}

void UASDamageModifierComponent::ModifyIncomingDamage(FASCombatDamagePacket& Packet, AActor* Source, AActor* Target) const
{
	for (auto Modifier : Modifiers)
	{
		if (!Modifier) continue;
		Modifier->ModifyIncomingDamage(Packet, Source, Target);
	}
}
