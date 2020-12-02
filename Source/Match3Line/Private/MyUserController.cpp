//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+

#include "MyUserController.h"


void AMyUserController::BeginPlay()
{
	Super::BeginPlay();

	MyUICanvas = CreateWidget<UMyCustomUMG>(this, wMyUICanvas);
	
	if (MyUICanvas)	
	MyUICanvas->AddToViewport();

	bShowMouseCursor = true;
}

void AMyUserController::UpdatePlayerScore(const int32& currentScore)
{
	if(MyUICanvas)
	MyUICanvas->UpdatePlayerScoreOnUI(currentScore);
}

void AMyUserController::UpdatePlayerMoves(const int32& currentRemainingMoves)
{
	if (MyUICanvas)
		MyUICanvas->UpdatePlayerMovesOnUI(currentRemainingMoves);
}

void AMyUserController::UpdateOnGameOverScreen()
{
	if (MyUICanvas)
		MyUICanvas->UpdateOnGameOverScreenUI();
}

void AMyUserController::UpdateFixingGridStart(const bool& bFixingGrid)
{
	if (MyUICanvas)
		MyUICanvas->UpdateFixingGridUI(bFixingGrid);
}
