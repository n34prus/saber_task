#pragma once

#include "CoreMinimal.h"
#include "CpCombatDamagePacket.h"
#include "CpDamageModifier.generated.h"

UCLASS(Abstract, Blueprintable, ClassGroup=(Combat))
class COMBAT_API UCpDamageModifier : public UObject
{
	GENERATED_BODY()

public:
	virtual void ModifyOutgoingDamage(FCombatDamagePacket& Packet, AActor* Target, FName& Msg) const {}
	virtual void ModifyIncomingDamage(FCombatDamagePacket& Packet, AActor* Source, FName& Msg) const {}
};