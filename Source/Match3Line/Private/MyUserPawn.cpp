//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+

#include "MyUserPawn.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"
#include "MyUserController.h"
#include "CubesMatrixManager.h"

#define MOUSE_RIGHT_CLICK	"R_Click"
#define MOUSE_LEFT_CLICK	"L_Click"

AMyUserPawn::AMyUserPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyUserPawn::BeginPlay()
{
	Super::BeginPlay();
	
	MyCubesMatrix = Cast<ACubesMatrixManager>	(UGameplayStatics::GetActorOfClass(GetWorld(), ACubesMatrixManager::StaticClass()));
	MyController =	Cast<AMyUserController>		(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
}

void AMyUserPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerIsClickingDelegate.Broadcast();
}

void AMyUserPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(MOUSE_LEFT_CLICK,	EKeys::LeftMouseButton));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(MOUSE_RIGHT_CLICK,	EKeys::RightMouseButton));

	PlayerInputComponent->BindAction(MOUSE_LEFT_CLICK,	IE_Pressed,	 this, &AMyUserPawn::PlayerStartClick);
	PlayerInputComponent->BindAction(MOUSE_LEFT_CLICK,	IE_Released, this, &AMyUserPawn::PlayerStopClick);
	
	PlayerInputComponent->BindAction(MOUSE_RIGHT_CLICK, IE_Pressed,	 this, &AMyUserPawn::RefreshCubes);
}

void AMyUserPawn::RefreshCubes()
{
	MyCubesMatrix->RefreshCubeColors();
}

void AMyUserPawn::PlayerStartClick()
{
	PlayerIsClickingDelegate.AddDynamic(this, &AMyUserPawn::LineTraceHitFromScreen);
}

void AMyUserPawn::PlayerStopClick()
{
	PlayerIsClickingDelegate.RemoveDynamic(this, &AMyUserPawn::LineTraceHitFromScreen);
	MyCubesMatrix->PlayerStopSelectingCubes();
}

void AMyUserPawn::LineTraceHitFromScreen()
{
	FHitResult Hit;
	bool bHitOccurred = MyController->GetHitResultUnderCursorForObjects(ObjectTypes, true, Hit);

	if (bHitOccurred)
	{
		if (Hit.GetActor())
			MyCubesMatrix->PlayerStartSelectingCubes(Hit.GetActor());
	}
}

