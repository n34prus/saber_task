#include "AbilitySystem/CombatAbility_Dash.h"

#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

void UCombatAbility_Dash::Activate()
{
	Super::Activate();
	
	ACharacter* Character = Cast<ACharacter>(OwnerActor);
	if (!Character)
	{
		return;
	}

	FVector Dir = Character->GetMovementComponent()->GetLastInputVector();
	Dir.Z = 0.0f;
	if (!Dir.IsNearlyZero())
	{
		Dir.Normalize();
		Character->SetActorRotation(Dir.Rotation());
	}
	
	//const FVector BackDirection = -Character->GetActorForwardVector();
	//Character->LaunchCharacter(BackDirection * DashStrength, true, true);
}
