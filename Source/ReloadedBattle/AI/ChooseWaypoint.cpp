// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseWaypoint.h"

EBTNodeResult::Type UChooseWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	// Get Patrol Points (here much broken down, can be in one line)
	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();

	if (!ensure(PatrolRoute)){return EBTNodeResult::Failed;}

	auto PatrolPoints = PatrolRoute->getPatrolPoints();
	if (PatrolPoints.Num() == 0){
		UE_LOG(LogTemp, Warning, TEXT("Guard %s is missing PatrolPoints!"))//,ControlledPawn*)
		return EBTNodeResult::Failed;
	}

	//SetNextWaypoint
	auto bbComp = OwnerComp.GetBlackboardComponent();
	auto Index = bbComp->GetValueAsInt(IndexKey.SelectedKeyName);

	bbComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	// TODO: Protect against empty patrol route

	// Cycle Index
	auto nextIndex = (Index + 1) % PatrolPoints.Num();
	bbComp->SetValueAsInt(IndexKey.SelectedKeyName, nextIndex);
	return EBTNodeResult::Succeeded;
};
