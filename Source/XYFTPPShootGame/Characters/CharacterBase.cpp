// Fill out your copyright notice in the Description page of Project Settings.


#include "XYFTPPShootGame//Characters/CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "SGPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "XYFTPPShootGame/Weapons/SGGunBase.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "XYFTPPShootGame/Controllers/SGPlayerController.h"
#include "XYFTPPShootGame/GameModes/XYFGameMode.h"
#include "XYFTPPShootGame/Weapons/SGThrowBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	GetCharacterMovement()->MaxWalkSpeed = 5000.f;

	DefaultHealth = 100.f;
	Health = 100.f;
	


	GetCapsuleComponent()->SetCollisionProfileName(FName("PlayerProfile"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	

	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// PlayerGun = CreateDefaultSubobject<ASGGunBase>(TEXT("PlayerGun"));
	//
	// BackGun = CreateDefaultSubobject<ASGGunBase>(TEXT("BackGun"));

	PlayerGunClass = PlayerGunClass1;

	BackGunClass = PlayerGunClass2;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(PlayerGunClass))
	{
		//SeverSpawnGun();
		PlayerGun = GetWorld()->SpawnActor<ASGGunBase>(PlayerGunClass);
		PlayerGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "right_hand_weapon");
		PlayerGun->SetOwner(this);
		PlayerGun1 = PlayerGun;
	}

	if(IsValid(BackGunClass))
	{
		//SeverBackSpawnGun();
		BackGun = GetWorld()->SpawnActor<ASGGunBase>(BackGunClass);
		BackGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "back_weapon");
		BackGun->SetOwner(this);
		PlayerGun2 = BackGun;
	}


	GetMesh()->HideBoneByName("weapon_r", EPhysBodyOp::PBO_None);
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);

	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ACharacterBase::LookUp);

	PlayerInputComponent->BindAxis("Turn", this, &ACharacterBase::Turn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::StartJump);

	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacterBase::StopJump);

	PlayerInputComponent->BindAction("SilenceWalk", IE_Pressed, this, &ACharacterBase::SilenceWalkStart);

	PlayerInputComponent->BindAction("SilenceWalk", IE_Released, this, &ACharacterBase::SilenceWalkEnd);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterBase::LeftEvent);

	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACharacterBase::LeftEventStop);
	

}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//复制当前生命值。
	DOREPLIFETIME(ACharacterBase, Health);
	DOREPLIFETIME(ACharacterBase, IsHoldGun);
	DOREPLIFETIME(ACharacterBase, PlayerGun);
	DOREPLIFETIME(ACharacterBase, BackGun);
	DOREPLIFETIME(ACharacterBase, isCharacterDie);
	DOREPLIFETIME(ACharacterBase, isCanFire);
	DOREPLIFETIME(ACharacterBase, PlayerGun1);
	DOREPLIFETIME(ACharacterBase, PlayerGun2);

}

float ACharacterBase::GetCurrentHealthPercent()
{
	return Health / DefaultHealth;
}

void ACharacterBase::SetCurrentHealth(float healthValue)
{
	// if (GetLocalRole() == ROLE_Authority)
	// {
	// 	Health = FMath::Clamp(healthValue, 0.f, DefaultHealth);
	// 	OnHealthUpdate();
	// }
	Health = FMath::Clamp(healthValue, 0.f, DefaultHealth);
	OnHealthUpdate();
}

void ACharacterBase::SeverSetCurrentHealth_Implementation(float healthValue)
{
	SetCurrentHealth(healthValue);
}

void ACharacterBase::Dead()
{
	if(isCharacterDie) return;
	isCharacterDie = true;
}


void ACharacterBase::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value * GetWorld()->GetDeltaSeconds() * GetCharacterMovement()->MaxWalkSpeed);
}

void ACharacterBase::LookUp(float Value)
{
	AddControllerPitchInput(Value * GetWorld()->GetDeltaSeconds() * TurnRate);
}

void ACharacterBase::Turn(float Value)
{
	AddControllerYawInput(Value * GetWorld()->GetDeltaSeconds() * TurnRate);
}

void ACharacterBase::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value * GetWorld()->GetDeltaSeconds() * GetCharacterMovement()->MaxWalkSpeed);
}

void ACharacterBase::StartJump()
{
	bPressedJump = true;
}

void ACharacterBase::StopJump()
{
	bPressedJump = false;
	StopJumping();
}

void ACharacterBase::SilenceWalkStart()
{
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
}

void ACharacterBase::SilenceWalkEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void ACharacterBase::Fire()
{

	if(!PlayerGun) return;
	if(!isCanFire) return;
	PlayerGun->ContinueFire();
	//DrawDebugCamera(GetWorld(), GetActorLocation(), GetActorRotation(), 90, 2, FColor::Red, true);
}

void ACharacterBase::LeftEvent()
{
	if(IsHoldGun)
	{
		Fire();
	}
	else
	{
		Throw();
	}
}

void ACharacterBase::LeftEventStop()
{
	if(IsHoldGun)
	{
		PlayerGun->StopFire();
	}
}

void ACharacterBase::SetIsCanFire(bool CanFire)
{
	isCanFire = CanFire;
}


bool ACharacterBase::IsDead()
{
	return isCharacterDie;
}



float ACharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	//SeverSetCurrentHealth();
	if(Health <= 0) return Health;
	SetCurrentHealth(Health - Damage);
	if(Health <= 0)
	{
		auto MyController = Cast<ASGPlayerController>(GetController());
		if(MyController)
		{
			MyController->AddDeadNum();
		}
		ASGPlayerController* DamagePlayer = Cast<ASGPlayerController>(EventInstigator);
		if(DamagePlayer)
		{
			DamagePlayer->AddKillNum();
		}
		Dead();
		UnPossessed();
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("DetroyCharacter"));
		GetWorld()->GetTimerManager().SetTimer(DieTimerHandle, TimerDelegate, 4.1f, false);
	}
	return Health;
}

ASGGunBase* ACharacterBase::GetPlayerGun()
{
	return PlayerGun;
}

void ACharacterBase::SetPlayerGun(ASGGunBase* OtherGun)
{
	PlayerGun = OtherGun;
}

void ACharacterBase::SeverSetPlayerGun_Implementation(ASGGunBase* OtherGun)
{
	SetPlayerGun(OtherGun);
}

ASGGunBase* ACharacterBase::GetBackGun()
{
	return BackGun;
}

void ACharacterBase::SeverSetBackGun_Implementation(ASGGunBase* OtherGun)
{
	SetBackGun(OtherGun);
}

void ACharacterBase::BackSpawnGunMulti_Implementation()
{
	BackGun = GetWorld()->SpawnActor<ASGGunBase>(BackGunClass);
	BackGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "back_weapon");
	BackGun->SetOwner(this);
	PlayerGun2 = BackGun;
}

void ACharacterBase::SpawnGunMulti_Implementation()
{
	
	PlayerGun = GetWorld()->SpawnActor<ASGGunBase>(PlayerGunClass);
	PlayerGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "right_hand_weapon");
	PlayerGun->SetOwner(this);
	PlayerGun1 = PlayerGun;
}

void ACharacterBase::SeverBackSpawnGun_Implementation()
{
	BackSpawnGunMulti();

}

void ACharacterBase::SeverSpawnGun_Implementation()
{
	SpawnGunMulti();
}

void ACharacterBase::SetBackGun(ASGGunBase* OtherGun)
{
	BackGun = OtherGun;
}

void ACharacterBase::SetIsHoldGun_Implementation(bool isHoldGun)
{
	IsHoldGun = isHoldGun;
}

void ACharacterBase::OnRep_isCharacterDie()
{
	if(isCharacterDie)
	{

	}
}

void ACharacterBase::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void ACharacterBase::OnRep_IsHoldGun()
{
}

void ACharacterBase::OnHealthUpdate()
{
}

