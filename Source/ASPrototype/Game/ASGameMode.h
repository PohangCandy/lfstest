// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ASGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(AS, Log, All);

/**
 * 
*/
UCLASS()
class ASPROTOTYPE_API AASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AASGameMode();
	UFUNCTION(BlueprintCallable, Category = "UMG_Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	virtual void PostLogin(APlayerController* NewPlayer) override;
	bool IsEscape;

	class AASCharacterPlayer* Player;
	class AASFinalTarget* Target;
	class AASEnemyBase* Enemy;
	class AASAIController* AI;
	void EscapeTimerStart(); 
	void View_WinWidget();
	void View_LoseWidget();
	void GameOver();
	class TSubclassOf<UUserWidget> CountDownWidgetClass;

	class UASCountDownWidget* CountDownWidgetUI;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;
};
