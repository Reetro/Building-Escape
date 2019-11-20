// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	/// Look for attached physics handler
  FindAttachedPhysicsComponent();

	/// Look for attached input component(Is created at runtime)
  FindAttachedInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Log, TEXT("You grabbed the object"));

  GetFirstPhysicsBodyInReach();
}

void UGrabber::Released()
{
	UE_LOG(LogTemp, Log, TEXT("You release the object"));
}

void UGrabber::FindAttachedPhysicsComponent()
{
  PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

  if (PhysicsHandle)
  {
    // Has Physics Handler	
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("%s does not have a physics handler"), *GetOwner()->GetName());
  }
}

void UGrabber::FindAttachedInputComponent()
{
  Input = GetOwner()->FindComponentByClass<UInputComponent>();

  if (Input)
  {
    Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
    Input->BindAction("Grab", IE_Released, this, &UGrabber::Released);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("%s does not have a Input Component"), *GetOwner()->GetName());
  }
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
  FVector PlayerViewPointLocation;
  FRotator PlayerViewPointRotation;

  GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation
  );

  FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

  /// Setup query parameters
  FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
  /// Linetrace for objects that are Physics bodies
  FHitResult Hit;
  GetWorld()->LineTraceSingleByObjectType(
    OUT Hit,
    PlayerViewPointLocation,
    LineTraceEnd,
    FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
    TraceParameters
  );

  AActor* ActorHit = Hit.GetActor();
  /// see if actor was if it was get it's name
  if (ActorHit)
  {
    UE_LOG(LogTemp, Log, TEXT("Objects name is %s"), *ActorHit->GetName());
  }

  return FHitResult();
}
