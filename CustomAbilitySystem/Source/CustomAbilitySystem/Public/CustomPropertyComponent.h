#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomModifier.h"
#include "CustomPropertyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageTaken, float, DamageAmount);

UCLASS(ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class CUSTOMABILITYSYSTEM_API UCustomPropertyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCustomPropertyComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	TMap<FName, float> BaseStats;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="Properties")
	TArray<FCustomModifier> ActiveModifiers;
	
	UPROPERTY(BlueprintAssignable, Category="Properties")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Properties")
	FOnDamageTaken OnDamageTaken;

public:

	UFUNCTION(BlueprintCallable, Category="Properties")
	float GetFinalStat(FName StatName) const;
	
	UFUNCTION(BlueprintCallable, Category="Properties")
	void ApplyModifiers(const TArray<FCustomModifier>& Modifiers);
	
	const TMap<FName, float>& GetAllStats() const { return BaseStats; }
};