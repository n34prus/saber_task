#include "CpCombatHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/DamageType.h"

UCpCombatHealthComponent::UCpCombatHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCpCombatHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    MaxHealth = FMath::Max(1.f, MaxHealth);
    CurrentHealth = MaxHealth;

    if (AActor* Owner = GetOwner())
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UCpCombatHealthComponent::HandleDamage);
    }
}

void UCpCombatHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (AActor* Owner = GetOwner())
    {
        Owner->OnTakeAnyDamage.RemoveDynamic(this, &UCpCombatHealthComponent::HandleDamage);
    }

    Super::EndPlay(EndPlayReason);
}

float UCpCombatHealthComponent::GetHealthPercent() const
{
    return MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f;
}

bool UCpCombatHealthComponent::IsDead() const
{
    return CurrentHealth <= 0.f;
}

void UCpCombatHealthComponent::Heal(float HealAmount)
{
    if (HealAmount <= 0.f || IsDead())
    {
        return;
    }

    const float OldHealth = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
    OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth - OldHealth);
}

void UCpCombatHealthComponent::ResetHealth()
{
    const float OldHealth = CurrentHealth;
    CurrentHealth = MaxHealth;
    OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth - OldHealth);
}

void UCpCombatHealthComponent::HandleDamage(
    AActor* DamagedActor,
    float Damage,
    const UDamageType* DamageType,
    AController* InstigatedBy,
    AActor* DamageCauser)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        return;
    }
    
    if (Damage <= 0.f || IsDead())
    {
        return;
    }

    const float OldHealth = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
    OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth - OldHealth);

    if (CurrentHealth <= 0.f)
    {
        OnDeath.Broadcast(GetOwner());
    }
}