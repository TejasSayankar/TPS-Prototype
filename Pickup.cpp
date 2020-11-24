// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

APickup::APickup() 
{
	ItemName = FString("Enter Item Name: ");
	InteractableHelpText = FString("Press E to pick up the item.");
	Weight = 0;
	PickupType = Pickup_Item;
}

void APickup::Interact_Implementation()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	if (Player->AddItemToInventory(this))
	{
		OnPickedUp();
	}
	GLog->Log("Pickup Class");
}

void APickup::OnPickedUp()
{
	
	GLog->Log("On Picked up");
	Destroy();
}

void APickup::Use_Implementation()
{
	GLog->Log("On Used");
}

uint8 APickup::GetPickupType()
{
	return PickupType;
}

