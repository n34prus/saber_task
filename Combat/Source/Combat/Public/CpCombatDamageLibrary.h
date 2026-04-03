#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CpCombatDamageTypes.h"
#include "CpCombatDamageLibrary.generated.h"

// DamageCauser, Target, Value
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnDealDamage, AActor*, AActor*, float);

UCLASS()
class COMBAT_API UCpCombatDamageLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	inline static FOnDealDamage OnDealDamage;
	
	UFUNCTION(BlueprintCallable, Category="Combat|Damage")
	static float ApplyCombatDamage(AActor* DamageCauser, AActor* Target, const FCombatDamagePacket& DamagePacket);
};