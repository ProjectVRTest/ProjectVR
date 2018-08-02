// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyCharacter/GripEnum.h"
#include "LeftHandMotionController.generated.h"

UCLASS()
class PROJECTVR_API ALeftHandMotionController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeftHandMotionController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�����Ʈ�ѷ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMotionControllerComponent* MotionController;
	//������� �ո�� �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* HandMesh;
	//�������� �ִ� ���͵��� �Ǻ��� ���� �ݸ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* GrabSphere;
	//��ȣ�ۿ뿵���� ǥ������ ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USteamVRChaperoneComponent* SteamVRChaperone;
	//�׸���ư�� �������� �ȴ������� �Ǻ����� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool WantToGrip;
	//�����տ� ���� ���� ���̴��� �Ⱥ��̴��� �Ǵ����� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
		bool VisibleShieldFlag;
	//�����տ� ��ü�� ��Ѵ��� �ȴ�Ѵ��� �Ǵ����� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool HandTouchActorFlag;
	//�տ� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AActor* AttachedActor;
	//���� ���� (����, ��� �ִ� ����, �����)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		E_HandState HandState;
	//�����Ʈ�ѷ� ������Ʈ�� �־��� ���� (�޼����� ���������� ��)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hand")
		EControllerHand Hand;
	//���к��� ��ġ�� ��������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
		class USceneComponent* ShieldAttachScene;
	//����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield")
		class APlayerShield* Shield;
	//���ǰ������ ��ġ�� ��������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PotionBag")
		class USceneComponent* PotionBagAttachScene;
	//���ǰ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PotionBag")
		class APotionBag* PotionBag;
	//�׸���ư�� ������ ȣ��Ǵ� �Լ�
	UFUNCTION()
		void GrabActor();
	//�׸���ư�� ���� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void ReleaseActor();
	//�ڱ� �ڽ������� ���Ͱ� �ִ��� Ȯ�����ִ� �Լ�
	UFUNCTION()
		AActor* GetActorNearHand();
	//�޼տ� �ٸ� ���Ͱ� �ε����� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	//���� �ٸ� ���Ϳ��� �������� �� �������� �� ȣ��
	UFUNCTION()
		void OnHandEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//�������� String���� ����ȯ���ִ� �Լ�
	UFUNCTION()
		FString GetEnumToString(EControllerHand Value);
};
