// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RELOADEDBATTLE_API AInfiniteTerrainGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AInfiniteTerrainGameMode();

	UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
	void PopulateBoundsVolumePool();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Pool")
	class UActorPool* NavMeshBoundsVolumePool; 
	class UActorPool* AllActorPool;

	TMap<TSubclassOf<class AActor>, float> ActorSize;

	TMap<TSubclassOf<class AActor>, float> GetRadiusSize();
	void AddRadiusSize(TSubclassOf<class AActor>, float);

private:
	void AddToPool(class ANavMeshBoundsVolume *VolumeToAdd);

};



