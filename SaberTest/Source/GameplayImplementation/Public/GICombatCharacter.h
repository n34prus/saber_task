#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASDamageModifierComponent.h"
#include "ASAbilityComponent.h"
#include "GICombatCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UHSHealthComponent;

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