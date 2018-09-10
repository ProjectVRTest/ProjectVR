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
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
		class USceneComponent* StateBarScene;		// 캐릭터 상태 바
	UPROPERTY(VisibleAnywhere, Category = "StateBar")
		class AHPStaminaBar* StateBar;		// 캐릭터 상태 바
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* ParryingEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
		class AMotionControllerCharacter* ShieldOwner;		// 오너
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Value")
		float ShieldPhysicsVelocityValue;			// 운동값

	class AMiniBoss* MiniBoss;
	bool IsActivation;

	// 투명도 변환
	UFUNCTION()
		void ConvertOfOpacity(float opacity);

	UFUNCTION()
	void OnShieldOverlapStart(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnShieldOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void RumbleLeftController(float Intensity);

};
