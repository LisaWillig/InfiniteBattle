// Fill out your copyright notice in the Description page of Project Settings.


#include "InfiniteTerrainGameMode.h"
#include "ReloadedBattleHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "ActorPool.h"
#include "EngineUtils.h"

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Character/BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AReloadedBattleHUD::StaticClass();

	NavMeshBoundsVolumePool = CreateDefaultSubobject<UActorPool>(FName("Nav Mesh Bounds Volume Pool"));
	AllActorPool = CreateDefaultSubobject<UActorPool>(FName("All Actor Pool"));

	ActorSize = TMap<TSubclassOf<class AActor>, float>();
}

void AInfiniteTerrainGameMode::PopulateBoundsVolumePool() {

	auto VolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());

	while (VolumeIterator) {
		AddToPool(*VolumeIterator);
		++VolumeIterator; // NOT ActorIterator++
	}


}

void AInfiniteTerrainGameMode::AddToPool(class ANavMeshBoundsVolume *VolumeToAdd) {
	NavMeshBoundsVolumePool->Add(VolumeToAdd);
}

TMap<TSubclassOf<AActor>, float> AInfiniteTerrainGameMode::GetRadiusSize() {
	return ActorSize;
}

void AInfiniteTerrainGameMode::AddRadiusSize(TSubclassOf<AActor> ToSpawn, float Radius) {
	ActorSize.Add(ToSpawn, Radius);
}


