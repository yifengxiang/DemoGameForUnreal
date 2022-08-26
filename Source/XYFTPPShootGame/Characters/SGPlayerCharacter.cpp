// Fill out your copyright notice in the Description page of Project Settings.


#include "XYFTPPShootGame//Characters/SGPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "XYFTPPShootGame/Controllers/SGPlayerController.h"

ASGPlayerCharacter::ASGPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	ThirdCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TirdCamera"));
	
	ThirdCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TirdCameraSpringArm"));
	ThirdCameraSpringArm->SetupAttachment(RootComponent);
	
	ThirdCamera->SetupAttachment(ThirdCameraSpringArm);
	
	ThirdCameraSpringArm->bUsePawnControlRotation = true;
	
}

void ASGPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

FVector ASGPlayerCharacter::GetCameraDirection()
{
	return ThirdCamera->GetForwardVector();
}

void ASGPlayerCharacter::Dead()
{
	Super::Dead();
	auto MyController = Cast<ASGPlayerController>(GetController());
	if(MyController)
	{
		//MyController->OnActorDie.Broadcast(MyController);
		MyController->AfterActorDie();
	}
}

void ASGPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}



void ASGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}
