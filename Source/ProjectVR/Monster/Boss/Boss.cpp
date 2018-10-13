// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss.h"
#include "BossAIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimBlueprint.h"

// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ABossAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>Boss_SK_Mesh(TEXT("SkeletalMesh'/Game/Assets/CharacterEquipment/Monster/Boss/Mesh/test_mesh.test_mesh'"));

	if (Boss_SK_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(Boss_SK_Mesh.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	static ConstructorHelpers::FObjectFinder<UClass>ABP_Boss(TEXT("AnimBlueprint'/Game/Blueprints/Monster/Boss/Blueprints/ABP_Boss.ABP_Boss_C'"));

	if (ABP_Boss.Succeeded())
	{
		UClass* BossAnimBlueprint = ABP_Boss.Object;

		if (BossAnimBlueprint)
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(BossAnimBlueprint);
		}
	}
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

