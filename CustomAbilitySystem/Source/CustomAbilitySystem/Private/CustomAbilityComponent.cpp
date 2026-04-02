#include "CustomAbilityComponent.h"

#include "CustomPropertyComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

UCustomAbilityComponent::UCustomAbilityComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UCustomAbilityComponent::IsAbilityAvailable(UCustomAbility* Ability) const
{
    if (!Ability) return false;
    
    for (const FName& Tag : Ability->RequiredTags)
    {
        if (!OwnerTags.Contains(Tag)) return false;
    }

    for (const FName& Tag : Ability->BlockingTags)
    {
        if (OwnerTags.Contains(Tag)) return false;
    }
    
    if (Cooldowns.Contains(Ability))
    {
        float EndTime = Cooldowns[Ability];
        if (GetWorld() && GetWorld()->GetTimeSeconds() < EndTime)
            return false;
    }

    // Check cost
    if (UCustomPropertyComponent* PropComp = GetOwner()->FindComponentByClass<UCustomPropertyComponent>())
    {
        for (const auto& CostPair : Ability->Cost)
        {
            float CurrentValue = PropComp->GetFinalStat(CostPair.Key);
            if (CurrentValue < CostPair.Value)
                return false;
        }
    }

    return true;
}

FCustomAbilityExecutionContext* UCustomAbilityComponent::GetExecutionContext(UCustomAbility* Ability)
{
    if (!Ability) return nullptr;
    return ExecutionContexts.Find(Ability);
}

bool UCustomAbilityComponent::TryActivateAbility(UCustomAbility* Ability)
{
    if (!IsAbilityAvailable(Ability)) return false;

    FCustomAbilityExecutionContext& CurrentExecutionContext = ExecutionContexts.Add(
        Ability,
        FCustomAbilityExecutionContext(GetOwner(), Ability));
    
    if (UCustomPropertyComponent* PropComp = GetOwner()->FindComponentByClass<UCustomPropertyComponent>())
    {
        CurrentExecutionContext.SnapshotParams = PropComp->GetAllStats();
        CurrentExecutionContext.RuntimeParams = CurrentExecutionContext.SnapshotParams;
        
        // Deduct cost
        for (const auto& CostPair : Ability->Cost)
        {
            FCustomModifier CostMod(CostPair.Key, -CostPair.Value, ECustomModifierOp::CmOp_Add, 0, Ability);
            PropComp->ApplyModifiers({CostMod});
        }
    }
    
    CurrentExecutionContext.AggregatedTags = OwnerTags;
    
    ActiveAbilities.Add(Ability);
    
    Ability->StartExecution(CurrentExecutionContext);
    PlayAbilityAnimation(Ability);
    
    Cooldowns.Add(Ability, GetWorld()->GetTimeSeconds() + Ability->Cooldown);

    return true;
}

bool UCustomAbilityComponent::TryActivateAbilityByClass(TSoftClassPtr<UCustomAbility> AbilityClassSoft)
{
    if (!AbilityClassSoft.IsValid())
    {
        UClass* LoadedClass = AbilityClassSoft.LoadSynchronous();
        if (!LoadedClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to load ability class!"));
            return false;
        }
        AbilityClassSoft = LoadedClass;
    }
    
    UCustomAbility* AbilityObj = NewObject<UCustomAbility>(this, AbilityClassSoft.Get());
    if (!AbilityObj) return false;
    
    return TryActivateAbility(AbilityObj);
}


void UCustomAbilityComponent::PlayAbilityAnimation(UCustomAbility* Ability)
{
    if (!Ability || !Ability->MontageToPlay) return;
   
    auto * SkeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
    if (!SkeletalMeshComponent) return;
    UAnimInstance* AnimInst = SkeletalMeshComponent->GetAnimInstance();
    if (!AnimInst) return;

    AnimInst->Montage_Play(Ability->MontageToPlay);
}

void UCustomAbilityComponent::CancelAbility(UCustomAbility* Ability)
{
    if (!Ability) return;
    
    FCustomAbilityExecutionContext* Context = ExecutionContexts.Find(Ability);
    if (!Context) return;
    
    if (Ability->GetState() != ECustomAbilityState::CaState_Idle)
    {
        Ability->Interrupt(*Context);
    }
    
    ActiveAbilities.Remove(Ability);
    
    ExecutionContexts.Remove(Ability);
}