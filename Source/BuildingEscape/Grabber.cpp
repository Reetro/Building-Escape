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

  FVector PlayerViewPointLocation;
  FRotator PlayerViewPointRotation;
  GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation
  );

  FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

  if (PhysicsHandle->GrabbedComponent)
  {
    PhysicsHandle->SetTargetLocation(LineTraceEnd);
  }
}

void UGrabber::Grab() {
  UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))

    /// LINE TRACE and see if we reach any actors with physics body collision channel set
  auto HitResult = GetFirstPhysicsBodyInReach();
  auto ComponentToGrab = HitResult.GetComponent();
  auto ActorHit = HitResult.GetActor();

  /// If we hit something then attach a physics handle
  if (ActorHit)
  {
    // attach physics handle
    PhysicsHandle->GrabComponent(
      ComponentToGrab,
      NAME_None,
      ComponentToGrab->GetOwner()->GetActorLocation(),
      true
    );
  }
}

void UGrabber::Released()
{
	UE_LOG(LogTemp, Log, TEXT("You release the object"));
  PhysicsHandle->ReleaseComponent();
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

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
  /// Get player view point this tick
  FVector PlayerViewPointLocation;
  FRotator PlayerViewPointRotation;
  GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation
  );

  FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

  /// Setup query parameters
  FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

  /// Line-trace (AKA ray-cast) out to reach distance
  FHitResult Hit;
  GetWorld()->LineTraceSingleByObjectType(
    OUT Hit,
    PlayerViewPointLocation,
    LineTraceEnd,
    FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
    TraceParameters
  );

  /// See what what we hit
  AActor* ActorHit = Hit.GetActor();
  if (ActorHit)
  {
    UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
  }

  return Hit;
}
