// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Styling/SlateBrush.h"
#include "SGGunBase.generated.h"



UCLASS()
class XYFTPPSHOOTGAME_API ASGGunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGGunBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_CurrentMagBullet();

	UFUNCTION()
	void OnRep_BackuptMagBullet();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** 属性复制 */
	virtual  void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void SeverFire();

	UFUNCTION(Server, Reliable)
	void SeverPlayFireEffect(FVector Location, FRotator Rotation);
	
	UFUNCTION(Server, Reliable)
	void SeverCauseDamage(UPrimitiveComponent* HitComponent, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MultiPlayFireEffect(FVector Location, FRotator Rotation);
	
	UFUNCTION(NetMulticast, Reliable)
	void PlayFireEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFireSound();

	void ContinueFire();

	void StopFire();

	UFUNCTION()
	void RayTraceResult(FVector StartPos, FVector EndPos);

	UFUNCTION(Server, Reliable)
	void SeverRayTraceResult(FVector StartPos, FVector EndPos);

	void RunRayCast();

	UFUNCTION(BlueprintCallable)
	FDamageEvent GetDamageEvent();
	
	//弹夹满容量时的子弹数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet")
	int TotalMagBullte;

	//当前弹夹中的子弹数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet", ReplicatedUsing=OnRep_CurrentMagBullet)
	int CurrentMagBullet;

	//后备弹夹中的子弹数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bullet", ReplicatedUsing=OnRep_BackuptMagBullet)
	int BackuptMagBullet;

	UFUNCTION(BlueprintCallable)
	int GetCurrentMagBullet();

	UFUNCTION(BlueprintCallable)
	void SetCurrentMagBullet(int BulletNum);

	UFUNCTION(BlueprintCallable)
	int GetBackuptMagBullet();

	UFUNCTION(BlueprintCallable)
	void SetBackuptMagBullet(int BulletNum);

	UFUNCTION(BlueprintCallable)
	int GetTotalMagBullet();

	FTimerHandle FireHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GunIndex")
	int GunIndex;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RootComponent", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaticMeshComponent", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FireComponent", meta = (AllowPrivateAccess = "true"))
	USceneComponent* FirePosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FireComponent", meta = (AllowPrivateAccess = "true"), Replicated)
	UParticleSystem* FireParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitComponent", meta = (AllowPrivateAccess = "true"), Replicated)
	UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DamageComponent", meta = (AllowPrivateAccess = "true"))
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DamageComponent", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;

	FDamageEvent MyDamageEvent;

	bool isCanFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FireComponent", meta = (AllowPrivateAccess = "true"))
	float FireRate;

};
