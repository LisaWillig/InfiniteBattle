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

	//Create Death Cam Spawn
	DeathCamSpawn = CreateDefaultSubobject<UArrowComponent>(TEXT("DeathCamSpawn"));
	DeathCamSpawn->SetupAttachment(GetCapsuleComponent());

	DeathCam = CreateDefaultSubobject<ACameraActor>(TEXT("DeathCam"));
	
	EffectVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("EffectVolume"));
	EffectVolume->SetupAttachment(GetCapsuleComponent());
	EffectVolume->SetRelativeLocation(FVector(-10.f, 0.f, 0.f));
	EffectVolume->SetBoxExtent(FVector(50.0f, 42.0f, 100.f));

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcess->SetupAttachment(EffectVolume);
	PostProcess->bEnabled = true;
	PostProcess->BlendWeight = 0.0;
	//PostProcess->bUnbound = false;
	PostProcess->SetVisibility(true);

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
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCollision"));

	}
	else {
		SpawnGun = GetWorld()->SpawnActor<AGun>(
			BGun,
			GetMesh()->GetSocketLocation(FName("GripPoint")),
			GetMesh()->GetSocketRotation(FName("GripPoint"))
			);
		SpawnGun->TP_AnimInstance = GetMesh()->GetAnimInstance();
		SpawnGun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}

	FOnTimelineFloat onTimelineCallback;
	FOnTimelineEventStatic onTimelineFinishedCallback;

	if (FloatCurve)
	{
		DamageFade = NewObject<UTimelineComponent>(this, FName("TimelineAnimation"));
		DamageFade->CreationMethod = EComponentCreationMethod::UserConstructionScript; // Indicate it comes from a blueprint so it gets cleared when we rerun construction scripts
		this->BlueprintCreatedComponents.Add(DamageFade); // Add to array so it gets saved
		DamageFade->SetNetAddressable();	// This component has a stable name that can be referenced for replication

		DamageFade->SetPropertySetObject(this); // Set which object the timeline should drive properties on
		DamageFade->SetDirectionPropertyName(FName("TimelineDirection"));

		DamageFade->SetLooping(false);
		//DamageFade->SetTimelineLength(5.0f);
		DamageFade->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		DamageFade->SetPlaybackPosition(0.0f, false);
		//Add the float curve to the timeline and connect it to your timelines's interpolation function
		onTimelineCallback.BindUFunction(this, FName{ TEXT("TimelineCallback") });
		//onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });
		DamageFade->AddInterpFloat(FloatCurve, onTimelineCallback);
		//DamageFade->SetTimelineFinishedFunc(onTimelineFinishedCallback);
		DamageFade->RegisterComponent();
	}
}


void AMannequin::TimelineCallback(float interpolatedVal)
{
	//UE_LOG(LogTemp, Warning, TEXT("TEST"))
	// This function is called for every tick in the timeline.
}

void AMannequin::TimelineFinishedCallback()
{
	// This function is called when the timeline finishes playing.
}

float AMannequin::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	calcNewHealth(DamageAmount);
	DamageFade->PlayFromStart();
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return 0;
}

void AMannequin::killCharacter() {
	SpawnDeathCamera();
	OldController->UnPossess();
}

void AMannequin::SpawnDeathCamera() {
	APlayerController* view = this->GetController()->CastToPlayerController();
	if (!Cast<ACameraActor>(view->GetViewTarget())) {
		FTransform spawn = DeathCamSpawn->GetRelativeTransform();
		FActorSpawnParameters SpawnParams;
		auto death = GetWorld()->SpawnActor<ACameraActor>(DeathCam->GetClass(),
			DeathCamSpawn->GetRelativeLocation(),
			DeathCamSpawn->GetRelativeRotation(),
			SpawnParams
			);
		view->SetViewTargetWithBlend(death, 3.0f, EViewTargetBlendFunction::VTBlend_Linear, 0.f, false);
	}
}

void AMannequin::calcNewHealth(float damage) {
	Health -= damage;
	if (Health <= 0) {
		Dead = true;
		killCharacter();
	}
}

void AMannequin::Restart(){

	if (IsPlayerControlled()) {
		OldController = this->GetController();
		APlayerController* MyController = OldController->CastToPlayerController();
		MyController->SetViewTargetWithBlend(this); 
	}
	Super::Restart();

}
// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DamageFade != NULL)
	{
		DamageFade->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
		float currentVal = DamageFade->GetPlaybackPosition();
		if (PostProcess) {
			PostProcess->BlendWeight = currentVal;
			
		}
	}

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