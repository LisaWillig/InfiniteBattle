// Fill out your copyright notice in the Description page of Project Settings.


#include "Grass.h"

UGrass::UGrass()
{
}


// Called when the game starts
void UGrass::BeginPlay()
{
	Super::BeginPlay();

	SpawnGrass();
}

void UGrass::SpawnGrass()
{
	for (size_t i = 0; i < SpawnCount; i++)
	{
		FVector Location = FMath::RandPointInBox(SpawningExtents);
		AddInstance(FTransform(Location));
	}
}