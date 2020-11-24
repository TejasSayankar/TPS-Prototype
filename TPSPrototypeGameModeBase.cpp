// Copyright Epic Games, Inc. All Rights Reserved.


#include "TPSPrototypeGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

ATPSPrototypeGameModeBase::ATPSPrototypeGameModeBase()
	: Super()
{
	//HUDClass = AInventoryHUD::StaticClass();

	HUDState = EHUDState::HS_Ingame;
}

void ATPSPrototypeGameModeBase::BeginPlay()
{
	ApplyHUDChanges();
}

void ATPSPrototypeGameModeBase::ApplyHUDChanges()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
	}

	switch (HUDState)
	{
	case EHUDState::HS_Ingame:
		ApplyHUD(IngameHUDClass, false, false);
		break;

	case EHUDState::HS_Inventory:
		ApplyHUD(InventoryHUDClass, true, true);
		break;

	default:
		ApplyHUD(IngameHUDClass, false, false);
		break;
	}
}

uint8 ATPSPrototypeGameModeBase::GetHUDState()
{
	return HUDState;
}

void ATPSPrototypeGameModeBase::ChangeHUDState(uint8 NewState)
{
	HUDState = NewState;
	ApplyHUDChanges();

}

bool ATPSPrototypeGameModeBase::ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	APlayerController* PController = GetWorld()->GetFirstPlayerController();

	if (WidgetToApply)
	{
		PController->bShowMouseCursor = bShowMouseCursor;
		PController->bEnableClickEvents = EnableClickEvents;

		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToApply);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
			return true;
		}
		return false;
	}
	return false;
}
