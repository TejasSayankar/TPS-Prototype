// Fill out your copyright notice in the Description page of Project Settings.


#include "Shooter.h"
#include "Perception/PawnSensingComponent.h"
#include "PlayerCharacter.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Math/UnrealMathUtility.h"
#include "HealthComp.h"

AShooter::AShooter()
{
	PawnSensingComponent->SetPeripheralVisionAngle(90.f);
	PawnSensingComponent->SightRadius = 2000.f;
	PawnSensingComponent->bSeePawns = true;
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AShooter::OnPlayerCaught);
	CanFire = false;
	Accuracy = 0.5f;
}

void AShooter::BeginPlay()
{
	Super::BeginPlay();
	//EnemyController = Cast<AEnemyAIController>(this->GetController());
	Player = nullptr;
	SpawnWeapon(WeaponClass);
}

void AShooter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if (Player->IsDetected)
	{
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation()));
	}*/
	if (Died && Weapon)
	{
		Weapon->DettachWeapon();
	}
}

void AShooter::OnPlayerCaught(APawn* Pawn)
{
	
	Player = Cast<APlayerCharacter>(Pawn);
	if (Player)
	{
		FRotator EnemyLookAtRotation= UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());
		SetActorRotation(FRotator(0.f,EnemyLookAtRotation.Yaw,0.f));
		Player->IsDetected = true;
		if (Weapon)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_HandleFire, this, &AShooter::FireShot, 0.1f, true);
			if (FMath::FRandRange(0.f, 1.f) <= Accuracy)
			{
				Player->HealthComponent->TakeDamage(Weapon->Damage);
			}
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_HandleFire);
	}
}

void AShooter::FireShot()
{

	if (!CanFire)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_HandleFire);
	}


	if (Weapon->MuzzleParticles != NULL)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->MuzzleParticles, Weapon->GunMesh->GetSocketTransform("MuzzleSocket"));
	}

	if (Weapon->ShellEjectParticles != NULL)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->ShellEjectParticles, Weapon->GunMesh->GetSocketTransform("ShellEjectionSocket"));
	}

	if (Weapon->FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Weapon->FireSound, GetActorLocation());
	}

	if (FireAnimation != NULL)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	CanFire = false;
}

void AShooter::SpawnWeapon(TSubclassOf<class AWeapon> WeaponClass1)
{
	if (WeaponClass1 != NULL)
	{
		FRotator SpawnRotation = GetActorRotation();
		FVector SpawnLocation = GetActorLocation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;
		//SpawnParams.Name = TEXT("Rifle");
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			Weapon = World->SpawnActor<AWeapon>(WeaponClass1, SpawnLocation, SpawnRotation, SpawnParams);
			if (Weapon)
			{
				Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "WeaponSocket");
				Weapon->SetActorEnableCollision(false);
				//Weapon->SphereCollision->DestroyComponent();
			}

			IsEquiped = true;
		}
	}
}

void AShooter::Die()
{
		Destroy();
}
