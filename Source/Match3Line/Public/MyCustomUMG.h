//+-------------------------------------------------------+
//| Project   : Match3LineDrawer UE 4.24				  |
//| Author    : github.com/LordWake					 	  |		
//+-------------------------------------------------------+

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyCustomUMG.generated.h"

class AMatch3LineGameModeBase;
UCLASS()
class MATCH3LINE_API UMyCustomUMG : public UUserWidget
{
	GENERATED_BODY()

public:

	#pragma region UI BIND FUNCTIONS

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerScoreOnUI(const int32& playerScore);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerMovesOnUI(const int32& playerMovesRemaining);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateOnGameOverScreenUI();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateFixingGridUI(bool bFixingGrid);

	UFUNCTION(BlueprintCallable)
	void RetryButtonFunction();

	UFUNCTION(BlueprintCallable)
	void ExitButtonFunction();

	#pragma endregion;

};
