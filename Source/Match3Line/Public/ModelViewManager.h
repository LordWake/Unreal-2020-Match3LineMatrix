//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubesMatrixManager.h"
#include "Sound/SoundCue.h"
#include "Components/LineBatchComponent.h"
#include "ModelViewManager.generated.h"


UCLASS()
class MATCH3LINE_API AModelViewManager : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AModelViewManager();

	void DrawLineMatch(const FVector& cubePos);
	void PlaySoundSFX(E_TYPE_OF_NOTIFY typeOfNotification);
	void SpawnVFX(const FVector& spawnPos, E_TYPE_OF_NOTIFY typeOfNotification);
	void RemoveLastPointInDrawLine();
	void StopDrawing();

private:
	
	UPROPERTY(EditDefaultsOnly)
	FLinearColor LineColor;

	UPROPERTY(EditDefaultsOnly)
	TMap<E_TYPE_OF_NOTIFY, UParticleSystem*> VFXParticles;

	UPROPERTY(EditDefaultsOnly)
	TMap<E_TYPE_OF_NOTIFY, USoundCue*> SFXClips;

	FTimerHandle RepeatFunctionTimer;
	ULineBatchComponent* LineBatcher;
	
	TArray<FVector> DrawLinePoints;

	void RenderLineOnScene();
	virtual void BeginPlay() override;
};
