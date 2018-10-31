// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossInVisibleState.h"
#include "Headers/BossAIHeader.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/CameraLocation.h"
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
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Boss->BlinkSmoke, Boss->GetActorLocation());			
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
			int RandomValue;
			FVector TeleportLocation;
			switch (Boss->CurrentBattleState)
			{
			case EBossBattleState::AddAttack:
				TeleportLocation = MyCharacer->CameraLocation->GetActorLocation() + FVector(200.0f, 0, 0);
				break;			
			case EBossBattleState::BattleWatch:
				RandomValue = FMath::RandRange(1, 10);

				//캐릭터의 배틀 상태에 따라서 계산해주는 텔포 위치가 달라지는 것을 구현해야함
				if (RandomValue > 6) //정해진 포인트로 이동
				{
					GLog->Log(FString::Printf(TEXT("정해진 포인트로 이동")));
					TeleportLocation = Boss->TeleportPoints[0]->GetActorLocation();
				}
				else // 캐릭터 앞으로 이동 
				{
					GLog->Log(FString::Printf(TEXT("캐릭터 앞으로 이동")));
					TeleportLocation = MyCharacer->CameraLocation->GetActorLocation() + FVector(200.0f, 0, 0);
				}
				break;
			}			
			SetTeleportLocation(Boss->CurrentBattleState,TeleportLocation);
		}
	}
}

void UBTTask_BossInVisibleState::SetTeleportLocation(EBossBattleState & BattleState,FVector & Location)
{
	AI->BBComponent->SetValueAsVector("TeleportLocation", Location);

	switch (BattleState)
	{
	case EBossBattleState::AddAttack:
		Boss->CurrentBlinkAttackState = EBossBlinkAttackState::Visible;
		break;
	case EBossBattleState::BattleWatch:
		Boss->CurrentBattleWatchState = EBossBattleWatchState::Visible;
		break;
	}	
	ExitFlag = true;
}
