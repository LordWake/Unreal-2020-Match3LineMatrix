//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyUserPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerHit);

class ACubesMatrixManager;
class AMyUserController;

UCLASS()
class MATCH3LINE_API AMyUserPawn : public APawn
{
	GENERATED_BODY()

	AMyUserPawn();

public:

	void RefreshCubes();

private:

	ACubesMatrixManager*	MyCubesMatrix;
	AMyUserController*		MyController;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	FOnPlayerHit PlayerIsClickingDelegate;

//--------------------------------------------------|
//	Functions										|
//--------------------------------------------------|

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayerStartClick();
	void PlayerStopClick();
	
	UFUNCTION()
	void LineTraceHitFromScreen();
};
