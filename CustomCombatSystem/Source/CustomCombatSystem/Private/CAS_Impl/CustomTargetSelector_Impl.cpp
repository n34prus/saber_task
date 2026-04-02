#include "CAS_Impl/CustomTargetSelector_Impl.h"
#include "Engine/OverlapResult.h"

void UCustomTargetSelector_Closest::SelectTargets_Implementation(
	FCustomAbilityExecutionContext& Context)
{
    Super::SelectTargets_Implementation(Context);

    AActor* Source = Context.Source;
    if (!Source) return;

    UWorld* World = Source->GetWorld();
    if (!World) return;

    const FVector SourceLoc = Source->GetActorLocation();

    float BestDistSq = MAX_flt;
    AActor* BestTarget = nullptr;

    TArray<FOverlapResult> Results;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(MaxRange);

    World->OverlapMultiByObjectType(
        Results,
        SourceLoc,
        FQuat::Identity,
        FCollisionObjectQueryParams(ECC_Pawn),
        Sphere
    );

    for (const FOverlapResult& Res : Results)
    {
        AActor* Candidate = Res.GetActor();
        if (!Candidate || Candidate == Source) continue;

        float DistSq = FVector::DistSquared(SourceLoc, Candidate->GetActorLocation());

        if (DistSq < BestDistSq)
        {
            BestDistSq = DistSq;
            BestTarget = Candidate;
        }
    }

    if (BestTarget)
    {
        Context.Targets.Reset();

        FCustomTargetInfo Info;
        Info.Target = BestTarget;
        Info.HitLocation = BestTarget->GetActorLocation();
        Info.HitNormal = FVector::UpVector;
        Info.Distance = FMath::Sqrt(BestDistSq);

        Context.Targets.Add(Info);
    }
}