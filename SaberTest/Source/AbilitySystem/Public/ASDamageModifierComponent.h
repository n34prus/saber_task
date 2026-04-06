#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASDamageModifier.h"
#include "ASDamageModifierComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDamageModifierMessageSignature, const FName, Message, const UObject*, Source);

UCLASS(Blueprintable, ClassGroup=(Combat))
class ABILITYSYSTEMMODULE_API UASDamageModifierComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	void BeginPlay() override;
	
	UASDamageModifier* AddModifierByClass(TSubclassOf<UASDamageModifier> ModifierClass);

	void RemoveModifierByClass(TSubclassOf<UASDamageModifier> ModifierClass);;
	
	void ModifyOutgoingDamage(FASCombatDamagePacket& Packet, AActor* Target) const;

	void ModifyIncomingDamage(FASCombatDamagePacket& Packet, AActor* Source) const;

	UPROPERTY(BlueprintAssignable, Category="Combat|Modifiers")
	FDamageModifierMessageSignature OnMessageCalled;

protected:

	UPROPERTY(EditAnywhere, Category="Combat|Modifiers")
	TArray<TSubclassOf<UASDamageModifier>> DefaultModifiers;
	
	UPROPERTY(EditAnywhere, Category="Combat|Modifiers")
	TArray<UASDamageModifier*> Modifiers;
};