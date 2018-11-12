// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBAttackReadyState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	//루트모션을 켜주는 행위는
	//루트본을 원점좌표로 설정해서 컬리전이 루트본의 움직임을 따라가게 해주는 행위로
	//루트모션 키기전,후 애니메이션 루트본 좌표의 차 만큼 컬리전을 움직여준다.

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			float Distance = AI->BBComponent->GetValueAsFloat("Distance");

			if (Distance < 300.0f)
			{
				float WallDistance = WallCheckDistance();

				int RandomPattern;

				if (WallDistance < 1200.f)
				{
					RandomPattern = FMath::RandRange(1, 11); //1~11
				}
				else
				{
					RandomPattern = FMath::RandRange(1, 15); //1~15
				}			

				if (RandomPattern > 0 && RandomPattern < 8)
				{
					MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::Idle;
					MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::Idle;
					MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::ShortAttackReady;
					MiniBoss->CurrentAttackState = EMiniBossAttackState::ShortAttack;
				}
				else if (RandomPattern > 7 && RandomPattern < 12)
				{
					AI->BBComponent->SetValueAsFloat("AttackAnimationWaitTime", 3.0f);
					MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
					MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::Idle;
					MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::ComboAttackStart;
					MiniBoss->CurrentAttackState = EMiniBossAttackState::ComboAttack;
				}
				else if (RandomPattern > 11)
				{
					MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::Idle;
					MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
					MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::TwoHandWidthReady;
					MiniBoss->CurrentAttackState = EMiniBossAttackState::WaveAttack;
				}
				MiniBoss->CurrentAnimState = EMiniBossAnimState::Attack;
			}
			else
			{
				MiniBoss->CurrentState = EMiniBossState::Chase;
				MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
			}
		}
	}
	return EBTNodeResult::Succeeded;
}

float UBTTask_MBAttackReadyState::WallCheckDistance()
{
	float WallDistance = 0;

	if (MiniBoss)
	{
		FVector TraceEnd = MiniBoss->GetActorLocation() + (MiniBoss->GetActorForwardVector()*-10000.0f);
		TArray<TEnumAsByte<EObjectTypeQuery>>ObjectTypes;

		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

		TArray<AActor*>IgonreActors;
		IgonreActors.Add(MiniBoss);
		IgonreActors.Add(MiniBoss->Sword);

		FHitResult HitResult;

		bool CanSpawn = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			MiniBoss->GetActorLocation() + FVector(0, 0, 80.0f),
			TraceEnd,
			ObjectTypes,
			true,
			IgonreActors,
			EDrawDebugTrace::None,
			HitResult,
			true
		);

		if (CanSpawn)
		{
			WallDistance = FVector::Distance(MiniBoss->GetActorLocation(), HitResult.Location);
			GLog->Log(FString::Printf(TEXT("WallDistance : %f "), WallDistance));
			GLog->Log(HitResult.Actor->GetName());
		}
		return WallDistance;
	}
	else
	{
		return WallDistance;
	}
}
