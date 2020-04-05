// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraActor.h"
#include "Engine/World.h"
#include "Curves/CurveFloat.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ArrowComponent.h"
#include "../Weapon/Gun.h"
#include "Components/TimelineComponent.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/Controller.h"
#include "Components/BoxComponent.h"
#include "Mannequin.generated.h"

UCLASS()
class RELOADEDBATTLE_API AMannequin : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AGun> BGun;



public:
	// Sets default values for this character's properties
	AMannequin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void UnPossessed() override;

	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
		) override;
	
	void Restart() override;

	void calcNewHealth(float damage);

	void SpawnDeathCamera();

	void killCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Controller")
	AController* OldController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Controller")
	UArrowComponent* DeathCamSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Stats")
	bool Dead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Stats")
	float Health = 100;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PullTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	class UTimelineComponent* DamageFade;

	void TimelineCallback(float interpolatedVal);
	void TimelineFinishedCallback();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	UCurveFloat* FloatCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	UBoxComponent* EffectVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	UPostProcessComponent* PostProcess;

	UPROPERTY()
	TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

	AGun* SpawnGun;
	ACameraActor* DeathCam;

};
