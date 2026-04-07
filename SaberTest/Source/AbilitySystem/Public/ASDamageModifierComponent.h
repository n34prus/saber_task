#pragma once

#include "CoreMinimal.h"
#include "ASDamageModifier.h"
#include "ASDamageModifierComponent.generated.h"

/**
 * Component that manages a collection of damage modifiers for an actor.
 * Processes damage through all registered modifiers in sequence.
 * 
 * Supports:
 * - Automatic initialization of default modifiers
 * - Runtime modifier addition/removal
 * - Separate processing for outgoing and incoming damage
 */

UCLASS(Blueprintable, ClassGroup=(Combat))
class ABILITYSYSTEMMODULE_API UASDamageModifierComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
	UASDamageModifier* AddModifierByClass(TSubclassOf<UASDamageModifier> ModifierClass);

	void RemoveModifierByClass(TSubclassOf<UASDamageModifier> ModifierClass);;
	
	void ModifyOutgoingDamage(FASCombatDamagePacket& Packet, AActor* Source, AActor* Target) const;

	void ModifyIncomingDamage(FASCombatDamagePacket& Packet, AActor* Source, AActor* Target) const;

protected:

	UPROPERTY(EditAnywhere, Category="Combat|Modifiers")
	TArray<TSubclassOf<UASDamageModifier>> DefaultModifiers;
	
	UPROPERTY(EditAnywhere, Category="Combat|Modifiers")
	TArray<UASDamageModifier*> Modifiers;
};