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

void ULeftHandWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PortionBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("PotionBox")));				// ������������ 'PotionBox'�� �Ǿ��ִ� ������ ã�� ĳ��Ʈ ��Ų��.(HorizonBox)
	ForwardHP = Cast<UProgressBar>(GetWidgetFromName(TEXT("ForwardHPBar")));			// ������������ 'ForwardHPBar'�� �Ǿ��ִ� ������ ã�� ĳ��Ʈ ��Ų��.(ProgressBar)
	BackwardHP = Cast<UProgressBar>(GetWidgetFromName(TEXT("BackwardHPBar")));		// ������������ 'BackwardHPBar'�� �Ǿ��ִ� ������ ã�� ĳ��Ʈ ��Ų��.(ProgressBar)
	State = Cast<UProgressBar>(GetWidgetFromName(TEXT("StaminaBar")));						// ������������ 'StaminaBar'�� �Ǿ��ִ� ������ ã�� ĳ��Ʈ ��Ų��.(ProgressBar)
	ForwardHP->SetPercent(1);			// ü�� �� Full�� ����
	BackwardHP->SetPercent(1);		// ü�� �� Full�� ����
	State->SetPercent(1);					// ���׹̳� �� Full�� ����

	PotionArray.Shrink();		// �迭�� �߰� �� �޸� ����ȭ
				
	ChangOfHP = 0.0f;
	ChangOfStamina = 0.0f;
	Damage = 0.0f;

	AssignAnimations();		// �ִϸ��̼��� ã��

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ULeftHandWidget::AutoGainStamina, 0.05f, false);		// �ڵ����� ���׹̳� ä���
}

void ULeftHandWidget::AssignAnimations()
{
	UProperty* prop = GetClass()->PropertyLink;
	
	//	Ŭ������ ��� �Ӽ��� ���캸�� ���� �ִϸ��̼� ã��
	while (prop != nullptr)
	{
		// Ŭ������ ã��
		if (prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* objectProp = Cast<UObjectProperty>(prop);

			// �ִϸ��̼� Ŭ������ ã��
			if (objectProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* object = objectProp->GetObjectPropertyValue_InContainer(this);		// ���� objectProp���� ������Ʈ�� object�� �����Ѵ�. �� object�� UWidgetAnimation Ŭ����

				WidgetAnim = Cast<UWidgetAnimation>(object);		// object ĳ��Ʈ
			}
		}
		prop = prop->PropertyLinkNext;		// ���� �Ӽ��� ã�ƺ�
	}
}

void ULeftHandWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// ����� ���� ������ �����Ͽ�, ���� ���󺯼��� ������Ű�� ���纯���� ������ȭ���� ���� ���󺯼����� ��ȭ��Ų��.

	// ���׹̳� ����
	if (CurrentStamina != VirtualStamina)				// ����� �����׹̳ʰ� �ٸ� �� ����
	{
		ChangOfStamina = InDeltaTime * 2.0f;		// ���׹̳����� ��ȭ���� ����
		if (CurrentStamina < VirtualStamina)			// ���׹̳� ����
		{
			CurrentStamina += ChangOfStamina;	// ���� ���׹̳� += ��ȭ��
			State->SetPercent(CurrentStamina);		// ����
			if (CurrentStamina > VirtualStamina)		// ���罺�׹̳ʿ� �����׹̳ʸ� ���߱�
				CurrentStamina = VirtualStamina;
		}
		else if (CurrentStamina > VirtualStamina)	// ���׹̳� ����
		{
			CurrentStamina -= ChangOfStamina;		// ���� ���׹̳� -= ��ȭ��
			State->SetPercent(CurrentStamina);		// ����
			if (CurrentStamina < VirtualStamina)		// ���罺�׹̳ʿ� �����׹̳ʸ� ���߱�
				CurrentStamina = VirtualStamina;
		}
		else		// ���
			ChangOfStamina = 0.0f;		// ���׹̳� ��ȭ�� 0.0
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
	else		// ü�°��Ұ� �Ͼ�� ���� �� �ִϸ��̼� ����
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
		ULeftHandWidget::PlayAnimation(WidgetAnim, 0.0f, 0);		// �ִϸ��̼� ����
	}		
}

void ULeftHandWidget::StopAnimations()
{
	if (WidgetAnim)
	{
		ULeftHandWidget::StopAnimation(WidgetAnim);				// �ִϸ��̼� ����
	}		
}

void ULeftHandWidget::ReceiveDamage(float _damage)
{
	VirtualHP -= _damage/100;			// �������� ��� ����ü���� ���ҽ�Ų��.

	ForwardHP->SetPercent(VirtualHP);		// ����HP����
	StartAnimations();			// ü�°��Ұ� ���� -> �ִϸ��̼� ����
}

void ULeftHandWidget::GainHP(float _gainHP)
{
	VirtualHP += _gainHP/100;		// ü�� ����

	if (VirtualHP >= 1.0f)
		VirtualHP = 1.0f;

	InvisiblePotion();		// ���� �̹��� pop
}

void ULeftHandWidget::UseStamina(float _useValue)
{
	VirtualStamina -= _useValue/100;		// ���׹̳� ����
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);			// ���׹̳� ��� ������ ��� ���׹̳� ȸ���� ����

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ULeftHandWidget::AutoGainStamina, 3.0f, false);		// �׸��� �ٷ� 3���� ����(���׹̳� �ڵ� ȸ��) 
}

void ULeftHandWidget::GainStamina(float _gainValue)
{
	VirtualStamina += _gainValue/100;			// ���׹̳� ����
}

void ULeftHandWidget::InvisiblePotion()
{
	if (PotionArray.IsValidIndex(0))
	{
		PotionArray.Pop()->SetVisibility(ESlateVisibility::Hidden);			// ���� �̹��� pop
	}		
}

void ULeftHandWidget::AutoGainStamina()
{
	if (CurrentStamina < 1.0f)			// ���׹̳ʰ� Full�� ���°� �ƴ� ���� ����
	{
		GainStamina(0.05);				// ��ġ��ŭ ���׹̳� ����

		if (VirtualStamina > 1.0f)
		{
			// �� ���� �Ǹ� ���׹̳ʸ� Full�� ������
			VirtualStamina = 1.0f;
		}	

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ULeftHandWidget::AutoGainStamina, 0.01f, false);			// ��� ���׹̳� ����
	}
	// GetWorld()->GetTimerManager().ClearTimer(th);
}





