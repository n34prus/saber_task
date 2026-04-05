#include "CpCombatArena_Impl.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ACpCombatArena::ACpCombatArena()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	RootComponent = Trigger;
	Trigger->SetCollisionProfileName(TEXT("Trigger"));
	Trigger->SetGenerateOverlapEvents(true);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACpCombatArena::OnOverlap);

	//SpawnCount = 3;
	SpawnSpacing = 100.f;
}

void ACpCombatArena::BeginPlay()
{
	Super::BeginPlay();
	
	BindToCombatSubsystem();
	SpawnActors();
}

void ACpCombatArena::Reset()
{
	Super::Reset();
	CombatSubsystem->ResetCombat();
	for (auto * Pawn : SpawnedPawns)
	{
		Pawn->Destroy();
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ACpCombatArena::SpawnActors);
}

void ACpCombatArena::SpawnActors()
{
	UClass* Class = EnemyPawnClass.LoadSynchronous();
	if (!Class) return;

	UWorld* World = GetWorld();
	if (!World) return;

	const FVector Center = GetActorLocation();
	const FVector Right = GetActorRightVector();

	int32 SpawnCount = 0;
	if (CombatSubsystem && SpawnCount <= 0)
	{
		SpawnCount = CombatSubsystem->GetDifficulty() * 10;
	}

	for (int32 i = 0; i < SpawnCount; ++i)
	{
		const float Offset = (i - (SpawnCount - 1) * 0.5f) * SpawnSpacing;
		const FVector Location = Center + Right * Offset;

		FActorSpawnParameters Params;
		Params.Owner = this;

		APawn* NewPawn = World->SpawnActor<APawn>(Class, Location, GetActorRotation(), Params);
		if (!NewPawn) continue;

		SpawnedPawns.Add(NewPawn);
	}
}

void ACpCombatArena::BindToCombatSubsystem()
{
	if (CombatSubsystem) return;
	CombatSubsystem = UCpCombatSubsystem::Get(GetWorld());
	if (!CombatSubsystem) return;
	CombatSubsystem->OnCombatStateChanged.AddDynamic(this, &ACpCombatArena::OnCombatStateChanged);
}

void ACpCombatArena::UnbindFromCombatSubsystem()
{
	CombatSubsystem->OnCombatStateChanged.RemoveAll(this);
}

void ACpCombatArena::OnCombatStateChanged(ECpCombatState NewState)
{
	if (NewState == ECpCombatState::CpCombat_Resulting)
	{
		//UnbindFromCombatSubsystem();
		if (WidgetClass.IsValid())
		{
			TSubclassOf<UUserWidget> WidgetClassRef = WidgetClass.LoadSynchronous();
			if (WidgetClassRef)
			{
				APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (PC)
				{
					auto * WidgetInstance = CreateWidget<UUserWidget>(PC, WidgetClassRef);
					if (WidgetInstance)
					{
						WidgetInstance->AddToViewport(3);
					}
				}
			}
		}
	}
}

void ACpCombatArena::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		OnTriggered(OtherActor);
}

void ACpCombatArena::OnTriggered(AActor* ByActor)
{
	UClass* ControllerClass = AiControllerClass.LoadSynchronous();
	if (!ControllerClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	if (SpawnedPawns.IsEmpty()) return;
	
	if (!CombatSubsystem) return;

	if (CombatSubsystem->GetCombatState() != ECpCombatState::CpCombat_None) return;

	for (auto * Pawn : SpawnedPawns)
	{
		if (!Pawn) continue;
		if (Pawn->GetController()) continue;

		FActorSpawnParameters Params;
		Params.Owner = this;

		AAIController* AI = World->SpawnActor<AAIController>(
			ControllerClass,
			Pawn->GetActorLocation(),
			Pawn->GetActorRotation(),
			Params
		);

		if (!AI) continue;

		AI->Possess(Pawn);

		CombatSubsystem->RegisterMember(Pawn);
	}
	
	CombatSubsystem->RegisterMember(ByActor);
	CombatSubsystem->StartCombat();
}
