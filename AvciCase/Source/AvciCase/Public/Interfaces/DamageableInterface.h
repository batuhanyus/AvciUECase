// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

class UHealthComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class AVCICASE_API IDamageableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnDeath() =0;

	virtual void OnHealthChanged() =0;

	virtual void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                                AController* InstigatedBy, AActor* DamageCauser) =0;
};