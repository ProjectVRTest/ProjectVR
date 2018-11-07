// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NMWeaponQuiver.generated.h"

UCLASS()
class PROJECTVR_API ANMWeaponQuiver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANMWeaponQuiver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* QuiverMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* NMArrowMeshOne;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* NMArrowMeshTwo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* NMArrowMeshThree;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* NMArrowMeshFour;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* NMArrowMeshFive;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMesh* NMArrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int ArrowCount;
	UPROPERTY()
		TArray<class UStaticMesh*> NMArrows;
	UFUNCTION()
		void ArrowPop();
	UFUNCTION()
		void ArrowPush(class UStaticMesh* Arrow);
};
