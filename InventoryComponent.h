// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROTOTYPE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	class AWeapon* Weapon;

	int MaxWeight;
	int CurrentWeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Blueprintable, Category = "Inventory")
		TArray<class APickup*> Items;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Blueprintable, Category = "Weapon")
		TSubclassOf<class AWeapon> WeaponClass;*/


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintPure, Category = "Inventory Function")
		bool AddItemToInventory(class APickup* Item); 

	UFUNCTION(BlueprintPure, Category = "Inventory Function")
		UTexture2D* GetThumbnailAtInventorySlot(int32 Slot);

	UFUNCTION(BlueprintPure, Category = "Inventory Function")
		FString GetItemNameAtInventorySlot(int32 Slot);

	UFUNCTION(BlueprintCallable, Category = "Inventory Function")
		void UseItemAtInventorySlot(int32 Slot);

};
