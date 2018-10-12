// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NavigationActor.generated.h"

UCLASS()
class PROJECTVR_API ANavigationActor : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavigationActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scene")
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
		class UParticleSystemComponent* Navigate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
		class UParticleSystem* StartNavigate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
		class UParticleSystem* EndNavigate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class ANavigationAIController* AI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		TArray<class ANavigationPoint*> Tagets;

	class ANavigationPoint* Taget;

	UFUNCTION()
		void NavigationEvent();

	int CurrentPoint;

	int TargetPoint;

	class ANavigationPoint* Register;
};

