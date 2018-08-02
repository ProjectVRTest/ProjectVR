// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/Table/ItemDataTable.h"
#include "Potion.generated.h"

DECLARE_DELEGATE(FTokenCompleteSignature);

UCLASS()
class PROJECTVR_API APotion : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FItemDataTable DataTable;

	FTokenCompleteSignature TokenCompleteDelegate;

	//���� �ٸ� ���Ϳ� ������ �ɶ� ȣ�� �Ǵ� �Լ�
	UFUNCTION()
		void OnPotionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};

