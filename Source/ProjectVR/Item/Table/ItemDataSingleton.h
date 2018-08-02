// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/Table/ItemDataTable.h"
#include "ItemDataSingleton.generated.h"

//아이템 테이블에 접근해주는 싱글톤 객체
UCLASS()
class PROJECTVR_API AItemDataSingleton : public AActor
{
	GENERATED_BODY()
private:	
	// Sets default values for this actor's properties
	AItemDataSingleton(); 
	static AItemDataSingleton* instance; //자기 자신을 담아둘 변수
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override; //틱은 사용하지 않으므로 꺼둔다.

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="DataTable")
	class UDataTable* DataTable; //에디터 상에 있는 데이터 테이블을 저장해둘 변수

	UFUNCTION()
		FItemDataTable& GetItemData(int ItemIndex); //외부에서 접근해서 인덱스값을 입력하면 해당하는 인덱스의 아이템의 정보를 반환해주는 함수

	void Create(); //싱글톤 객체 생성
	static AItemDataSingleton* GetInstatnce(); //싱글톤 객체 반환
	static void Destroy(); //싱글톤 객체 파괴
	
	
};
