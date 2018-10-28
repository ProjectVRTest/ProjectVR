// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NextTarget.h"

#include "Object/NavigationActor/NavigationActor.h"
#include "Object/NavigationActor/NavigationAIController.h"
#include "Particles/ParticleSystemComponent.h"			// 파티클 시스템
#include "Particles/ParticleSystem.h"			// 파티클 시스템
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_NextTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ANavigationAIController* AI = Cast<ANavigationAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ANavigationActor* Navi = Cast<ANavigationActor>(AI->GetPawn());

		if (Navi)
		{
			if (Navi->CurrentPoint == Navi->Targets.Num() - 1)
			{
				Navi->Navigate->DeactivateSystem();

				if(Navi->EndNavigate)
				Navi->Navigate = UGameplayStatics::SpawnEmitterAttached(Navi->EndNavigate, Navi->Scene,
					NAME_None, Navi->GetActorLocation(), Navi->GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
				UE_LOG(LogTemp, Log, TEXT("Complete"));
			}
			
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
