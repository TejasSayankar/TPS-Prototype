// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "TPSPrototypeGameModeBase.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "InventoryComponent.h"
#include "Interactable.h"
#include "Pickup.h"
#include "Weapon.h"
#include "Medkit.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComp.h"
#include "EnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"




// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(36.f, 92.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom_Standing"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;


	TPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	TPSCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TPSCamera->bUsePawnControlRotation = true;



	GetCharacterMovement()->AirControl = 0.05f;
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	WalkSpeed = 250.f;
	Reloading = false;
	IsEquiped = false;
	Reach = 500.f;

	HealthComponent = CreateDefaultSubobject<UHealthComp>(TEXT("Health"));
	HealthComponent->Health = 100;

	Weapon = nullptr;

	IsDetected = false;

	TargetFOV = 90.f;
	FOV = 90.f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Items.SetNum(10);

	CurrentInteractable = nullptr;
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthComponent->Health <= 0.f && !Died)
	{
		//Die();
		Died = true;
		if (DieAnimation)
		{
			PlayAnimMontage(DieAnimation);
		}
		GetCharacterMovement()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(5.0f);
	}

	FOV = FMath::Lerp(TPSCamera->FieldOfView, TargetFOV, 0.5f);
	TPSCamera->SetFieldOfView(FOV);
	
	GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, WalkSpeed, GetWorld()->GetDeltaSeconds(), 0.5f);

	CheckForInteractable();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jumping);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::UnSprint);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::ToggleCrouch);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::StartFire);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &APlayerCharacter::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::ReloadWithTimer);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::Aiming);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::StopAiming);

	PlayerInputComponent->BindAction("ToggleEquip", IE_Pressed, this, &APlayerCharacter::ToggleEquip);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);

	PlayerInputComponent->BindAction("ToggleInventory", IE_Pressed, this, &APlayerCharacter::ToggleInventory);

	PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &APlayerCharacter::Heal);

}


void APlayerCharacter::MoveForward(float Amount)
{
	if (Controller != NULL && Amount != 0.f)
	{
		FRotator ControlRotation = GetControlRotation();
		FRotator YawRotation(0, ControlRotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);


		AddMovementInput(Direction, Amount);
	}

}

void APlayerCharacter::MoveRight(float Amount)
{
	if (Controller != NULL && Amount != 0.f)
	{
		FRotator ControlRotation = GetControlRotation();
		FRotator YawRotation(0, ControlRotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Amount);
	}

}

void APlayerCharacter::Sprint()
{
	if (!bIsCrouched)
	{
		WalkSpeed = 500.f;
	}

}

void APlayerCharacter::UnSprint()
{
	if (!bIsCrouched)
	{ 
		WalkSpeed = 250.f;
	}
}

void APlayerCharacter::ToggleCrouch()
{
	if (!bIsCrouched)
	{
		Super::Crouch();
	}
	else
	{
		Super::UnCrouch();
	}
}

void APlayerCharacter::Jumping()
{
	if (bIsCrouched)
	{
		Super::UnCrouch();
	}
	else
	{
		Super::Jump();
	}
}

bool APlayerCharacter::AddItemToInventory(APickup* Item)
{
	if (Item)
	{
		const int32 AvailableSlot = Items.Find(nullptr);

		if (AvailableSlot != INDEX_NONE)
		{
			Items[AvailableSlot] = Item;
			return true;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You cant carry any more items!"));
		return false;
	}
	return false;
}

UTexture2D* APlayerCharacter::GetThumbnailAtInventorySlot(int32 Slot)
{
	if (Items[Slot])
	{
		return Items[Slot]->PickupThumbnail;
	}
	return nullptr;
}

UTexture2D* APlayerCharacter::GetWeaponThumbnail()
{
	if (Weapon)
	{
		return Weapon->PickupThumbnail;
	}
	return nullptr;
}

FString APlayerCharacter::GetItemNameAtInventorySlot(int32 Slot)
{
	if (Items[Slot])
	{
		return Items[Slot]->ItemName;
	}
	return FString("None");
}

FString APlayerCharacter::GetCurrentAmmo()
{
	if (Weapon)
	{
		return FString(FString::FromInt(Weapon->CurrentAmmo));
	}
	return FString("00");
}

FString APlayerCharacter::GetHealth()
{
	return FString(FString::FromInt(HealthComponent->Health));
}

FString APlayerCharacter::GutCurrentMagazineAmmo()
{
	if (Weapon)
	{
		return FString(FString::FromInt(Weapon->CurrentMagazineAmmo));
	}
	return FString("00");
}

void APlayerCharacter::UseItemAtInventorySlot(int32 Slot)
{
	if (Items[Slot]) {
		Items[Slot]->Use_Implementation();
		Items[Slot] = NULL;
	}

}

void APlayerCharacter::Die()
{
	if (DieAnimation)
	{
		PlayAnimMontage(DieAnimation);
		APlayerController* PController = Cast<APlayerController>(Controller);
		UKismetSystemLibrary::QuitGame(GetWorld(), PController, EQuitPreference::Quit, true);
		GetWorldTimerManager().SetTimer(TimerHandle_HandleDeath, this, &APlayerCharacter::Quit, 1.0f, true);
	}
}

void APlayerCharacter::Quit()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleDeath);
	APlayerController* PController = Cast<APlayerController>(Controller);
	UKismetSystemLibrary::QuitGame(GetWorld(), PController, EQuitPreference::Quit, true);
	
}


void APlayerCharacter::StartFire()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Starting fire before checking weapon"));
	if (Weapon != nullptr && IsEquiped)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Starting fire after checking weapon"));
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFire, this, &APlayerCharacter::FireShot, Weapon->TimeBetweenShots, true);
	}
}

void APlayerCharacter::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFire);
}

void APlayerCharacter::FireShot()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d %d"), Weapon->CurrentMagazineAmmo, Weapon->CurrentAmmo));
	if (!this->GetCharacterMovement()->IsFalling())
	{
		if (Weapon->CurrentMagazineAmmo > 0 && !Reloading)
		{
			FHitResult HitResult;
			FVector StartTrace = TPSCamera->GetComponentLocation();
			FVector EndTrace = ((TPSCamera->GetForwardVector()) * Weapon->Range) + StartTrace;
			FCollisionQueryParams TraceParams = FCollisionQueryParams(SCENE_QUERY_STAT(WeaponTrace), false, this);
			if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Pawn, TraceParams))
			{
				if (Weapon->ImpactParticles != NULL)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Weapon->ImpactParticles, FTransform(HitResult.ImpactNormal.Rotation(), HitResult.ImpactPoint));
				}
				AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(HitResult.Actor);
				if (Enemy)
				{
					Enemy->HealthComponent->TakeDamage(Weapon->Damage);
					GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::Printf(TEXT("Enemy Health: %f"), Enemy->HealthComponent->Health));
				}
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

			Weapon->CurrentMagazineAmmo--;
		}
		else
		{
			if (Weapon->CurrentAmmo > 0 && !Reloading)
			{

				ReloadWithTimer();

			}
		}
	}

}

void APlayerCharacter::Reload()
{

	Weapon->Reload();
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleReload);
	Reloading = false;
}

void APlayerCharacter::ReloadWithTimer()
{

	if (Weapon && IsEquiped)
	{
		if (Weapon->CurrentAmmo > 0 && !Reloading)
		{
			Reloading = true;
			if (ReloadAnimation)
			{
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if (AnimInstance)
				{
					AnimInstance->Montage_Play(ReloadAnimation, 1.0f);
				}
			}
			GetWorldTimerManager().SetTimer(TimerHandle_HandleReload, this, &APlayerCharacter::Reload, Weapon->ReloadWaitTime, false);
		}
	}
}

void APlayerCharacter::Aiming()
{
	TargetFOV = 45.f;
	
}

void APlayerCharacter::StopAiming()
{
	TargetFOV = 90.f;
	
	
}

void APlayerCharacter::ToggleEquip()
{
	if (Weapon)
	{
		if (IsEquiped)
		{
			IsEquiped = false;
			if (UnEquipAnimation)
			{
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if (AnimInstance)
				{
					AnimInstance->Montage_Play(UnEquipAnimation, 1.0f);
				}
			}
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d %d"), Weapon->CurrentMagazineAmmo, Weapon->CurrentAmmo));
			IsEquiped = true;
			if (EquipAnimation)
			{
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if (AnimInstance)
				{
					AnimInstance->Montage_Play(EquipAnimation, 1.0f);
				}
			}
		}
	}


}

void APlayerCharacter::Equip()
{
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "WeaponSocket");
}

void APlayerCharacter::UnEquip()
{
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "RifleSocket");
}

void APlayerCharacter::ToggleInventory()
{
	ATPSPrototypeGameModeBase* GameMode = Cast<ATPSPrototypeGameModeBase>(GetWorld()->GetAuthGameMode());

	if (GameMode->GetHUDState() == GameMode->HS_Ingame)
	{
		GameMode->ChangeHUDState(GameMode->HS_Inventory);
	}
	else
	{
		GameMode->ChangeHUDState(GameMode->HS_Ingame);
	}
}

void APlayerCharacter::Interact()
{
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact_Implementation();
	}
}

void APlayerCharacter::CheckForInteractable()
{

	FHitResult HitResult;

	FVector StartTrace = TPSCamera->GetComponentLocation();
	FVector EndTrace = (TPSCamera->GetForwardVector() * Reach) + StartTrace;

	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic, CQP);

	AInteractable* InteractableObj = Cast<AInteractable>(HitResult.GetActor());

	if (!InteractableObj)
	{
		HelpText = FString("");
		CurrentInteractable = nullptr;
	}
	else 
	{
		CurrentInteractable = InteractableObj;
		HelpText = CurrentInteractable->InteractableHelpText;
	}
}

void APlayerCharacter::AttachWeapon(AWeapon* WeaponObject)
{
	if (Weapon) 
	{
		Weapon->DettachWeapon();
		Weapon = nullptr;
	}
	Weapon = WeaponObject;
	if (!IsEquiped)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "RifleSocket");
	}
	else
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), "WeaponSocket");
	}
	//Weapon->BoxCollider->Deactivate();
	Weapon->SetActorEnableCollision(false);
	//Weapon->GunMesh->SetEnableGravity(false);
	Weapon->GunMesh->SetSimulatePhysics(false);
	Weapon->GunMesh->SetEnableGravity(true);
}


void APlayerCharacter::Heal()
{
	for (int i = 0; i < 10; i++)
	{
		AMedkit* Medkit = Cast<AMedkit>(Items[i]);
		if (Medkit)
		{
			Medkit->Use_Implementation();
			//GetWorldTimerManager().ClearTimer(TimerHandle_HandleHeal);
			return;
		}
	}
	
}

void APlayerCharacter::HealWithTimer()
{
	if (HealthComponent->Health < 100.f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleHeal, this, &APlayerCharacter::Heal, 5.f, false);
	}
}