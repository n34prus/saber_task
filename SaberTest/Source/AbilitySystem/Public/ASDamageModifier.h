#pragma once

#include "CoreMinimal.h"
#include "ASCombatDamagePacket.h"
#include "ASDamageModifier.generated.h"

/**
 * Abstract base class for damage modification logic.
 * Allows implementation of various combat mechanics like armor, critical hits, buffs, etc.
 * 
 * Modifiers can affect:
 * - Outgoing damage (from attacker's perspective)
 * - Incoming damage (from defender's perspective)
 * 
 * Used by UASDamageModifierComponent to create modifier chains.
 */

UCLASS(Abstract, Blueprintable, ClassGroup=(Combat))
class ABILITYSYSTEMMODULE_API UASDamageModifier : public UObject
{
	GENERATED_BODY()

public:
	virtual void ModifyOutgoingDamage(FASCombatDamagePacket& Packet, AActor* Source, AActor* Target) const {}
	virtual void ModifyIncomingDamage(FASCombatDamagePacket& Packet, AActor* Source, AActor* Target) const {}
};