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

};
