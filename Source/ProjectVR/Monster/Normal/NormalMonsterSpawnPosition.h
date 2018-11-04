// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "State/NormalMonsterState/NormalMonsterState.h"
#include "NormalMonsterSpawnPosition.generated.h"

/**
 *
 */
UCLASS()
class PROJECTVR_API ANormalMonsterSpawnPosition : public ATargetPoint
{
	GENERATED_BODY()

public:
	ANormalMonsterSpawnPosition();

	UPROPERTY()
		class ANormalMonsterAIController* NormalMonsterAI;
	UPROPERTY()
		class ANormalMonster* NormalMonster;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monsterkind")
		ENormalMonsterKind NormalMonsterkind;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monsterkind")
		bool NowSpawn;
	UPROPERTY()
		FTimerHandle FindTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UParticleSystem* SpawnEffect;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void FindTarget();
	UFUNCTION()
		void MonsterSpawn();
};
