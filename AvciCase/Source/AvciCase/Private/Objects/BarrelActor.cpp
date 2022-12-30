// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/BarrelActor.h"

#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "UI/HealthBarWidget.h"

// Sets default values
ABarrelActor::ABarrelActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABarrelActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABarrelActor::OnDeath()
{
	Super::OnDeath();

	MID->SetVectorParameterValue("Colour", FLinearColor::Black);
	FVector3d Impulse = FVector3d(FMath::RandRange(0, 200), FMath::RandRange(0, 300), 1000);
	StaticMeshComponent->AddImpulse(Impulse, NAME_None, true);

	GetWorld()->SpawnActor<AActor>(ExplosionEffectBP, GetActorLocation(), GetActorRotation());
}

// Called every frame
void ABarrelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}