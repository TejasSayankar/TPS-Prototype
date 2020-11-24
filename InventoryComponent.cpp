// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Pickup.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Items.SetNum(10);
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


bool UInventoryComponent::AddItemToInventory(APickup* Item)
{
	if (Item)
	{
		/*AWeapon* WeaponObj = Cast<AWeapon>(Item);
		if (WeaponObj)
		{
			Weapon = WeaponObj;
			return true;
		}*/
		const int32 AvailableSlot = Items.Find(nullptr);

		/*if ((CurrentWeight + Item->Weight) <= MaxWeight)
		{
			CurrentWeight += Item->Weight;
			Items.Add(Item);
			return true;
		}*/
		if (AvailableSlot != INDEX_NONE)
		{
			Items[AvailableSlot] = Item;
			return true;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You cant carry any more items!"));
		return false;
	}
	return false;
}

UTexture2D* UInventoryComponent::GetThumbnailAtInventorySlot(int32 Slot)
{
	if (Items[Slot])
	{
		return Items[Slot]->PickupThumbnail;
	}
	return nullptr;
}

FString UInventoryComponent::GetItemNameAtInventorySlot(int32 Slot)
{
	if (Items[Slot])
	{
		return Items[Slot]->ItemName;
	}
	return FString("None");
}

void UInventoryComponent::UseItemAtInventorySlot(int32 Slot)
{
	if (Items[Slot]) {
		Items[Slot]->Use_Implementation();
		//Items.Remove();
		Items[Slot] = NULL;
	}

}