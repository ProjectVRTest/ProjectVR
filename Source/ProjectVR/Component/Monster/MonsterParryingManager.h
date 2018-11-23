// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterParryingManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTVR_API UMonsterParryingManager : public UActorComponent
{
	GENERATED_BODY()
private:

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	UMonsterParryingManager();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void ParryingPointInit(class AActor* Monster, TArray<FName> &_ParryingPoints);
	UFUNCTION()
		void ParryingPointSet(class ACharacter * Monster, TArray<FName>& _ParryingPoints, float CurrentHP, float MaxHP, int32 &ParryingPointMaxCount);
	UFUNCTION()
		void ParryingPointValueSet(class ACharacter* Monster, TArray<FName>& _ParryingPoints, int32 ParryingPointMaxCount); //HP가 50%이하인경우 지정한 패링포인트 수만큼 랜덤하게 스폰해주는 함수
};
