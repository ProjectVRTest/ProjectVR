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

	ForwardHP = Cast<UProgressBar>(GetWidgetFromName(TEXT("ForwardHPBar")));			// ������������ 'ForwardHPBar'�� �Ǿ��ִ� ������ ã�� ĳ��Ʈ ��Ų��.(ProgressBar)
	BackwardHP = Cast<UProgressBar>(GetWidgetFromName(TEXT("BackwardHPBar")));		// ������������ 'BackwardHPBar'�� �Ǿ��ִ� ������ ã�� ĳ��Ʈ ��Ų��.(ProgressBar)
	State = Cast<UProgressBar>(GetWidgetFromName(TEXT("StaminaBar")));						// ������������ 'StaminaBar'�� �Ǿ��ִ� ������ ã�� ĳ��Ʈ ��Ų��.(ProgressBar)

	ForwardHP->SetPercent(1.0f);			// �� ü�� �� Full�� ����
	BackwardHP->SetPercent(1.0f);			// �� ü�� �� Full�� ����
	State->SetPercent(1.0f);				// ���׹̳� �� Full�� ����

	ChangOfHP = 0.0f;			// HP ��ȭ��
	ChangOfStamina = 0.0f;		// Stemina ��ȭ��
	Damage = 0.0f;				// ������

	PC = Cast<AMotionControllerPC>(GetOwningPlayer());			// ������ �÷��̾���Ʈ�ѷ�Ŭ������ �ҷ���	
	if (PC)
	{
		MyCharacter = Cast<AMotionControllerCharacter>(PC->GetPawn());		// ������ ���� ĳ��Ʈ��. ü�� / ���׹̳� ������ �ҷ��ð���
		if (MyCharacter)
		{
			CurrentHP = MyCharacter->CurrentHp / MyCharacter->MaxHp;
			MaxHP = CurrentHP;

			CurrentStamina = MyCharacter->CurrentStamina / MyCharacter->MaxStamina;
			MaxStamina = CurrentStamina;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(AutoTimerHandle, this, &UHitBloodyWidget::AutoGainStamina, 0.05f, false);		// �ڵ����� ���׹̳� ä���

	FillAnimationsMap();
}

void UHitBloodyWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	VirtualHP = MyCharacter->CurrentHp / MyCharacter->MaxHp;

	if (VirtualHP >= 1.0f)	VirtualHP = 1.0f;

	VirtualStamina = MyCharacter->CurrentStamina / MyCharacter->MaxStamina;

	ForwardHP->SetPercent(VirtualHP);

	// ����� ���� ������ �����Ͽ�, ���� ���󺯼��� ������Ű�� ���纯���� ������ȭ���� ���� ���󺯼����� ��ȭ��Ų��.

	// ���׹̳� ����
	if (CurrentStamina != VirtualStamina)				// ����� �����׹̳ʰ� �ٸ� �� ����
	{
		ChangOfStamina = InDeltaTime * 2.0f;		// ���׹̳����� ��ȭ���� ����
		if (CurrentStamina < VirtualStamina)			// ���׹̳� ����
		{
			CurrentStamina += ChangOfStamina;	// ���� ���׹̳� += ��ȭ��
			State->SetPercent(CurrentStamina);		// ����
			if (CurrentStamina > VirtualStamina)
			{
				CurrentStamina = VirtualStamina;   // ���罺�׹̳ʿ� �����׹̳ʸ� ���߱�
			}
		}
		else if (CurrentStamina > VirtualStamina)	// ���׹̳� ����
		{
			CurrentStamina -= ChangOfStamina;		// ���� ���׹̳� -= ��ȭ��
			State->SetPercent(CurrentStamina);		// ����
			if (CurrentStamina < VirtualStamina)
			{
				CurrentStamina = VirtualStamina; // ���罺�׹̳ʿ� �����׹̳ʸ� ���߱�
			}
		}
		else
		{
			ChangOfStamina = 0.0f;		// ���׹̳� ��ȭ�� 0.0
		}
	}


	// ü�� ����
	if (CurrentHP != VirtualHP)			// ����� ���� HP�� �ٸ� �� ����
	{
		ChangOfHP = InDeltaTime * 0.5f;		// HP���� ��ȭ���� ����
		if (CurrentHP < VirtualHP)				// HP ����
		{
			CurrentHP += ChangOfHP;					// ���� HP += ��ȭ��
			BackwardHP->SetPercent(CurrentHP);	// ����
			ForwardHP->SetPercent(CurrentHP);		// ����
			if (CurrentHP > VirtualHP)					// ����HP�� ����HP�� ���߱�
				CurrentHP = VirtualHP;
		}
		else if (CurrentHP > VirtualHP)					// HP ����(Forward HP�ٴ� ������ �ް� �̹� ����� ����)
		{
			CurrentHP -= ChangOfHP;					// ���� HP -= ��ȭ��
			BackwardHP->SetPercent(CurrentHP);	// ����
			if (CurrentHP < VirtualHP)					// ����HP�� ����HP�� ���߱�
				CurrentHP = VirtualHP;
		}
		else				// ���
			ChangOfHP = 0.0f;			// ���׹̳� ��ȭ�� 0.0
	}
}

UWidgetAnimation * UHitBloodyWidget::GetAnimationByName(FName AnimationName) const			// �ִϸ��̼� ã��
{
	UWidgetAnimation* const* WidgetAnim = AnimationsMap.Find(AnimationName);		// �̸����� �ִϸ��̼� ã��
	if (WidgetAnim)
	{
		return *WidgetAnim;		// ã�� �ִϸ��̼� ��ȯ
	}
	return nullptr;
}

bool UHitBloodyWidget::PlayAnimationByName(FName AnimationName, float StartAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed)		// �ִϸ��̼� ����
{
	UWidgetAnimation* WidgetAnim = GetAnimationByName(AnimationName);		// �̸����� ã�� �ִϸ��̼� ����
	if (WidgetAnim)
	{
		PlayAnimation(WidgetAnim, StartAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed);		// �ִϸ��̼� ����
		return true;
	}
	return false;
}

void UHitBloodyWidget::FillAnimationsMap()			// ���� �ִϸ��̼� Ž��
{
	AnimationsMap.Empty();

	UProperty* Prop = GetClass()->PropertyLink;

	// Run through all properties of this class to find any widget animations
	while (Prop != nullptr)
	{
		// Only interested in object properties
		if (Prop->GetClass() == UObjectProperty::StaticClass())			// UObjectProperty������ Ŭ���� ã��
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			if (ObjProp)
			{
				// Only want the properties that are widget animations
				if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())		// UWidgetAnimation������ Ŭ���� ã��
				{
					UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);

					if (Obj)
					{
						UWidgetAnimation* WidgetAnim = Cast<UWidgetAnimation>(Obj);			// ã�� �ִϸ��̼��� �Լ��� ����� �� �ֵ��� ��

						if (WidgetAnim != nullptr && WidgetAnim->MovieScene != nullptr)
						{
							FName AnimName = WidgetAnim->MovieScene->GetFName();			// ������ �ִϸ��̼� �̸� ����
							AnimationsMap.Add(AnimName, WidgetAnim);						// �̸��� �迭�� ����
						}
					}
				}
			}			
		}

		Prop = Prop->PropertyLinkNext;		// ���� Ž��
	}
}


void UHitBloodyWidget::GetDamage(float damage)
{
	VirtualHP -= damage / 100;			// �������� ��� ����ü���� ���ҽ�Ų��.

	ForwardHP->SetPercent(VirtualHP);		// ����HP����
}

void UHitBloodyWidget::RecoveryHP(float recoveryHP)
{
	VirtualHP += recoveryHP / 100;		// ü�� ����

	if (VirtualHP >= 1.0f)
		VirtualHP = 1.0f;
}

void UHitBloodyWidget::UseStamina(float value)
{
	VirtualStamina -= value / 100;		// ���׹̳� ����
	GetWorld()->GetTimerManager().ClearTimer(AutoTimerHandle);			// ���׹̳� ��� ������ ��� ���׹̳� ȸ���� ����

	GetWorld()->GetTimerManager().SetTimer(AutoTimerHandle, this, &UHitBloodyWidget::AutoGainStamina, 3.0f, false);		// �׸��� �ٷ� 3���� ����(���׹̳� �ڵ� ȸ��) 
}

void UHitBloodyWidget::RecoveryStamina(float value)
{
	VirtualStamina += value / 100;			// ���׹̳� ����
}

void UHitBloodyWidget::AutoGainStamina()
{
	if (CurrentStamina < 1.0f)			// ���׹̳ʰ� Full�� ���°� �ƴ� ���� ����
	{
		RecoveryStamina(0.05);				// ��ġ��ŭ ���׹̳� ����

		if (VirtualStamina > 1.0f)
		{
			// �� ���� �Ǹ� ���׹̳ʸ� Full�� ������
			VirtualStamina = 1.0f;
		}

		GetWorld()->GetTimerManager().SetTimer(AutoTimerHandle, this, &UHitBloodyWidget::AutoGainStamina, 0.01f, false);			// ��� ���׹̳� ����
	}
}
