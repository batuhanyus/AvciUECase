// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NeutralAIController.generated.h"

/**
 *
 */
UCLASS()
class AVCICASE_API ANeutralAIController : public AAIController
{
	GENERATED_BODY()

public:
	bool bIsAngry;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTree;
};