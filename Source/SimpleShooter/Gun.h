// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Visual Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = "Visual Components")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Visual Components")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Audio Components")
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere, Category = "Visual Components")
	UParticleSystem* HitFlash;

	UPROPERTY(EditAnywhere, Category = "Audio Components")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Gun Components")
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere, Category = "Gun Components")
	float Damage = 10;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;
};
