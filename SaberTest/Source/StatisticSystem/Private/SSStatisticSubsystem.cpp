#include "SSStatisticSubsystem.h"
#include "ASDamageModifierComponent.h"
#include "CSCombatSubsystem.h"

void USSStatisticSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	for (const auto * Stat : StatNames)
	{
		TotalStats.Add(Stat, 0.f);
		SessionStats.Add(Stat, 0.f);
	}
	
	Load();	// override total stats

	// todo: better to use UGameplayStatics delegates
	GetWorld()->GetTimerManager().SetTimer(
	ScanTimerHandle,
	this,
	&USSStatisticSubsystem::Scan,
	0.5f,
	true
	);
}

void USSStatisticSubsystem::Deinitialize()
{
	Save();
	Super::Deinitialize();
}

float USSStatisticSubsystem::GetSessionStat(FName StatName) const
{
	return SessionStats[StatName];
}

float USSStatisticSubsystem::GetTotalStat(FName StatName) const
{
	return TotalStats[StatName];
}

void USSStatisticSubsystem::ClearTotalStat()
{
	for (const auto * Stat : StatNames)
	{
		TotalStats.Add(Stat, 0.f);
		SessionStats.Add(Stat, 0.f);
	}
}

void USSStatisticSubsystem::BindToNewPlayer(AActor* OldPlayerActor, AActor* NewPlayerActor)
{
	UnbindFromCombatEvents();
	UnbindFromPlayerModifiers(OldPlayerActor);
	BindToCombatEvents();
	BindToPlayerModifiers(NewPlayerActor);
}

void USSStatisticSubsystem::BindToCombatEvents()
{
	if (auto * CombatSubsystem = UCSCombatSubsystem::Get(GetWorld()))
	{
		CombatSubsystem->OnCombatMemberDealDamage.AddDynamic(this, &USSStatisticSubsystem::HandleDamage);
		CombatSubsystem->OnCombatStateChanged.AddDynamic(this, &USSStatisticSubsystem::HandleCombatStateChanged);
		CombatSubsystem->OnCombatMemberDeath.AddDynamic(this, &USSStatisticSubsystem::HandleDeath);
	}
}

void USSStatisticSubsystem::BindToPlayerModifiers(AActor* NewPlayerActor)
{
	if (!NewPlayerActor) return;
	UASDamageModifierComponent* Component = NewPlayerActor->FindComponentByClass<UASDamageModifierComponent>();
	if (!Component) return;
	Component->OnMessageCalled.AddDynamic(this, &USSStatisticSubsystem::HandleModifier);
}

void USSStatisticSubsystem::UnbindFromCombatEvents()
{
	if (auto * CombatSubsystem = UCSCombatSubsystem::Get(GetWorld()))
	{
		CombatSubsystem->OnCombatMemberDealDamage.RemoveAll(this);
		CombatSubsystem->OnCombatStateChanged.RemoveAll(this);
		CombatSubsystem->OnCombatMemberDeath.RemoveAll(this);
	}
}

void USSStatisticSubsystem::UnbindFromPlayerModifiers(AActor* OldPlayerActor)
{
	if (!OldPlayerActor) return;
	UASDamageModifierComponent* Component = OldPlayerActor->FindComponentByClass<UASDamageModifierComponent>();
	if (!Component) return;
	Component->OnMessageCalled.RemoveAll(this);
}

void USSStatisticSubsystem::Scan()
{
	AActor* NewPlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (NewPlayerActor && (PlayerActor != NewPlayerActor))
	{
		BindToNewPlayer(PlayerActor, NewPlayerActor);
		PlayerActor = NewPlayerActor;
	}
}

void USSStatisticSubsystem::HandleDamage(AActor* DamageCauser, AActor* Target, float Value)
{
	if (Value <= 0.f) return;
	if (DamageCauser == Target) return;
	if (DamageCauser == PlayerActor)
	{
		constexpr const TCHAR* StatName = TEXT("DamageProduced");
		static_assert(IsValidStatName(StatName), "Invalid name!");
		SessionStats[StatName] += Value;
		//SessionDamageProduced += Value;
		return;
	}
	if (Target == PlayerActor)
	{
		constexpr const TCHAR* StatName = TEXT("DamageRecieved");
		static_assert(IsValidStatName(StatName), "Invalid name!");
		SessionStats[StatName] += Value;
		//SessionDamageRecieved += Value;
		return;
	}
}

void USSStatisticSubsystem::HandleDeath(AActor* DeadActor)
{
	if (DeadActor != PlayerActor)
	{
		constexpr const TCHAR* StatName = TEXT("ExperiencePoints");
		static_assert(IsValidStatName(StatName), "Invalid name!");
		SessionStats[StatName] += 100;
		//SessionExperiencePoints += 100;	// can be overrided by enemy archetype
	}
}

void USSStatisticSubsystem::HandleCombatStateChanged(ECpCombatState NewCombatState)
{
	switch (NewCombatState)
	{
	case ECpCombatState::CpCombat_None:
			for (auto Name : StatNames)
				SessionStats[Name] = 0.f;
			break;

		case ECpCombatState::CpCombat_Finished:
			for (auto Name : StatNames)
				TotalStats[Name] += SessionStats[Name];
			Save();
			break;

		default:
			break;
	}
}

void USSStatisticSubsystem::HandleModifier(const FName Message, const UObject* Source)
{
	if (Message == "Crit")
	{
		constexpr const TCHAR* StatName = TEXT("CritCount");
		static_assert(IsValidStatName(StatName), "Invalid name!");
		SessionStats[StatName] ++;
		//SessionCritCount++;
	}
}

void USSStatisticSubsystem::Load()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		SaveObject = Cast<UCpStatisticSaveGame>(
			UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex)
		);

		if (SaveObject)
		{
			for (auto Stat : StatNames)
			{
				if (SaveObject->TotalStats.Contains(Stat))
					TotalStats[Stat] = SaveObject->TotalStats[Stat];
			}
		}
	}
	else
	{
		SaveObject = Cast<UCpStatisticSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UCpStatisticSaveGame::StaticClass())
		);
	}
}

void USSStatisticSubsystem::Save()
{
	if (!SaveObject)
	{
		SaveObject = Cast<UCpStatisticSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UCpStatisticSaveGame::StaticClass())
		);
	}

	SaveObject->TotalStats = TotalStats;

	UGameplayStatics::SaveGameToSlot(SaveObject, SlotName, UserIndex);
}
