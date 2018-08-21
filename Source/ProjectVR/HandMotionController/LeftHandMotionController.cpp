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

												 //스켈레탈메쉬 컴포넌트에 스켈레탈을 넣어주기 위해 에디터 상에 있는 스켈레탈메쉬를 가져와서 SK_LeftHand에 넣어준다.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_LeftHand(TEXT("SkeletalMesh'/Game/Assets/MyCharacter/Hand/Mesh/MannequinHand_Right.MannequinHand_Right'"));
	if (SK_LeftHand.Succeeded()) //불러오는데 성공햇으면
	{
		HandMesh->SetSkeletalMesh(SK_LeftHand.Object); //스켈레탈메쉬컴포넌트의 스켈레탈메쉬에 앞에서 가져온 스켈레탈메쉬를 넣어준다.
	}

	HandMesh->SetRelativeRotation(FRotator(0, 0, 90.0f)); //스켈레탈 메쉬가 정면을 바라보게 각도를 변경한다.

	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabComponent")); //손주위에 있는 액터들을 판별하기 위한 콜리전을 스피어콜리전으로 생성해서 GrabShere에 넣는다.
	GrabSphere->SetupAttachment(HandMesh); //생성한 스피어콜리전을 HandMesh에 붙인다.

	GrabSphere->SetRelativeLocation(FVector(14.0f, 0, 0)); //스피어콜리전의 위치를 조정한다.
	GrabSphere->SetSphereRadius(10.0f); //스피어콜리전의 크기를 지정한다.
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

	ShieldAttachScene->SetRelativeLocation(FVector(-18.0f, -31.0f, 8.0f)); //위치를 조정한다.
	ShieldAttachScene->SetRelativeRotation(FRotator(13.0f, 129.0f, -90.0f)); //방패 씬컴포넌트의 각도와

																			 //포션가방을 붙일 씬컴포넌트를 생성해서 PotionBagAttachScene에 저장한다.
	PotionBagAttachScene = CreateDefaultSubobject<USceneComponent>(TEXT("PotionAttachScene"));
	PotionBagAttachScene->SetupAttachment(HandMesh); //생성한 씬컴포넌트를 HandMesh에 붙인다.

	PotionBagAttachScene->SetRelativeLocation(FVector(-13.0f, 0, 1.8f)); //위치를 조정한다.
	PotionBagAttachScene->SetRelativeRotation(FRotator(0, 93.0f, 0));

	//왼손의 애니메이션을 지정해주기 위해 에디터상에 있는 애니메이션블루프린트를 가져와서 ABP_Hand에 넣는다.
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint>ABP_Hand(TEXT("AnimBlueprint'/Game/Blueprints/MyCharacter/Hand/ABP_LeftHand.ABP_LeftHand'"));

	if (ABP_Hand.Succeeded()) //불러오는데 성공했으면
	{
		HandMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint); //왼손의 애니메이션모드를 블루프린트로 지정한다.
		HandMesh->SetAnimInstanceClass(ABP_Hand.Object->GeneratedClass); //왼손의 애니메이션블루프린트를 ABP_Hand로 지정한다.
	}

	Tags.Add(FName(TEXT("LeftHand"))); //왼손의 태그를 LeftHand로 정한다.
	Tags.Add(FName(TEXT("DisregardForLeftHand")));
	Tags.Add(FName(TEXT("DisregardForRightHand")));
}

// Called when the game starts or when spawned
void ALeftHandMotionController::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnActorOption;//스폰할때의 옵션을 지정하기 위해 FActorSpawnParameters를 선언한다.
	SpawnActorOption.Owner = this; //스폰할 액터의 소유자를 this로 지정한다.
	SpawnActorOption.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; //스폰하는 액터가 콜리전에 상관없이 항상 스폰되도록 지정한다.

																									   //스폰할 액터를 Attach할때의 옵션을 지정하기위해 FAttachmentTransfromRules를 선언하고 
																									   //붙일위치는 타겟으로, 붙일각도도 타겟으로, 크기는 월드크기에 맞게끔 붙여준다.
	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	////방패를 쉴드 씬 컴포넌트에 스폰시킨다.
	Shield = GetWorld()->SpawnActor<APlayerShield>(Shield->StaticClass(), ShieldAttachScene->GetComponentLocation(), ShieldAttachScene->GetComponentRotation(), SpawnActorOption);
	////방패를 AttachRules를 토대로 쉴드 씬 컴포넌트에 붙인다.
	Shield->AttachToComponent(ShieldAttachScene, AttachRules);

	PotionBag = GetWorld()->SpawnActor<APotionBag>(PotionBag->StaticClass(), PotionBagAttachScene->GetComponentLocation(), PotionBagAttachScene->GetComponentRotation(), SpawnActorOption);
	PotionBag->AttachToComponent(PotionBagAttachScene, AttachRules);

	GrabSphere->OnComponentBeginOverlap.AddDynamic(this, &ALeftHandMotionController::OnComponentBeginOverlap);

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

	//UE_LOG(LogTemp, Log, TEXT("%f"), GrabSphere->GetPhysicsLinearVelocity().Size());

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

//그립버튼을 누를때 호출되는 함수
void ALeftHandMotionController::GrabActor()
{
	AActor* NearestMesh; //근처에 있는 액터를 저장해둘 변수
	WantToGrip = true; //쥔상태로 바꾼다.

	if (HandTouchActorFlag)//손에 액터가 부딪힌 상태이면
	{
		NearestMesh = GetActorNearHand(); //근처에 있는 액터가 뭔지 판별한다.
		if (NearestMesh) //근처에 액터가 있으면
		{
			// 왼손은 어떤 것도 집을 수 없다.
			// 문을 여는 것이라면 붙일 수는 있겠지만, 손을 따라다니지는 않는다.
		}
	}
}

//그립버튼을 뗄때 호출되는 함수
void ALeftHandMotionController::ReleaseActor()
{
	if (AttachedActor)
	{
		WantToGrip = false;//쥐지않은 상태로 바꾼다.
		VisibleShieldFlag = false;			// 단지 그랩을 풀었을 뿐. 오버랩엔드 함수에서 액터로부터 빠져나오면 true로 바꾼다.
		if (AttachedActor->GetRootComponent()->GetAttachParent() == MotionController)
		{
			if (AttachedActor->ActorHasTag("Door"))			// 왼손이 잡을 수 있는것은 문 뿐이다.
			{
				// 문을 잡았다 놓았을 때
			}
		}
	}
}

//손주위에 있는 액터들 중에서 가장 가까이 있는 액터를 구하는 함수
AActor * ALeftHandMotionController::GetActorNearHand()
{
	TArray<AActor*> OverlappingActors;//손주위에 있는 액터들을 담기위해 선언해준 변수

	FVector GrabSphereLocation; //손 콜리전의 위치를 저장해둘 변수
	FVector OverlappingActorLocation; //콜리전에 부딪힌 액터의 위치를 저장해둘 변수
	FVector SubActorLocation; //손 콜리전과 콜리전에 부딪힌 액터의 거리값을 저장해둘 변수
	AActor* NearestOverlappingActor = nullptr; //손에서 가장가까이 있는 액터를 저장해둘 변수 
	float NearestOverlap = 10000.0f;//가장 가까이 있는 액터를 판별해주기 위해 임의로 지정한 거리값

	GrabSphere->GetOverlappingActors(OverlappingActors, AActor::StaticClass()); //손 콜리전에 부딪힌 모든액터들을 가져와서 앞서 선언해준 배열에 담는다.
	GrabSphereLocation = GrabSphere->GetComponentLocation(); //손 콜리전의 위치를 저장해둔다.

	for (AActor* OverlappingActor : OverlappingActors) //배열에 담겨있는 액터들을 돌면서
	{
		if (OverlappingActor->ActorHasTag("DisregardForRightHand") || OverlappingActor->ActorHasTag("DisregardForLeftHand")) //안에 담겨 있는 액터가 캐릭터, 오른손, 검, 방패이면 
		{
			continue; //무시하고 다음번 배열로 속행한다.
		}
		else if (OverlappingActor->ActorHasTag("Door"))		// 접한 액터가 'Door'라는 태그를 가지고 있으면 실행.
		{
			OverlappingActorLocation = OverlappingActor->GetActorLocation();// 배열에 있는 액터의 거리를 저장한다.
			SubActorLocation = OverlappingActorLocation - GrabSphereLocation; // 부딪힌 액터와 손콜리전 사이의 위치를 구한다.
			if (SubActorLocation.Size() < NearestOverlap) //부딪힌 액터와 손콜리전 사이의 거리가 이전 액터의 거리보다 작으면
			{
				NearestOverlappingActor = OverlappingActor; //부딪힌 액터를 가장가까이 있는 액터로 정한다.
			}
		}
	}
	return NearestOverlappingActor; //반복문을 돌며 구한 가장가까이 있는 액터를 반환한다.
}

void ALeftHandMotionController::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Dog"))
	{
		ADog* RagdollDog = Cast<ADog>(OtherActor);

		if (RagdollDog->AttachActor)		// 개가 물고 있지 않으면 왼손과 어떤 상호작용을 해도 무시할 수 있어야 함
		{
			if (GrabSphere->GetPhysicsLinearVelocity().Size() >= 350.0f)
			{
				AMotionControllerCharacter* Character = Cast<AMotionControllerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

				if (Character->RightHand->AttachDog)
				{
					UGameplayStatics::ApplyDamage(OtherActor, 10.0f, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, nullptr);		// 오버랩된 액터에 데미지 전달
				}
			}
		}
	}

	if (OtherActor->ActorHasTag("Door"))			// 문에서 오버랩 되면 실행
	{
		VisibleShieldFlag = false;					// 방패를 보이게 하지 않는다.
		Shield->SetActorHiddenInGame(true);			// 방패를 숨긴다.(보이지 않게 한다.)
		HandTouchActorFlag = true;					// 손에서 물체와 부딪혔다.
		return;
	}

	if (OtherActor->ActorHasTag("DisregardForLeftHand"))
	{
		return;
	}
}

void ALeftHandMotionController::OnHandEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Door"))			// 문에서 오버랩 엔드 되면 실행
	{
		VisibleShieldFlag = true;					// 방패를 볼 수 있다.
		HandTouchActorFlag = false;					// 손에서 물체와 부딪히지 않았다.
		Shield->SetActorHiddenInGame(false);		// 방패를 숨기지 않는다.(보이게 한다)
		return;
	}
	if (OtherActor->ActorHasTag("DisregardForLeftHand"))
	{
		return;
	}
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
