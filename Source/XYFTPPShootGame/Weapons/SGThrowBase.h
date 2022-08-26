// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGThrowBase.generated.h"

UCLASS()
class XYFTPPSHOOTGAME_API ASGThrowBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGThrowBase();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FDamageEvent GetDamageEvent();

	UFUNCTION()
	void SeverCauseDamage(UPrimitiveComponent* HitComponent, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	void CauseDamage(UPrimitiveComponent* HitComponent, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DamageComponent")
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DamageComponent")
	TSubclassOf<UDamageType> DamageType;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RootComponent", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaticMeshComponent", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ThrowMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitComponent", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BoomParticle;

	FDamageEvent MyDamageEvent;
};
