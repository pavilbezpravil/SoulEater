// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoulEaterGameMode.h"
#include "SoulEaterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASoulEaterGameMode::ASoulEaterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
