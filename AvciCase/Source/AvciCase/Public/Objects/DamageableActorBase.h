// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/DamageType.h"
#include "Interfaces/DamageableInterface.h"
#include "Interfaces/HealthDisplayerInterface.h"
#include "DamageableActorBase.generated.h"

UCLASS(Abstract)
class AVCICASE_API ADamageableActorBase : public AActor, public IDamageableInterface, public IHealthDisplayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADamageableActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly)
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY()
	UMaterialInstanceDynamic* MID;

	UFUNCTION()
	virtual void OnDeath() override;

	UFUNCTION()
	virtual void OnHealthChanged() override;

	UFUNCTION()
	virtual void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                                AController* InstigatedBy, AActor* DamageCauser) override;

	virtual void UpdateHealthBar() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};