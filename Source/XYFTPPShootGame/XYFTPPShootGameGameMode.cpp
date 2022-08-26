// Copyright Epic Games, Inc. All Rights Reserved.

#include "XYFTPPShootGameGameMode.h"
#include "XYFTPPShootGameHUD.h"
#include "XYFTPPShootGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AXYFTPPShootGameGameMode::AXYFTPPShootGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AXYFTPPShootGameHUD::StaticClass();
}
