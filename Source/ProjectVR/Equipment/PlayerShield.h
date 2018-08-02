// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerShield.generated.h"

UCLASS()
class PROJECTVR_API APlayerShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USceneComponent* ShieldScene;				// �� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* ShieldMesh;			// �޽��� ���и� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UWidgetComponent* CharacterStateWidget;		// ĳ���� ���� ���� ( ���� / ü�� / ���׹̳� ) ������Ʈ

	UFUNCTION()
		void ConvertOfOpacity(float opacity);

	UPROPERTY()
		class UMaterialParameterCollectionInstance* CollectionInstance;
};
