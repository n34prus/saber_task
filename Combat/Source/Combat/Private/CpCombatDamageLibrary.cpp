#include "CpCombatDamageLibrary.h"
#include "Modifiers/CpDamageModifierComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Components/ActorComponent.h"

static void ApplyModifiersFromActor(AActor* Actor, bool bOutgoing, FCombatDamagePacket& Packet, AActor* OtherActor)
{
	if (!Actor) return;

	if (UCpDamageModifierComponent* Modifier =	Actor->FindComponentByClass<UCpDamageModifierComponent>())
	{
		if (bOutgoing)
		{
			Modifier->ModifyOutgoingDamage(Packet, OtherActor);
		}
		else
		{
			Modifier->ModifyIncomingDamage(Packet, OtherActor);
		}
	}
}

float UCpCombatDamageLibrary::ApplyCombatDamage(AActor* DamageCauser, AActor* Target, const FCombatDamagePacket& DamagePacket)
{
	if (!Target || DamagePacket.BaseDamage <= 0.f)
	{
		return 0.f;
	}

	FCombatDamagePacket Packet = DamagePacket;
	Packet.FinalDamage = Packet.BaseDamage;

	ApplyModifiersFromActor(DamageCauser, true, Packet, Target);
	ApplyModifiersFromActor(Target, false, Packet, DamageCauser);

	Packet.FinalDamage = FMath::Max(0.f, Packet.FinalDamage);
	if (Packet.FinalDamage <= 0.f)
	{
		return 0.f;
	}

	UGameplayStatics::ApplyDamage(
		Target,
		Packet.FinalDamage,
		nullptr,
		DamageCauser,
		UDamageType::StaticClass()
	);

	OnDealDamage.Broadcast(DamageCauser, Target, Packet.FinalDamage);
	
	return Packet.FinalDamage;
}