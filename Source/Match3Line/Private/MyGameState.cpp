//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+

#include "MyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "MyUserController.h"

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();
	MyUserController = Cast<AMyUserController>(GetWorld()->GetFirstPlayerController());
	initialMovesRemaining = movesRemaining;
	
	changePitchPercent = (25 * initialMovesRemaining) / 100;

	BackGroundSpawnedAudio = UGameplayStatics::SpawnSound2D(GetWorld(), BackgroundMusic, 1.f, 1.f, 0.f, nullptr, false, false);
}

void AMyGameState::SubtractAPlayerMove()
{
	movesRemaining--;
	MyUserController->UpdatePlayerMoves(movesRemaining);

	if(movesRemaining == changePitchPercent)
	BackGroundSpawnedAudio->SetPitchMultiplier(1.25f);

	if(movesRemaining <= 0)
	TheGameIsOver();
}

void AMyGameState::TheGameIsOver()
{
	BackGroundSpawnedAudio->SetPitchMultiplier(1.0f);
	MyUserController->UpdateOnGameOverScreen();
}

void AMyGameState::ResetGame()
{
	movesRemaining = initialMovesRemaining;
	MyUserController->UpdatePlayerMoves(movesRemaining);
}
