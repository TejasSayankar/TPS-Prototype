// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Medkit.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROTOTYPE_API AMedkit : public APickup
{
	GENERATED_BODY()

public:

	AMedkit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing Properties")
		float HealingAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Properties")
		class UStaticMeshComponent* Mesh;

	void Use_Implementation() override;

	FTimerHandle TimerHandle_HandleHeal;

};
