// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/DamageableActorBase.h"

#include "Components/WidgetComponent.h"
#include "UI/HealthBarWidget.h"

// Sets default values
ADamageableActorBase::ADamageableActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
}

// Called when the game starts or when spawned
void ADamageableActorBase::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ADamageableActorBase::OnTakeRadialDamage);
	HealthComponent->OnDeath.AddDynamic(this, &ADamageableActorBase::OnDeath);
	HealthComponent->OnHealthChanged.AddDynamic(this, &ADamageableActorBase::OnHealthChanged);

	MID = StaticMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, StaticMeshComponent->GetMaterial(0));
}

// Called every frame
void ADamageableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADamageableActorBase::OnDeath()
{
}

void ADamageableActorBase::OnHealthChanged()
{
	UpdateHealthBar();
}

void ADamageableActorBase::UpdateHealthBar()
{
	if (!HealthBarWidgetComponent->GetUserWidgetObject()) return;

	Cast<UHealthBarWidget>(HealthBarWidgetComponent->GetUserWidgetObject())->SetHealthPercentage(
		HealthComponent->GetHealthPercentage());
}

void ADamageableActorBase::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                              AController* InstigatedBy, AActor* DamageCauser)
{
	HealthComponent->ReduceHealth(Damage);
}