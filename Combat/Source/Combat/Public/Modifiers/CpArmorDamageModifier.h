#pragma once

#include "CoreMinimal.h"
#include "CpDamageModifierComponent.h"
#include "CpArmorDamageModifier.generated.h"

UCLASS(ClassGroup=(Combat))
class COMBAT_API UCpArmorDamageModifier : public UCpDamageModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float Armor = 0.f;
	
	virtual void ModifyIncomingDamage(FCombatDamagePacket& Packet, AActor* Source, FName& Msg) const override;
};