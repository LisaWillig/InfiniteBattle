// Fill out your copyright notice in the Description page of Project Settings.


#include "Mannequin.h"
AMannequin::AMannequin()
{

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetVisibility(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	if (BGun == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Gun blueprint missing."));
		return;
	}
	
	if (IsPlayerControlled()) {
		SpawnGun = GetWorld()->SpawnActor<AGun>(
			BGun,
			Mesh1P->GetSocketLocation(FName("GripPoint")),
			Mesh1P->GetSocketRotation(FName("GripPoint"))
			);
		SpawnGun->FP_AnimInstance = Mesh1P->GetAnimInstance();
		SpawnGun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		InputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);
	}
	else {
		SpawnGun = GetWorld()->SpawnActor<AGun>(
			BGun,
			GetMesh()->GetSocketLocation(FName("GripPoint")),
			GetMesh()->GetSocketRotation(FName("GripPoint"))
			);
		SpawnGun->TP_AnimInstance = GetMesh()->GetAnimInstance();
		SpawnGun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}

}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMannequin::UnPossessed() {
	
	if (SpawnGun != NULL) {
		SpawnGun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	Super::UnPossessed();
}
void AMannequin::PullTrigger()
{
	SpawnGun->OnFire();
}