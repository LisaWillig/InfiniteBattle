// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ReloadedBattleGameMode.h"
#include "ReloadedBattleHUD.h"
#include "Character_1P/FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AReloadedBattleGameMode::AReloadedBattleGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Character/BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AReloadedBattleHUD::StaticClass();
}
