// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ASGGunBase;
class ASGThrowBase;
UCLASS()
class XYFTPPSHOOTGAME_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** 属性复制 */
	virtual  void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealthPercent();

	/** 当前生命值的存值函数。将此值的范围限定在0到MaxHealth之间，并调用OnHealthUpdate。仅在服务器上调用。*/
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCurrentHealth(float healthValue);

	UFUNCTION(Server, Reliable)
	void SeverSetCurrentHealth(float healthValue);
	
	virtual void Dead();

	void MoveForward(float Value);

	void LookUp(float Value);

	void Turn(float Value);

	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
	void StartJump();

	void StopJump();

	void SilenceWalkStart();

	void SilenceWalkEnd();

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	void LeftEvent();

	UFUNCTION(BlueprintCallable)
	void LeftEventStop();

	UFUNCTION(BlueprintImplementableEvent)
	void Throw();
	
	UFUNCTION(BlueprintCallable)
	void SetIsCanFire(bool CanFire);

	UFUNCTION(BlueprintCallable)
	bool IsDead();
	

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	ASGGunBase* GetPlayerGun();

	
	UFUNCTION(BlueprintCallable)
	void SetPlayerGun(ASGGunBase* OtherGun);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SeverSetPlayerGun(ASGGunBase* OtherGun);

	UFUNCTION(BlueprintCallable)
	ASGGunBase* GetBackGun();

	
	UFUNCTION(BlueprintCallable)
	void SetBackGun(ASGGunBase* OtherGun);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SeverSetBackGun(ASGGunBase* OtherGun);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetIsHoldGun(bool isHoldGun);

	UFUNCTION(Server, Reliable)
	void SeverSpawnGun();

	UFUNCTION(NetMulticast, Reliable)
	void SpawnGunMulti();

	UFUNCTION(Server, Reliable)
	void SeverBackSpawnGun();

	UFUNCTION(NetMulticast, Reliable)
	void BackSpawnGunMulti();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponComponent", meta = (AllowPrivateAccess = "true"), Replicated)
	ASGGunBase* PlayerGun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponComponent", meta = (AllowPrivateAccess = "true"), Replicated)
	ASGGunBase* BackGun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponComponent", meta = (AllowPrivateAccess = "true"), Replicated)
	ASGGunBase* PlayerGun1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponComponent", meta = (AllowPrivateAccess = "true"), Replicated)
	ASGGunBase* PlayerGun2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponComponent", meta = (AllowPrivateAccess = "true"))
	ASGThrowBase* PlayerGrenade;

	UPROPERTY(Replicated)
	bool isCanFire = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponComponent", meta = (AllowPrivateAccess = "true"), ReplicatedUsing=OnRep_IsHoldGun)
	bool IsHoldGun = true;

	UPROPERTY(ReplicatedUsing=OnRep_isCharacterDie)
	bool isCharacterDie = false;

	FTimerHandle DieTimerHandle;

	UFUNCTION()
	void OnRep_isCharacterDie();
	

	UFUNCTION(BlueprintImplementableEvent)
	void DetroyCharacter();

protected:
	/** RepNotify，用于同步对当前生命值所做的更改。*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION()
	void OnRep_IsHoldGun();
	
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
	float Health;

	/** 响应要更新的生命值。修改后，立即在服务器上调用，并在客户端上调用以响应RepNotify*/
	void OnHealthUpdate();


private:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponComponent", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASGGunBase> PlayerGunClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponComponent", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASGGunBase> BackGunClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponComponent", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASGGunBase> PlayerGunClass1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponComponent", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASGGunBase> PlayerGunClass2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponComponent", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASGThrowBase> PlayerThrowClass;
	
	
	float TurnRate = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealthComponent", meta = (AllowPrivateAccess = "true"))
	float DefaultHealth;
	

};
