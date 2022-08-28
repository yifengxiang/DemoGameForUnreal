// Fill out your copyright notice in the Description page of Project Settings.


#include "XYFTPPShootGame/Controllers/SGPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "XYFTPPShootGame/GameModes/XYFPVPGameMode.h"

void ASGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// UUserWidget* HUD = CreateWidget(this, HUDClass);
	// if(HUD != nullptr)
	// {
	// 	HUD->AddToViewport();
	// }
}

void ASGPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASGPlayerController, KillNum);
	DOREPLIFETIME(ASGPlayerController, DeadNum);
}

void ASGPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

int ASGPlayerController::GetKillNum()
{
	return KillNum;
}

int ASGPlayerController::GetDeadNum()
{
	return DeadNum;
}

void ASGPlayerController::AddKillNum()
{
	KillNum += 1;
	if(KillNum == 10)
	{
		CallSeverEndGame(true);
	}
}

void ASGPlayerController::AddDeadNum()
{
	DeadNum += 1;
	if(DeadNum == 10)
	{
		CallSeverEndGame(false);
	}
}

void ASGPlayerController::CallSeverEndGame_Implementation(bool isWin)
{
	if(Cast<AXYFPVPGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		MutiEndGame(isWin);
	}

}

void ASGPlayerController::MutiEndGame_Implementation(bool isWin)
{
	EndGame(isWin);
}

