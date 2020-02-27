// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "../ActorPool.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Kismet/GameplayStatics.h"
#include "../InfiniteTerrainGameMode.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NavigationBoundsOffset = FVector(2000, 0, 0);
	MinExtend = FVector(100, -2000, 0);
	MaxExtend = FVector(4000, 2000, 0);

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	
	GameModeBase = UGameplayStatics::GetGameMode(GetWorld());
	Instance = Cast<AInfiniteTerrainGameMode>(GameModeBase);
	/*if (Instance != nullptr) {
		ActorSize = Instance->GetRadiusSize();
		Instance->ActorSize
	}*/
	UE_LOG(LogTemp, Warning, TEXT("Begin Play"))

	Super::BeginPlay();

}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (Pool != nullptr && NavMeshBoundsVolume != nullptr) {
		Pool->Return(NavMeshBoundsVolume);
	}
	Super::EndPlay(EndPlayReason);
	
}

void ATile::SetPool(UActorPool* InPool) {
	
	Pool = InPool; 

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume == nullptr) {
		return;
	}
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation()+NavigationBoundsOffset);
	FNavigationSystem::Build(*GetWorld());
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int spawnMin, int spawnMax, float scaleMin, float scaleMax) {
	calculateRadius(ToSpawn);
	RandomlyPlaceActors(ToSpawn, spawnMin, spawnMax, scaleMin, scaleMax);
}

void ATile::PlaceAIPawns(TSubclassOf<AActor> ToSpawn, int spawnNumber) {
	if (!Instance->ActorSize.Contains(ToSpawn)) {
		Instance->ActorSize.Add(ToSpawn, 50);
	}
	RandomlyPlaceActors(ToSpawn, spawnNumber, spawnNumber, 1, 1);
	}


template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int spawnMin, int spawnMax, float scaleMin, float scaleMax) {

	int spawnNumber = FMath::RandRange(spawnMin, spawnMax);
	FSpawnPosition SpawnPosition;

	for (int i = 0; i < spawnNumber; i++) {
		SpawnPosition.Scale = FMath::RandRange(scaleMin, scaleMax);
		bool found; 
		if (ActorSize.Num() != 0) {
			found = FindEmptyLocation(ActorSize[ToSpawn], SpawnPosition.Scale, SpawnPosition.Location);
		}
		else { 
			found = FindEmptyLocation(100, SpawnPosition.Scale, SpawnPosition.Location); 
		}
		if (found) {
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

void ATile::calculateRadius(TSubclassOf<AActor> ToSpawn) {
	
	if (!Instance->ActorSize.Contains(ToSpawn)) {
		FVector BoundsExtent;
		FVector Origin;
		AActor* TestObject = GetWorld()->SpawnActor(ToSpawn);
		TestObject->GetActorBounds(false, Origin, BoundsExtent);
		TestObject->Destroy();
		BoundsExtent.Z = 0;
		float ImprovedRadii = BoundsExtent.Size();
		float Radius = ImprovedRadii * 1.1; 
		Instance->ActorSize.Add(ToSpawn, Radius);
		UE_LOG(LogTemp, Warning, TEXT("Actorsize Calculated"))
	}
}

bool ATile::FindEmptyLocation(float Radius, float Scale, FVector& SpawnPoint){

	bool hasHit;
	for (int i = 0; i < 50; i++) {
		SpawnPoint = FMath::RandPointInBox(FBox(MinExtend, MaxExtend));
		hasHit = DoesCollide(SpawnPoint, Radius * Scale);
		if (hasHit != true) {
			return true;
		}
	}
	return !hasHit;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition){
	AActor* Spawned = GetWorld()->SpawnActor(ToSpawn);
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale, SpawnPosition.Scale, SpawnPosition.Scale));
	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->SetActorRelativeRotation(FRotator(0, FMath::RandRange(0, 360), 0));
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	//AllActors->Add(Spawned);
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition& SpawnPosition) {
	
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->SetActorRelativeRotation(FRotator(0, FMath::RandRange(0, 360), 0));
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SpawnDefaultController();
	Spawned->Tags.Add(FName("Guard"));
}

bool ATile::DoesCollide(FVector Location, float Radius) {
	FHitResult FHitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	bool hasHit = GetWorld()->SweepSingleByChannel(
		FHitResult,
		GlobalLocation,
		GlobalLocation,// +FVector(0.2,0.2,0.2),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius));

	//FColor resultColor = hasHit ? FColor(255, 0, 0) : FColor(0, 255, 0);

	return hasHit;
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

