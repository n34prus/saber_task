#pragma once

#include "CoreMinimal.h"
#include "CustomModifier.h"
//#include "CustomAbility.h"
//#include "CustomTargetSelector.h"
//#include "CustomEffect.h"
#include "CustomAbilityExecutionContext.generated.h"

class UCustomAbility;

USTRUCT(BlueprintType)
struct CUSTOMABILITYSYSTEM_API FCustomTargetInfo
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadOnly)
    AActor* Target = nullptr;
    
    UPROPERTY(BlueprintReadOnly)
    FVector HitLocation = FVector::ZeroVector;
    
    UPROPERTY(BlueprintReadOnly)
    FVector HitNormal = FVector::UpVector;
    
    UPROPERTY(BlueprintReadOnly)
    FName BoneName = NAME_None;
    
    UPROPERTY(BlueprintReadOnly)
    UPrimitiveComponent* HitComponent = nullptr;
    
    UPROPERTY(BlueprintReadOnly)
    float Distance = 0.f;

public:
    FCustomTargetInfo() = default;

    FCustomTargetInfo(AActor* InTarget, const FVector& InLocation, const FVector& InNormal);

    FCustomTargetInfo(const FHitResult& Hit);

    bool IsValid() const
    {
        return Target != nullptr;
    }
};

USTRUCT(BlueprintType)
struct CUSTOMABILITYSYSTEM_API FCustomAbilityExecutionContext
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite)
    AActor* Source = nullptr;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FCustomTargetInfo> Targets;
    
    UPROPERTY(BlueprintReadWrite)
    UCustomAbility* Ability = nullptr;

    UPROPERTY(BlueprintReadWrite)
    TMap<FName, float> SnapshotParams;

    UPROPERTY(BlueprintReadWrite)
    TMap<FName, float> RuntimeParams;

    UPROPERTY(BlueprintReadWrite)
    TArray<FCustomModifier> RuntimeModifiers;

    UPROPERTY(BlueprintReadWrite)
    TSet<FName> AggregatedTags;

public:
    FCustomAbilityExecutionContext() = default;

    FCustomAbilityExecutionContext(AActor* InSource, UCustomAbility* InAbility)
        : Source(InSource), Ability(InAbility)
    {}
    
    void ApplyModifiers();

    void AddModifier(const FCustomModifier& Mod);

    void RemoveModifier(const FCustomModifier& Mod);
};