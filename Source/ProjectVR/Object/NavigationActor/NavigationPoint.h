// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "NavigationPoint.generated.h"

DECLARE_DELEGATE(FNavEvent);

/**
 * 
 */

UCLASS()
class PROJECTVR_API ANavigationPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
		ANavigationPoint();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Collision;
	
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);		// 오버랩이벤트로 데미지를 전달하기위한 함수

	FNavEvent NaviEvent;
};
