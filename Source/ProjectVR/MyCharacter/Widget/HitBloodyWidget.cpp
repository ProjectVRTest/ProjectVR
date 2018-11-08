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


	PC = Cast<AMotionControllerPC>(GetOwningPlayer());			// ������ �÷��̾���Ʈ�ѷ�Ŭ������ �ҷ���	
	if (PC)
	{
		MyCharacter = Cast<AMotionControllerCharacter>(PC->GetPawn());		// ������ ���� ĳ��Ʈ��. ü�� / ���׹̳� ������ �ҷ��ð���
	}
	FillAnimationsMap();
}

void UHitBloodyWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	
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
