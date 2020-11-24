// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROTOTYPE_API APickup : public AInteractable
{
	GENERATED_BODY()

public:

	APickup();

	enum EPickupType
	{
		Pickup_None,
		Pickup_Weapon,
		Pickup_Item
	};


	virtual void Interact_Implementation() override;

	void OnPickedUp();

	UFUNCTION(BlueprintNativeEvent)
		void Use();

	virtual void Use_Implementation();

	uint8 GetPickupType();

public:

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
		uint8 PickupType;

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
		FString ItemName;

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
		int32 Weight;

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
		UTexture2D* PickupThumbnail;



};
