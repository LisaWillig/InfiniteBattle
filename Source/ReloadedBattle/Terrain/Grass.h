// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Grass.generated.h"

/**
 * 
 */
UCLASS()
class RELOADEDBATTLE_API UGrass : public UHierarchicalInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
    UGrass();

    UPROPERTY(EditDefaultsOnly, Category = Spawning)
    FBox SpawningExtents;

    UPROPERTY(EditDefaultsOnly, Category = Spawning)
    int SpawnCount;

    // Called when the game starts
    virtual void BeginPlay() override;

private:
    void SpawnGrass();
};
