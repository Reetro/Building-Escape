// Fill out your copyright notice in the Description page of Project Settings.


#include "PostionReporter.h"

// Sets default values for this component's properties
UPostionReporter::UPostionReporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPostionReporter::BeginPlay()
{
	Super::BeginPlay();
	FString ObjectName = GetOwner()->GetName();
	FString ObjectPos = GetOwner()->GetTransform().GetLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("Objects name is %s"), *ObjectName);
	UE_LOG(LogTemp, Warning, TEXT("Objects postion is %s"), *ObjectPos);
}


// Called every frame
void UPostionReporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}