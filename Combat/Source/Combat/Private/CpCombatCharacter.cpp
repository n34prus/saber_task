#include "CpCombatCharacter.h"

#include "Camera/CameraComponent.h"
#include "CpCombatHealthComponent.h"
#include "CpCombatSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"

ACpCombatCharacter::ACpCombatCharacter()
{
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetCapsuleComponent());
    CameraBoom->TargetArmLength = 350.f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    HealthComponent = CreateDefaultSubobject<UCpCombatHealthComponent>(TEXT("HealthComponent"));
    AbilityComponent = CreateDefaultSubobject<UCombatAbilityComponent>(TEXT("AbilityComponent"));
    ModifierComponent = CreateDefaultSubobject<UCpDamageModifierComponent>(TEXT("ModifierComponent"));
}

void ACpCombatCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (HealthComponent)
    {
        HealthComponent->OnDeath.AddDynamic(this, &ACpCombatCharacter::OnDeath);
        HealthComponent->OnHealthChanged.AddDynamic(this, &ACpCombatCharacter::OnHealthChanged);
    }
}

void ACpCombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACpCombatCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACpCombatCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACpCombatCharacter::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACpCombatCharacter::LookUp);

    PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &ACpCombatCharacter::Attack);
    PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &ACpCombatCharacter::Dash);
}

void ACpCombatCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.f)
    {
        const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ACpCombatCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.f)
    {
        const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void ACpCombatCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void ACpCombatCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void ACpCombatCharacter::Attack()
{
    // better to add/remove abilities on Register/Unregister actor as member
    if (auto * CombatSubsystem = UCpCombatSubsystem::Get(GetWorld()))
    {
        if ((CombatSubsystem->GetCombatState() == ECpCombatState::CpCombat_Active) &&
            (CombatSubsystem->IsMemberRegistered(this)))
        {
            AbilityComponent->ActivateAbilityByTag("Attack");
        }
    }
}

void ACpCombatCharacter::Dash()
{
    AbilityComponent->ActivateAbilityByTag("Dash");
}

void ACpCombatCharacter::OnHealthChanged(float NewHealth, float Delta)
{
    if (NewHealth <= 0.f) return;
    if (Delta >= 0.f) return;
    AbilityComponent->ActivateAbilityByTag("HitReaction");
}

void ACpCombatCharacter::OnDeath(AActor* DeadActor)
{
    if (Controller)
    {
        Controller->UnPossess();
    }
    
    UCapsuleComponent* Capsule = GetCapsuleComponent();
    Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    Capsule->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
    Capsule->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    
    USkeletalMeshComponent* MeshComp = GetMesh();
    MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetAllBodiesSimulatePhysics(true);
    MeshComp->WakeAllRigidBodies();

    MeshComp->bBlendPhysics = true;
}
