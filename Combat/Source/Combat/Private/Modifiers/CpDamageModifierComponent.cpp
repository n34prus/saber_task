#include "Modifiers/CpDamageModifierComponent.h"

void UCpDamageModifierComponent::BeginPlay()
{
	Super::BeginPlay();
	for (auto Class : DefaultModifiers)
	{
		AddModifierByClass(Class);
	}
}

UCpDamageModifier* UCpDamageModifierComponent::AddModifierByClass(TSubclassOf<UCpDamageModifier> ModifierClass)
{
	UCpDamageModifier* NewMod = NewObject<UCpDamageModifier>(this, ModifierClass);
	Modifiers.Add(NewMod);
	return NewMod;
}

void UCpDamageModifierComponent::RemoveModifierByClass(TSubclassOf<UCpDamageModifier> ModifierClass)
{
	for (int32 i = Modifiers.Num() - 1; i >= 0; --i)
	{
		if (Modifiers[i]->GetClass() == ModifierClass)
		{
			Modifiers.RemoveAt(i);
		}
	}
}

void UCpDamageModifierComponent::ModifyOutgoingDamage(FCombatDamagePacket& Packet, AActor* Target) const
{
	for (auto Modifier : Modifiers)
	{
		if (!Modifier) continue;
		FName Msg = NAME_None;
		Modifier->ModifyOutgoingDamage(Packet, Target, Msg);
		if (Msg != NAME_None) OnMessageCalled.Broadcast(Msg, this);
	}
}

void UCpDamageModifierComponent::ModifyIncomingDamage(FCombatDamagePacket& Packet, AActor* Source) const
{
	for (auto Modifier : Modifiers)
	{
		if (!Modifier) continue;
		FName Msg = NAME_None;
		Modifier->ModifyIncomingDamage(Packet, Source, Msg);
		if (Msg != NAME_None) OnMessageCalled.Broadcast(Msg, this);
	}
}
