// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterWeapon.h"


// Sets default values
AMonsterWeapon::AMonsterWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMonsterWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AMonsterWeapon::GetDamage()
{
	return Damage;
}

void AMonsterWeapon::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}

