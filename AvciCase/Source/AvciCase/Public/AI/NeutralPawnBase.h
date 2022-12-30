// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NeutralAIController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/DamageableInterface.h"
#include "Interfaces/HealthDisplayerInterface.h"
#include "NeutralPawnBase.generated.h"


UCLASS()
class AVCICASE_API ANeutralPawnBase : public APawn, public IDamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANeutralPawnBase();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY()
	UMaterialInstanceDynamic* MID;

	UPROPERTY(VisibleAnywhere)
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ExplosionEffectBP;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ANeutralPawnBase> NeutralPawnBP;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ANeutralAIController> NeutralAIBP;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnDeath() override;

	UFUNCTION()
	virtual void OnHealthChanged() override;

	UFUNCTION()
	virtual void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                                AController* InstigatedBy, AActor* DamageCauser) override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void SpawnNewNeutral();
};