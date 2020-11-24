// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSPrototypeGameModeBase.generated.h"




UCLASS()
class TPSPROTOTYPE_API ATPSPrototypeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ATPSPrototypeGameModeBase();

	virtual void BeginPlay() override;


	enum EHUDState : uint8
	{
		HS_Ingame,
		HS_Inventory
	};
	



	void ApplyHUDChanges();

	uint8 GetHUDState();

	UFUNCTION(BlueprintCallable, Category = "HUD Functions")
		void ChangeHUDState(uint8 NewState);

	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents);

protected:

	uint8 HUDState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidget", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> InventoryHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidget", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> IngameHUDClass;

	UPROPERTY()
		class UUserWidget* CurrentWidget;
};
