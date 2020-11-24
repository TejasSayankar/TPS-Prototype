// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPrototype.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "AIPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	PlayerKey = "Target";
	LocationToGoKey = "LocationToGo";

	CurrentPatrolPoint = 0;
	
}

void AEnemyAIController::OnPossess(APawn* PawnComp)
{
	Super::OnPossess(PawnComp);

	AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(PawnComp);

	if (EnemyChar)
	{
		if (EnemyChar->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(EnemyChar->BehaviorTree->BlackboardAsset));
		}

		PatrolPoints = EnemyChar->GetPatrolPoints();
		//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPatrolPoint::StaticClass(), PatrolPoints);

		BehaviorComp->StartTree(*EnemyChar->BehaviorTree);
	}
}

void AEnemyAIController::SetPlayerCaught(APawn* PawnComp)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(PlayerKey, PawnComp);
	}
}
