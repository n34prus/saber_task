#include "AbilitySystem/GIDamageModifier_Crit.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include "EBEventBusSubsystem.h"
#include "PTGameplayTags.h"

void UGIDamageModifier_Crit::ModifyOutgoingDamage(FASCombatDamagePacket& Packet, AActor* Source, AActor* Target) const
{
	const float Chance = Packet.CriticalChance > 0.f ? Packet.CriticalChance : CriticalChance;
	const float Multiplier = Packet.CriticalMultiplier > 0.f ? Packet.CriticalMultiplier : CriticalMultiplier;

	if (Chance > 0.f && FMath::FRand() <= Chance)
	{
		Packet.bWasCritical = true;
		Packet.FinalDamage *= Multiplier;

		UWorld* World = GetWorld();
		UGameInstance* GI = World ? World->GetGameInstance() : nullptr;
		if (!GI) return;
		if (UEBEventBusSubsystem* Bus = GI->GetSubsystem<UEBEventBusSubsystem>())
		{
			FEBEventData Event;
			Event.Topic = PluginTags::TAG_Combat_Crit;
			Event.Source = Source;
			Event.Target = Target;
			Bus->Publish(Event);
		}
	}
}