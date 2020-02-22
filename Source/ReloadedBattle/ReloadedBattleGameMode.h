// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ReloadedBattleGameMode.generated.h"

UCLASS(minimalapi)
class AReloadedBattleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AReloadedBattleGameMode();

	void AddToPool();
};



