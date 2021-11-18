// Copyright Epic Games, Inc. All Rights Reserved.

#include "DZ_04GameMode.h"
#include "DZ_04Character.h"
#include "UObject/ConstructorHelpers.h"

ADZ_04GameMode::ADZ_04GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
