// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Shooter.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROTOTYPE_API AShooter : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:

	//class AEnemyAIController* EnemyController;
	class APlayerCharacter* Player;

	class AWeapon* Weapon;

	UPROPERTY(EditAnywhere, Category = "Weapon")
		TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = "Animation")
		class UAnimMontage* FireAnimation;

	bool CanFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Blueprintable, Category = "Weapon")
		bool IsEquiped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Blueprintable, Category = "Weapon")
		float Accuracy;



public:
	AShooter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnPlayerCaught(APawn* Pawn) override;

	void FireShot();

	void SpawnWeapon(TSubclassOf<class AWeapon> WeaponClass1);

	
	virtual void Die() override;

	FTimerHandle TimerHandle_HandleFire;

};
