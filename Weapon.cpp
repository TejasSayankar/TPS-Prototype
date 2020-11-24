// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	/*BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(RootComponent);*/

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(RootComponent);
	/*GunMesh->SetSimulatePhysics(true);
	GunMesh->SetEnableGravity(true);*/
	GunMesh->SetMobility(EComponentMobility::Movable);
	
}

void AWeapon::BeginPlay()
{
	TimeBetweenShots = float(1 / FireRate);
}

void AWeapon::Interact_Implementation()
{
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	Player->AttachWeapon(this);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Interact"));
	GLog->Log("Interact Gun class: Interact() Should be seen");
}

void AWeapon::Reload()
{

	if (CurrentAmmo < MagazineSize - CurrentMagazineAmmo)
	{
		CurrentMagazineAmmo += CurrentAmmo;
		CurrentAmmo = 0;
	}
	else
	{
		CurrentAmmo -= (MagazineSize - CurrentMagazineAmmo);
		CurrentMagazineAmmo += (MagazineSize - CurrentMagazineAmmo);;

	}
}


void AWeapon::DettachWeapon()
{
	
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorEnableCollision(true);
	FVector Location = GetActorLocation();
	Location.Z = 30.f;
	SetActorLocation(Location);
	SetActorRotation(FRotator(0.f, -90.f, 0.f));
	/*BoxCollider->Activate();
	BoxCollider->SetSimulatePhysics(true);
	BoxCollider->SetEnableGravity(true);*/
	//GunMesh->SetupAttachment(BoxCollider);
	//GunMesh->SetSimulatePhysics(true);
	/*GunMesh->SetSimulatePhysics(true);
	GunMesh->SetEnableGravity(true);
	GunMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GunMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);*/
	

}

