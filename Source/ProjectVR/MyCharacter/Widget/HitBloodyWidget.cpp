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


	PC = Cast<AMotionControllerPC>(GetOwningPlayer());			// 게임의 플레이어컨트롤러클래스를 불러옴	
	if (PC)
	{
		MyCharacter = Cast<AMotionControllerCharacter>(PC->GetPawn());		// 게임의 폰을 캐스트함. 체력 / 스테미너 정보를 불러올거임
	}
	FillAnimationsMap();
}

void UHitBloodyWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	
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
