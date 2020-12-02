//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/App.h"
#include "CubesMatrixManager.generated.h"

#pragma region Enums

UENUM(BlueprintType)
enum class E_TYPE_OF_CUBE : uint8
{
	COLOR1				UMETA(DisplayName = "Color1"),
	COLOR2				UMETA(DisplayName = "Color2"),
	COLOR3				UMETA(DisplayName = "Color3"),
	COLOR4				UMETA(DisplayName = "Color4"),
	COLOR5				UMETA(DisplayName = "Color5"),
	SPECIALCOLOR		UMETA(DisplayName = "SpecialColor")
};

UENUM(BlueprintType)
enum class E_TYPE_OF_NOTIFY : uint8
{
	SELECT_CUBE_SFX		UMETA(DisplayName = "Select"),
	UNSELECT_CUBE_SFX	UMETA(DisplayName = "UnSelect"),
	ON_MATCH_SFX		UMETA(DisplayName = "OnMatch"),
	ON_FAILED_MATCH_SFX UMETA(DisplayName = "FailMatch"),
	
	ON_SPAWN_CUBE_VFX	UMETA(DisplayName = "SpawnCube"),
	ON_KILL_CUBE_VFX	UMETA(DisplayName = "KillCube")
};

#pragma endregion;

class AModelViewManager;
class AMyGameState;
class AMyPlayerState;
class ACubeActor;
class AMyUserController;

UCLASS()
class MATCH3LINE_API ACubesMatrixManager : public AActor
{
	GENERATED_BODY()
	
public:	

	ACubesMatrixManager();

	/// <summary> Takes the cube selected by the user and checks if it has to add it to "SelectedCubes"</summary>
	void PlayerStartSelectingCubes(AActor* HitObject);
	/// <summary> Check if the user "has match" and calls OnDestroyCube passing every selected cube</summary>
	void PlayerStopSelectingCubes();
	/// <summary> Called from MyUserPawn, iterates through all the 2D Matrix and change every cube color and type</summary>
	void RefreshCubeColors();

private:

	AMyUserController*	MyUserController;
	AModelViewManager*	ModelView;
	AMyGameState*		MyGameState;
	AMyPlayerState*		MyPlayerState;

	TMap<ACubeActor*, E_TYPE_OF_CUBE> AllCubesAndTypes;

	UPROPERTY(EditDefaultsOnly)
	TArray<UMaterialInterface*> AllMaterials;
	TArray<ACubeActor*>			SelectedCubes;

	ACubeActor* CubesMatrix [10][7];

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACubeActor> MyCubeObject;

	FTimerHandle ReOrderTimerHandle;
	FTimerHandle ReFillTimerHandle;
	FTimerHandle CubesFeedBackTimerHandle;

	const uint8_t gridWidth			= 10;
	const uint8_t gridHeight		= 7;
	const uint8_t minCubesToMatch	= 3;

	uint8_t delayWidthCounter		= 0;
	uint8_t delayHeightCounter		= 0;
	uint8_t delayIterations			= 0;
	uint8_t reOrderCubesRepeating	= 0;
	
	const float distanceBetweenCubes	= 110.0f;

	bool bOnFixingGrid = false;

//--------------------------------------------------|
//	Functions										|
//--------------------------------------------------|
	
	virtual void BeginPlay() override;

	/// <summary> Creates the 2DMatrix, it doesn't create the cubes. </summary>
	void SpawnAllCubesGridMatrix();
	/// <summary> Creates the cubes. Called from SpawnAllCubesGridMatrix </summary>
	void SpawnCubeActor(const FVector& cubePosition, const int32& i, const int32& j);
	/// <summary> Disable the cubes when player release click button. </summary>
	void OnDestroyCube(ACubeActor* ThisCube);
	/// <summary> Bridge to ReOrderAllCubesWithDelay </summary>
	void ReOrderAllCubes();
	/// <summary> Bridge to ReFillGridWithDelay </summary>
	void ReFillEmptySpaces();
	/// <summary> Move the cubes selected by the player </summary>
	void SelectedCubesFeedBack();
	
	/// <summary> Iterates through all the 2DMatrix and check which cubes needs to move down </summary>
	UFUNCTION()
	void ReOrderAllCubesWithDelay();
	/// <summary> Iterates through all the 2DMatrix and check which cubes are hidden to activate them </summary>
	UFUNCTION()
	void ReFillGridWithDelay();

	/// <summary> Called from PlayerSelectingCubes() to know a specific cube position in the 2D Matrix </summary>
	TArray<uint8_t> ReturnMeCubeIndex(ACubeActor* ThisCube);
};
