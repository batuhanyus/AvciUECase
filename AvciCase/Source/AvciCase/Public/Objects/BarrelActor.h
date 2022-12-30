// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageableActorBase.h"
#include "GameFramework/Actor.h"
#include "BarrelActor.generated.h"

UCLASS()
class AVCICASE_API ABarrelActor : public ADamageableActorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABarrelActor();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ExplosionEffectBP;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnDeath() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};