#pragma once

#include "CoreMinimal.h"
#include "CustomAbilityTypes.h"
#include "CustomModifier.generated.h"

USTRUCT(BlueprintType)
struct CUSTOMABILITYSYSTEM_API FCustomModifier
{
	GENERATED_BODY()

public:

	// property to modify
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Stat;

	// modifier value
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0.f;

	// modifier type (operation)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECustomModifierOp Op = ECustomModifierOp::CmOp_Add;

	// priority
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority = 0;

	// source initializer
	TWeakObjectPtr<UObject> Source;

public:

	FCustomModifier() = default;

	FCustomModifier(FName InStat, float InValue, ECustomModifierOp InOp, int32 InPriority, UObject* InSource)
		: Stat(InStat), Value(InValue), Op(InOp), Priority(InPriority), Source(InSource)
	{}

	FORCEINLINE bool IsValid() const
	{
		return Stat != NAME_None;
	}

};