// Fill out your copyright notice in the Description page of Project Settings.

#include "RightHandMotionController.h"
#include "Public/MotionControllerComponent.h" //모션컨트롤러 컴포넌트 헤더파일
#include "Components/SkeletalMeshComponent.h" //스켈레탈메쉬 컴포넌트 헤더파일
#include "Components/SphereComponent.h" //스피어콜리전 컴포넌트 헤더파일
#include "SteamVRChaperoneComponent.h" //스팀 VR 헤더파일

#include "UObject/ConstructorHelpers.h" //오브젝트를 찾는 헤더파일

#include "Animation/AnimBlueprint.h" //애니메이션 블루프린트 헤더파일

#include "Equipment/PlayerSword.h" //검 헤더파일
#include "Components/StaticMeshComponent.h" //스테틱메쉬 컴포넌트 헤더파일
#include "Engine/StreamableManager.h"
#include "Item/Potion.h"					// 포션을 생성해서 손에 붙여야할 때 포션 파일 참조
#include "Kismet/GameplayStatics.h"							// 포션 먹을 때 캐릭터를 불러와서 참조
#include "MyCharacter/MotionControllerCharacter.h"
#include "Item/PotionBag.h"
#include "GameInstance/VRGameInstance.h"

#include "Components/BoxComponent.h"
#include "Monster/Dog/Dog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include "Components/WidgetInteractionComponent.h"

#include "Monster/Dog/Dog.h"
#include "Monster/Dog/DogAIController.h"
#include "Object/Door/LockKey.h"		// 문열 때 키 생성
#include "Object/Door/DoorLock.h"		// 이벤트
#include "Object/Door/LockedDoor.h"		// 이벤트

// Sets default values
ARightHandMotionController::ARightHandMotionController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//모션컨트롤러 컴포넌트를 생성해서 MotionController에 넣는다.
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	//생성한 모션컨트롤러 컴포넌트를 RootComponent에 붙인다.
	SetRootComponent(MotionController);

	//HandMesh에 사용할 스켈레탈 메쉬 컴포넌트를 생성해서 HandMesh에 넣는다.
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	//생성한 HandMesh를 MotionController에 붙인다.
	HandMesh->SetupAttachment(MotionController);

	//HandMesh에 넣을 스켈레탈 메쉬를 에디터상에서 찾아서 SK_RightHand에 넣는다.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_RightHand(TEXT("SkeletalMesh'/Game/Assets/CharacterEquipment/MyCharacter/Hand/Mesh/MannequinHand_Right.MannequinHand_Right'"));

	if (SK_RightHand.Succeeded()) //성공적으로 찾앗는지 확인한다.
	{
		//찾앗으면
		//HandMesh에 찾은 스켈레탈메쉬를 넣어준다.
		HandMesh->SetSkeletalMesh(SK_RightHand.Object);
	}

	HandMesh->SetRelativeLocation(FVector(-15.0f, 1.9f, 9.9f));
	HandMesh->SetRelativeRotation(FRotator(-45.0f, 0, 90.0f)); //넣어준 스켈레탈 메쉬가 정면을 보도록 Roll방향으로 90도 돌린다.
	HandMesh->bGenerateOverlapEvents = true; //오버랩 이벤트가 발생할 수 있도록 켜준다.
	HandMesh->SetCollisionProfileName(FName("NoCollision")); //콜리전 프리셋을 OverlapOnlyPawn으로 바꿔서 Pawn은 오버랩되고 나머지는 블록되게 바꿔준다. 


															 //손주위에 있는 물체들을 감지하기 위한 스피어 콜리전컴포넌트를 생성해서 GrabShpere에 넣는다.
	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabComponent"));
	//생성한 스피어 콜리전 컴포넌트를 HandMesh에 붙인다.
	GrabSphere->SetupAttachment(HandMesh);
	//생성한 스피어 콜리전의 위치값을 조정한다.
	GrabSphere->SetRelativeLocation(FVector(14.0f, 0, 0));
	//생성한 스피어 콜리전의 크기값을 조정한다.
	GrabSphere->SetSphereRadius(10.0f);

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(HandMesh);
	OverlapSphere->SetCollisionProfileName("OverlapAll");
	OverlapSphere->SetRelativeLocation(FVector(9.0f, 3.4f, -1.6f));
	OverlapSphere->SetSphereRadius(7.0f);
	OverlapSphere->bHiddenInGame = true;

	interaction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Interaction"));
	interaction->SetupAttachment(GrabSphere);
	interaction->TraceChannel = ECollisionChannel::ECC_Visibility;

	//오른손의 상호작용 영역을 표시해줄 컴포넌트인 SteamVRChaperone 컴포넌트를 생성해서 StemVRChaperone에 넣어준다.
	SteamVRChaperone = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("SteamVRChaperone"));

	//초기값 설정
	WantToGrip = true; //잡은 상태로 바꿔준다.		///////////////////////////////////////////////////////////////////////////////////
	AttachedActor = nullptr;//손에 붙은 액터를 nullptr로 초기화한다.
	HandState = E_HandState::Grab; //손에 상태를 Grab상태로 바꾼다.		///////////////////////////////////////////////////////////////////////////////////
	HandFormState = EHandFormState::WeaponHandGrab;
	Hand = EControllerHand::Right; //Hand에 Right값을 넣어준다.

								   //모션컨트롤러의 MotionSource에 넣어주기위해 열거형을 이름으로 형변환해서
	FString HandName = GetEnumToString(Hand);
	MotionController->MotionSource = (FName(*HandName)); //넣어준다.

														 //손에 사용할 애니메이션블루프린트를 에디터에서 찾아서 ABP_Hand에 넣어준다.
	static ConstructorHelpers::FObjectFinder<UClass>ABP_Hand(TEXT("AnimBlueprint'/Game/Blueprints/MyCharacter/Hand/ABP_RightHand.ABP_RightHand_C'"));

	if (ABP_Hand.Succeeded()) //애니메이션블루프린트를 찾앗으면
	{
		UClass* RightHandAnimBlueprint = ABP_Hand.Object;

		if (RightHandAnimBlueprint)
		{
			HandMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint); //손의 애니메이션모드를 블루프린트형으로 바꿔주고
			HandMesh->SetAnimInstanceClass(RightHandAnimBlueprint); //위에서 찾은 애니메이션블루프린트를 꽂아준다.
		}
	}

	// 포션생성 위치
	PotionPosition = CreateDefaultSubobject<USceneComponent>(TEXT("PotionPosition"));
	PotionPosition->SetupAttachment(HandMesh);
	PotionPosition->SetRelativeLocation(FVector(11.768803f, 0.847741f, -3.021931f));
	PotionPosition->SetRelativeRotation(FRotator(3.364257f, -86.961861f, 177.434341f));

	// 포션생성 위치
	AttachDogPosition = CreateDefaultSubobject<USceneComponent>(TEXT("AttachDogPosition"));
	AttachDogPosition->SetupAttachment(HandMesh);
	AttachDogPosition->SetRelativeLocation(FVector(-10.0f, 71.845093f, -38.933586f));
	AttachDogPosition->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	//손에 무기를 붙이기 위해 사용할 씬컴포넌트를 생성해서 SwordAttachScene에 넣어준다.
	SwordAttachScene = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponScene"));
	SwordAttachScene->SetupAttachment(HandMesh);// 생성한 검 씬컴포넌트를 HandMesh에 붙인다.
												//붙일 검의 방향을 조절해주기위해


	SwordAttachScene->SetRelativeRotation(FRotator(0, 60.0f, -175.082443f)); //롤 방향으로 180도 돌리고
	SwordAttachScene->SetRelativeLocation(FVector(3.238229f, 5.621831f, -3.814407f)); //x축으로 10만큼 이동시킨다.

	KeySocket = CreateDefaultSubobject<USceneComponent>(TEXT("KeySocket"));
	KeySocket->SetupAttachment(HandMesh);

	KeySocket->SetRelativeLocation(FVector(10.0f, 0.0f, 0.5f));
	KeySocket->SetRelativeRotation(FRotator(88.0f, 180.0f, 180.0f));
	// 88.0f 
	interaction->InteractionDistance = 100.0f;

	bGrabPotion = false;		// 포션 없음

								// 개한테 물렸을 때, 한 방향으로 지속되는 시간
	DefaultTime = 0.2f;
	SwayTime = DefaultTime;
	SwayCount = 0;
	TolerateCount = 0;

	VisibleSwordFlag = true; //초기에는 검을 보여준다.		///////////////////////////////////////////////////////////////////////////////////
	HandTouchActorFlag = true; //처음에는 오른손에 검이 붙여있으므로 true로 해준다
	bisRightGrab = false;
	Tags.Add(FName(TEXT("RightHand"))); //오른손에 태그를 달아준다.
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ARightHandMotionController::BeginPlay()
{
	Super::BeginPlay();

	// 오너 설정
	HandOwner = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	FActorSpawnParameters SpawnActorOption; //액터를 스폰할때 쓰일 구조체 변수
	SpawnActorOption.Owner = this; //스폰할 액터의 주인을 현재 클래스로 정한다.

								   //액터를 스폰할때 충돌처리를 어떻게 할것인지 정해주는 옵션
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//액터를 스폰할때 충돌과 관계없이 스폰시킨다.

																									  //액터를 Attack시킬 때의 옵션
																									  //위치와 각도는 붙이는 타겟에 맞게끔 하고
																									  //스케일은 스폰하는 월드에 맞게끔 계산하여 정한다.
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);


	//현재 월드에 검을 스폰시킨다.
	Sword = GetWorld()->SpawnActor<APlayerSword>(Sword->StaticClass(), SwordAttachScene->GetComponentLocation(), SwordAttachScene->GetComponentRotation(), SpawnActorOption);

	if (Sword)
	{
		Sword->AttachToComponent(HandMesh, AttachRules, TEXT("CharacterSwordSocket"));
	}
	//Sword->AttachToComponent(SwordAttachScene, AttachRules);//스폰한 검을 SwordAttachScene에 붙인다.

	//손에 다른 액터가 부딪히면 호출할 함수(OnHandBeginOverlap)를 바인딩한다.
	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &ARightHandMotionController::OnHandBeginOverlap);
	//손에 다른 액터가 부딪히고 벗어날때 호출할 함수(OnHandEndOverlap)를 바인딩한다.
	OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &ARightHandMotionController::OnHandEndOverlap);

	if (HandMesh)
	{
		//HandMesh->SetWorldRotation(FRotator(-60.0f,0,90.0f));
	}

}

// Called every frame
void ARightHandMotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	AActor* NearestMesh; //손 근처에 있는 액터를 저장할 변수

						 //손에 붙은 액터가 있거나 그랩상태이거나 검이 보이는 상태이면
	if (AttachedActor != nullptr || WantToGrip == true || VisibleSwordFlag)
	{
		HandState = E_HandState::Grab; //손의 상태를 Grab으로 바꾼다.
	}
	else //위의 상태가 아니면
	{
		NearestMesh = GetActorNearHand(); //손 주위에 액터가 있는지 확인한다.
		if (NearestMesh != nullptr) //액터가 있다면
		{
			HandState = E_HandState::CanGrab; //쥘수 있는 상태로 바꾼다.
		}
		else //액터가 없다면
		{
			if (WantToGrip) //액터가 없는데 그랩상태이면
			{
				HandState = E_HandState::Grab; //그랩상태로 바꾼다.
			}
			else //액터도 없고 그랩상태도 아니면
			{
				HandState = E_HandState::Open; //오픈상태로 바꾼다.
			}
		}
	}


	if (AttachDog)
	{
		Prelinear = Currentlinear;

		HandCurrentPosistion = GrabSphere->GetComponentLocation() - GetActorLocation();
		HandMoveDelta = HandCurrentPosistion - HandPreviousPosistion;
		HandMoveVelocity = HandMoveDelta / DeltaTime;
		HandPreviousPosistion = HandCurrentPosistion;

		Currentlinear = HandMoveVelocity.Size();


		if (Currentlinear > 45.0f) stack++;
		else
		{
			if (stack > 0) stack = 0;
		}
	}
}

//그립버튼을 눌럿을시 호출되는 함수
void ARightHandMotionController::GrabActor()
{
	if (bGrabKey) return;
	AActor* NearestMesh; //근처에 있는 액터를 저장해둘 변수
	WantToGrip = true; //쥔상태로 바꾼다.
	bisRightGrab = true;
	if (HandTouchActorFlag)//손에 액터가 부딪힌 상태이면
	{
		HandFormState = EHandFormState::PotionHandGrab;
		NearestMesh = GetActorNearHand(); //근처에 있는 액터가 뭔지 판별한다.
		if (NearestMesh) //근처에 액터가 있으면
		{
			Sword->SetActorHiddenInGame(true);	// 검을 숨김 -> GrabActor함수로 인해 근처에 액터가 있을 때 검을 보이지 않게 한다.
			if (NearestMesh->ActorHasTag("Door"))
			{
				AttachedActor = NearestMesh;
				// 문을 여는 것이라면 붙일 수는 있겠지만, 손을 따라다니지는 않는다.
			}
			//else if (NearestMesh->ActorHasTag("Lock"))
			//{
			//	UE_LOG(LogTemp, Log, TEXT("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL"));
			//	if (HandOwner->bHasKey)
			//	{
			//		UE_LOG(LogTemp, Log, TEXT("Event@@@@@@@@@@@@@@@@@@@@@@@"));
			//		// 자물쇠에 그랩하면 문열리고 나머지 다 사라짐
			//		ADoorLock* Lock = Cast<ADoorLock>(NearestMesh);
			//		if (Lock)
			//		{
			//			Lock->OpenEvent.ExecuteIfBound();
			//			Lock->Destroy();
			//			HandOwner->bHasKey = false;
			//			HandNomalState();
			//		}
			//	}
			//}
			else if (NearestMesh->ActorHasTag("PotionBag"))
			{
				APotionBag* PotionBag = Cast<APotionBag>(NearestMesh);

				if (PotionBag)
				{
					if (PotionBag->Potions.Num()>0)
					{
						FActorSpawnParameters SpawnActorOption; //액터를 스폰할때 쓰일 구조체 변수
						SpawnActorOption.Owner = this; //스폰할 액터의 주인을 현재 클래스로 정한다.

													   //액터를 스폰할때 충돌처리를 어떻게 할것인지 정해주는 옵션
						SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//액터를 스폰할때 충돌과 관계없이 스폰시킨다.

																														  //액터를 Attack시킬 때의 옵션
																														  //위치와 각도는 붙이는 타겟에 맞게끔 하고
																														  //스케일은 스폰하는 월드에 맞게끔 계산하여 정한다.
						FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

						Potion = PotionBag->PotionPop();
						//현재 월드에 검을 스폰시킨다.
						//Potion = GetWorld()->SpawnActor<APotion>(Potion->StaticClass(), PotionPosition->GetComponentLocation(),	PotionPosition->GetComponentRotation(), SpawnActorOption);

						if (Potion)
						{
							GLog->Log(FString::Printf(TEXT("가방에서 꺼낸 포션이 잇음")));
							// 손에 붙임
							AttachedActor = Potion;
							//Potion->Mesh->SetSimulatePhysics(true);
							//Potion->SetActorRelativeScale3D(FVector(0.22f, 0.15f, 0.15f));
							if (Potion->Mesh)
							{
								Potion->Mesh->SetCollisionProfileName(TEXT("OverlapAll"));
								Potion->SetActorRelativeScale3D(FVector(1.1f, 1.1f, 1.1f));
								Potion->AttachToComponent(PotionPosition, AttachRules);//스폰한 검을 SwordAttachScene에 붙인다.
								HandGrabState();
								bGrabPotion = true;
							}							
						}
						else
						{
							GLog->Log(FString::Printf(TEXT("가방에서 꺼낸 포션이 없음")));
						}

						UVRGameInstance* GI = Cast<UVRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

						if (GI)
						{
							GI->PotionCountUpdate(PotionBag->PotionCount);
						}
					}
					else
					{
						HandNomalState();
					}
				}
				else
				{
					GLog->Log(FString::Printf(TEXT("가방 없음")));
				}
			}
			else
			{
				// 손에 붙임
				AttachedActor = NearestMesh;
				// NearestMesh의 SimulationPhsics를 해제한다.	-> 손을 따라다녀야 하므로
				FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
				NearestMesh->GetRootComponent()->AttachToComponent(MotionController, AttachRules);
			}
		}
	}
}

void ARightHandMotionController::ReleaseActor()
{
	if (bGrabKey) return;

	bisRightGrab = false;
	if (AttachedActor)
	{
		HandFormState = EHandFormState::WeaponHandGrab;
		WantToGrip = false;
		VisibleSwordFlag = false;			// 단지 그랩을 풀었을 뿐. 오버랩엔드 함수에서 액터로부터 빠져나오면 true로 바꾼다.
		if (AttachedActor->GetRootComponent()->GetAttachParent() == MotionController)
		{
			if (AttachedActor->ActorHasTag("Door"))
			{

			}
			else		// 문 이외의 것들
			{
				AttachedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);			// 잡은 물체와 뗀다
			}
		}
		else if (AttachedActor->GetRootComponent()->GetAttachParent() == PotionPosition)
		{
			// 잡은 액터가 포션일 때 실행
			if (AttachedActor->ActorHasTag("Potion"))
			{
				APotion* AttachPotion = Cast<APotion>(AttachedActor);
				if (AttachPotion)
				{
					AActor* NearestMesh = GetActorNearHand();

					if (NearestMesh)
					{
						if (NearestMesh->ActorHasTag("PotionBag"))
						{
							APotionBag* PotionBag = Cast<APotionBag>(NearestMesh);
							if (PotionBag)
							{
								PotionBag->PotionPush(AttachPotion);
								HandNomalState();
							}
							else
							{
								bGrabPotion = false;
								AttachPotion->BagInputFlag = true;
								AttachPotion->Mesh->SetCollisionProfileName("NoCollision");
								AttachPotion->Mesh->SetSimulatePhysics(true);
								AttachedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);			// 잡은 물체와 뗀다					
							}
						}
					}
					else
					{
						AttachPotion->BagInputFlag = true;
						AttachPotion->Mesh->SetSimulatePhysics(true);
						AttachedActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);			// 잡은 물체와 뗀다		
					}
				}
			}
		}
		AttachedActor = nullptr;			// 현재 잡은 것이 없다.
	}
	else			// 메뉴에서 그랩 후 릴리즈하면 아무것도 없는 오픈상태여야하기 때문에 잡힌 것이 없을 때 진행
	{
		if (Sword->bHidden)		// 칼이 숨겨져 있을 때(메뉴 범위에 손이 들어가있을 때)
			HandOpenState();		// 릴리즈 시 오픈상태로 변환
	}
}

AActor * ARightHandMotionController::GetActorNearHand()
{
	TArray<AActor*> OverlappingActors;

	FVector GrabSphereLocation;
	FVector OverlappingActorLocation;
	FVector SubActorLocation;
	AActor* NearestOverlappingActor = nullptr;
	float NearestOverlap = 10000.0f;
	GrabSphere->GetOverlappingActors(OverlappingActors, AActor::StaticClass());
	GrabSphereLocation = GrabSphere->GetComponentLocation();

	for (AActor* OverlappingActor : OverlappingActors)
	{
		//GLog->Log(UKismetSystemLibrary::GetObjectName(OverlappingActor));
		if (OverlappingActor->ActorHasTag("DisregardForRightHand") || OverlappingActor->ActorHasTag("DisregardForLeftHand"))
		{
			continue;
		}
		else if (OverlappingActor->ActorHasTag("PotionBag"))
		{
			NearestOverlappingActor = OverlappingActor;
			break;
		}
		else if (OverlappingActor->ActorHasTag("Door"))
		{
			NearestOverlappingActor = OverlappingActor;
			break;
		}
		else		// 기타 액터
		{
			OverlappingActorLocation = OverlappingActor->GetActorLocation();
			SubActorLocation = OverlappingActorLocation - GrabSphereLocation;
			if (SubActorLocation.Size() < NearestOverlap)
			{
				NearestOverlappingActor = OverlappingActor;
				break;
			}
		}
	}
	return NearestOverlappingActor;
}

void ARightHandMotionController::HandNomalState()
{
	HandTouchActorFlag = false;
	WantToGrip = true;
	VisibleSwordFlag = true;
	Sword->SetActorHiddenInGame(false); // 검 보이게함
	AttachedActor = nullptr;		// 포션이 삭제되므로 손에 붙은 액터를 null값으로 바꾼다.
}

void ARightHandMotionController::HandOpenState()
{
	HandTouchActorFlag = true;
	WantToGrip = false;
	VisibleSwordFlag = false;
	Sword->SetActorHiddenInGame(true); //검을 숨김
}

void ARightHandMotionController::HandGrabState()
{
	HandTouchActorFlag = false;
	WantToGrip = true;
	VisibleSwordFlag = false;
	Sword->SetActorHiddenInGame(true); //검을 숨김
}

void ARightHandMotionController::OnHandBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("LockedDoor"))
	{
		if (HandOwner->bHasKey)
			LockedDoor = Cast<ALockedDoor>(OtherActor);
	}

	if (OtherComp->ComponentHasTag("BodyLock"))
	{
		if (HandOwner->bHasKey)
		{
			LockedDoor->GetLockKey();
			HandNomalState();
			Key->Destroy();
			LockedDoor = NULL;
		}
		return;
	}

	if (bGrabKey) return;

	if (OtherComp->ComponentHasTag("LockArea"))
	{
		if (HandOwner->bHasKey)
		{
			bGrabKey = true;
			HandGrabState();
			FActorSpawnParameters SpawnActorOption; //액터를 스폰할때 쓰일 구조체 변수
			SpawnActorOption.Owner = this; //스폰할 액터의 주인을 현재 클래스로 정한다.
			SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//액터를 스폰할때 충돌과 관계없이 스폰시킨다.
			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

			Key = GetWorld()->SpawnActor<ALockKey>(Key->StaticClass(), KeySocket->GetComponentLocation(), KeySocket->GetComponentRotation(), SpawnActorOption);

			if (Key)
			{
				Key->AttachToComponent(KeySocket, AttachRules);
			}
		}
		return;
	}

	// 종류 : 포션박스, 머리, 문, 기타액터
	// 컴포넌트 태그 중 왼손, 오른손 무시의 태그가 있으면 무시 (Head 무시)
	if (OtherComp->ComponentHasTag("DisregardForLeftHand") || OtherComp->ComponentHasTag("DisregardForRightHand"))
	{
		if (OtherComp->ComponentHasTag("Head"))
		{
			HandNomalState();
		}
		return;
	}
	if (AttachedActor)
	{
		return;
	}
	// 메뉴의 공간 범위안에 들어가면 검을 지우는 오픈상태로 되게 함
	if (OtherComp->ComponentHasTag("GrabRange"))
	{
		HandOpenState();
		return;
	}

	if (OtherActor->ActorHasTag("PotionBag"))		// 'PotionBox'라는 태그를 가진 액터일 때
	{
		HandOpenState();
		return;
	}
	else if (OtherActor->ActorHasTag("Door"))		// 'Door'라는 태그를 가진 액터일 때
	{
		HandOpenState();
		return;
	}
	else if (OtherActor->ActorHasTag("DisregardForRightHand"))
	{
		return;
	}
	else 		// 다른 액터일 때
	{
		HandOpenState();
		return;
	}
}

void ARightHandMotionController::OnHandEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("LockedDoor"))
		LockedDoor = NULL;

	if (OtherComp->ComponentHasTag("LockArea"))
	{
		if (Key)
		{
			bGrabKey = false;
			Key->Destroy();
		}
		HandNomalState();
	}

	// 컴포넌트 태그 중 왼손, 오른손 무시의 태그가 있으면 무시 (Head 무시)
	if (OtherComp->ComponentHasTag("DisregardForLeftHand") || OtherComp->ComponentHasTag("DisregardForRightHand"))
		return;

	// 메뉴의 공간 범위를 나가게되면 검을 든 보통상태로 있게 함
	if (OtherComp->ComponentHasTag("GrabRange"))
	{
		HandNomalState();
		return;
	}

	if (OtherActor->ActorHasTag("DisregardForRightHand"))
		return;

	if (HandState != E_HandState::Grab)
		HandNomalState();

	return;
}



FString ARightHandMotionController::GetEnumToString(EControllerHand Value)
{
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EControllerHand"), true);
	if (!enumPtr)
	{
		return FString("Invalid");
	}
	return enumPtr->GetEnumName((int32)Value);
}


// UE_LOG(LogTemp, Log, TEXT("InPotionRangeBox"));