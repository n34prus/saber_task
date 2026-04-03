#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "CpCombatCharacter.h"
#include "CpCombatAIController.generated.h"

UCLASS()
class COMBAT_API ACpCombatAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACpCombatAIController()
	{
		//PrimaryActorTick.bCanEverTick = true;
	}
	virtual void OnUnPossess() override
	{
		Super::OnUnPossess();
		Destroy();
	}
	
protected:
	
	virtual void Tick(float DeltaSeconds) override
	{
		Super::Tick(DeltaSeconds);

		ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		APawn* ControlledPawn = GetPawn();

		if (!Player || !ControlledPawn)
		{
			return;
		}

		const FVector MyLoc = ControlledPawn->GetActorLocation();
		const FVector PlayerLoc = Player->GetActorLocation();

		FVector ToPlayer = PlayerLoc - MyLoc;
		const float Dist = ToPlayer.Size();
		
		ToPlayer.Z = 0.f;
		ToPlayer.Normalize();

		ControlledPawn->SetActorRotation(ToPlayer.Rotation());

		// ring
		if (Dist >= InnerRadius && Dist <= OuterRadius)
		{
			bWasInner = false;
			return;
		}

		// outer
		if (Dist > OuterRadius)
		{
			const FVector TargetPos = PlayerLoc - ToPlayer * ((InnerRadius + OuterRadius) * 0.5f);
			ControlledPawn->AddMovementInput(ToPlayer);
		}
		// inner
		else if (Dist < InnerRadius)
		{
			const FVector TargetPos = PlayerLoc - ToPlayer * OuterRadius;
			ControlledPawn->AddMovementInput(-ToPlayer);

			float RandVal = bWasInner ? 1.f : FMath::FRand();
			if (RandVal < ChanceToAtack)
			{
				UCombatAbilityComponent* AbilityComponent = ControlledPawn->FindComponentByClass<UCombatAbilityComponent>();
				if (AbilityComponent)
				{
					AbilityComponent->ActivateAbilityByTag("Attack");
				}
			}
			bWasInner = true;
		}
	}

protected:

	UPROPERTY(EditAnywhere, Category="AI")
	float InnerRadius = 200.f;
	
	UPROPERTY(EditAnywhere, Category="AI")
	float OuterRadius = 300.f;
	
	UPROPERTY(EditAnywhere, Category="AI")
	float AcceptanceRadius = 25.f;

	UPROPERTY(EditAnywhere, Category="AI")
	float ChanceToAtack = .5f;

private:
	bool bWasInner = false;
};