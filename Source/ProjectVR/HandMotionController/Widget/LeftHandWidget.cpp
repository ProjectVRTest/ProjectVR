// Fill out your copyright notice in the Description page of Project Settings.

#include "LeftHandWidget.h"
#include "Components/ProgressBar.h"
#include "Components/HorizontalBox.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SlateWrapperTypes.h"

#include "MyCharacter/MotionControllerPC.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "TimerManager.h"

void ULeftHandWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PortionBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("PotionBox")));				// 유저위젯상의 'PotionBox'로 되어있는 변수를 찾아 캐스트 시킨다.(HorizonBox)
	ForwardHP = Cast<UProgressBar>(GetWidgetFromName(TEXT("ForwardHPBar")));			// 유저위젯상의 'ForwardHPBar'로 되어있는 변수를 찾아 캐스트 시킨다.(ProgressBar)
	BackwardHP = Cast<UProgressBar>(GetWidgetFromName(TEXT("BackwardHPBar")));		// 유저위젯상의 'BackwardHPBar'로 되어있는 변수를 찾아 캐스트 시킨다.(ProgressBar)
	State = Cast<UProgressBar>(GetWidgetFromName(TEXT("StaminaBar")));						// 유저위젯상의 'StaminaBar'로 되어있는 변수를 찾아 캐스트 시킨다.(ProgressBar)
	ForwardHP->SetPercent(1);			// 체력 바 Full로 설정
	BackwardHP->SetPercent(1);		// 체력 바 Full로 설정
	State->SetPercent(1);					// 스테미너 바 Full로 설정

	PotionArray.Shrink();		// 배열에 추가 후 메모리 최적화
				
	ChangOfHP = 0.0f;
	ChangOfStamina = 0.0f;
	Damage = 0.0f;

	PC = Cast<AMotionControllerPC>(GetOwningPlayer());

	if (PC)
	{
		MyCharacter = Cast<AMotionControllerCharacter>(PC->GetPawn());

		if (MyCharacter)
		{
			MaxHP = MyCharacter->MaxHp; 
		}
	}

	AssignAnimations();		// 애니메이션을 찾음

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ULeftHandWidget::AutoGainStamina, 0.05f, false);		// 자동으로 스테미너 채우기
}

void ULeftHandWidget::AssignAnimations()
{
	UProperty* prop = GetClass()->PropertyLink;
	
	//	클래스의 모든 속성을 살펴보고 위젯 애니메이션 찾음
	while (prop != nullptr)
	{
		// 클래스를 찾음
		if (prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* objectProp = Cast<UObjectProperty>(prop);

			// 애니메이션 클래스를 찾음
			if (objectProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* object = objectProp->GetObjectPropertyValue_InContainer(this);		// 현재 objectProp에서 오브젝트를 object에 저장한다. 이 object는 UWidgetAnimation 클래스

				WidgetAnim = Cast<UWidgetAnimation>(object);		// object 캐스트
			}
		}
		prop = prop->PropertyLinkNext;		// 다음 속성을 찾아봄
	}
}

void ULeftHandWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 가상과 현재 변수가 존재하여, 먼저 가상변수를 증감시키고 현재변수를 증감변화량에 맞춰 가상변수까지 변화시킨다.

	// 스테미너 증감
	if (CurrentStamina != VirtualStamina)				// 현재와 가상스테미너가 다를 때 실행
	{
		ChangOfStamina = InDeltaTime * 2.0f;		// 스테미너증감 변화량을 설정
		if (CurrentStamina < VirtualStamina)			// 스테미너 증가
		{
			CurrentStamina += ChangOfStamina;	// 현재 스테미너 += 변화량
			State->SetPercent(CurrentStamina);		// 설정
			if (CurrentStamina > VirtualStamina)
			{
				CurrentStamina = VirtualStamina;   // 현재스테미너와 가상스테미너를 맞추기
			}				
		}
		else if (CurrentStamina > VirtualStamina)	// 스테미너 감소
		{
			CurrentStamina -= ChangOfStamina;		// 현재 스테미너 -= 변화량
			State->SetPercent(CurrentStamina);		// 설정
			if (CurrentStamina < VirtualStamina)
			{
				CurrentStamina = VirtualStamina; // 현재스테미너와 가상스테미너를 맞추기
			}				
		}
		else
		{
			ChangOfStamina = 0.0f;		// 스테미너 변화량 0.0
		}			
	}

	// 체력 증감
	if (CurrentHP != VirtualHP)			// 현재와 가상 HP가 다를 때 실행
	{
		ChangOfHP = InDeltaTime * 0.5f;		// HP증감 변화량을 설정
		if (CurrentHP < VirtualHP)				// HP 증가
		{
			CurrentHP += ChangOfHP;					// 현재 HP += 변화량
			BackwardHP->SetPercent(CurrentHP);	// 설정
			ForwardHP->SetPercent(CurrentHP);		// 설정
			if (CurrentHP > VirtualHP)					// 현재HP와 가상HP를 맞추기
				CurrentHP = VirtualHP;
		}
		else if (CurrentHP > VirtualHP)					// HP 감소(Forward HP바는 공격을 받고 이미 실행된 상태)
		{
			CurrentHP -= ChangOfHP;					// 현재 HP -= 변화량
			BackwardHP->SetPercent(CurrentHP);	// 설정
			if (CurrentHP < VirtualHP)					// 현재HP와 가상HP를 맞추기
				CurrentHP = VirtualHP;
		}
		else				// 평시
			ChangOfHP = 0.0f;			// 스테미너 변화량 0.0
	}
	else		// 체력감소가 일어나지 않을 때 애니메이션 정지
	{
		if (ULeftHandWidget::IsPlayingAnimation())
		{
			StopAnimations();
		}
	}
}

void ULeftHandWidget::StartAnimations()
{
	if (WidgetAnim)
	{
		ULeftHandWidget::PlayAnimation(WidgetAnim, 0.0f, 0);		// 애니메이션 실행
	}		
}

void ULeftHandWidget::StopAnimations()
{
	if (WidgetAnim)
	{
		ULeftHandWidget::StopAnimation(WidgetAnim);				// 애니메이션 종료
	}		
}

void ULeftHandWidget::ReceiveDamage(float _damage)
{
	VirtualHP -= _damage/100;			// 데미지를 얻어 가상체력을 감소시킨다.

	ForwardHP->SetPercent(VirtualHP);		// 가상HP설정
	StartAnimations();			// 체력감소가 시작 -> 애니메이션 시작
}

void ULeftHandWidget::GainHP(float _gainHP)
{
	VirtualHP += _gainHP/100;		// 체력 증가

	if (VirtualHP >= 1.0f)
		VirtualHP = 1.0f;

	InvisiblePotion();		// 포션 이미지 pop
}

void ULeftHandWidget::UseStamina(float _useValue)
{
	VirtualStamina -= _useValue/100;		// 스테미너 감소
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);			// 스테미너 사용 동작은 잠시 스테미너 회복을 멈춤

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ULeftHandWidget::AutoGainStamina, 3.0f, false);		// 그리고 바로 3초후 예약(스테미너 자동 회복) 
}

void ULeftHandWidget::GainStamina(float _gainValue)
{
	VirtualStamina += _gainValue/100;			// 스테미너 증가
}

void ULeftHandWidget::InvisiblePotion()
{
	if (PotionArray.IsValidIndex(0))
	{
		PotionArray.Pop()->SetVisibility(ESlateVisibility::Hidden);			// 포션 이미지 pop
	}		
}

void ULeftHandWidget::AutoGainStamina()
{
	if (CurrentStamina < 1.0f)			// 스테미너가 Full인 상태가 아닐 때만 실행
	{
		GainStamina(0.05);				// 수치만큼 스테미너 증가

		if (VirtualStamina > 1.0f)
		{
			// 다 차게 되면 스테미너를 Full과 맞춰줌
			VirtualStamina = 1.0f;
		}	

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ULeftHandWidget::AutoGainStamina, 0.01f, false);			// 계속 스테미너 증가
	}
	// GetWorld()->GetTimerManager().ClearTimer(th);
}





