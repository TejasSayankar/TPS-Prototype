// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TPSPROTOTYPE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	float Reach;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Blueprintable, Category = "Components")
		class UCameraComponent* TPSCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Blueprintable, Category = "Components")
		class USpringArmComponent* CameraBoom;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
		class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
		class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
		class UAnimMontage* EquipAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
		class UAnimMontage* UnEquipAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
		class UAnimMontage* DieAnimation;


	float WalkSpeed;

	bool Reloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool IsEquiped;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool IsDetected;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool IsDead;*/

	bool Picked;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		class UParticleSystem* ImpactParticles;*/

	/*UPROPERTY(EditAnywhere)
		TArray<AItem*> Inventory;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		FString HelpText;

	class AInteractable* CurrentInteractable;

	class AWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Blueprintable, Category = "Inventory")
		TArray<class APickup*> Items;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Blueprintable, Category = "Properties")
	class UHealthComp* HealthComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
		bool Died;

	float FOV;

	float TargetFOV;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void MoveForward(float Amount);

	void MoveRight(float Amount);

	void Sprint();

	void UnSprint();

	void ToggleCrouch();

	void Jumping();

	void StartFire();

	void StopFire();

	void FireShot();

	void Reload();

	void ReloadWithTimer();

	void ToggleInventory();

	void Interact();

	void CheckForInteractable();

	void Aiming();

	void StopAiming();

	void ToggleEquip();

	UFUNCTION(BlueprintCallable)
		void Equip();

	UFUNCTION(BlueprintCallable)
		void UnEquip();

	void Heal();

	void HealWithTimer();

	FTimerHandle TimerHandle_HandleFire;

	FTimerHandle TimerHandle_HandleReload;

	FTimerHandle TimerHandle_HandleHeal;

	FTimerHandle TimerHandle_HandleDeath;

	void AttachWeapon(AWeapon* WeaponObject);

	UFUNCTION(BlueprintPure, Category = "Inventory Function")
		bool AddItemToInventory(class APickup* Item);

	UFUNCTION(BlueprintPure, Category = "Inventory Function")
		UTexture2D* GetThumbnailAtInventorySlot(int32 Slot);

	UFUNCTION(BlueprintPure, Category = "UI Function")
		UTexture2D* GetWeaponThumbnail();

	UFUNCTION(BlueprintPure, Category = "Inventory Function")
		FString GetItemNameAtInventorySlot(int32 Slot);

	UFUNCTION(BlueprintPure, Category = "UI Function")
		FString GetCurrentAmmo();

	UFUNCTION(BlueprintPure, Category = "UI Function")
		FString GetHealth();

	UFUNCTION(BlueprintPure, Category = "UI Function")
		FString GutCurrentMagazineAmmo();

	UFUNCTION(BlueprintCallable, Category = "Inventory Function")
		void UseItemAtInventorySlot(int32 Slot);

	UFUNCTION()
		void Die();

	void Quit();

	UFUNCTION()
	bool GetEquiped() const { return IsEquiped; }

};
