// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "XYFPVPGameMode.generated.h"

/**
 * 
 */
UCLASS()
class XYFTPPSHOOTGAME_API AXYFPVPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnActorDie(APlayerController* PlayerController);
	
};
