#include "AbilitySystem/GIDamageModifier_Armor.h"
#include "Math/UnrealMathUtility.h"

void UGIDamageModifier_Armor::ModifyIncomingDamage(FASCombatDamagePacket& Packet, AActor* Source, AActor* Target) const
{
	const float EffectiveArmor = FMath::Max(0.f, Armor - Packet.ArmorPenetration);
	Packet.FinalDamage *= 100.f / (100.f + EffectiveArmor);
}