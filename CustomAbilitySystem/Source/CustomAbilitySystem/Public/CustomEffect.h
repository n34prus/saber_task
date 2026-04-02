#pragma once

#include "CoreMinimal.h"
#include "CustomModifier.h"
#include "CustomAbility.h"
#include "CustomAbilityExecutionContext.h"
#include "CustomPropertyComponent.h"
//#include "UObject/NoExportTypes.h"
#include "CustomEffect.generated.h"

UCLASS(Blueprintable, BlueprintType)
class CUSTOMABILITYSYSTEM_API UCustomEffect : public UObject
{
    GENERATED_BODY()

public:

    // seconds, 0 = single exec
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    float Period = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    FTimerHandle TimerHandle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    TArray<FCustomModifier> Modifiers;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    TSet<FName> RequiredTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    TSet<FName> BlockingTags;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    TArray<TSubclassOf<UCustomAbility>> TriggerAbilities;

public:

    UFUNCTION(BlueprintCallable, Category="Effect")
    void Execute(FCustomAbilityExecutionContext InContext, AActor* Target)
    {
        if (!Target || !InContext.Ability)
            return;
        
        for (const FName& Tag : RequiredTags)
        {
            if (!InContext.AggregatedTags.Contains(Tag))
                return;
        }

        for (const FName& Tag : BlockingTags)
        {
            if (InContext.AggregatedTags.Contains(Tag))
                return;
        }
        
        // Add modifiers to context
        for (const FCustomModifier& Mod : Modifiers)
        {
            FCustomModifier ModCopy = Mod;
            ModCopy.Source = this;
            InContext.AddModifier(ModCopy);
        }
        
        // Apply final modified values to target's PropertyComponent
        if (UCustomPropertyComponent* TargetPropComp = Target->FindComponentByClass<UCustomPropertyComponent>())
        {
            TArray<FCustomModifier> ModsToApply;
            for (const auto& ParamPair : InContext.RuntimeParams)
            {
                float BaseValue = InContext.SnapshotParams.Contains(ParamPair.Key) ? InContext.SnapshotParams[ParamPair.Key] : 0.f;
                float Difference = ParamPair.Value - BaseValue;
                
                if (FMath::Abs(Difference) > KINDA_SMALL_NUMBER)
                {
                    FCustomModifier FinalMod(ParamPair.Key, Difference, ECustomModifierOp::CmOp_Add, 0, this);
                    ModsToApply.Add(FinalMod);
                }
            }
            
            if (ModsToApply.Num() > 0)
            {
                TargetPropComp->ApplyModifiers(ModsToApply);
            }
        }
        
        // Trigger abilities on target
        for (const TSubclassOf<UCustomAbility>& AbilityClass : TriggerAbilities)
        {
            if (AbilityClass)
            {
                if (UCustomAbilityComponent* TargetAbilityComp = Target->FindComponentByClass<UCustomAbilityComponent>())
                {
                    UCustomAbility* NewAbility = NewObject<UCustomAbility>(Target, AbilityClass);
                    if (NewAbility)
                    {
                        TargetAbilityComp->TryActivateAbility(NewAbility);
                    }
                }
            }
        }

        // Timer
        //if ()
    }
};