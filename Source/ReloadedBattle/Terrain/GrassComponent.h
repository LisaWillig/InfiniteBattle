// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GrassComponent.generated.h"

/**
 * 
 */
UCLASS()
class RELOADEDBATTLE_API UGrassComponent : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrassComponent();

	/*UPROPERTY(EditDefaultsOnly, Category = Spawning)
		FBox SpawningExtents;
	UPROPERTY(EditDefaultsOnly, Category = Spawning)
		int SpawnCount;
	*/
/*
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void SpawnGrass();
*/
};

