// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROTOTYPE_API AWeapon : public APickup
{
	GENERATED_BODY()

public:

	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunation")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunation")
		float Range;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunation")
		int MagazineSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunation")
		int CurrentMagazineAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunation")
		int CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunation")
		int MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunation")
		float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammunation")
		float ReloadWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		class UParticleSystem* MuzzleParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		class UParticleSystem* ShellEjectParticles;

	class APlayerCharacter* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
		float TimeBetweenShots;


protected:

	virtual void BeginPlay() override;


public:

	void Interact_Implementation() override;

	void Reload();

	//void OnPickedUp();

	//void DropWeapon();

	void DettachWeapon();
	
};
