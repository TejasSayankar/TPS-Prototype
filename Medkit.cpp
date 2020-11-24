  // Fill out your copyright notice in the Description page of Project Settings.


#include "Medkit.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComp.h"

AMedkit::AMedkit()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
	
	HealingAmount = 0.f;
}

void AMedkit::Use_Implementation()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->HealthComponent->Heal(HealingAmount);
	/*if (Health < 100)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleHeal, this, &APlayerCharacter::Heal, 5.f, false);
	}*/
	OnPickedUp();
}

//void AMedkit::Heal()
//{
//	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
//	if (Player->Health + HealingAmount >= 100)
//	{
//		Player->Health = 100.f;
//	}
//	else
//	{
//		Player->Health += HealingAmount;
//	}
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Health: %f"), Player->Health));
//}




