// Copyright Epic Games, Inc. All Rights Reserved.

#include "AFarmSimulationGameMode.h"
#include "AFarmSimulationCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAFarmSimulationGameMode::AAFarmSimulationGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
