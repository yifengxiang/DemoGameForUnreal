// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XYFTPPShootGame/Characters/CharacterBase.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeadDelegate);

UCLASS()
class XYFTPPSHOOTGAME_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	virtual void Dead() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BeAttacked();

	UPROPERTY(BlueprintAssignable, Category = "DeadEvent")
	FDeadDelegate OnEnemyDead;
	
};
