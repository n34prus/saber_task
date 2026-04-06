#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GICombatAIController.generated.h"

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