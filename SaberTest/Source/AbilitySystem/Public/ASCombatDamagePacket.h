#pragma once

#include "CoreMinimal.h"
#include "ASCombatDamagePacket.generated.h"

USTRUCT(BlueprintType)
struct FASCombatDamagePacket
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float BaseDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float FinalDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float CriticalChance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float CriticalMultiplier = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float ArmorPenetration = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	bool bWasCritical = false;

	FASCombatDamagePacket() = default;

	explicit FASCombatDamagePacket(float InBaseDamage)
		: BaseDamage(InBaseDamage), FinalDamage(InBaseDamage)
	{
	}
};