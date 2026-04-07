#include "SSStatisticSubsystem.h"
#include "Kismet/GameplayStatics.h"


void USSStatisticSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
		
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

void USSStatisticSubsystem::AddValueToSessionStat(FGameplayTag StatTag, float Value)
{
	if (!SessionStats.Contains(StatTag)) SessionStats.Add(StatTag, Value);
	else SessionStats[StatTag] += Value;
}

void USSStatisticSubsystem::AddValueToTotalStat(FGameplayTag StatTag, float Value)
{
	if (!TotalStats.Contains(StatTag)) TotalStats.Add(StatTag, Value);
	else TotalStats[StatTag] += Value;
}

float USSStatisticSubsystem::GetSessionStat(FGameplayTag StatTag) const
{
	if (!SessionStats.Contains(StatTag)) return 0.f;
	return SessionStats[StatTag];
}

float USSStatisticSubsystem::GetTotalStat(FGameplayTag StatTag) const
{
	if (!TotalStats.Contains(StatTag)) return 0.f;
	return TotalStats[StatTag];
}

void USSStatisticSubsystem::ClearStats()
{
	TotalStats.Empty();
	SessionStats.Empty();
}

void USSStatisticSubsystem::UpdateBindings()
{
	UnbindFromBusEvents();
	BindToBusEvents();

}

void USSStatisticSubsystem::BindToBusEvents()
{
	if (UEBEventBusSubsystem* Bus = GetGameInstance()->GetSubsystem<UEBEventBusSubsystem>())
	{
		Bus->SubscribeObject(PluginTags::TAG_Combat_Damage, this, &USSStatisticSubsystem::HandleDamage);
		Bus->SubscribeObject(PluginTags::TAG_Health_Death, this, &USSStatisticSubsystem::HandleDeath);
		Bus->SubscribeObject(PluginTags::TAG_Combat_StateChanged, this, &USSStatisticSubsystem::HandleCombatStateChanged);
		Bus->SubscribeObject(PluginTags::TAG_Combat_Crit, this, &USSStatisticSubsystem::HandleCrit);
	}
}

void USSStatisticSubsystem::UnbindFromBusEvents()
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
		UpdateBindings();
		PlayerActor = NewPlayerActor;
	}
}

void USSStatisticSubsystem::HandleDamage(const FEBEventData& Event)
{
	if (Event.FloatValue <= 0.f) return;
	if (Event.Source == Event.Target) return;
	if (Event.Source == PlayerActor)
	{
		AddValueToSessionStat(PluginTags::TAG_Stat_DamageProduced, Event.FloatValue);
		return;
	}
	if (Event.Target == PlayerActor)
	{
		AddValueToSessionStat(PluginTags::TAG_Stat_DamageRecieved, Event.FloatValue);
		return;
	}
}

void USSStatisticSubsystem::HandleDeath(const FEBEventData& Event)
{
	if (Event.Source != PlayerActor)
	{
		AddValueToSessionStat(PluginTags::TAG_Stat_ExperiencePoints, 100);
	}
}

void USSStatisticSubsystem::HandleCombatStateChanged(const FEBEventData& Event)
{
	if (Event.Key == PluginTags::TAG_Combat_State_None)
	{
		SessionStats.Empty();
	}
	else if (Event.Key == PluginTags::TAG_Combat_State_Finished)
	{
		for (const auto & Pair : SessionStats)
		{
			AddValueToTotalStat(Pair.Key, Pair.Value);
			//TotalStats[Pair.Key] += Pair.Value;
		}
		Save();
	}
}

void USSStatisticSubsystem::HandleCrit(const FEBEventData& Event)
{
	if (Event.Source != PlayerActor) return;
	AddValueToSessionStat(PluginTags::TAG_Stat_CritCount, 1);
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
			ClearStats();
			TotalStats = SaveObject->TotalStats;
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
