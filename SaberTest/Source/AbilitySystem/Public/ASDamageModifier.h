#pragma once

#include "CoreMinimal.h"
#include "ASCombatDamagePacket.h"
#include "ASDamageModifier.generated.h"

UCLASS(Abstract, Blueprintable, ClassGroup=(Combat))
class ABILITYSYSTEMMODULE_API UASDamageModifier : public UObject
{
	GENERATED_BODY()

public:
	virtual void ModifyOutgoingDamage(FASCombatDamagePacket& Packet, AActor* Source, AActor* Target) const {}
	virtual void ModifyIncomingDamage(FASCombatDamagePacket& Packet, AActor* Source, AActor* Target) const {}
};