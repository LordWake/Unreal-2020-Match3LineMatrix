//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+

#include "CubesMatrixManager.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameState.h"
#include "CubeActor.h"
#include "MyUserController.h"
#include "MyPlayerState.h"
#include "ModelViewManager.h"

ACubesMatrixManager::ACubesMatrixManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACubesMatrixManager::BeginPlay()
{
	Super::BeginPlay();
	
	ModelView			= Cast<AModelViewManager>	(UGameplayStatics::GetActorOfClass(GetWorld(), AModelViewManager::StaticClass()));
	MyPlayerState		= Cast<AMyPlayerState>		(GetWorld()->GetGameState()->PlayerArray[0]);
	MyGameState			= Cast<AMyGameState>		(GetWorld()->GetGameState());
	MyUserController	= Cast<AMyUserController>	(GetWorld()->GetFirstPlayerController());

	SpawnAllCubesGridMatrix();

	GetWorldTimerManager().SetTimer(CubesFeedBackTimerHandle, this,
	&ACubesMatrixManager::SelectedCubesFeedBack, 0.1f, true, 0.0f);
}

void ACubesMatrixManager::SpawnAllCubesGridMatrix()
{
	//Makes the 2D Matrix and ask to spawn a cube.
	float zBase = 0.0f;
	float yBase = 0.0f;

	for (int32 i = 0; i < gridWidth; i++)
	{
		for (int32 j = 0; j < gridHeight; j++)
		{			
			SpawnCubeActor(GetActorLocation() + FVector(0, yBase, zBase + j * distanceBetweenCubes), i,	j);
		}
		yBase -= distanceBetweenCubes;
	}
}

void ACubesMatrixManager::SpawnCubeActor(const FVector& cubePosition, const int32& i, const int32& j)
{
	//Creates a cube with a value and a color type, and save it into a 2D Matrix.
	int32 randomValue = FMath::RandRange(0, AllMaterials.Num() - 1);
	
	FActorSpawnParameters parameters;

	ACubeActor* ThisCube =	GetWorld()->SpawnActor<ACubeActor>(MyCubeObject, cubePosition,
							GetActorRotation(), parameters);
		
	//Saves cube in CubesMatrix.
	CubesMatrix[i][j] = static_cast<ACubeActor*>(ThisCube);
	
	//Changes cube material.
	ThisCube->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, AllMaterials[randomValue]);
	
	//Saves this cube and his type of value.
	AllCubesAndTypes.Emplace(ThisCube, static_cast<E_TYPE_OF_CUBE>(randomValue));
}

void ACubesMatrixManager::PlayerStartSelectingCubes(AActor* HitObject)
{
	//If we are re-ordering or re-filling the grid, it will ignore de Player Input.
	if(bOnFixingGrid) return;

	if (ACubeActor* TempCube = static_cast<ACubeActor*>(HitObject))
	{
		//If it is our first cube, we add it.
		if (SelectedCubes.Num() == 0)
		{
			SelectedCubes.Emplace(TempCube);
			ModelView->DrawLineMatch(TempCube->GetActorLocation());
			ModelView->PlaySoundSFX(E_TYPE_OF_NOTIFY::SELECT_CUBE_SFX);
		}

		else
		{
			//I save the X and Y position of this Current Cube in the 2D Matrix.
			TArray<uint8_t> XY_Indexs			= ReturnMeCubeIndex(TempCube);
			//I save the X and Y position of the cube I select before in the 2D Matrix.
			TArray<uint8_t> XY_OLDCUBE_Indexs	= ReturnMeCubeIndex(SelectedCubes[SelectedCubes.Num() - 1]);

			//If I didn't have the cube I'm Selected, it means is a new one.
			if (!SelectedCubes.Contains(TempCube))
			{
				//It checks adjacents cubes in 2D Matrix. It checks one To Right, one to Left, one Up and one Down.
				if (XY_Indexs[0] == XY_OLDCUBE_Indexs[0] - 1 && XY_Indexs[1] == XY_OLDCUBE_Indexs[1] ||
					XY_Indexs[0] == XY_OLDCUBE_Indexs[0] + 1 && XY_Indexs[1] == XY_OLDCUBE_Indexs[1] ||
					XY_Indexs[1] == XY_OLDCUBE_Indexs[1] - 1 && XY_Indexs[0] == XY_OLDCUBE_Indexs[0] ||
					XY_Indexs[1] == XY_OLDCUBE_Indexs[1] + 1 && XY_Indexs[0] == XY_OLDCUBE_Indexs[0])
				{
					//All right, we don't have add this cube before and is a adjacent to us. Now, we check if->
					//(1) = This cube is the same type as the last one we save?
					//(2) = The last cube we save is a special cube type? which means we can add it too.
					//(3) = This cube is a special cube type? which means we can add it
					if (AllCubesAndTypes[TempCube] == AllCubesAndTypes[SelectedCubes[SelectedCubes.Num() - 1]]	 ||
						AllCubesAndTypes[SelectedCubes[SelectedCubes.Num() - 1]] == E_TYPE_OF_CUBE::SPECIALCOLOR ||
						AllCubesAndTypes[TempCube] == E_TYPE_OF_CUBE::SPECIALCOLOR)
					{
						SelectedCubes.Emplace(TempCube);
						ModelView->PlaySoundSFX(E_TYPE_OF_NOTIFY::SELECT_CUBE_SFX);
						ModelView->DrawLineMatch(TempCube->GetActorLocation());
					}
				}
			}

			//If we already have the cube the player is selecting...
			else
			{
				//If we have more than two cubes selected, it means the player is going backwards.
				if (SelectedCubes.Num() > 1)
				{
					//Now if this cube is equal to the last one we save 
					//we can be 100% sure that the player is going backwards 
					if (TempCube == SelectedCubes[SelectedCubes.Num() - 2])
					{
						SelectedCubes[SelectedCubes.Num() - 1]->SetActorRotation(FRotator(0, 0, 0));
						//So, it removes this cube from the selected ones.
						SelectedCubes.RemoveAt(SelectedCubes.Num() - 1);
						//It asks for some feedback sound.
						ModelView->PlaySoundSFX(E_TYPE_OF_NOTIFY::UNSELECT_CUBE_SFX);
						//It removes the last point in the draw line.
						ModelView->RemoveLastPointInDrawLine();
					}
				}
			}
		}
	}
}

void ACubesMatrixManager::PlayerStopSelectingCubes()
{
	for (auto& Elem : SelectedCubes)
		Elem->SetActorRotation(FRotator(0,0,0));
	
	//If the player selected the minimum number of cubes required
	if (SelectedCubes.Num() >= minCubesToMatch)
	{
		//We pass all the cubes to this function in charge of deactivating them
		for (auto& Elem : SelectedCubes)
		{
			OnDestroyCube(Elem);
		}

		//Some feedback sound.
		ModelView->PlaySoundSFX(E_TYPE_OF_NOTIFY::ON_MATCH_SFX);
		//Subtrack one movement.
		MyGameState->SubtractAPlayerMove();
		//Add score to player state
		MyPlayerState->AddPlayerScore(SelectedCubes.Num());

		//And now that the grid is with empty cubes, we re order it.
		ReOrderAllCubes();
	}
	//If the player didn't selected the minimum number of cubes required
	else
		//Some feedback sound when the player didn't match.
		ModelView->PlaySoundSFX(E_TYPE_OF_NOTIFY::ON_FAILED_MATCH_SFX);

	//Empties all selected cubes list.
	SelectedCubes.Empty();
	//And stops drawing the line.
	ModelView->StopDrawing();
}

void ACubesMatrixManager::SelectedCubesFeedBack()
{
	if (SelectedCubes.Num() > 0)
	{
		for (auto& Elem : SelectedCubes)
		{
			int32 randomvalue = FMath::RandRange(-5, 5);
			Elem->SetActorRotation(FRotator(0, 0, randomvalue));
		}
	}
}

void ACubesMatrixManager::RefreshCubeColors()
{
	//If we are re-ordering, re-filling the grid or clicking cubes, it will ignore de Player Input.
	if (bOnFixingGrid || SelectedCubes.Num() > 0) return;
	
	//Empties all selected cubes list.
	AllCubesAndTypes.Empty();

	//Iterates through all 2D Matrix to change cubes values.
	for (int32 i = 0; i < gridWidth; i++)
	{
		for (int32 j = 0; j < gridHeight; j++)
		{
			int32 randomValue = FMath::RandRange(0, AllMaterials.Num() - 1);

			ACubeActor* ThisCube = CubesMatrix[i][j];
			ThisCube->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, AllMaterials[randomValue]);

			AllCubesAndTypes.Emplace(ThisCube, (E_TYPE_OF_CUBE)randomValue);
		}
	}
}

void ACubesMatrixManager::OnDestroyCube(ACubeActor* ThisCube)
{
	//Just disable the cube and play a sound.
	ThisCube->SetActorHiddenInGame(true);
	ThisCube->SetActorEnableCollision(false);
	//Spawn a particle.
	ModelView->SpawnVFX(ThisCube->GetActorLocation(), E_TYPE_OF_NOTIFY::ON_KILL_CUBE_VFX);
}

void ACubesMatrixManager::ReOrderAllCubes()
{
	//Sets this to true so it ignores player's inputs.
	bOnFixingGrid = true;
	//Sets this to 1 to ignore the cubes on the floor.
	delayHeightCounter = 1;
	
	float delayTime = 0.0005f;

	//Some feedback UI.
	MyUserController->UpdateFixingGridStart(bOnFixingGrid);
	//This function re-order all cubes with a little bit of delay feedback.
	GetWorldTimerManager().SetTimer(ReOrderTimerHandle, this, &ACubesMatrixManager::ReOrderAllCubesWithDelay, delayTime, true, 0);	
}

void ACubesMatrixManager::ReOrderAllCubesWithDelay()
{	
	//If reOrderCubesRepeating == gridHeight it means that we have iterate the whole 2D Matrix "X" times
	//and we can be sure that all the cube's been re-ordered.
	if (reOrderCubesRepeating == gridHeight)
	{
		GetWorldTimerManager().ClearTimer(ReOrderTimerHandle);
	
		delayWidthCounter = 0;
		delayHeightCounter = 0;
		delayIterations = 0;
		reOrderCubesRepeating = 0;

		//Once it's reorder the whole grid, now fills the empty spaces.
		ReFillEmptySpaces();
	}
	
	else
	{
		//Subtracts gridWidth once because we don't want to check the cubes on the floor.
		if (delayIterations == (gridWidth * gridHeight) - gridWidth)
		{
			//If it's iterate the whole grid, adds one to "reOrder Cube Repeating" (to know when to stop) 
			//And then, reset his value to start check everything again.
			reOrderCubesRepeating++;
			delayIterations = 0;
		}
				
		else
		{
			//if we are on the top of the gridHeight
			//we reset this value and check one more in X.
			if (delayHeightCounter == gridHeight)
			{
				delayHeightCounter = 1;
				
				delayWidthCounter = delayWidthCounter == gridWidth - 1 ? 0 : delayWidthCounter = delayWidthCounter + 1;
			}

			//If this cube is not hidden and the one below it is, we can make this cube go down.
			if (!CubesMatrix[delayWidthCounter][delayHeightCounter]->IsHidden() &&
				 CubesMatrix[delayWidthCounter][delayHeightCounter - 1]->IsHidden())
			{
				//We Swap cubes
				ACubeActor* DestroyedCube	= CubesMatrix[delayWidthCounter][delayHeightCounter - 1];
				ACubeActor* ThisCube		= CubesMatrix[delayWidthCounter][delayHeightCounter];

				FVector destroyedCubePosition	= DestroyedCube->GetActorLocation();
				FVector ThisCubePosition		= ThisCube->GetActorLocation();

				CubesMatrix[delayWidthCounter][delayHeightCounter]		->SetActorLocation(destroyedCubePosition);
				CubesMatrix[delayWidthCounter][delayHeightCounter - 1]	->SetActorLocation(ThisCubePosition);

				CubesMatrix[delayWidthCounter][delayHeightCounter]		= DestroyedCube;
				CubesMatrix[delayWidthCounter][delayHeightCounter - 1]	= ThisCube;
			}
			
			//Once it went through this column, he moves to the next.
			delayHeightCounter++;
			delayIterations++;
		}
	}
}

void ACubesMatrixManager::ReFillEmptySpaces()
{	
	float delayTime = 0.005f;

	//This function activate hidden cubes with a little bit of delay feedback.
	GetWorldTimerManager().SetTimer(ReFillTimerHandle, this, &ACubesMatrixManager::ReFillGridWithDelay, delayTime, true, 0);
}

void ACubesMatrixManager::ReFillGridWithDelay()
{
	//If delayIterations >= all cubes amount it means that we have iterate the whole 2D Matrix.
	if (delayIterations >= gridWidth * gridHeight)
	{
		GetWorldTimerManager().ClearTimer(ReFillTimerHandle);
		
		delayWidthCounter = 0;
		delayHeightCounter = 0;
		delayIterations = 0;
		
		//Some feedback UI.
		bOnFixingGrid = false;
		MyUserController->UpdateFixingGridStart(bOnFixingGrid);
	}
	
	else
	{
		delayWidthCounter  = delayWidthCounter  == gridWidth  ? 0 : delayWidthCounter;
		delayHeightCounter = delayHeightCounter	== gridHeight ? 0 : delayHeightCounter;

		//If this cube is hidden...
		if (CubesMatrix[delayWidthCounter][delayHeightCounter]->IsHidden())
		{
			//Spawns a particle
			ModelView->SpawnVFX(CubesMatrix[delayWidthCounter][delayHeightCounter]->GetActorLocation(), E_TYPE_OF_NOTIFY::ON_SPAWN_CUBE_VFX);

			//Activates this cube's plane and physics
			CubesMatrix[delayWidthCounter][delayHeightCounter]->SetActorHiddenInGame(false);
			CubesMatrix[delayWidthCounter][delayHeightCounter]->SetActorEnableCollision(true);

			//it assigns it a random color and save this cube.
			int32 randomValue	= FMath::RandRange(0, AllMaterials.Num() - 1);
			auto ThisCube		= CubesMatrix[delayWidthCounter][delayHeightCounter];
			
			ThisCube->FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, AllMaterials[randomValue]);

			AllCubesAndTypes.Emplace(ThisCube, (E_TYPE_OF_CUBE)randomValue);
		}
	}
	
	delayHeightCounter++;
	delayWidthCounter++;
	delayIterations++;
}

TArray<uint8_t> ACubesMatrixManager::ReturnMeCubeIndex(ACubeActor* ThisCube)
{
	//Iterates the whole 2D Matrix and If a cubes match 
	//returns an array with two values = X and Y position.
	for (int i = 0; i < gridWidth; i++)
	{
		for (int j = 0; j < gridHeight; j++)
		{
			if (CubesMatrix[i][j] == ThisCube)
			{
				TArray<uint8_t> TempArray;
				TempArray.Emplace(i);
				TempArray.Emplace(j);
				return TempArray;
			}
		}
	}
	return TArray<uint8_t>();
}