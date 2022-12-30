// Copyright Epic Games, Inc. All Rights Reserved.

#include "AvciCaseGameMode.h"
#include "AvciCaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAvciCaseGameMode::AAvciCaseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
