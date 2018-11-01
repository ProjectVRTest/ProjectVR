// Fill out your copyright notice in the Description page of Project Settings.

#include "LeftHandMotionController.h"
#include "Public/MotionControllerComponent.h" //모션컨트롤러 컴포넌트 헤더파일
#include "Components/SkeletalMeshComponent.h" //스켈레탈메쉬 컴포넌트 헤더파일
#include "Components/SphereComponent.h" //스피어콜리전 컴포넌트 헤더파일
#include "SteamVRChaperoneComponent.h" //상호작용역영을 보여주는 컴포넌트 헤더파일
#include "UObject/ConstructorHelpers.h" //
#include "Animation/AnimBlueprint.h"

#include "Equipment/PlayerShield.h"
#include "Item/PotionBag.h"

#include "MyCharacter/MotionControllerCharacter.h"
#include "HandMotionController/RightHandMotionController.h"
#include "Kismet/GameplayStatics.h"
#include "Monster/Dog/Dog.h"		// 왼손으로 개를 때릴때, 개가 물고 있는지를 알기 위해서 형변환 해줘야함
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Haptics/HapticFeedbackEffect_Base.h"
#include "Components/WidgetInteractionComponent.h"			// 상호작용

// Sets default values
ALeftHandMotionController::ALeftHandMotionController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController")); //모션컨트롤러 컴포넌트를 생성해서 MotionController에 넣는다.
	SetRootComponent(MotionController);
	//MotionController->SetupAttachment(RootComponent); //생성해준 MotionController를 RootComponent에 붙인다.

	
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh")); //스켈레탈메쉬 컴포넌트를 생성해서 HandMesh에 넣는다.
	HandMesh->SetupAttachment(MotionController); //생성해준 HandMesh를 MotionController에 붙인다.
	HandMesh->SetRelativeLocation(FVector(-15.0f, 0, 7.1f));
	HandMesh->SetRelativeRotation(FRotator(-50.0f, 0, 90.0f));

												 //스켈레탈메쉬 컴포넌트에 스켈레탈을 넣어주기 위해 에디터 상에 있는 스켈레탈메쉬를 가져와서 SK_LeftHand에 넣어준다.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_LeftHand(TEXT("SkeletalMesh'/Game/Assets/CharacterEquipment/MyCharacter/Hand/Mesh/MannequinHand_Right.MannequinHand_Right'"));
	if (SK_LeftHand.Succeeded()) //불러오는데 성공햇으면
	{
		HandMesh->SetSkeletalMesh(SK_LeftHand.Object); //스켈레탈메쉬컴포넌트의 스켈레탈메쉬에 앞에서 가져온 스켈레탈메쉬를 넣어준다.
	}

	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabComponent")); //손주위에 있는 액터들을 판별하기 위한 콜리전을 스피어콜리전으로 생성해서 GrabShere에 넣는다.
	GrabSphere->SetupAttachment(HandMesh); //생성한 스피어콜리전을 HandMesh에 붙인다.

	GrabSphere->SetRelativeLocation(FVector(14.0f, 0, 0)); //스피어콜리전의 위치를 조정한다.
	GrabSphere->SetSphereRadius(10.0f); //스피어콜리전의 크기를 지정한다.

	interaction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Interaction"));
	interaction->SetupAttachment(GrabSphere);

	SteamVRChaperone = CreateDefaultSubobject<USteamVRChaperoneComponent>(TEXT("SteamVRChaperone"));

	WantToGrip = false; //쥐고 있지 않은 상황으로 초기값을 설정한다.
	AttachedActor = nullptr; //붙일 액터를 초기값으로 nullptr로 설정하낟.

	HandState = E_HandState::Grab; //왼손의 상태를 그랩상태로 초기화한다.
	Hand = EControllerHand::Left; //모션컨트롤러컴포넌트에 넣어주기 위해 Hand를 Left로 초기화한다.
	VisibleShieldFlag = true;	  // 그랩상태여야 보이므로 평시에는 투명도를 준다.

	FString HandName = GetEnumToString(Hand); //Left를 스트링으로 형변환해서 HandName에 넣고
	MotionController->MotionSource = (FName(*HandName)); //모션컨트롤러컴포넌트의 MotionSource에 넣는다.

	ShieldAttachScene = CreateDefaultSubobject<USceneComponent>(TEXT("ShieldAttachScene")); //방패를 붙일 씬컴포넌트를 생성해서 ShieldAttachScene에 넣는다.
	ShieldAttachScene->SetupAttachment(HandMesh);//생성한 씬컴포넌트를 HandMesh에 붙인다.
	//ShieldAttachScene->SetRelativeLocation(FVector(-18.0f, -31.0f, 8.0f)); //위치를 조정한다.
	//ShieldAttachScene->SetRelativeRotation(FRotator(13.0f, 129.0f, -90.0f)); //방패 씬컴포넌트의 각도와

																			 //포션가방을 붙일 씬컴포넌트를 생성해서 PotionBagAttachScene에 저장한다.
	PotionBagAttachScene = CreateDefaultSubobject<USceneComponent>(TEXT("PotionAttachScene"));
	PotionBagAttachScene->SetupAttachment(HandMesh); //생성한 씬컴포넌트를 HandMesh에 붙인다.

	PotionBagAttachScene->SetRelativeLocation(FVector(-13.0f, 0, 1.8f)); //위치를 조정한다.
	PotionBagAttachScene->SetRelativeRotation(FRotator(0, 93.0f, 0));

	//왼팔 부분 비어있는 공간을 채워주기 위한 스테틱 메쉬컴포넌트를 생성해준다.
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(HandMesh); //손메쉬에 붙이고
	SphereMesh->SetRelativeLocation(FVector(-18.0f, -0.07149f, -0.485587f)); //위치를 조정하고
	SphereMesh->SetRelativeScale3D(FVector(0.04f, 0.09f, 0.07f )); //크기도 조정하고
	SphereMesh->SetCollisionProfileName(TEXT("NoCollision")); //콜리전 반응은 없앤다.
	
	//앞에서 생성한 SphereMesh에 메쉬를 넣어주기위해 에디터에서 메쉬를 찾아온다.
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sphere(TEXT("StaticMesh'/Game/Assets/CharacterEquipment/MyCharacter/Hand/Mesh/Sphere.Sphere'"));

	//찾는것에 성공하면
	if (SM_Sphere.Succeeded())
	{
		//넣는다.
		SphereMesh->SetStaticMesh(SM_Sphere.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>M_Sphere(TEXT("Material'/Game/Assets/CharacterEquipment/MyCharacter/Hand/Materials/NewMaterial.NewMaterial'"));

	if (M_Sphere.Succeeded())
	{
		SphereMesh->SetMaterial(0, M_Sphere.Object);
	}	

	static ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base> HapticEffect(TEXT("HapticFeedbackEffect_Curve'/Game/Assets/CharacterEquipment/MyCharacter/Hand/HandHaptics.HandHaptics'"));

	if (HapticEffect.Succeeded())
	{
		VRHapticEffect = HapticEffect.Object;
	}	
	
	//왼손의 애니메이션을 지정해주기 위해 에디터상에 있는 애니메이션블루프린트를 가져와서 ABP_Hand에 넣는다.
	static ConstructorHelpers::FObjectFinder<UClass>ABP_Hand(TEXT("AnimBlueprint'/Game/Blueprints/MyCharacter/Hand/ABP_LeftHand.ABP_LeftHand_C'"));

	if (ABP_Hand.Succeeded()) //불러오는데 성공했으면
	{
		UClass* LeftHandAnimBlueprint = ABP_Hand.Object;

		if (LeftHandAnimBlueprint)
		{
			HandMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint); //왼손의 애니메이션모드를 블루프린트로 지정한다.
			HandMesh->SetAnimInstanceClass(LeftHandAnimBlueprint); //왼손의 애니메이션블루프린트를 ABP_Hand로 지정한다.
		}		
	}

	Tags.Add(FName(TEXT("LeftHand"))); //왼손의 태그를 LeftHand로 정한다.
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ALeftHandMotionController::BeginPlay()
{
	Super::BeginPlay();

	// 오너 설정
	HandOwner = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	FActorSpawnParameters SpawnActorOption;//스폰할때의 옵션을 지정하기 위해 FActorSpawnParameters를 선언한다.
	SpawnActorOption.Owner = this; //스폰할 액터의 소유자를 this로 지정한다.
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //스폰하는 액터가 콜리전에 상관없이 항상 스폰되도록 지정한다.

																									   //스폰할 액터를 Attach할때의 옵션을 지정하기위해 FAttachmentTransfromRules를 선언하고 
																									   //붙일위치는 타겟으로, 붙일각도도 타겟으로, 크기는 월드크기에 맞게끔 붙여준다.
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	////방패를 쉴드 씬 컴포넌트에 스폰시킨다.
	Shield = GetWorld()->SpawnActor<APlayerShield>(Shield->StaticClass(), ShieldAttachScene->GetComponentLocation(), ShieldAttachScene->GetComponentRotation(), SpawnActorOption);
	////방패를 AttachRules를 토대로 쉴드 씬 컴포넌트에 붙인다.
	if (Shield)
	{
		Shield->AttachToComponent(HandMesh, AttachRules,TEXT("CharacterShieldSocket"));
	}	

	PotionBag = GetWorld()->SpawnActor<APotionBag>(PotionBag->StaticClass(), PotionBagAttachScene->GetComponentLocation(), PotionBagAttachScene->GetComponentRotation(), SpawnActorOption);
	PotionBag->AttachToComponent(PotionBagAttachScene, AttachRules);

	GrabSphere->OnComponentBeginOverlap.AddDynamic(this, &ALeftHandMotionController::OnComponentBeginOverlap);
	GrabSphere->OnComponentEndOverlap.AddDynamic(this, &ALeftHandMotionController::OnHandEndOverlap);
	//기본 메쉬가 오른쪽으로 돌아가 잇으므로
	//왼손으로 표현하기 위해 크기와 각도를 조정해준다.
	if (HandMesh)
	{
		HandMesh->SetWorldScale3D(FVector(1.0f, 1.0f, -1.0f));
		//HandMesh->SetWorldRotation(FRotator(0, 100.0f, 30.0f));
	}
}

// Called every frame
void ALeftHandMotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HandOwner)
	{
		HandCurrentPosistion = GrabSphere->GetComponentLocation() - GetActorLocation();
		HandMoveDelta = HandCurrentPosistion - HandPreviousPosistion;
		HandMoveVelocity = HandMoveDelta / DeltaTime;
		HandPreviousPosistion = HandCurrentPosistion;
	}

	//틱을 돌때마다 근처에 액터가 있는지, 붙은 액터가 있는지, 그립을 누른 상태인지 판별해서
	//손의 애니메이션 상태를 업데이트 한다.
	AActor* NearestMesh;  //근처에 있는 액터를 저장해둘 변수

	if (AttachedActor != nullptr || WantToGrip == true || VisibleShieldFlag) //붙은 액터가 있거나, 쥔상태이면
	{
		HandState = E_HandState::Grab; //손의 상태를 Grab으로 바꿔서 쥔상태로 애니메이션을 출력한다.
	}
	else //붙은 액터가 없거나, 쥔상태가 아니면
	{
		NearestMesh = GetActorNearHand(); //근처에 있는 액터가 있는지 판별한다.
		if (NearestMesh != nullptr) //근처에 액터가 있으면
		{
			HandState = E_HandState::CanGrab; //손의 상태를 CanGrab으로 바꿔서 쥘수 있는 상태로 애니메이션을 출력한다.
		}
		else //근처에 액터가 없다면
		{
			if (WantToGrip) //쥔상태인지 판별하고
			{
				HandState = E_HandState::Grab; //쥔상태이면 손의 상태를 Grab으로 바꾼다.
			}
			else
			{
				HandState = E_HandState::Open; //쥔상태가 아니면 손의 상태를 Open으로 바꾼다.
			}
		}
	}
}

//그립버튼을 눌럿을시 호출되는 함수
void ALeftHandMotionController::GrabActor()
{
	AActor* NearestMesh; //근처에 있는 액터를 저장해둘 변수
	WantToGrip = true; //쥔상태로 바꾼다.
	bisLeftGrab = true;
	if (HandTouchActorFlag)//손에 액터가 부딪힌 상태이면
	{
		NearestMesh = GetActorNearHand(); //근처에 있는 액터가 뭔지 판별한다.
		if (NearestMesh) //근처에 액터가 있으면
		{
			Shield->SetActorHiddenInGame(true);	// 검을 숨김 -> GrabActor함수로 인해 근처에 액터가 있을 때 검을 보이지 않게 한다.
			if (NearestMesh->ActorHasTag("Door"))
			{
				AttachedActor = NearestMesh;
				// 문을 여는 것이라면 붙일 수는 있겠지만, 손을 따라다니지는 않는다.
			}
		}
	}
}

void ALeftHandMotionController::ReleaseActor()
{
	bisLeftGrab = false;
	if (AttachedActor)
	{
		WantToGrip = false;
		VisibleShieldFlag = false;			// 단지 그랩을 풀었을 뿐. 오버랩엔드 함수에서 액터로부터 빠져나오면 true로 바꾼다.
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
		AttachedActor = nullptr;			// 현재 잡은 것이 없다.
	}
	else			// 메뉴에서 그랩 후 릴리즈하면 아무것도 없는 오픈상태여야하기 때문에 잡힌 것이 없을 때 진행
	{
		if (Shield->bHidden)		// 칼이 숨겨져 있을 때(메뉴 범위에 손이 들어가있을 때)
			HandOpenState();		// 릴리즈 시 오픈상태로 변환
	}
}

AActor * ALeftHandMotionController::GetActorNearHand()
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

void ALeftHandMotionController::HandNomalState()
{
	HandTouchActorFlag = false;
	WantToGrip = true;
	VisibleShieldFlag = true;
	Shield->SetActorHiddenInGame(false); // 방패 보이게함
	AttachedActor = nullptr;	
}

void ALeftHandMotionController::HandOpenState()
{
	HandTouchActorFlag = true;
	WantToGrip = false;
	VisibleShieldFlag = false;
	Shield->SetActorHiddenInGame(true); //방패 숨김
}

void ALeftHandMotionController::HandGrabState()
{
	HandTouchActorFlag = false;
	WantToGrip = true;
	VisibleShieldFlag = false;
	Shield->SetActorHiddenInGame(true); //방패 숨김
}

void ALeftHandMotionController::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
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

void ALeftHandMotionController::OnHandEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Door"))
	{
		UE_LOG(LogTemp, Log, TEXT("Door@@@@@"));
		HandNomalState();
		return;
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


FString ALeftHandMotionController::GetEnumToString(EControllerHand Value)
{
	//전체 패키지중 UEum의 타입에서 EControllerHand를 찾아서 enumPtr에 넣어준다.
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EControllerHand"), true);
	if (!enumPtr) //enumPtr이 비어 있으면
	{
		return FString("Invalid"); //불가능하다고 리턴하고
	}
	//가능하면 
	return enumPtr->GetEnumName((int32)Value);
}
