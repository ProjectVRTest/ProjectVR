// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSword.generated.h"

UCLASS()
class PROJECTVR_API APlayerSword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* SwordMesh;				// �޽��� ���� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCapsuleComponent* SwordCollision;				// �˿� �ݸ����� ���̱� ���� ������Ʈ

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Timer;			// �������� ������ �������� Ÿ�̸�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Damage;		// ������
	
	UFUNCTION()
		void OnSwordOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);		// �������̺�Ʈ�� �������� �����ϱ����� �Լ�
	
	UFUNCTION()
		void ConvertOfOpacity(float opacity);

	UPROPERTY()
		class UMaterialParameterCollectionInstance* CollectionInstance;
};
