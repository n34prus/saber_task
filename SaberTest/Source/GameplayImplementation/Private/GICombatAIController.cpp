#include "GICombatAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GICombatCharacter.h"
#include "CSCombatSubsystem.h"

void AGICombatAIController::OnUnPossess()
{
	Super::OnUnPossess();
	Destroy();
}

void AGICombatAIController::Tick(float DeltaSeconds)
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
		float Mux = bWasInner ? -1.f : 0.2f;
		ControlledPawn->AddMovementInput(ToPlayer, Mux);
		return;
	}

	// outer
	if (Dist > OuterRadius)
	{
		bWasInner = false;
		ControlledPawn->AddMovementInput(ToPlayer);
	}
	// inner
	else if (Dist < InnerRadius)
	{
		ControlledPawn->AddMovementInput(ToPlayer, -0.75f);

		float RandVal = bWasInner ? 1.f : FMath::FRand();
		if (RandVal < ChanceToAtack)
		{
			UASAbilityComponent* AbilityComponent = ControlledPawn->FindComponentByClass<UASAbilityComponent>();
			if (AbilityComponent)
			{
				// same as character: better to add/remove abils instead of this check
				if (auto * CombatSubsystem = UCSCombatSubsystem::Get(GetWorld()))
				{
					if ((CombatSubsystem->GetCombatState() == ECpCombatState::CpCombat_Active) &&
						(CombatSubsystem->IsMemberRegistered(ControlledPawn)))
					{
						TWeakObjectPtr<UASAbilityComponent> WeakAC = AbilityComponent;
						FTimerDelegate Delegate;
						Delegate.BindLambda([WeakAC]()
						{
							if (WeakAC.IsValid())
							{
								WeakAC.Get()->ActivateAbilityByTag(PluginTags::TAG_Ability_Attack);
							}
						});
						FTimerHandle Tmp;
						GetWorldTimerManager().SetTimer(Tmp,	Delegate, 0.3f,	false);
					}
				}
			}
		}
		bWasInner = true;
	}
}
