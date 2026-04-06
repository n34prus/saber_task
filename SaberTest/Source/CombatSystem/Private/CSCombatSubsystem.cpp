#include "CSCombatSubsystem.h"

#include "ASDamageModifierComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Components/ActorComponent.h"

static void ApplyModifiersFromActor(AActor* Actor, bool bOutgoing, FASCombatDamagePacket& Packet, AActor* OtherActor)
{
	if (!Actor) return;

	if (UASDamageModifierComponent* Modifier =	Actor->FindComponentByClass<UASDamageModifierComponent>())
	{
		if (bOutgoing)
		{
			Modifier->ModifyOutgoingDamage(Packet, OtherActor);
		}
		else
		{
			Modifier->ModifyIncomingDamage(Packet, OtherActor);
		}
	}
}

float UCSCombatSubsystem::ApplyCombatDamage(AActor* DamageCauser, AActor* Target, const FASCombatDamagePacket& DamagePacket)
{
	if (!Target || DamagePacket.BaseDamage <= 0.f)
	{
		return 0.f;
	}

	FASCombatDamagePacket Packet = DamagePacket;
	Packet.FinalDamage = Packet.BaseDamage;

	ApplyModifiersFromActor(DamageCauser, true, Packet, Target);
	ApplyModifiersFromActor(Target, false, Packet, DamageCauser);

	Packet.FinalDamage = FMath::Max(0.f, Packet.FinalDamage);
	if (Packet.FinalDamage <= 0.f)
	{
		return 0.f;
	}

	UGameplayStatics::ApplyDamage(
		Target,
		Packet.FinalDamage,
		nullptr,
		DamageCauser,
		UDamageType::StaticClass()
	);

	OnCombatMemberDealDamage.Broadcast(DamageCauser, Target, Packet.FinalDamage);
	
	return Packet.FinalDamage;
}

void UCSCombatSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	OnCombatStateChanged.AddDynamic(this, &UCSCombatSubsystem::OnCombatStateChanged_Implementation);
}

void UCSCombatSubsystem::Deinitialize()
{
	OnCombatStateChanged.RemoveAll(this);
	Super::Deinitialize();
}

bool UCSCombatSubsystem::StartCombat()
{
	if (CombatState == ECpCombatState::CpCombat_None)
	{
		SetCombatState(ECpCombatState::CpCombat_Init);
		return true;
	}
	return false;
}

bool UCSCombatSubsystem::StopCombat()
{
	if (CombatState == ECpCombatState::CpCombat_Active)
	{
		SetCombatState(ECpCombatState::CpCombat_Finished);
		return true;
	}
	return false;
}

void UCSCombatSubsystem::ResetCombat()
{
	auto CachedMember = CombatMembers;
	for (auto& Pair : CachedMember)
	{
		if (Pair.Key) UnregisterMember(Pair.Key);
	}
	CombatMembers.Empty();
	SetCombatState(ECpCombatState::CpCombat_None);
}

bool UCSCombatSubsystem::RegisterMember(AActor* Member)
{
	if (!IsValid(Member)) return false;
	if (IsMemberRegistered(Member)) return true;
	if (auto * HealthComponent = Member->FindComponentByClass<UHSHealthComponent>())
	{
		CombatMembers.Add(Member, HealthComponent);
		HealthComponent->OnDeath.AddDynamic(this, &UCSCombatSubsystem::OnDeath);
		UE_LOG(LogTemp, Warning, TEXT("Registered: %s | Total: %d"), *GetNameSafe(Member), CombatMembers.Num());
		return true;
	}
	return false;
}

bool UCSCombatSubsystem::UnregisterMember(AActor* Member)
{
	if (!Member) return false;
	if (auto * HealthComponent = CombatMembers[Member])
	{
		HealthComponent->OnDeath.RemoveAll(this);
	}
	int32 Matches = CombatMembers.Remove(Member);
	if (Matches <= 0) return false;
	UE_LOG(LogTemp, Warning, TEXT("Unregistered: %s | Total: %d"), *GetNameSafe(Member), CombatMembers.Num());
	return true;
}

bool UCSCombatSubsystem::IsMemberRegistered(AActor* Member)
{
	return CombatMembers.Contains(Member);
}

void UCSCombatSubsystem::SetDifficulty(float NewDifficulty)
{
	Difficulty = FMath::Clamp(NewDifficulty, 0.f, 1.f);
	OnDifficultyChanged.Broadcast(Difficulty);
}

float UCSCombatSubsystem::GetDifficulty()
{
	return Difficulty;
}

void UCSCombatSubsystem::OnCombatStateChanged_Implementation(ECpCombatState NewState)
{
	// we have no impl transmissions so lets make them rapid
	ECpCombatState TargetState = NewState;
	
	switch (NewState)
	{
		case ECpCombatState::CpCombat_Init:
			PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			if (PlayerActor && RegisterMember(PlayerActor))
			{
				TargetState = ECpCombatState::CpCombat_Active;
			}
			else
			{
				TargetState = ECpCombatState::CpCombat_None;
			}
			break;
		
		case ECpCombatState::CpCombat_Finished:
			TargetState = ECpCombatState::CpCombat_Resulting;
			break;
		
		default:
			break;
	}

	if (TargetState != NewState)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(
			FTimerDelegate::CreateWeakLambda(this, [this, TargetState]()
			{
				SetCombatState(TargetState);
			})
		);
	}
}

void UCSCombatSubsystem::OnDeath(AActor* DeadActor)
{
	if (!DeadActor) return;
	UnregisterMember(DeadActor);
	OnCombatMemberDeath.Broadcast(DeadActor);
	CheckFinishCondition();
}

void UCSCombatSubsystem::CheckFinishCondition()
{
	if (CombatState != ECpCombatState::CpCombat_Active) return;
	
	ensure(PlayerActor);			// something unexpected
	if (!PlayerActor) return;

	// player is dead
	if (!CombatMembers.Find(PlayerActor))
	{
		StopCombat();
		return;
	}

	// only one member left - probably player
	if (CombatMembers.Num() <= 1)
	{
		StopCombat();
		return;
	}
		
}
