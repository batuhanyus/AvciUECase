// Fill out your copyright notice in the Description page of Project Settings.


#include "Weaponry/ProjectileActorBase.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileActorBase::AProjectileActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void AProjectileActorBase::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileActorBase::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
                                     bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
                                     const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile hit something NOTIFY"));


	//Double damage.
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 50.0f, GetActorLocation(), 1000.0f,
	                                    UDamageType::StaticClass(),
	                                    TArray<AActor*>(), this, GetInstigatorController(), true);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), 50.0f, GetActorLocation(), 300.0f,
	                                        UDamageType::StaticClass(),
	                                        TArray<AActor*>(), this, GetInstigatorController(), true);

	Destroy();
}


// Called every frame
void AProjectileActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}