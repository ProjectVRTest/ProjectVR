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
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_EndNavigate(TEXT("ParticleSystem'/Game/Assets/Effect/ES_Skill/PS_GPP_SpiritPurple.PS_GPP_SpiritPurple'"));
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
	TargetPoint = 0;

	// 타겟
	Taget = NULL;

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

	if (Tagets.Num() >= TargetPoint + 1)
	{
		Taget = Tagets[TargetPoint];
		Register = Cast<ANavigationPoint>(Tagets[TargetPoint]);
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
		AI->BBComponent->SetValueAsObject("Target", Taget);
		AI->BBComponent->SetValueAsBool("bIsSame", CurrentPoint==TargetPoint);
		AI->BBComponent->SetValueAsBool("bIsMax", TargetPoint ==Tagets.Num());
	}
}

void ANavigationActor::NavigationEvent()
{
	UE_LOG(LogTemp, Log, TEXT("Pizza"));

	
	TargetPoint++;
	if (Tagets.Num() > TargetPoint)
	{
		if (Tagets[TargetPoint])
		{
			Taget = Tagets[TargetPoint];
			UE_LOG(LogTemp, Log, TEXT("Next Target %s"), *Taget->GetName());
			Register = Cast<ANavigationPoint>(Tagets[TargetPoint]);
			Register->NaviEvent.BindUObject(this, &ANavigationActor::NavigationEvent);
		}
	}

	if (CurrentPoint == Tagets.Num() - 1)
	{
		Navigate->DeactivateSystem();
		Navigate = UGameplayStatics::SpawnEmitterAttached(EndNavigate, RootComponent, NAME_None, GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
		UE_LOG(LogTemp, Log, TEXT("%s"), *EndNavigate->GetName());
	}

}



