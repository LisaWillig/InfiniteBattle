// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolRoute.h"

TArray<AActor*> UPatrolRoute::getPatrolPoints() const {
	return PatrolPointsArr;
}