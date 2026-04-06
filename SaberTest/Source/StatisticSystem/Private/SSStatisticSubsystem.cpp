#include "SSStatisticSubsystem.h"
#include "Kismet/GameplayStatics.h"

#include "PTGameplayTags.h"

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
	BindToCombatEvents();

}

void USSStatisticSubsystem::BindToCombatEvents()
{
	if (UEBEventBusSubsystem* Bus = GetGameInstance()->GetSubsystem<UEBEventBusSubsystem>())
	{
		Bus->SubscribeObject(PluginTags::TAG_Combat_Damage, this, &USSStatisticSubsystem::HandleDamage);
		Bus->SubscribeObject(PluginTags::TAG_Health_Death, this, &USSStatisticSubsystem::HandleDeath);
		Bus->SubscribeObject(PluginTags::TAG_Combat_StateChanged, this, &USSStatisticSubsystem::HandleCombatStateChanged);
		Bus->SubscribeObject(PluginTags::TAG_Combat_Crit, this, &USSStatisticSubsystem::HandleCrit);
	}
}

void USSStatisticSubsystem::UnbindFromCombatEvents()
{
	if (UEBEventBusSubsystem* Bus = GetGameInstance()->GetSubsystem<UEBEventBusSubsystem>())
	{
		Bus->UnsubscribeAll(this);
	}
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

void USSStatisticSubsystem::HandleDamage(const FEBEventData& Event)
{
	if (Event.FloatValue <= 0.f) return;
	if (Event.Source == Event.Target) return;
	if (Event.Source == PlayerActor)
	{
		constexpr const TCHAR* StatName = TEXT("DamageProduced");
		static_assert(IsValidStatName(StatName), "Invalid name!");
		SessionStats[StatName] += Event.FloatValue;
		return;
	}
	if (Event.Target == PlayerActor)
	{
		constexpr const TCHAR* StatName = TEXT("DamageRecieved");
		static_assert(IsValidStatName(StatName), "Invalid name!");
		SessionStats[StatName] += Event.FloatValue;
		return;
	}
}

void USSStatisticSubsystem::HandleDeath(const FEBEventData& Event)
{
	if (Event.Source != PlayerActor)
	{
		constexpr const TCHAR* StatName = TEXT("ExperiencePoints");
		static_assert(IsValidStatName(StatName), "Invalid name!");
		SessionStats[StatName] += 100;
	}
}

void USSStatisticSubsystem::HandleCombatStateChanged(const FEBEventData& Event)
{
	if (Event.Key == PluginTags::TAG_Combat_State_None)
	{
		for (auto Name : StatNames) SessionStats[Name] = 0.f;
	}
	else if (Event.Key == PluginTags::TAG_Combat_State_Finished)
	{
		for (auto Name : StatNames)	TotalStats[Name] += SessionStats[Name];
		Save();
	}
}

void USSStatisticSubsystem::HandleCrit(const FEBEventData& Event)
{
	constexpr const TCHAR* StatName = TEXT("CritCount");
	static_assert(IsValidStatName(StatName), "Invalid name!");
	SessionStats[StatName] ++;
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
