// Fill out your copyright notice in the Description page of Project Settings.


#include "XYFTPPShootGame//Weapons/SGGunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/LineBatchComponent.h"
#include "XYFTPPShootGame/Characters/CharacterBase.h"
#include "XYFTPPShootGame/Characters/SGPlayerCharacter.h"
#include "Components/LineBatchComponent.h"
#include "XYFTPPShootGame/Controllers/SGPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values
ASGGunBase::ASGGunBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(Root);

	FirePosition = CreateDefaultSubobject<USceneComponent>(TEXT("FirePosition"));
	FirePosition->SetupAttachment(GunMesh);

	FireParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("FireParticle"));

	HitParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("HitParticle"));

}

// Called when the game starts or when spawned
void ASGGunBase::BeginPlay()
{
	Super::BeginPlay();

	MyDamageEvent.DamageTypeClass = DamageType;
	
}

void ASGGunBase::OnRep_CurrentMagBullet()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString("OnRep_CurrentMagBullet"));
}

void ASGGunBase::OnRep_BackuptMagBullet()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString("OnRep_BackuptMagBullet"));
}

// Called every frame
void ASGGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASGGunBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASGGunBase, CurrentMagBullet);
	DOREPLIFETIME(ASGGunBase, BackuptMagBullet);
	DOREPLIFETIME(ASGGunBase, FireParticle);
	DOREPLIFETIME(ASGGunBase, HitParticle);
}

void ASGGunBase::SeverFire_Implementation()
{
	PlayFireEffect();
}

void ASGGunBase::PlayFireEffect_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, FireParticle, FirePosition->GetComponentLocation(), UGameplayStatics::GetPlayerController(this, 0)->GetControlRotation(), FVector(0.1, 0.1, 0.1));
}

void ASGGunBase::Fire()
{
	ASGPlayerCharacter* PlayerCharacter = Cast<ASGPlayerCharacter>(GetOwner());
	if(PlayerCharacter)
	{
		if(!PlayerCharacter->isCanFire)
		{
			return;
		}
	}
	if(CurrentMagBullet <= 0)
	{
		if(Cast<ASGPlayerCharacter>(GetOwner()))
		{
			StopFire();
			ASGPlayerController* MyController = Cast<ASGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
			MyController->OnReload.Broadcast();
		}
		return;
	}
	PlayFireSound();
	CurrentMagBullet -= 1;
	SeverFire();
	if(Cast<ASGPlayerCharacter>(GetOwner()))
	{
		Cast<ASGPlayerCharacter>(GetOwner())->AddControllerPitchInput(-0.2);
		float LeftAndRightOffset = FMath::RandRange(-0.1f, 0.1f);
		Cast<ASGPlayerCharacter>(GetOwner())->AddControllerYawInput(LeftAndRightOffset);
	}
	
	//UGameplayStatics::SpawnEmitterAttached(FireParticle, GunMesh, "MuzzleFlashSocket");
	RunRayCast();
}

void ASGGunBase::SeverCauseDamage_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetOwner()->GetInstigatorController(), this, DamageType);
}

void ASGGunBase::MultiPlayFireEffect_Implementation(FVector Location, FRotator Rotation)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, Location, FRotator::ZeroRotator, true);
}

void ASGGunBase::SeverPlayFireEffect_Implementation(FVector Location, FRotator Rotation)
{
	MultiPlayFireEffect(Location, Rotation);
}

void ASGGunBase::ContinueFire()
{
	GetWorld()->GetTimerManager().SetTimer(FireHandle, this, &ASGGunBase::Fire, 1.f / FireRate, true);
}

void ASGGunBase::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(FireHandle);
}

void ASGGunBase::RayTraceResult(FVector StartPos, FVector EndPos)
{
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	FHitResult RayTraceHitResult;
	if (GetWorld()->LineTraceSingleByChannel(RayTraceHitResult, StartPos, EndPos, ECC_GameTraceChannel1, CollisionQueryParams)) {

		//ULineBatchComponent* const LineBatch = GetWorld()->PersistentLineBatcher;
		//if (LineBatch != nullptr) {
		//	LineBatch->DrawLine(StartPos, EndPos, FLinearColor::Red, 10, 0.f, 5.f);
		//}

		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, RayTraceHitResult.Location, FRotator::ZeroRotator, true);
		SeverPlayFireEffect(RayTraceHitResult.Location, FRotator::ZeroRotator);
		ACharacterBase* DamagedActor = Cast<ACharacterBase>(RayTraceHitResult.Actor);
		if (DamagedActor) {
			FVector HitFromDirection = GetOwner()->GetActorLocation() - DamagedActor->GetActorLocation();
			FHitResult HitResult;
			SeverCauseDamage(RayTraceHitResult.GetComponent(), DamagedActor, HitFromDirection, HitResult);
			//DamagedActor->TakeDamage(Damage, MyDamageEvent, GetOwner()->GetInstigatorController(), this);
		}
		if(Cast<ASGPlayerCharacter>(GetOwner()))
		{
			 ASGPlayerController* MyController = Cast<ASGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
			MyController->OnHit.Broadcast(RayTraceHitResult.Location);
		}
		
		
	}
}


void ASGGunBase::SeverRayTraceResult_Implementation(FVector StartPos, FVector EndPos)
{
	RayTraceResult(StartPos, EndPos);
}

void ASGGunBase::RunRayCast()
{
	//FVector StartPos = FirePosition->GetComponentLocation();
	FVector StartPos;
	FRotator Direction;

	//FRotator ViewPointRotator;

	ACharacterBase* MyOwner = Cast<ACharacterBase>(GetOwner());
	if(!IsValid(MyOwner)) return;
	AController* OwnerController = MyOwner->GetController();
	// if(!IsValid(OwnerController))
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString("dont"));
	// 	return;
	// }
		
	
	FVector EndPos(0.f, 0.f, 0.f);
	OwnerController->GetPlayerViewPoint(StartPos, Direction);
	
	//Direction = Direction + FRotator(0, 90, 0);
	FVector temp;
	//MyController->GetPlayerViewPoint(temp, ViewPointRotator);
	EndPos = StartPos + Direction.Vector() * 10000;
	//EndPos.Z += 100.f;
	RayTraceResult(StartPos, EndPos);
	//SeverRayTraceResult(StartPos, EndPos);
}

FDamageEvent ASGGunBase::GetDamageEvent()
{
	return MyDamageEvent;
}

int ASGGunBase::GetCurrentMagBullet()
{
	return CurrentMagBullet;
}

void ASGGunBase::SetCurrentMagBullet(int BulletNum)
{
	CurrentMagBullet = BulletNum;
}

int ASGGunBase::GetBackuptMagBullet()
{
	return BackuptMagBullet;
}

void ASGGunBase::SetBackuptMagBullet(int BulletNum)
{
	BackuptMagBullet = BulletNum;
}

int ASGGunBase::GetTotalMagBullet()
{
	return TotalMagBullte;
}

