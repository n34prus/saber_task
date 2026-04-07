#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASDamageModifierComponent.h"
#include "ASAbilityComponent.h"
#include "HSHealthComponent.h"
#include "GICombatCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UHSHealthComponent;

/**
 * Character class with integrated combat systems (can be controlled by AI or Player both).
 * Provides movement, camera control, combat abilities, health, and damage modifiers.
 * 
 * Key Features:
 * - Third-person camera setup with spring arm
 * - Integrated health, ability, and damage modifier components
 * - Input handling for movement and combat actions
 * - Combat state awareness for ability restrictions
 * - Automatic hit reactions on damage
 * - Death handling with ragdoll physics
 * - Controller rotation configuration for proper camera control
 */

UCLASS()
class GAMEPLAYIMPLEMENTATIONMODULE_API AGICombatCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AGICombatCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UHSHealthComponent> HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UASAbilityComponent> AbilityComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UASDamageModifierComponent> ModifierComponent;
    
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