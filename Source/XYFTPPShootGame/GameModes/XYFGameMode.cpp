// Fill out your copyright notice in the Description page of Project Settings.


#include "XYFTPPShootGame//GameModes/XYFGameMode.h"

bool AXYFGameMode::GetIsTaskOneFinish()
{
	return isTaskOneFinish;
}

void AXYFGameMode::SetIsTaskOneFinish(bool isFinish)
{
	isTaskOneFinish = isFinish;
	OnTaskOneFinish.Broadcast(isFinish);
}

bool AXYFGameMode::GetIsTaskTwoFinish()
{
	return isTaskTwoFinish;
}

void AXYFGameMode::SetIsTaskTwoFinish(bool isFinish)
{
	isTaskTwoFinish = isFinish;
	OnTaskTwoFinish.Broadcast(isFinish);
}
