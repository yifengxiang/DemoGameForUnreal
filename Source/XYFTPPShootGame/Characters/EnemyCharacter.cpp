// Fill out your copyright notice in the Description page of Project Settings.


#include "XYFTPPShootGame/Characters/EnemyCharacter.h"

#include "SGPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void AEnemyCharacter::Dead()
{
	if(isCharacterDie)
	{
		return;
	}
	Super::Dead();
	// ASGPlayerCharacter* PlayerCharacter = Cast<ASGPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	// PlayerCharacter->AddKillNum();
	OnEnemyDead.Broadcast();
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	BeAttacked();
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
