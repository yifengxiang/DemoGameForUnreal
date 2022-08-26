// Fill out your copyright notice in the Description page of Project Settings.


#include "XYFTPPShootGame//Weapons/SGThrowBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/LineBatchComponent.h"
#include "XYFTPPShootGame/Characters/CharacterBase.h"
#include "XYFTPPShootGame/Characters/SGPlayerCharacter.h"
#include "Components/LineBatchComponent.h"
#include "XYFTPPShootGame/Controllers/SGPlayerController.h"

// Sets default values
ASGThrowBase::ASGThrowBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ThrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ThrowMesh"));
	ThrowMesh->SetupAttachment(Root);

	BoomParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("BoomParticle"));
}

// Called when the game starts or when spawned
void ASGThrowBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASGThrowBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FDamageEvent ASGThrowBase::GetDamageEvent()
{
	return MyDamageEvent;
}

void ASGThrowBase::CauseDamage(UPrimitiveComponent* HitComponent, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	SeverCauseDamage(HitComponent, OtherActor, NormalImpulse, Hit);
}

void ASGThrowBase::SeverCauseDamage(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                                   FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetOwner()->GetInstigatorController(), this, DamageType);
}

