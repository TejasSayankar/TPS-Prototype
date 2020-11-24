// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class TPSPROTOTYPE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:	
	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TArray<AActor*> PatrolPoints;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UPawnSensingComponent* PawnSensingComponent;

	FORCEINLINE TArray<AActor*> GetPatrolPoints() const { return PatrolPoints; }

	class UHealthComp* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
		class UAnimMontage* DieAnimation;

	UPROPERTY(BlueprintReadOnly, Category = "Properties")
		bool Died;

	UFUNCTION()
		virtual void Die();

private:
	UFUNCTION()
		virtual void OnPlayerCaught(APawn* Pawn);

	
};