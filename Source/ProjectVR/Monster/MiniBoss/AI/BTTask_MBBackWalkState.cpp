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
	IsAfterEffectOn = false;	
	MaxEffectNumber = 10;	
}

EBTNodeResult::Type UBTTask_MBBackWalkState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	SumDelta = 0;
	CurrentEffectNumber = 0;
	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			if (!IsAfterEffectOn)
			{
				IsAfterEffectOn = true;
				MiniBoss->GetMesh()->SetMaterial(0, MiniBoss->OpacityMaterials);
				MiniBoss->Sword->SwordMesh->SetMaterial(0, MiniBoss->OpacityMaterials);

				UParticleSystemComponent* AfterImageStart = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MiniBoss->AfterImageStartEffect, MiniBoss->GetActorLocation());

				FParticleSysParam SysParam;
				SysParam.Actor = MiniBoss;
				SysParam.ParamType = EParticleSysParamType::PSPT_Actor;
				SysParam.Name = FName(TEXT("VertSurfaceActor"));
				SysParam.Scalar = 0;
				SysParam.Scalar_Low = 0;
				SysParam.Vector = FVector::ZeroVector;
				SysParam.Vector_Low = FVector::ZeroVector;

				AfterImageStart->InstanceParameters.Add(SysParam);

				GetWorld()->GetTimerManager().SetTimer(AfterImageStartTimer, this, &UBTTask_MBBackWalkState::AfterImageOn, RenderTime, true);

				AfterImageEnd = UGameplayStatics::SpawnEmitterAttached(MiniBoss->AfterImageEndEffect, MiniBoss->GetCapsuleComponent(), NAME_None);	

				SysParam.Name = FName(TEXT("VertSurfaceActor2"));

				AfterImageEnd->InstanceParameters.Add(SysParam);
				GetWorld()->GetTimerManager().SetTimer(AfterImageEndTimer, this, &UBTTask_MBBackWalkState::AfterImageOff, TotalTime, false);
			}
		}
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_MBBackWalkState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (AI)
	{
		Distance = AI->BBComponent->GetValueAsFloat("Distance");
	}

	SumDelta += DeltaSeconds;
	if (MiniBoss)
	{
		MiniBoss->GetCharacterMovement()->AddImpulse((MiniBoss->GetActorForwardVector()*-150.0f)+MiniBoss->GetActorUpVector(), true);
		//MiniBoss->AddMovementInput(MiniBoss->GetActorForwardVector(), -1.0f);
		if (SumDelta > 2.0f || Distance <150.0f)
		{
			MiniBoss->BackAttack = false;
			AfterImageOff();
			GetWorld()->GetTimerManager().ClearTimer(AfterImageStartTimer);
			GetWorld()->GetTimerManager().ClearTimer(AfterImageEndTimer);
			MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
			
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);			
		}		
	}
}

void UBTTask_MBBackWalkState::AfterImageOn()
{
	if (MiniBoss)
	{
		if (CurrentEffectNumber == MaxEffectNumber || !IsAfterEffectOn)
		{
			GetWorld()->GetTimerManager().ClearTimer(AfterImageStartTimer);
		}
		else
		{
			UParticleSystemComponent* AfterImageStart=UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MiniBoss->AfterImageStartEffect, MiniBoss->GetActorLocation());

			FParticleSysParam SysParam;
			SysParam.Actor = MiniBoss;
			SysParam.ParamType = EParticleSysParamType::PSPT_Actor;
			SysParam.Name = FName(TEXT("VertSurfaceActor"));
			SysParam.Scalar = 0;
			SysParam.Scalar_Low = 0;
			SysParam.Vector = FVector::ZeroVector;
			SysParam.Vector_Low = FVector::ZeroVector;

			AfterImageStart->InstanceParameters.Add(SysParam);

			CurrentEffectNumber++;
		}
	}
}

void UBTTask_MBBackWalkState::AfterImageOff()
{
	if (MiniBoss)
	{
		MiniBoss->GetMesh()->SetMaterial(0, MiniBoss->DefaultMaterials);
		MiniBoss->Sword->SwordMesh->SetMaterial(0, MiniBoss->DefaultMaterials);
		IsAfterEffectOn = false;
		AfterImageEnd->Deactivate();
	}
}
