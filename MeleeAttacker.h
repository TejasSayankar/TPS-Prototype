// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "MeleeAttacker.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROTOTYPE_API AMeleeAttacker : public AEnemyCharacter
{
	GENERATED_BODY()




public:
	AMeleeAttacker();

	virtual void BeginPlay() override;

	virtual void OnPlayerCaught(APawn* Pawn) override;
	
};
