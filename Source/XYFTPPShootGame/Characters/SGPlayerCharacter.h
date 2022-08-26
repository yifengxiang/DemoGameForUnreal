// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XYFTPPShootGame/Characters/CharacterBase.h"
#include "SGPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class XYFTPPSHOOTGAME_API ASGPlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	ASGPlayerCharacter();

	/** 属性复制 */
	virtual  void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FVector GetCameraDirection();

	virtual  void Dead() override;

	virtual void Tick(float DeltaSeconds) override;



protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraComponent", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CameraComponent", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* ThirdCameraSpringArm;
	
};
