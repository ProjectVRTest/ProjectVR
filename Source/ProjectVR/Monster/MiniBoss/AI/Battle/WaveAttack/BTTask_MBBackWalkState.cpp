// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBBackWalkState.h"
#include "Headers/MiniBossAIHeader.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

void UBTTask_MBBackWalkState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
	
	InVisibleValue = 0;
}

EBTNodeResult::Type UBTTask_MBBackWalkState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{	
	AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());
	ExitFlag = false;
	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MiniBoss->InVisibleStartEffect, MiniBoss->GetActorLocation());
			GetWorld()->GetTimerManager().SetTimer(ToForceStateTimer, this, &UBTTask_MBBackWalkState::ToForceState, 0.8f, false);
			GetWorld()->GetTimerManager().SetTimer(InVisibleTimer, this, &UBTTask_MBBackWalkState::InVisible, 0.02f, true);
		}
	}	
	return EBTNodeResult::InProgress;
}

void UBTTask_MBBackWalkState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (ExitFlag)
	{		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_MBBackWalkState::ToForceState()
{
	if (MiniBoss)
	{
		MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::WavebackWalkForce;
		ExitFlag = true;
	}
}

void UBTTask_MBBackWalkState::InVisible()
{
	if (MiniBoss)
	{
		if (InVisibleValue > 1.0f)
		{
			InVisibleValue = 0;
			GetWorld()->GetTimerManager().ClearTimer(InVisibleTimer);
			GetWorld()->GetTimerManager().ClearTimer(ToForceStateTimer);			
			MiniBoss->GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Amount"), InVisibleValue);
		}
		else
		{
			InVisibleValue += 0.01;
			MiniBoss->GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Amount"), InVisibleValue);
		}		
	}
}