// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossInVisibleState.h"
#include "Headers/BossAIHeader.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyTargetPoint.h"

void UBTTask_BossInVisibleState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BossInVisibleState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ExitFlag = false;

	AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		Boss = Cast<ABoss>(AI->GetPawn());

		if (Boss)
		{
			AMotionControllerCharacter* MyCharacer = Cast<AMotionControllerCharacter>(Boss->Target);

			Boss->GetMesh()->SetMaterial(0, Boss->OpacityMaterials);
			Boss->GetMesh()->SetMaterial(1, Boss->OpacityMaterials);
			Boss->GetMesh()->SetCollisionProfileName("NoCollision");
			GetWorld()->GetTimerManager().SetTimer(InVisibleTimer, this, &UBTTask_BossInVisibleState::InVisible, 1.0f, false);
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_BossInVisibleState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (ExitFlag)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}	
}

void UBTTask_BossInVisibleState::InVisible()
{
	if (Boss)
	{
		AMotionControllerCharacter* MyCharacer = Cast<AMotionControllerCharacter>(Boss->Target);

		if (MyCharacer)
		{
			FVector TelpoLocation = MyCharacer->AttackPoints[0]->GetActorLocation();
			AI->BBComponent->SetValueAsVector("TelepoteLocation", TelpoLocation);
			Boss->CurrentBlinkAttackState = EBossBlinkAttackState::Visible;
			ExitFlag = true;
		}
	}
}
