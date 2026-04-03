#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CpDamageModifier.h"
#include "CpDamageModifierComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDamageModifierMessageSignature, const FName, Message, const UObject*, Source);

UCLASS(Blueprintable, ClassGroup=(Combat))
class COMBAT_API UCpDamageModifierComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void BeginPlay() override;
	
	UCpDamageModifier* AddModifierByClass(TSubclassOf<UCpDamageModifier> ModifierClass);

	void RemoveModifierByClass(TSubclassOf<UCpDamageModifier> ModifierClass);;
	
	void ModifyOutgoingDamage(FCombatDamagePacket& Packet, AActor* Target) const;

	void ModifyIncomingDamage(FCombatDamagePacket& Packet, AActor* Source) const;

	UPROPERTY(BlueprintAssignable, Category="Combat|Modifiers")
	FDamageModifierMessageSignature OnMessageCalled;

protected:

	UPROPERTY(EditAnywhere, Category="Combat|Modifiers")
	TArray<TSubclassOf<UCpDamageModifier>> DefaultModifiers;
	
	UPROPERTY(EditAnywhere, Category="Combat|Modifiers")
	TArray<UCpDamageModifier*> Modifiers;
};