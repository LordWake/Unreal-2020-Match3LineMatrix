//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+

#include "ModelViewManager.h"
#include "Kismet/GameplayStatics.h"

AModelViewManager::AModelViewManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AModelViewManager::BeginPlay()
{
	Super::BeginPlay();
	
	LineBatcher = GetWorld()->PersistentLineBatcher;

	GetWorldTimerManager().SetTimer(RepeatFunctionTimer, this,
	&AModelViewManager::RenderLineOnScene, 0.1f, true, 0.0f);
}

void AModelViewManager::DrawLineMatch(const FVector& cubePos)
{
	DrawLinePoints.Emplace(FVector(cubePos.X - 10, cubePos.Y, cubePos.Z));
}

void AModelViewManager::RenderLineOnScene()
{
	if (DrawLinePoints.Num() > 1)
	{
		for (int32 i = 0; i < DrawLinePoints.Num() - 1; i++)
		{
			LineBatcher->DrawLine(DrawLinePoints[i], DrawLinePoints[i + 1],
				LineColor, 10.0f, 10.0f, 0.1f);
		}
	}
}

void AModelViewManager::PlaySoundSFX(E_TYPE_OF_NOTIFY typeOfNotification)
{
	UGameplayStatics::PlaySound2D(GetWorld(), 
	SFXClips[typeOfNotification], 1.0f, 1.0f, 0.0f);
}

void AModelViewManager::SpawnVFX(const FVector& spawnPos, E_TYPE_OF_NOTIFY typeOfNotification)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
	VFXParticles[typeOfNotification], FVector(spawnPos.X - 20, spawnPos.Y, spawnPos.Z),
	FRotator(0, 0, 180), FVector(1.0f, 1.0f, 1.0f), true, EPSCPoolMethod::AutoRelease, true);
}

void AModelViewManager::RemoveLastPointInDrawLine()
{
	DrawLinePoints.RemoveAt(DrawLinePoints.Num() - 1);
}

void AModelViewManager::StopDrawing()
{
	DrawLinePoints.Empty();
}


