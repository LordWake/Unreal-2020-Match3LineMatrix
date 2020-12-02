//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+

#include "MyCustomUMG.h"
#include "Match3LineGameModeBase.h"

void UMyCustomUMG::RetryButtonFunction()
{
	AMatch3LineGameModeBase* GameMode = (AMatch3LineGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->RetryGame();
}

void UMyCustomUMG::ExitButtonFunction()
{
	AMatch3LineGameModeBase* GameMode = (AMatch3LineGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->ExitGame();
}
