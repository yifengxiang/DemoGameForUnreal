// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SGPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitDelegate, FVector, HitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorDie, APlayerController*, PlayerController);

/**
 * 
 */
class UUserWidget;
UCLASS()
class XYFTPPSHOOTGAME_API ASGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//ASGPlayerController();
	
protected:
	virtual void BeginPlay() override;

public:
	//virtual void Tick(float DeltaSeconds) override;

	/** 属性复制 */
	virtual  void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual  void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category = "BulletHit")
	FHitDelegate OnHit;

	UPROPERTY(BlueprintAssignable, Category = "BulletHit")
	FReloadDelegate OnReload;

	UPROPERTY(BlueprintAssignable, Category = "ActorDie")
	FOnActorDie OnActorDie;

	UFUNCTION(BlueprintImplementableEvent)
	void AfterActorDie();

	UFUNCTION(NetMulticast, Reliable)
	void MutiEndGame(bool isWin);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kill", Replicated)
	int KillNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kill", Replicated)
	int DeadNum = 0;

	UFUNCTION(BlueprintCallable)
	int GetKillNum();

	UFUNCTION(BlueprintCallable)
	int GetDeadNum();

	UFUNCTION(BlueprintCallable)
	void AddKillNum();

	UFUNCTION(BlueprintCallable)
	void AddDeadNum();

	UFUNCTION(Server, Reliable)
	void CallSeverEndGame(bool isWin);

	UFUNCTION(BlueprintImplementableEvent)
	void EndGame(bool isWin);

private:

	
	
};
