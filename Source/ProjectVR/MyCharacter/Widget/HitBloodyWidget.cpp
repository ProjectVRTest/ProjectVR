// Fill out your copyright notice in the Description page of Project Settings.

#include "HitBloodyWidget.h"
#include "MyCharacter/MotionControllerCharacter.h"
#include "MyCharacter/MotionControllerPC.h"
#include "Animation/WidgetAnimation.h"
#include "Public/MovieScene.h"

#include "Components/ProgressBar.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"						// For Player

void UHitBloodyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ForwardHP = Cast<UProgressBar>(GetWidgetFromName(TEXT("ForwardHPBar")));			// 유저위젯상의 'ForwardHPBar'로 되어있는 변수를 찾아 캐스트 시킨다.(ProgressBar)
	BackwardHP = Cast<UProgressBar>(GetWidgetFromName(TEXT("BackwardHPBar")));		// 유저위젯상의 'BackwardHPBar'로 되어있는 변수를 찾아 캐스트 시킨다.(ProgressBar)
	State = Cast<UProgressBar>(GetWidgetFromName(TEXT("StaminaBar")));						// 유저위젯상의 'StaminaBar'로 되어있는 변수를 찾아 캐스트 시킨다.(ProgressBar)

	ForwardHP->SetPercent(1.0f);			// 앞 체력 바 Full로 설정
	BackwardHP->SetPercent(1.0f);			// 뒤 체력 바 Full로 설정
	State->SetPercent(1.0f);				// 스테미너 바 Full로 설정

	ChangOfHP = 0.0f;			// HP 변화량
	ChangOfStamina = 0.0f;		// Stemina 변화량
	Damage = 0.0f;				// 데미지

	PC = Cast<AMotionControllerPC>(GetOwningPlayer());			// 게임의 플레이어컨트롤러클래스를 불러옴	
	if (PC)
	{
		MyCharacter = Cast<AMotionControllerCharacter>(PC->GetPawn());		// 게임의 폰을 캐스트함. 체력 / 스테미너 정보를 불러올거임
		if (MyCharacter)
		{
			CurrentHP = MyCharacter->CurrentHp / MyCharacter->MaxHp;
			MaxHP = CurrentHP;

			CurrentStamina = MyCharacter->CurrentStamina / MyCharacter->MaxStamina;
			MaxStamina = CurrentStamina;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(AutoTimerHandle, this, &UHitBloodyWidget::AutoGainStamina, 0.05f, false);		// 자동으로 스테미너 채우기

	FillAnimationsMap();
}

void UHitBloodyWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	VirtualHP = MyCharacter->CurrentHp / MyCharacter->MaxHp;

	if (VirtualHP >= 1.0f)	VirtualHP = 1.0f;

	VirtualStamina = MyCharacter->CurrentStamina / MyCharacter->MaxStamina;

	ForwardHP->SetPercent(VirtualHP);

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
}

UWidgetAnimation * UHitBloodyWidget::GetAnimationByName(FName AnimationName) const			// 애니메이션 찾기
{
	UWidgetAnimation* const* WidgetAnim = AnimationsMap.Find(AnimationName);		// 이름으로 애니메이션 찾기
	if (WidgetAnim)
	{
		return *WidgetAnim;		// 찾은 애니메이션 반환
	}
	return nullptr;
}

bool UHitBloodyWidget::PlayAnimationByName(FName AnimationName, float StartAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed)		// 애니메이션 실행
{
	UWidgetAnimation* WidgetAnim = GetAnimationByName(AnimationName);		// 이름으로 찾은 애니메이션 저장
	if (WidgetAnim)
	{
		PlayAnimation(WidgetAnim, StartAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed);		// 애니메이션 실행
		return true;
	}
	return false;
}

void UHitBloodyWidget::FillAnimationsMap()			// 위젯 애니메이션 탐색
{
	AnimationsMap.Empty();

	UProperty* Prop = GetClass()->PropertyLink;

	// Run through all properties of this class to find any widget animations
	while (Prop != nullptr)
	{
		// Only interested in object properties
		if (Prop->GetClass() == UObjectProperty::StaticClass())			// UObjectProperty형식의 클래스 찾기
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			if (ObjProp)
			{
				// Only want the properties that are widget animations
				if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())		// UWidgetAnimation형식의 클래스 찾기
				{
					UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);

					if (Obj)
					{
						UWidgetAnimation* WidgetAnim = Cast<UWidgetAnimation>(Obj);			// 찾은 애니메이션을 함수를 사용할 수 있도록 함

						if (WidgetAnim != nullptr && WidgetAnim->MovieScene != nullptr)
						{
							FName AnimName = WidgetAnim->MovieScene->GetFName();			// 위젯의 애니메이션 이름 저장
							AnimationsMap.Add(AnimName, WidgetAnim);						// 이름을 배열에 저장
						}
					}
				}
			}			
		}

		Prop = Prop->PropertyLinkNext;		// 다음 탐색
	}
}


void UHitBloodyWidget::GetDamage(float damage)
{
	VirtualHP -= damage / 100;			// 데미지를 얻어 가상체력을 감소시킨다.

	ForwardHP->SetPercent(VirtualHP);		// 가상HP설정
}

void UHitBloodyWidget::RecoveryHP(float recoveryHP)
{
	VirtualHP += recoveryHP / 100;		// 체력 증가

	if (VirtualHP >= 1.0f)
		VirtualHP = 1.0f;
}

void UHitBloodyWidget::UseStamina(float value)
{
	VirtualStamina -= value / 100;		// 스테미너 감소
	GetWorld()->GetTimerManager().ClearTimer(AutoTimerHandle);			// 스테미너 사용 동작은 잠시 스테미너 회복을 멈춤

	GetWorld()->GetTimerManager().SetTimer(AutoTimerHandle, this, &UHitBloodyWidget::AutoGainStamina, 3.0f, false);		// 그리고 바로 3초후 예약(스테미너 자동 회복) 
}

void UHitBloodyWidget::RecoveryStamina(float value)
{
	VirtualStamina += value / 100;			// 스테미너 증가
}

void UHitBloodyWidget::AutoGainStamina()
{
	if (CurrentStamina < 1.0f)			// 스테미너가 Full인 상태가 아닐 때만 실행
	{
		RecoveryStamina(0.05);				// 수치만큼 스테미너 증가

		if (VirtualStamina > 1.0f)
		{
			// 다 차게 되면 스테미너를 Full과 맞춰줌
			VirtualStamina = 1.0f;
		}

		GetWorld()->GetTimerManager().SetTimer(AutoTimerHandle, this, &UHitBloodyWidget::AutoGainStamina, 0.01f, false);			// 계속 스테미너 증가
	}
}
