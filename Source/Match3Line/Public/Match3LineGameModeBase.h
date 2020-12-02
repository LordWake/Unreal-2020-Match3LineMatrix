// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Match3LineGameModeBase.generated.h"

UCLASS()
class MATCH3LINE_API AMatch3LineGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	void RetryGame();
	void ExitGame();
};
