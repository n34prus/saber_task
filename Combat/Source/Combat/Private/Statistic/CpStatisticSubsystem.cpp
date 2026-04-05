#include "Statistic/CpStatisticSubsystem.h"
#include "Modifiers/CpDamageModifierComponent.h"
#include "CpCombatSubsystem.h"

void UCpStatisticSubsystem::Initialize(FSubsystemCollectionBase& Collection)
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
	&UCpStatisticSubsystem::Scan,
	0.5f,
	true
	);
}

void UCpStatisticSubsystem::Deinitialize()
{
	Save();
	Super::Deinitialize();
}

float UCpStatisticSubsystem::GetSessionStat(FName StatName) const
{
	return SessionStats[StatName];
}

float UCpStatisticSubsystem::GetTotalStat(FName StatName) const
{
	return TotalStats[StatName];
}

void UCpStatisticSubsystem::ClearTotalStat()
{
	for (const auto * Stat : StatNames)
	{
		TotalStats.Add(Stat, 0.f);
		SessionStats.Add(Stat, 0.f);
	}
}

void UCpStatisticSubsystem::BindToNewPlayer(AActor* OldPlayerActor, AActor* NewPlayerActor)
{
	UnbindFromCombatEvents();
	UnbindFromPlayerModifiers(OldPlayerActor);
	BindToCombatEvents();
	BindToPlayerModifiers(NewPlayerActor);
}

void UCpStatisticSubsystem::BindToCombatEvents()
{
	if (auto * CombatSubsystem = UCpCombatSubsystem::Get(GetWorld()))
	{
		CombatSubsystem->OnCombatMemberDealDamage.AddDynamic(this, &UCpStatisticSubsystem::HandleDamage);
		CombatSubsystem->OnCombatStateChanged.AddDynamic(this, &UCpStatisticSubsystem::HandleCombatStateChanged);
		CombatSubsystem->OnCombatMemberDeath.AddDynamic(this, &UCpStatisticSubsystem::HandleDeath);
	}
}

void UCpStatisticSubsystem::BindToPlayerModifiers(AActor* NewPlayerActor)
{
	if (!NewPlayerActor) return;
	UCpDamageModifierComponent* Component = NewPlayerActor->FindComponentByClass<UCpDamageModifierComponent>();
	if (!Component) return;
	Component->OnMessageCalled.AddDynamic(this, &UCpStatisticSubsystem::HandleModifier);
}

void UCpStatisticSubsystem::UnbindFromCombatEvents()
{
	if (auto * CombatSubsystem = UCpCombatSubsystem::Get(GetWorld()))
	{
		CombatSubsystem->OnCombatMemberDealDamage.RemoveAll(this);
		CombatSubsystem->OnCombatStateChanged.RemoveAll(this);
		CombatSubsystem->OnCombatMemberDeath.RemoveAll(this);
	}
}

void UCpStatisticSubsystem::UnbindFromPlayerModifiers(AActor* OldPlayerActor)
{
	if (!OldPlayerActor) return;
	UCpDamageModifierComponent* Component = OldPlayerActor->FindComponentByClass<UCpDamageModifierComponent>();
	if (!Component) return;
	Component->OnMessageCalled.RemoveAll(this);
}

void UCpStatisticSubsystem::Scan()
{
	AActor* NewPlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (NewPlayerActor && (PlayerActor != NewPlayerActor))
	{
		BindToNewPlayer(PlayerActor, NewPlayerActor);
		PlayerActor = NewPlayerActor;
	}
}

void UCpStatisticSubsystem::HandleDamage(AActor* DamageCauser, AActor* Target, float Value)
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

void UCpStatisticSubsystem::HandleDeath(AActor* DeadActor)
{
	if (DeadActor != PlayerActor)
	{
		constexpr const TCHAR* StatName = TEXT("ExperiencePoints");
		static_assert(IsValidStatName(StatName), "Invalid name!");
		SessionStats[StatName] += 100;
		//SessionExperiencePoints += 100;	// can be overrided by enemy archetype
	}
}

void UCpStatisticSubsystem::HandleCombatStateChanged(ECpCombatState NewCombatState)
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

void UCpStatisticSubsystem::HandleModifier(const FName Message, const UObject* Source)
{
	if (Message == "Crit")
	{
		constexpr const TCHAR* StatName = TEXT("CritCount");
		static_assert(IsValidStatName(StatName), "Invalid name!");
		SessionStats[StatName] ++;
		//SessionCritCount++;
	}
}

void UCpStatisticSubsystem::Load()
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

void UCpStatisticSubsystem::Save()
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
