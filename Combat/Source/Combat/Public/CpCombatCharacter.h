#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CpCombatDamageTypes.h"
#include "Modifiers/CpDamageModifierComponent.h"
#include "AbilitySystem/CombatAbilityComponent.h"
#include "CpCombatCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCpCombatHealthComponent;

UCLASS()
class COMBAT_API ACpCombatCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ACpCombatCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UCpCombatHealthComponent> HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UCombatAbilityComponent> AbilityComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UCpDamageModifierComponent> ModifierComponent;

    /*
    UPROPERTY(EditDefaultsOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
    TSoftClassPtr<UCombatAbility> AttackAbility;

    UPROPERTY(EditDefaultsOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
    TSoftClassPtr<UCombatAbility> DashAbility;

    UPROPERTY(EditDefaultsOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
    TSoftClassPtr<UCombatAbility> HitReactionAbility;
    */

    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);

    void Attack();
    void Dash();
    
    UFUNCTION()
    void OnDeath(AActor* DeadActor);
    UFUNCTION()
    void OnHealthChanged(float NewHealth, float Delta);
};