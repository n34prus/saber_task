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

void UASDamageModifierComponent::ModifyOutgoingDamage(FASCombatDamagePacket& Packet, AActor* Target) const
{
	for (auto Modifier : Modifiers)
	{
		if (!Modifier) continue;
		FName Msg = NAME_None;
		Modifier->ModifyOutgoingDamage(Packet, Target, Msg);
		if (Msg != NAME_None) OnMessageCalled.Broadcast(Msg, this);
	}
}

void UASDamageModifierComponent::ModifyIncomingDamage(FASCombatDamagePacket& Packet, AActor* Source) const
{
	for (auto Modifier : Modifiers)
	{
		if (!Modifier) continue;
		FName Msg = NAME_None;
		Modifier->ModifyIncomingDamage(Packet, Source, Msg);
		if (Msg != NAME_None) OnMessageCalled.Broadcast(Msg, this);
	}
}
