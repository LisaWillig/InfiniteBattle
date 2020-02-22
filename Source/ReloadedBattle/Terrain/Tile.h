// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class UActorPool; 

USTRUCT()
struct FSpawnPosition {
	GENERATED_USTRUCT_BODY()
	FVector Location;
	float Scale;
	float Rotation;
};

UCLASS()
class RELOADEDBATTLE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game starts or when spawned
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtend;
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtend;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Terrain")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, 
		int spawnMin = 0,
		int spawnMax = 1,
		float scaleMin = 0.9,
		float scaleMax = 1.1);

	template<class T>
	void RandomlyPlaceActors(TSubclassOf<T> ToSpawn,
		int spawnMin,
		int spawnMax,
		float scaleMin,
		float scaleMax,
		float Radius);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void PlaceAIPawns(TSubclassOf<AActor> ToSpawn,
		int spawnNumber = 0);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* InPool);


	void PositionNavMeshBoundsVolume();
	float calculateRadius(TSubclassOf<AActor> ToSpawn);

	UActorPool* Pool;
	AActor* NavMeshBoundsVolume;
	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset; 

	/*TArray<FSpawnPosition> RandomSpawnPositions(
		float Radius, 
		int spawnMin, 
		int spawnMax, 
		float scaleMin, 
		float scaleMax);*/

private:

	bool DoesCollide(FVector Location, float Radius);
	void PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnPosition& SpawnPosition);
	void PlaceActor(TSubclassOf<APawn> ToSpawn, const FSpawnPosition& SpawnPosition);
	bool FindEmptyLocation(float Radius, float Scale, FVector& SpawnPoint);
	

};
