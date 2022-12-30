// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileActorBase.h"
#include "GameFramework/Actor.h"
#include "WeaponActorBase.generated.h"

UCLASS()
class AVCICASE_API AWeaponActorBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponActorBase();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	float CooldownTime;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileActorBase> RocketProjectileBP;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Shoot();

private:
	bool bIsOnCooldown;

	void ResetCooldown();
};