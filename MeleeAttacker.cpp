// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttacker.h"
#include "Perception/PawnSensingComponent.h"
#include "EnemyAIController.h"


AMeleeAttacker::AMeleeAttacker()
{
	PawnSensingComponent->SetPeripheralVisionAngle(90.f);
}

void AMeleeAttacker::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AMeleeAttacker::OnPlayerCaught);
	}
}

void AMeleeAttacker::OnPlayerCaught(APawn* Pawn)
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());

	if (AIController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You have been caught!"));
		AIController->SetPlayerCaught(Pawn);
	}
}
