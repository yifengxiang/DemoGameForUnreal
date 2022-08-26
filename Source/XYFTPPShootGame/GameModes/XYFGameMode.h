// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "XYFGameMode.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskFinishDelegate, bool, isFinish);
UCLASS()
class XYFTPPSHOOTGAME_API AXYFGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool GetIsTaskOneFinish();
	UFUNCTION(BlueprintCallable)
	void SetIsTaskOneFinish(bool isFinish);
	UFUNCTION(BlueprintCallable)
	bool GetIsTaskTwoFinish();
	UFUNCTION(BlueprintCallable)
	void SetIsTaskTwoFinish(bool isFinish);

	UPROPERTY(BlueprintAssignable, Category = "Task")
	FOnTaskFinishDelegate OnTaskOneFinish;

	UPROPERTY(BlueprintAssignable, Category = "Task")
	FOnTaskFinishDelegate OnTaskTwoFinish;

	UFUNCTION(BlueprintImplementableEvent)
	void OnActorDie(APlayerController* PlayerController);
	
private:
	bool isTaskOneFinish = false;

	bool isTaskTwoFinish = false;
};
