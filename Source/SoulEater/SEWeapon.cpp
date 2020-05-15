// Fill out your copyright notice in the Description page of Project Settings.


#include "SEWeapon.h"

// Sets default values
ASEWeapon::ASEWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	if (GetInstigator()) {
		// CollisionComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	}

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
}

// Called every frame
void ASEWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void ASEWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}
