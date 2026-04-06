#pragma once

#include "CoreMinimal.h"
#include "ASDamageModifierComponent.h"
#include "GIDamageModifier_Crit.generated.h"

UCLASS(ClassGroup=(Combat))
class GAMEPLAYIMPLEMENTATIONMODULE_API UGIDamageModifier_Crit : public UASDamageModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float CriticalChance = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float CriticalMultiplier = 2.f;

	virtual void ModifyOutgoingDamage(FASCombatDamagePacket& Packet, AActor* Source, AActor* Target) const override;
};