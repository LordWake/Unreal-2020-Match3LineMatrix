//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

class AMyUserController;
UCLASS()
class MATCH3LINE_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	void AddPlayerScore(const int32& numOfDestroyedCubes);
	void ResetGame();

private:
	
	AMyUserController* MyUserController;

	int32 playerScore = 0;

	virtual void BeginPlay() override;
};
