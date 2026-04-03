#pragma once

#include "CoreMinimal.h"
#include "CpDamageModifierComponent.h"
#include "CpCritDamageModifier.generated.h"

UCLASS(ClassGroup=(Combat))
class COMBAT_API UCpCritDamageModifier : public UCpDamageModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float CriticalChance = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float CriticalMultiplier = 2.f;

	virtual void ModifyOutgoingDamage(FCombatDamagePacket& Packet, AActor* Target, FName& Msg) const override;
};