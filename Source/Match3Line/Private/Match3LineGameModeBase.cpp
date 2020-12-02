// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Match3LineGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyGameState.h"
#include "MyUserPawn.h"
#include "MyPlayerState.h"

void AMatch3LineGameModeBase::RetryGame()
{
	AMyPlayerState* PS	= Cast<AMyPlayerState>	(GetWorld()->GetGameState()->PlayerArray[0]);
	AMyGameState* GS	= Cast<AMyGameState>	(GetWorld()->GetGameState());
	AMyUserPawn* MyPawn = Cast<AMyUserPawn>		(GetWorld()->GetFirstPlayerController()->GetPawn());

	GS->ResetGame();
	PS->ResetGame();
	MyPawn->RefreshCubes();
}

void AMatch3LineGameModeBase::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), 
	GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
