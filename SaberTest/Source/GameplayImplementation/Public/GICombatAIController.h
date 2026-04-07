#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GICombatAIController.generated.h"

/**
 * Simple AI controller implementing combat behavior around the player.
 * Creates engaging combat by maintaining distance and attacking at appropriate intervals.
 * 
 * Key Features:
 * - Distance-based positioning (inner radius, outer radius)
 * - Dynamic movement behavior based on distance to player
 * - Random attack timing with configurable probability
 * - Combat state awareness for attack restrictions
 * - Automatic cleanup on unpossess
 */

UCLASS()
class GAMEPLAYIMPLEMENTATIONMODULE_API AGICombatAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGICombatAIController()
	{
		//PrimaryActorTick.bCanEverTick = true;
	}
	virtual void OnUnPossess() override;

protected:
	
	virtual void Tick(float DeltaSeconds) override;

protected:

	UPROPERTY(EditAnywhere, Category="AI")
	float InnerRadius = 150.f;
	
	UPROPERTY(EditAnywhere, Category="AI")
	float OuterRadius = 300.f;
	
	UPROPERTY(EditAnywhere, Category="AI")
	float AcceptanceRadius = 25.f;

	UPROPERTY(EditAnywhere, Category="AI")
	float ChanceToAtack = .3f;

private:
	bool bWasInner = false;
};