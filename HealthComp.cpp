// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComp.h"

// Sets default values for this component's properties
UHealthComp::UHealthComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComp::TakeDamage(float HealthAmount)
{
	if (Health - HealthAmount < 0.f)
	{
		Health = 0.f;
	}
	else
	{
		Health -= HealthAmount;
		
	}
}


void UHealthComp::Heal(int HealAmount)
{
	if (Health + HealAmount >= 100.f)
	{
		Health = 100.f;
	}
	else
	{
		Health += HealAmount;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Health: %f"), Player->Health));
}





