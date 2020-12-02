//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+

#include "MyPlayerState.h"
#include "MyUserController.h"

void AMyPlayerState::BeginPlay()
{
	Super::BeginPlay();
	MyUserController = Cast<AMyUserController>(GetWorld()->GetFirstPlayerController());
}

void AMyPlayerState::AddPlayerScore(const int32& numOfDestroyedCubes)
{
	int32 fibonacciResult = 1;
	uint8_t beforeNum = 0;
	uint8_t auxNum;

	for (int32 i = 1; i <= numOfDestroyedCubes; i++)
	{
		auxNum = fibonacciResult;
		fibonacciResult += beforeNum;
		beforeNum = auxNum;
	}

	playerScore += fibonacciResult;
	MyUserController->UpdatePlayerScore(playerScore);
}

void AMyPlayerState::ResetGame()
{
	playerScore = 0;
	MyUserController->UpdatePlayerScore(playerScore);
}