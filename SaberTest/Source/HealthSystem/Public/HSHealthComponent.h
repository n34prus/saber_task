#pragma once

#include "HSHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCombatHealthChangedSignature, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCombatDeathSignature, AActor*, DeadActor);

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class HEALTHSYSTEMMODULE_API UHSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHSHealthComponent();

	UFUNCTION(BlueprintCallable, Category="Combat")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category="Combat")
	float GetHealthValue() const;

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool IsDead() const;

	UFUNCTION(BlueprintCallable, Category="Combat")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category="Combat")
	void ResetHealth();

	UPROPERTY(BlueprintAssignable, Category="Combat")
	FCombatHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Combat")
	FCombatDeathSignature OnDeath;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat", meta=(ClampMin="1.0"))
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	float CurrentHealth = 100.f;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void HandleDamage(
		AActor* DamagedActor,
		float Damage,
		const UDamageType* DamageType,
		AController* InstigatedBy,
		AActor* DamageCauser);
};