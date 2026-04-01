#include "CustomAbilityExecutionContext.h"

FCustomTargetInfo::FCustomTargetInfo(AActor* InTarget, const FVector& InLocation, const FVector& InNormal): Target(InTarget)
	, HitLocation(InLocation)
	, HitNormal(InNormal)
{}

FCustomTargetInfo::FCustomTargetInfo(const FHitResult& Hit)
{
	Target = Hit.GetActor();
	HitLocation = Hit.ImpactPoint;
	HitNormal = Hit.ImpactNormal;
	BoneName = Hit.BoneName;
	HitComponent = Hit.GetComponent();
	Distance = Hit.Distance;
}

void FCustomAbilityExecutionContext::ApplyModifiers()
{
	RuntimeParams = SnapshotParams;

	RuntimeModifiers.Sort([](const FCustomModifier& A, const FCustomModifier& B)
	{
		return A.Priority > B.Priority;
	});
	
	for (const FCustomModifier& Mod : RuntimeModifiers)
	{
		float* ParamValue = RuntimeParams.Find(Mod.Stat);
		if (!ParamValue)
		{
			RuntimeParams.Add(Mod.Stat, 0.f);
			ParamValue = RuntimeParams.Find(Mod.Stat);
		}

		switch (Mod.Op)
		{
		case ECustomModifierOp::CmOp_Add:
			*ParamValue += Mod.Value;
			break;
		case ECustomModifierOp::CmOp_Multiply:
			*ParamValue *= Mod.Value;
			break;
		default:
			break;
		}
	}
}

void FCustomAbilityExecutionContext::AddModifier(const FCustomModifier& Mod)
{
	RuntimeModifiers.Add(Mod);
	ApplyModifiers();
}

void FCustomAbilityExecutionContext::RemoveModifier(const FCustomModifier& Mod)
{
	RuntimeModifiers.RemoveAll([&](const FCustomModifier& Existing)
	{
		return Existing.Stat == Mod.Stat && Existing.Source == Mod.Source && Existing.Op == Mod.Op;
	});
	ApplyModifiers();
}
