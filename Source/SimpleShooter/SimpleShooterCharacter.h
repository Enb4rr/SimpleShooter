// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SimpleShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API ASimpleShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASimpleShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
    class UInputMappingContext* InputMapping;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	void ShootAI();

private:

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputAction* LookInputAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputAction* JumpInputAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputAction* ShootInputAction;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* Gun; 

	UPROPERTY(EditDefaultsOnly, Category = "Character Stats")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health; 

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpUp(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);

};
