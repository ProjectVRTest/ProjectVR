// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NextTarget.h"

#include "Object/NavigationActor/NavigationActor.h"
#include "Object/NavigationActor/NavigationAIController.h"
#include "Particles/ParticleSystemComponent.h"			// ��ƼŬ �ý���
#include "Particles/ParticleSystem.h"			// ��ƼŬ �ý���

EBTNodeResult::Type UBTTask_NextTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ANavigationAIController* AI = Cast<ANavigationAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ANavigationActor* Navi = Cast<ANavigationActor>(AI->GetPawn());

		if (Navi)
		{
			Navi->CurrentPoint++;
			
			if (Navi->CurrentPoint  == Navi->Tagets.Num()-1)
			{
				
				//Navi->Navigate->SetTemplate(Navi->EndNavigate);
				//Navi->Navigate->SetTemplate(Navi->EndNavigate);
				if (Navi->EndNavigate)
				{
					
					//Navi->Navigate->SetTemplate(Navi->EndNavigate);
					UE_LOG(LogTemp, Log, TEXT("%s"), *Navi->EndNavigate->GetName());
				}
			}
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
