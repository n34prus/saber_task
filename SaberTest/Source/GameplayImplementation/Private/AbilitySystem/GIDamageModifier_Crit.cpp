#include "AbilitySystem/GIDamageModifier_Crit.h"
#include "Math/UnrealMathUtility.h"

void UGIDamageModifier_Crit::ModifyOutgoingDamage(FASCombatDamagePacket& Packet, AActor* Target, FName& Msg) const
{
	const float Chance = Packet.CriticalChance > 0.f ? Packet.CriticalChance : CriticalChance;
	const float Multiplier = Packet.CriticalMultiplier > 0.f ? Packet.CriticalMultiplier : CriticalMultiplier;

	if (Chance > 0.f && FMath::FRand() <= Chance)
	{
		Packet.bWasCritical = true;
		Packet.FinalDamage *= Multiplier;

		Msg = "Crit";
	}


}