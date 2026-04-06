#include "CSCombatSubsystem.h"

#include "ASDamageModifierComponent.h"
#include "Kismet/GameplayStatics.h"


static void ApplyModifiersFromActor(AActor* Actor, bool bOutgoing, FASCombatDamagePacket& Packet, AActor* OtherActor)
{
	if (!Actor) return;

	if (UASDamageModifierComponent* Modifier =	Actor->FindComponentByClass<UASDamageModifierComponent>())
	{
		if (bOutgoing)
		{
			Modifier->ModifyOutgoingDamage(Packet, Actor, OtherActor);
		}
		else
		{
			Modifier->ModifyIncomingDamage(Packet, OtherActor, Actor);
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

	if (UEBEventBusSubsystem* Bus = GetGameInstance()->GetSubsystem<UEBEventBusSubsystem>())
	{
		FEBEventData Event;
		Event.Topic = PluginTags::TAG_Combat_Damage;
		Event.FloatValue = Packet.FinalDamage;
		Event.Source = DamageCauser;
		Event.Target = Target;
		
		Bus->Publish(Event);
	}
	
	return Packet.FinalDamage;
}

void UCSCombatSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	//OnCombatStateChanged.AddDynamic(this, &UCSCombatSubsystem::OnCombatStateChanged_Implementation);
}

void UCSCombatSubsystem::Deinitialize()
{
	//OnCombatStateChanged.RemoveAll(this);
	Super::Deinitialize();
}

const FGameplayTag UCSCombatSubsystem::GetTagFromState(ECpCombatState State)
{
	switch (State)
	{
		case ECpCombatState::CpCombat_None:
			return PluginTags::TAG_Combat_State_None;
		case ECpCombatState::CpCombat_Init:
			return PluginTags::TAG_Combat_State_Init;
		case ECpCombatState::CpCombat_Active:
			return PluginTags::TAG_Combat_State_Active;
		case ECpCombatState::CpCombat_Finished:
			return PluginTags::TAG_Combat_State_Finished;
		case ECpCombatState::CpCombat_Resulting:
			return PluginTags::TAG_Combat_State_Resulting;
		default:
			return PluginTags::TAG_Combat_State_None;
	}
}

bool UCSCombatSubsystem::StartCombat()
{
	if (CombatState == ECpCombatState::CpCombat_None)
	{
		BindToCombatEvents();
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
		UnbindFromCombatEvents();
		return true;
	}
	return false;
}

void UCSCombatSubsystem::ResetCombat()
{
	if (!IsValid(this))
	{
		UE_LOG(LogTemp, Warning, TEXT("ResetCombat called on invalid subsystem!"));
		return;
	}
	auto CachedMember = CombatMembers;
	for (auto * Actor : CachedMember)
	{
		if (Actor) UnregisterMember(Actor);
	}
	CombatMembers.Empty();
	SetCombatState(ECpCombatState::CpCombat_None);
}

bool UCSCombatSubsystem::RegisterMember(AActor* Member)
{
	if (!IsValid(Member)) return false;
	if (IsMemberRegistered(Member)) return true;

	CombatMembers.Add(Member);
	UE_LOG(LogTemp, Warning, TEXT("Registered: %s | Total: %d"), *GetNameSafe(Member), CombatMembers.Num());
	return true;
}

bool UCSCombatSubsystem::UnregisterMember(AActor* Member)
{
	if (!Member) return false;
	
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

void UCSCombatSubsystem::SetCombatState(ECpCombatState NewState)
{
	CombatState = NewState;
	OnCombatStateChanged_Implementation(NewState);
	if (UEBEventBusSubsystem* Bus = GetGameInstance()->GetSubsystem<UEBEventBusSubsystem>())
	{
		FEBEventData Event;
		Event.Topic = PluginTags::TAG_Combat_StateChanged;
		Event.Key = GetTagFromState(NewState);
		Bus->Publish(Event);
	}
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

void UCSCombatSubsystem::BindToCombatEvents()
{
	if (UEBEventBusSubsystem* Bus = GetGameInstance()->GetSubsystem<UEBEventBusSubsystem>())
	{
		Bus->SubscribeObject(PluginTags::TAG_Health_Death, this, &UCSCombatSubsystem::HandleDeath);
	}
}

void UCSCombatSubsystem::UnbindFromCombatEvents()
{
	if (UEBEventBusSubsystem* Bus = GetGameInstance()->GetSubsystem<UEBEventBusSubsystem>())
	{
		Bus->UnsubscribeAll(this);
	}
}

void UCSCombatSubsystem::HandleDeath(const FEBEventData& Event)
{
	AActor* Actor = Cast<AActor>(Event.Source);
	if (!Actor) return;
	if (!CombatMembers.Contains(Actor)) return;
	UnregisterMember(Actor);
	CheckFinishCondition();
}