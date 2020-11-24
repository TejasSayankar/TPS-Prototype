// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROTOTYPE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()



public:

	class UBehaviorTreeComponent* BehaviorComp;

	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = AI)
		FName LocationToGoKey;

	UPROPERTY(EditDefaultsOnly, Category = AI)
		FName PlayerKey;

	TArray<AActor*> PatrolPoints;

	int32 CurrentPatrolPoint = 0;
	
public:

	AEnemyAIController();

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	FORCEINLINE TArray<AActor*> GetPatrolPoints() const { return PatrolPoints; }

	virtual void OnPossess(APawn* Pawn) override;

	void SetPlayerCaught(APawn* Pawn);
};
