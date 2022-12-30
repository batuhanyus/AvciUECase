// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/NeutralAIController.h"

#include "AI/NeutralPawnBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ANeutralAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree != nullptr)
		RunBehaviorTree(BehaviorTree);
}

void ANeutralAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsAngry)
	{
		//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsAngry"), true);
	}
}