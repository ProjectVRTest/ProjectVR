// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniBossWeapon.generated.h"

UCLASS()
class PROJECTVR_API AMiniBossWeapon : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
		float Damage; //중간보스 무기 데미지

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AMiniBossWeapon();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* SwordMesh; //무기 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsWeaponAttack; //현재 무기가 공격가능한 상태인지 알려주는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsParryingAttack; //현재 무기가 패링이 가능한 상태인지 알려주는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UMaterialInterface* DefaultMaterials;	//무기의 초기 머터리얼
	UFUNCTION()
		float GetDamage(); //중간보스 무기의 현재 데미지를 가지고온다.
	UFUNCTION()
		void SetDamage(float NewDamge); //중간보스 무기의 현재 데미지를 새로 셋팅한다.
	UFUNCTION() //중간보스 무기 메쉬가 오버랩이 시작될때 호출되는 함수
		void WeaponBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
