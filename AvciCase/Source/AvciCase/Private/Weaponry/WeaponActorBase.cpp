// Fill out your copyright notice in the Description page of Project Settings.


#include "Weaponry/WeaponActorBase.h"

// Sets default values
AWeaponActorBase::AWeaponActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void AWeaponActorBase::BeginPlay()
{
	Super::BeginPlay();

	CooldownTime = 3.0f;
}

// Called every frame
void AWeaponActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponActorBase::Shoot()
{
	if (RocketProjectileBP && !bIsOnCooldown)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		FRotator SpawnRotation = GetActorRotation();
		FVector SpawnLocation = GetActorLocation() + (SpawnRotation.Vector() * 100);

		GetWorld()->SpawnActor<AProjectileActorBase>(RocketProjectileBP, SpawnLocation, SpawnRotation, SpawnParams);

		bIsOnCooldown = true;
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AWeaponActorBase::ResetCooldown, CooldownTime, false);
	}
}

void AWeaponActorBase::ResetCooldown()
{
	bIsOnCooldown = false;
}