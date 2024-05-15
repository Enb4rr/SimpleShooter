// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleShooterCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"

// Sets default values
ASimpleShooterCharacter::ASimpleShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASimpleShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

    Health = MaxHealth;

    Gun = GetWorld() -> SpawnActor<AGun>(GunClass);
    GetMesh() -> HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
    Gun -> AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
    Gun -> SetOwner(this);
}

// Called every frame
void ASimpleShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASimpleShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get the player controller
    APlayerController* PC = Cast<APlayerController>(GetController());
 
    // Get the local player subsystem
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
    // Clear out existing mapping, and add our mapping
    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext(InputMapping, 0);

	// Get the EnhancedInputComponent
	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	PEI -> BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ASimpleShooterCharacter::Move);
	PEI -> BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ASimpleShooterCharacter::Look);
	PEI -> BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ASimpleShooterCharacter::JumpUp);
    PEI -> BindAction(ShootInputAction, ETriggerEvent::Triggered, this, &ASimpleShooterCharacter::Shoot);
}

void ASimpleShooterCharacter::Move(const FInputActionValue& Value)
{
    if (Controller != nullptr)
    {
        const FVector2D MoveValue = Value.Get<FVector2D>();
        const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);
 
        // Forward/Backward direction
        if (MoveValue.Y != 0.f)
        {
            // Get forward vector
            const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
 
            AddMovementInput(Direction, MoveValue.Y);
        }
 
        // Right/Left direction
        if (MoveValue.X != 0.f)
        {
            // Get right vector
            const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
 
            AddMovementInput(Direction, MoveValue.X);
        }
    }
}
 
void ASimpleShooterCharacter::Look(const FInputActionValue& Value)
{
    if (Controller != nullptr)
    {
        const FVector2D LookValue = Value.Get<FVector2D>();
 
        if (LookValue.X != 0.f)
        {
            AddControllerYawInput(LookValue.X * -1.f);
        }
 
        if (LookValue.Y != 0.f)
        {
            AddControllerPitchInput(LookValue.Y);
        }
    }
}

void ASimpleShooterCharacter::JumpUp(const FInputActionValue& Value)
{
	if(Controller != nullptr) Jump();
}

void ASimpleShooterCharacter::Shoot(const FInputActionValue& Value)
{
    if(Controller != nullptr) Gun -> PullTrigger();
}

float ASimpleShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    DamageToApply = FMath::Min(Health, DamageToApply);
    Health -= DamageToApply;

    if(IsDead())
    {
        ASimpleShooterGameModeBase* GameMode = GetWorld() -> GetAuthGameMode<ASimpleShooterGameModeBase>();
        if(GameMode != nullptr)
        {
            GameMode -> PawnKilled(this);
        }

        DetachFromControllerPendingDestroy();
        GetCapsuleComponent() -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    return DamageToApply;
}

bool ASimpleShooterCharacter::IsDead() const
{
    if(Health <= 0) return true;
    else return false;
}

float ASimpleShooterCharacter::GetHealthPercent() const
{
    return Health / MaxHealth;
}

void ASimpleShooterCharacter::ShootAI()
{
    Gun -> PullTrigger();
}

