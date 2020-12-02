//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+

#pragma once

#include "CoreMinimal.h"
#include "MyCustomUMG.h"
#include "GameFramework/PlayerController.h"
#include "MyUserController.generated.h"

UCLASS()
class MATCH3LINE_API AMyUserController : public APlayerController
{
	GENERATED_BODY()

public:
	
	void UpdatePlayerScore(const int32& currentScore);
	void UpdatePlayerMoves(const int32& currentRemainingMoves);
	void UpdateOnGameOverScreen();
	void UpdateFixingGridStart(const bool& bFixingGrid);

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMyCustomUMG> wMyUICanvas;

	UMyCustomUMG* MyUICanvas;

	virtual void BeginPlay() override;
};
