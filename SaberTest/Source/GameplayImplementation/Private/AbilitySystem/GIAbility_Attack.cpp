#include "AbilitySystem/GIAbility_Attack.h"

#include "ASCombatDamagePacket.h"
#include "CSCombatSubsystem.h"
#include "Engine/OverlapResult.h"
#include "HSHealthComponent.h"

void UGIAbility_Attack::Activate()
{
	Super::Activate();
	
	const float ForwardOffset = AttackRange-ProbeRadius;

	const FVector Forward = OwnerActor->GetActorForwardVector();
	const FVector Center = OwnerActor->GetActorLocation() + Forward * ForwardOffset;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerActor);

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	TArray<FOverlapResult> Overlaps;

	bool bHit = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		Center,
		FQuat::Identity,
		ObjParams,
		FCollisionShape::MakeSphere(ProbeRadius),
		Params
	);
    
	//DrawDebugSphere(GetWorld(), Center, ProbeRadius, 16, FColor::Yellow, false, 0.5f);

	if (!bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("No overlaps"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Overlaps: %d"), Overlaps.Num());

	for (const FOverlapResult& Result : Overlaps)
	{
		AActor* HitActor = Result.GetActor();

		if (!HitActor || HitActor == OwnerActor)
			continue;

		// better to use separated filters
		APawn* SourcePawn = Cast<APawn>(OwnerActor);
		APawn* TargetPawn = Cast<APawn>(HitActor);
		if (SourcePawn && TargetPawn)
		{
			if (!SourcePawn->IsPlayerControlled() && !TargetPawn->IsPlayerControlled())
				continue;
		}

		UHSHealthComponent* HealthComp = Cast<UHSHealthComponent>(
		HitActor->GetComponentByClass(UHSHealthComponent::StaticClass()));
		if (!HealthComp) continue;
		if (HealthComp->IsDead()) continue;

		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *GetNameSafe(HitActor));

		const FVector From = OwnerActor->GetActorLocation();
		FVector To = HitActor->GetActorLocation();
		FVector Direction = (To - From).GetSafeNormal();
		To.Z = From.Z;
		FRotator LookAt = (To - From).Rotation();
		OwnerActor->SetActorRotation(FRotator(0.f, LookAt.Yaw, 0.f));
		
		FASCombatDamagePacket Packet(BaseAttackDamage);
		Packet.CriticalChance = AttackCriticalChance;
		Packet.CriticalMultiplier = AttackCriticalMultiplier;
		Packet.ArmorPenetration = AttackArmorPenetration;

		
		if (auto * CombatSubsystem = UCSCombatSubsystem::Get(GetWorld()))
		{
			CombatSubsystem->ApplyCombatDamage(OwnerActor, HitActor, Packet);
		}
		
		break;
	}
}
