// Fill out your copyright notice in the Description page of Project Settings.

#include "NavigationActor.h"
#include "Object/NavigationActor/NavigationPoint.h"
#include "Particles/ParticleSystemComponent.h"			// 파티클 시스템
#include "UObject/ConstructorHelpers.h"					// 경로
#include "BehaviorTree/BehaviorTree.h"
#include "Object/NavigationActor/NavigationAIController.h"
#include "Particles/ParticleSystem.h"			// 파티클 시스템
#include "ParticleDefinitions.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
ANavigationActor::ANavigationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(RootComponent);

	Navigate = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Navigate"));

	// 시작과 끝 파티클 
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Navigate(TEXT("ParticleSystem'/Game/Assets/Effect/Navigation/PS_GPP_Firefly.PS_GPP_Firefly'"));
	if (P_Navigate.Succeeded())
	{
		StartNavigate = P_Navigate.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_EndNavigate(TEXT("ParticleSystem'/Game/Assets/Effect/Navigation/PS_GPP_Butterfly.PS_GPP_Butterfly'"));
	if (P_EndNavigate.Succeeded())
	{
		EndNavigate= P_EndNavigate.Object;
	}

	// 파티클 적용
	//Navigate->SetTemplate(StartNavigate);

	Navigate->SetupAttachment(Scene);


	// AI
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>Monster_BehaviorTree(TEXT("BehaviorTree'/Game/Blueprints/Object/Navigation/AI/BT_Navigation.BT_Navigation'"));
	if (Monster_BehaviorTree.Succeeded())
	{
		BehaviorTree = Monster_BehaviorTree.Object;
	}

	AIControllerClass = ANavigationAIController::StaticClass();

	// 현재 타겟과 목표 타겟
	CurrentPoint = -1;
	TargetPoint = -1;

	// 타겟
	Target = NULL;

	// 태그
	Tags.Add(FName("Navigation"));
}

// Called when the game starts or when spawned
void ANavigationActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 파티클 버그?로 SetTemplate은 처음밖에 사용못해서 SpawnEmitter로 붙여줌
	Navigate = UGameplayStatics::SpawnEmitterAttached(StartNavigate, RootComponent, NAME_None, GetActorLocation(), GetActorRotation(),
		EAttachLocation::KeepWorldPosition, false);

	if (Targets.Num() -1>= TargetPoint)
	{
		TargetPoint++;
		Target = Targets[TargetPoint];
		Register = Cast<ANavigationPoint>(Targets[TargetPoint]);
		Register->NaviEvent.BindUObject(this, &ANavigationActor::NavigationEvent);
	}
}

// Called every frame
void ANavigationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AI = Cast<ANavigationAIController>(GetController());

	if (AI)
	{
		AI->BBComponent->SetValueAsInt("CurrentPoint", CurrentPoint);
		AI->BBComponent->SetValueAsInt("TargetPoint", TargetPoint);
		AI->BBComponent->SetValueAsObject("Target", Target);
		AI->BBComponent->SetValueAsBool("bIsSame", CurrentPoint==TargetPoint);
		AI->BBComponent->SetValueAsBool("bIsMax", TargetPoint ==Targets.Num());
	}
}

void ANavigationActor::NavigationEvent()
{
	CurrentPoint++;
	TargetPoint++;

	if (Targets.Num() > TargetPoint)
	{
		if (Targets[TargetPoint])
		{
			Target = Targets[TargetPoint];
			if (Register)
				Register->Collision->bGenerateOverlapEvents = false;
			Register = Cast<ANavigationPoint>(Targets[TargetPoint]);
			Register->NaviEvent.BindUObject(this, &ANavigationActor::NavigationEvent);
		}
	}

	if (CurrentPoint == Targets.Num() - 1)
	{
		Navigate->DeactivateSystem();
		TargetPoint = Targets.Num();
		if (EndNavigate)
			Navigate = UGameplayStatics::SpawnEmitterAttached(EndNavigate, RootComponent,
				NAME_None, GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
		UE_LOG(LogTemp, Log, TEXT("%d  /  %d"), TargetPoint, Targets.Num() );
	}
}



