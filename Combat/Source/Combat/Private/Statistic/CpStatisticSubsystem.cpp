#include "Statistic/CpStatisticSubsystem.h"
#include "Modifiers/CpDamageModifierComponent.h"
#include "CpCombatDamageLibrary.h"

void UCpStatisticSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Load();

	// todo: better to use UGameplayStatics delegates
	GetWorld()->GetTimerManager().SetTimer(
	ScanTimerHandle,
	this,
	&UCpStatisticSubsystem::Scan,
	0.5f,
	true
	);
	/*
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			if (APawn* Pawn = PC->GetPawn())
			{
				BindToPlayerDamage(Pawn);
			}

		}
		
		World->AddOnActorSpawnedHandler(
			FOnActorSpawned::FDelegate::CreateUObject(this, &UCpStatisticSubsystem::OnActorSpawned)
		);
	}*/
}

void UCpStatisticSubsystem::Deinitialize()
{
	Save();
	Super::Deinitialize();
}

void UCpStatisticSubsystem::OnActorSpawned(AActor* Actor)
{
	if (!Actor) return;
		
	APawn* Pawn = Cast<APawn>(Actor);
	if (!Pawn) return;

	APlayerController* PC = Pawn->GetController<APlayerController>();
	if (!PC) return;

	if (PC->GetLocalPlayer() && PC->GetLocalPlayer()->GetControllerId() == 0)
	{
		BindToPlayerDamage(Pawn);
	}
}

void UCpStatisticSubsystem::BindToNewPlayer(AActor* OldPlayerActor, AActor* NewPlayerActor)
{
	UnbindFromPlayerDamage(OldPlayerActor);
	UnbindFromPlayerModifiers(OldPlayerActor);
	BindToPlayerDamage(NewPlayerActor);
	BindToPlayerModifiers(NewPlayerActor);
}

void UCpStatisticSubsystem::BindToPlayerDamage(AActor* NewPlayerActor)
{
	if (!NewPlayerActor) return;
	UCpCombatDamageLibrary::OnDealDamage.AddUObject(this, &UCpStatisticSubsystem::HandleDamage);
}

void UCpStatisticSubsystem::BindToPlayerModifiers(AActor* NewPlayerActor)
{
	if (!NewPlayerActor) return;
	UCpDamageModifierComponent* Component = NewPlayerActor->FindComponentByClass<UCpDamageModifierComponent>();
	if (!Component) return;
	Component->OnMessageCalled.AddDynamic(this, &UCpStatisticSubsystem::HandleModifier);
}

void UCpStatisticSubsystem::UnbindFromPlayerDamage(AActor* OldPlayerActor)
{
	if (!OldPlayerActor) return;
	UCpCombatDamageLibrary::OnDealDamage.RemoveAll(this);
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
		SessionDamageProduced += Value;
		TotalDamageProduced += Value;
		return;
	}
	if (Target == PlayerActor)
	{
		SessionDamageRecieved += Value;
		TotalDamageRecieved += Value;
		return;
	}
}

void UCpStatisticSubsystem::HandleModifier(const FName Message, const UObject* Source)
{
	if (Message == "Crit")
	{
		SessionCritCount++;
		TotalCritCount++;
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
			TotalDamageProduced = SaveObject->TotalDamageAllTime;
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

	SaveObject->TotalDamageAllTime = TotalDamageProduced;

	UGameplayStatics::SaveGameToSlot(SaveObject, SlotName, UserIndex);
}
