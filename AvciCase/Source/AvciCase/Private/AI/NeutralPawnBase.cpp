// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NeutralPawnBase.h"

#include "AI/NeutralAIController.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UI/HealthBarWidget.h"

// Sets default values
ANeutralPawnBase::ANeutralPawnBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
}

// Called when the game starts or when spawned
void ANeutralPawnBase::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ANeutralPawnBase::OnTakeRadialDamage);
	HealthComponent->OnDeath.AddDynamic(this, &ANeutralPawnBase::OnDeath);
	HealthComponent->OnHealthChanged.AddDynamic(this, &ANeutralPawnBase::OnHealthChanged);

	MID = StaticMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, StaticMeshComponent->GetMaterial(0));
}

void ANeutralPawnBase::OnDeath()
{
	MID->SetVectorParameterValue("Colour", FLinearColor::Black);
	FVector3d Impulse = FVector3d(FMath::RandRange(0, 200), FMath::RandRange(0, 300), 1000);
	StaticMeshComponent->AddImpulse(Impulse, NAME_None, true);

	SpawnNewNeutral();

	GetWorld()->SpawnActor<AActor>(ExplosionEffectBP, GetActorLocation(), GetActorRotation());

	Destroy();
}

void ANeutralPawnBase::OnHealthChanged()
{
	MID->SetVectorParameterValue("Colour", FLinearColor::Red);

	AController* MyController = GetController();
	ANeutralAIController* AIController = Cast<ANeutralAIController>(MyController);

	if (AIController)
		AIController->bIsAngry = true;
}

void ANeutralPawnBase::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                          AController* InstigatedBy, AActor* DamageCauser)
{
	HealthComponent->ReduceHealth(Damage);
}


// Called every frame
void ANeutralPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANeutralPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANeutralPawnBase::SpawnNewNeutral()
{
	FVector3d SpawnLocation = FVector3d(FMath::RandRange(-3000, 3000), FMath::RandRange(-3000, 3000), 100);
	ANeutralPawnBase* SpawnedPawn = GetWorld()->SpawnActor<ANeutralPawnBase>(
		GetClass(), SpawnLocation, GetActorRotation());

	ANeutralAIController* SpawnedController = GetWorld()->SpawnActor<ANeutralAIController>(
		NeutralAIBP, SpawnLocation, GetActorRotation());

	if (!SpawnedController)return;


	SpawnedController->Possess(SpawnedPawn);
}