#pragma once

#include "CoreMinimal.h"
#include "ASDamageModifierComponent.h"
#include "GIDamageModifier_Armor.generated.h"

UCLASS(ClassGroup=(Combat))
class GAMEPLAYIMPLEMENTATIONMODULE_API UGIDamageModifier_Armor : public UASDamageModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float Armor = 0.f;
	
	virtual void ModifyIncomingDamage(FASCombatDamagePacket& Packet, AActor* Source, AActor* Target) const override;
};