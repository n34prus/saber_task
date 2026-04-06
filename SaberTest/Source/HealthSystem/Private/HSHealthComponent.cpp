#include "HSHealthComponent.h"
#include "EBEventBusSubsystem.h"
#include "PTGameplayTags.h"

UHSHealthComponent::UHSHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHSHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    MaxHealth = FMath::Max(1.f, MaxHealth);
    CurrentHealth = MaxHealth;

    if (AActor* Owner = GetOwner())
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UHSHealthComponent::HandleDamage);
    }
}

void UHSHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (AActor* Owner = GetOwner())
    {
        Owner->OnTakeAnyDamage.RemoveDynamic(this, &UHSHealthComponent::HandleDamage);
    }

    Super::EndPlay(EndPlayReason);
}

float UHSHealthComponent::GetHealthPercent() const
{
    return MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f;
}

float UHSHealthComponent::GetHealthValue() const
{
    return CurrentHealth;
}

bool UHSHealthComponent::IsDead() const
{
    return CurrentHealth <= 0.f;
}

void UHSHealthComponent::Heal(float HealAmount)
{
    if (HealAmount <= 0.f || IsDead())
    {
        return;
    }

    const float OldHealth = CurrentHealth;
    CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
    OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth - OldHealth);
}

void UHSHealthComponent::ResetHealth()
{
    const float OldHealth = CurrentHealth;
    CurrentHealth = MaxHealth;
    OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth - OldHealth);
}

void UHSHealthComponent::HandleDamage(
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
        GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
        {
            OnDeath.Broadcast(GetOwner());
            if (UEBEventBusSubsystem* Bus = GetWorld()->GetGameInstance()->GetSubsystem<UEBEventBusSubsystem>())
            {
                FEBEventData Event;
                Event.Topic = PluginTags::TAG_Health_Death;
                Event.Source = GetOwner();
                Bus->Publish(Event);
            }
        });
    }
}