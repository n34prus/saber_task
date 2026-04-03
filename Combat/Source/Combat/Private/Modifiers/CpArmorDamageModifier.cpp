#include "Modifiers/CpArmorDamageModifier.h"
#include "Math/UnrealMathUtility.h"

void UCpArmorDamageModifier::ModifyIncomingDamage(FCombatDamagePacket& Packet, AActor* Source, FName& Msg) const
{
	const float EffectiveArmor = FMath::Max(0.f, Armor - Packet.ArmorPenetration);
	Packet.FinalDamage *= 100.f / (100.f + EffectiveArmor);
}