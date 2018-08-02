// Fill out your copyright notice in the Description page of Project Settings.

#include "LeftHandMotionController.h"
#include "Public/MotionControllerComponent.h" //�����Ʈ�ѷ� ������Ʈ �������
#include "Components/SkeletalMeshComponent.h" //���̷�Ż�޽� ������Ʈ �������
#include "Components/SphereComponent.h" //���Ǿ��ݸ��� ������Ʈ �������
#include "SteamVRChaperoneComponent.h" //��ȣ�ۿ뿪���� �����ִ� ������Ʈ �������
#include "UObject/ConstructorHelpers.h" //
#include "Animation/AnimBlueprint.h"

#include "Equipment/PlayerShield.h"
#include "Item/PotionBag.h"

// Sets default values
ALeftHandMotionController::ALeftHandMotionController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController")); //�����Ʈ�ѷ� ������Ʈ�� �����ؼ� MotionController�� �ִ´�.
	MotionController->SetupAttachment(RootComponent); //�������� MotionController�� RootComponent�� ���δ�.

	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh")); //���̷�Ż�޽� ������Ʈ�� �����ؼ� HandMesh�� �ִ´�.
	HandMesh->SetupAttachment(MotionController); //�������� HandMesh�� MotionController�� ���δ�.

												 //���̷�Ż�޽� ������Ʈ�� ���̷�Ż�� �־��ֱ� ���� ������ �� �ִ� ���̷�Ż�޽��� �����ͼ� SK_LeftHand�� �־��ش�.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_LeftHand(TEXT("SkeletalMesh'/Game/Assets/MyCharacter/Hand/Mesh/MannequinHand_Right.MannequinHand_Right'"));
	if (SK_LeftHand.Succeeded()) //�ҷ����µ� ����������
	{
		HandMesh->SetSkeletalMesh(SK_LeftHand.Object); //���̷�Ż�޽�������Ʈ�� ���̷�Ż�޽��� �տ��� ������ ���̷�Ż�޽��� �־��ش�.
	}

	HandMesh->SetRelativeRotation(FRotator(0, 0, 90.0f)); //���̷�Ż �޽��� ������ �ٶ󺸰� ������ �����Ѵ�.

	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabComponent")); //�������� �ִ� ���͵��� �Ǻ��ϱ� ���� �ݸ����� ���Ǿ��ݸ������� �����ؼ� GrabShere�� �ִ´�.
	GrabSphere->SetupAttachment(HandMesh); //������ ���Ǿ��ݸ����� HandMesh�� ���δ�.

	GrabSphere->SetRelativeLocation(FVector(14.0f, 0, 0)); //���Ǿ��ݸ����� ��ġ�� �����Ѵ�.
	GrabSphere->SetSphereRadius(10.0f); //���Ǿ��ݸ����� ũ�⸦ �����Ѵ�.
	SteamVRChaperone = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("SteamVRChaperone"));

	WantToGrip = false; //��� ���� ���� ��Ȳ���� �ʱⰪ�� �����Ѵ�.
	AttachedActor = nullptr; //���� ���͸� �ʱⰪ���� nullptr�� �����ϳ�.

	HandState = E_HandState::Grab; //�޼��� ���¸� �׷����·� �ʱ�ȭ�Ѵ�.
	Hand = EControllerHand::Left; //�����Ʈ�ѷ�������Ʈ�� �־��ֱ� ���� Hand�� Left�� �ʱ�ȭ�Ѵ�.
	VisibleShieldFlag = true;	  // �׷����¿��� ���̹Ƿ� ��ÿ��� ������ �ش�.

	FString HandName = GetEnumToString(Hand); //Left�� ��Ʈ������ ����ȯ�ؼ� HandName�� �ְ�
	MotionController->MotionSource = (FName(*HandName)); //�����Ʈ�ѷ�������Ʈ�� MotionSource�� �ִ´�.

	ShieldAttachScene = CreateDefaultSubobject<USceneComponent>(TEXT("ShieldAttachScene")); //���и� ���� ��������Ʈ�� �����ؼ� ShieldAttachScene�� �ִ´�.
	ShieldAttachScene->SetupAttachment(HandMesh);//������ ��������Ʈ�� HandMesh�� ���δ�.

	ShieldAttachScene->SetRelativeRotation(FRotator(0, 0, -90.0f)); //���� ��������Ʈ�� ������
	ShieldAttachScene->SetRelativeLocation(FVector(10.0f, 20.0f, 0)); //��ġ�� �����Ѵ�.

																	  //���ǰ����� ���� ��������Ʈ�� �����ؼ� PotionBagAttachScene�� �����Ѵ�.
	PotionBagAttachScene = CreateDefaultSubobject<USceneComponent>(TEXT("PotionAttachScene"));
	PotionBagAttachScene->SetupAttachment(HandMesh); //������ ��������Ʈ�� HandMesh�� ���δ�.

	PotionBagAttachScene->SetRelativeLocation(FVector(-8.0f, 0, -18.0f)); //��ġ�� �����Ѵ�.
	PotionBagAttachScene->SetRelativeRotation(FRotator(0, 90.0f, 0));

	//�޼��� �ִϸ��̼��� �������ֱ� ���� �����ͻ� �ִ� �ִϸ��̼Ǻ������Ʈ�� �����ͼ� ABP_Hand�� �ִ´�.
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint>ABP_Hand(TEXT("AnimBlueprint'/Game/Blueprints/MyCharacter/Hand/ABP_LeftHand.ABP_LeftHand'"));

	if (ABP_Hand.Succeeded()) //�ҷ����µ� ����������
	{
		HandMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint); //�޼��� �ִϸ��̼Ǹ�带 �������Ʈ�� �����Ѵ�.
		HandMesh->SetAnimInstanceClass(ABP_Hand.Object->GeneratedClass); //�޼��� �ִϸ��̼Ǻ������Ʈ�� ABP_Hand�� �����Ѵ�.
	}

	Tags.Add(FName(TEXT("LeftHand"))); //�޼��� �±׸� LeftHand�� ���Ѵ�.
}

// Called when the game starts or when spawned
void ALeftHandMotionController::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnActorOption;//�����Ҷ��� �ɼ��� �����ϱ� ���� FActorSpawnParameters�� �����Ѵ�.
	SpawnActorOption.Owner = this; //������ ������ �����ڸ� this�� �����Ѵ�.
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //�����ϴ� ���Ͱ� �ݸ����� ������� �׻� �����ǵ��� �����Ѵ�.

	 //������ ���͸� Attach�Ҷ��� �ɼ��� �����ϱ����� FAttachmentTransfromRules�� �����ϰ� 
	//������ġ�� Ÿ������, ���ϰ����� Ÿ������, ũ��� ����ũ�⿡ �°Բ� �ٿ��ش�.
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	//���и� ���� �� ������Ʈ�� ������Ų��.
	Shield = GetWorld()->SpawnActor<APlayerShield>(Shield->StaticClass(), ShieldAttachScene->GetComponentLocation(), ShieldAttachScene->GetComponentRotation(), SpawnActorOption);
	//���и� AttachRules�� ���� ���� �� ������Ʈ�� ���δ�.
	Shield->AttachToComponent(ShieldAttachScene, AttachRules);

	PotionBag = GetWorld()->SpawnActor<APotionBag>(PotionBag->StaticClass(), PotionBagAttachScene->GetComponentLocation(), PotionBagAttachScene->GetComponentRotation(), SpawnActorOption);
	PotionBag->AttachToComponent(PotionBagAttachScene, AttachRules);

	//�⺻ �޽��� ���������� ���ư� �����Ƿ�
	//�޼����� ǥ���ϱ� ���� ũ��� ������ �������ش�.
	if (HandMesh)
	{
		HandMesh->SetWorldScale3D(FVector(1.0f, 1.0f, -1.0f));
		HandMesh->SetWorldRotation(FRotator(0, 100.0f, 30.0f));
	}
}

// Called every frame
void ALeftHandMotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ƽ�� �������� ��ó�� ���Ͱ� �ִ���, ���� ���Ͱ� �ִ���, �׸��� ���� �������� �Ǻ��ؼ�
	//���� �ִϸ��̼� ���¸� ������Ʈ �Ѵ�.
	AActor* NearestMesh;  //��ó�� �ִ� ���͸� �����ص� ����

	if (AttachedActor != nullptr || WantToGrip == true || VisibleShieldFlag) //���� ���Ͱ� �ְų�, ������̸�
	{
		HandState = E_HandState::Grab; //���� ���¸� Grab���� �ٲ㼭 ����·� �ִϸ��̼��� ����Ѵ�.
	}
	else //���� ���Ͱ� ���ų�, ����°� �ƴϸ�
	{
		NearestMesh = GetActorNearHand(); //��ó�� �ִ� ���Ͱ� �ִ��� �Ǻ��Ѵ�.
		if (NearestMesh != nullptr) //��ó�� ���Ͱ� ������
		{
			HandState = E_HandState::CanGrab; //���� ���¸� CanGrab���� �ٲ㼭 ��� �ִ� ���·� �ִϸ��̼��� ����Ѵ�.
		}
		else //��ó�� ���Ͱ� ���ٸ�
		{
			if (WantToGrip) //��������� �Ǻ��ϰ�
			{
				HandState = E_HandState::Grab; //������̸� ���� ���¸� Grab���� �ٲ۴�.
			}
			else
			{
				HandState = E_HandState::Open; //����°� �ƴϸ� ���� ���¸� Open���� �ٲ۴�.
			}
		}
	}
}

//�׸���ư�� ������ ȣ��Ǵ� �Լ�
void ALeftHandMotionController::GrabActor()
{
	AActor* NearestMesh; //��ó�� �ִ� ���͸� �����ص� ����
	WantToGrip = true; //����·� �ٲ۴�.

	if (HandTouchActorFlag)//�տ� ���Ͱ� �ε��� �����̸�
	{
		NearestMesh = GetActorNearHand(); //��ó�� �ִ� ���Ͱ� ���� �Ǻ��Ѵ�.
		if (NearestMesh) //��ó�� ���Ͱ� ������
		{
			// �޼��� � �͵� ���� �� ����.
			// ���� ���� ���̶�� ���� ���� �ְ�����, ���� ����ٴ����� �ʴ´�.
		}
	}
}

//�׸���ư�� ���� ȣ��Ǵ� �Լ�
void ALeftHandMotionController::ReleaseActor()
{
	if (AttachedActor)
	{
		WantToGrip = false;//�������� ���·� �ٲ۴�.
		VisibleShieldFlag = false;			// ���� �׷��� Ǯ���� ��. ���������� �Լ����� ���ͷκ��� ���������� true�� �ٲ۴�.
		if (AttachedActor->GetRootComponent()->GetAttachParent() == MotionController)
		{
			if (AttachedActor->ActorHasTag("Door"))			// �޼��� ���� �� �ִ°��� �� ���̴�.
			{
				// ���� ��Ҵ� ������ ��
			}
		}
	}
}

//�������� �ִ� ���͵� �߿��� ���� ������ �ִ� ���͸� ���ϴ� �Լ�
AActor * ALeftHandMotionController::GetActorNearHand()
{
	TArray<AActor*> OverlappingActors;//�������� �ִ� ���͵��� ������� �������� ����

	FVector GrabSphereLocation; //�� �ݸ����� ��ġ�� �����ص� ����
	FVector OverlappingActorLocation; //�ݸ����� �ε��� ������ ��ġ�� �����ص� ����
	FVector SubActorLocation; //�� �ݸ����� �ݸ����� �ε��� ������ �Ÿ����� �����ص� ����
	AActor* NearestOverlappingActor = nullptr; //�տ��� ���尡���� �ִ� ���͸� �����ص� ���� 
	float NearestOverlap = 10000.0f;//���� ������ �ִ� ���͸� �Ǻ����ֱ� ���� ���Ƿ� ������ �Ÿ���

	GrabSphere->GetOverlappingActors(OverlappingActors, AActor::StaticClass()); //�� �ݸ����� �ε��� �����͵��� �����ͼ� �ռ� �������� �迭�� ��´�.
	GrabSphereLocation = GrabSphere->GetComponentLocation(); //�� �ݸ����� ��ġ�� �����صд�.

	for (AActor* OverlappingActor : OverlappingActors) //�迭�� ����ִ� ���͵��� ���鼭
	{
		if (OverlappingActor->ActorHasTag("Character") || OverlappingActor->ActorHasTag("RightHand") || OverlappingActor->ActorHasTag("PlayerSword")
			|| OverlappingActor->ActorHasTag("PlayerShield") || OverlappingActor->ActorHasTag("PotionBag")) //�ȿ� ��� �ִ� ���Ͱ� ĳ����, ������, ��, �����̸� 
		{
			continue; //�����ϰ� ������ �迭�� �����Ѵ�.
		}
		else if (OverlappingActor->ActorHasTag("Door"))		// ���� ���Ͱ� 'Door'��� �±׸� ������ ������ ����.
		{
			OverlappingActorLocation = OverlappingActor->GetActorLocation();// �迭�� �ִ� ������ �Ÿ��� �����Ѵ�.
			SubActorLocation = OverlappingActorLocation - GrabSphereLocation; // �ε��� ���Ϳ� ���ݸ��� ������ ��ġ�� ���Ѵ�.
			if (SubActorLocation.Size() < NearestOverlap) //�ε��� ���Ϳ� ���ݸ��� ������ �Ÿ��� ���� ������ �Ÿ����� ������
			{
				NearestOverlappingActor = OverlappingActor; //�ε��� ���͸� ���尡���� �ִ� ���ͷ� ���Ѵ�.
			}
		}
	}
	return NearestOverlappingActor; //�ݺ����� ���� ���� ���尡���� �ִ� ���͸� ��ȯ�Ѵ�.
}

void ALeftHandMotionController::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("PlayerSword") || OtherActor->ActorHasTag("LeftHand") || OtherActor->ActorHasTag("Character") || OtherActor->ActorHasTag("PlayerShield") || OtherActor->ActorHasTag("RightHand"))
	{
		return;
	}

	if (OtherActor->ActorHasTag("Door"))			// ������ ������ �Ǹ� ����
	{
		VisibleShieldFlag = false;					// ���и� ���̰� ���� �ʴ´�.
		Shield->SetActorHiddenInGame(true);			// ���и� �����.(������ �ʰ� �Ѵ�.)
		HandTouchActorFlag = true;					// �տ��� ��ü�� �ε�����.
		return;
	}
}

void ALeftHandMotionController::OnHandEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor->ActorHasTag("PlayerSword") || OtherActor->ActorHasTag("LeftHand") || OtherActor->ActorHasTag("Character") || OtherActor->ActorHasTag("PlayerShield") || OtherActor->ActorHasTag("RightHand"))
	{
		return;
	}
	if (OtherActor->ActorHasTag("Door"))			// ������ ������ ���� �Ǹ� ����
	{
		VisibleShieldFlag = true;					// ���и� �� �� �ִ�.
		HandTouchActorFlag = false;					// �տ��� ��ü�� �ε����� �ʾҴ�.
		Shield->SetActorHiddenInGame(false);		// ���и� ������ �ʴ´�.(���̰� �Ѵ�)
		return;
	}
}

FString ALeftHandMotionController::GetEnumToString(EControllerHand Value)
{
	//��ü ��Ű���� UEum�� Ÿ�Կ��� EControllerHand�� ã�Ƽ� enumPtr�� �־��ش�.
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EControllerHand"), true);
	if (!enumPtr) //enumPtr�� ��� ������
	{
		return FString("Invalid"); //�Ұ����ϴٰ� �����ϰ�
	}
	//�����ϸ� 
	return enumPtr->GetEnumName((int32)Value);
}
