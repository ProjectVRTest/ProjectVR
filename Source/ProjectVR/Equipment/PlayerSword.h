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
		class UStaticMeshComponent* SwordMesh;				// 메쉬에 검을 붙일 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCapsuleComponent* SwordCollision;				// 검에 콜리전을 붙이기 위한 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Timer;			// 지속적인 공격을 막기위한 타이머
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Damage;		// 데미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
		class AMotionControllerCharacter* SwordOwner;			// 오너 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
		class UParticleSystem* BloodEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
		class UParticleSystem* OrbHitEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UAudioComponent* SwordSoundComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USoundCue* SwrodSound;
	
	// 칼의 운동 값
	FVector SwordCurrentPosistion, SwordMoveDelta, SwordPreviousPosistion, SwordMoveVelocity;
	
	// 공격
	UFUNCTION()
		void OnSwordOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);		// 오버랩이벤트로 데미지를 전달하기위한 함수
	
	UFUNCTION()
		void RumbleRightController(float Intensity);

};
