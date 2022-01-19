// Copyright David Tan 2021


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check if the physics handle has a grabbed component
	if (!PhysicsHandle) {return;}
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object we are holding 
		PhysicsHandle->SetTargetLocation(CalculatePlayerReachPoint());
	}
}


void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component Found on %s"), *(GetOwner()->GetName()));
	}
}


void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();  // UInputComponent is attached to Pawn objects by default, so no need for null check
	if (InputComponent)  // but we will do the check anyway
	{
		InputComponent->BindAction(FName(TEXT("Grab")), IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction(FName(TEXT("Grab")), IE_Released, this, &UGrabber::Release);
	}
}


void UGrabber::Grab()
{
	// Try to reach any Actors with a PhysicsBody 
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// If found something, attach component to physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle) {return;}
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab, 
			NAME_None, 
			CalculatePlayerReachPoint()
		);
	}
}


void UGrabber::Release()
{
	// Remove grabbed component from physics handle
	if (!PhysicsHandle) {return;}
	PhysicsHandle->ReleaseComponent();
}


FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector LineTraceEnd = CalculatePlayerReachPoint();
	FVector PlayerViewPointLocation = CalculatePlayerViewPoint().Key;

	// Draw a line from player showing player's reach
	// DrawDebugLine(
	// 	GetWorld(),
	// 	PlayerViewPointLocation,
	// 	LineTraceEnd,
	// 	FColor(0, 255, 0),
	// 	false,
	// 	0.0f,
	// 	0,
	// 	5.0f
	// );

	// Raycast out to a certain distance (Reach)
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit, 
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	); 

	return Hit;
}


FVector UGrabber::CalculatePlayerReachPoint() const
{
	FVector PlayerViewPointLocation = CalculatePlayerViewPoint().Key;
	FRotator PlayerViewPointRotation = CalculatePlayerViewPoint().Value;

	// Log the viewpoint info
	// UE_LOG(LogTemp, Warning, TEXT("Location: %s; Rotation: %s"), 
	// 	*(PlayerViewPointLocation.ToString()), 
	// 	*(PlayerViewPointRotation.ToString())
	// );

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}


TPair<FVector,FRotator> UGrabber::CalculatePlayerViewPoint() const
{
	// Get player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);
	
	return TPair<FVector,FRotator>(PlayerViewPointLocation, PlayerViewPointRotation);
}

