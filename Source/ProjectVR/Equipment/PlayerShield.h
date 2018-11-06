// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerShield.generated.h"

UCLASS()
class PROJECTVR_API APlayerShield : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* ShieldMesh;			// 메쉬에 방패를 붙일 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class UBoxComponent* ShieldCollision;			// 방패 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* StateBarScene;		// 캐릭터 상태 바
	UPROPERTY(VisibleAnywhere, Category = "StateBar")
		class AHPStaminaBar_2* StateBar;		// 캐릭터 상태 바
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* ParryingEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* ShieldBlockEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
		class AMotionControllerCharacter* ShieldOwner;		// 오너
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShieldVelocity")
		FVector ShieldCurrentPosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShieldVelocity")
		FVector ShieldMoveDelta;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShieldVelocity")
		FVector ShieldPreviousPosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShieldVelocity")
		FVector ShieldMoveVelocity;
	UPROPERTY()
		FTimerHandle GlobalTimeTimer; //패링성공시 글로벌 타임을 조절할때 필요한 타이머 변수
	UPROPERTY()
		bool IsActivation; //방패가 활성화 중인지 아닌지 확인해줄 변수
	UPROPERTY()
		bool IsMiniBossWeaponOverlap; //중간보스 무기가 현재 방패와 겹쳐진 상태인지 확인해줄 변수
	UPROPERTY()
		bool IsBossWeaponOverlap; //보스 무기가 현재 방패와 겹쳐진 상태인지 확인해줄 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Materials")
		class UMaterialInterface* ShieldVisibleMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Materials")
		class UMaterialInterface* ShieldInVisibleMaterial;


	UFUNCTION()
		void ConvertOfOpacity(float opacity); //방패를 활성,비활성 시킬때 투명도를 정해줄 함수
	UFUNCTION()
		void OnShieldOverlapStart(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void OnShieldOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void RumbleLeftController(float Intensity);
	UFUNCTION()
		void GlobalTimeInit();
	UFUNCTION()
		void OrbWaveCrash(class AActor* Orb);
};
