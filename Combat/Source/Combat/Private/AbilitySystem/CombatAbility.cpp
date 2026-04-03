#include "AbilitySystem/CombatAbility.h"
#include "GameFramework/Character.h"

void UCombatAbility::Init(AActor* InOwner)
{
	OwnerActor = InOwner;
	LastUseTime = -FLT_MAX;
}

bool UCombatAbility::CanActivate() const
{
	if (!OwnerActor)
	{
		return false;
	}

	UWorld* World = OwnerActor->GetWorld();
	if (!World)
	{
		return false;
	}

	return (World->GetTimeSeconds() - LastUseTime) >= Cooldown;
}

void UCombatAbility::TryActivate()
{
	if (!CanActivate())
	{
		return;
	}

	Activate();

	if (OwnerActor && OwnerActor->GetWorld())
	{
		LastUseTime = OwnerActor->GetWorld()->GetTimeSeconds();
	}
}

float UCombatAbility::GetCooldownRemaining() const
{
	if (!OwnerActor || !OwnerActor->GetWorld())
	{
		return 0.0f;
	}

	const float Elapsed = OwnerActor->GetWorld()->GetTimeSeconds() - LastUseTime;
	return FMath::Max(0.0f, Cooldown - Elapsed);
}

const TArray<FName>& UCombatAbility::GetTags() const
{
	return Tags;
}

void UCombatAbility::Activate()
{
	UAnimMontage* Anim = Animation.LoadSynchronous();

	ACharacter* Char = Cast<ACharacter>(OwnerActor);
	if (Char && Anim)
	{
		Char->PlayAnimMontage(Anim);
	}
}
