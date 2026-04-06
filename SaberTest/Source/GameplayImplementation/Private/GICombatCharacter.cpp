#include "GICombatCharacter.h"

#include "Camera/CameraComponent.h"
#include "HSHealthComponent.h"
#include "CSCombatSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"

AGICombatCharacter::AGICombatCharacter()
{
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    GetCharacterMovement()->bRunPhysicsWithNoController = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetCapsuleComponent());
    CameraBoom->TargetArmLength = 350.f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    HealthComponent = CreateDefaultSubobject<UHSHealthComponent>(TEXT("HealthComponent"));
    AbilityComponent = CreateDefaultSubobject<UASAbilityComponent>(TEXT("AbilityComponent"));
    ModifierComponent = CreateDefaultSubobject<UASDamageModifierComponent>(TEXT("ModifierComponent"));
}

void AGICombatCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (HealthComponent)
    {
        HealthComponent->OnDeath.AddDynamic(this, &AGICombatCharacter::OnDeath);
        HealthComponent->OnHealthChanged.AddDynamic(this, &AGICombatCharacter::OnHealthChanged);
    }
}

void AGICombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AGICombatCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AGICombatCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AGICombatCharacter::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AGICombatCharacter::LookUp);

    PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AGICombatCharacter::Attack);
    PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AGICombatCharacter::Dash);
}

void AGICombatCharacter::MoveForward(float Value)
{
    if (Controller && Value != 0.f)
    {
        const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AGICombatCharacter::MoveRight(float Value)
{
    if (Controller && Value != 0.f)
    {
        const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void AGICombatCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void AGICombatCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void AGICombatCharacter::Attack()
{
    // better to add/remove abilities on Register/Unregister actor as member
    if (auto * CombatSubsystem = UCSCombatSubsystem::Get(GetWorld()))
    {
        if ((CombatSubsystem->GetCombatState() == ECpCombatState::CpCombat_Active) &&
            (CombatSubsystem->IsMemberRegistered(this)))
        {
            AbilityComponent->ActivateAbilityByTag("Attack");
        }
    }
}

void AGICombatCharacter::Dash()
{
    AbilityComponent->ActivateAbilityByTag("Dash");
}

void AGICombatCharacter::OnHealthChanged(float NewHealth, float Delta)
{
    if (NewHealth <= 0.f) return;
    if (Delta >= 0.f) return;
    AbilityComponent->ActivateAbilityByTag("HitReaction");
}

void AGICombatCharacter::OnDeath(AActor* DeadActor)
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
