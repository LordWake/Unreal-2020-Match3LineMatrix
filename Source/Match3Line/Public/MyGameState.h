//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+
#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameState.generated.h"

class AMyUserController;
UCLASS()
class MATCH3LINE_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	
	void SubtractAPlayerMove();
	void ResetGame();

private:
	
	AMyUserController* MyUserController;
	UAudioComponent* BackGroundSpawnedAudio;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* BackgroundMusic;

	int32 initialMovesRemaining = 0;
	int32 changePitchPercent	= 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 movesRemaining;

	void TheGameIsOver();
	virtual void BeginPlay() override;
};
