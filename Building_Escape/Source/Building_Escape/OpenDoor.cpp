// Copyright David Tan 2021


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	FindPressurePlate();
	SetupAudioComponent();

	// ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > PressurePlateActivationMass)
	{
		// Activate trigger to open door
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else 
	{
		// If the door has been open longer than DoorCloseDelay, close it
		if (DoorLastOpened + DoorCloseDelay < GetWorld()->GetTimeSeconds())
		{
			CloseDoor(DeltaTime);
		}
	}
}


void UOpenDoor::OpenDoor(float DeltaTime) 
{
	// UE_LOG(LogTemp, Warning, TEXT("Transform: %s"), *GetOwner()->GetActorTransform().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw: %f"), GetOwner()->GetActorRotation().Yaw);

	FRotator DoorRotation = GetOwner()->GetActorRotation();
	// Change Yaw of DoorRotation
	// DoorRotation.Yaw = FMath::Lerp(CurrentYaw, FinalYaw, 0.02f);  // Exponential change but not framerate independent; see below code
	// DoorRotation.Yaw = FMath::FInterpConstantTo(CurrentYaw, FinalYaw, DeltaTime, 45.0f);  // Linear change and framework independent
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed);  // Exponential change and framerate independent
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	bCloseDoorSoundPlayed = false;

	if (!AudioComponent) {return;}
	if (!bOpenDoorSoundPlayed)
	{
		AudioComponent->Play();
		bOpenDoorSoundPlayed = true;
	}
}


void UOpenDoor::CloseDoor(float DeltaTime) 
{
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	bOpenDoorSoundPlayed = false;

	if (!AudioComponent) {return;}
	if (!bCloseDoorSoundPlayed)
	{	
		AudioComponent->Play();
		bCloseDoorSoundPlayed = true;
	}
}


float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.0f;

	// Find all overlapping Actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) {return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Sum their masses
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}


void UOpenDoor::SetupAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing audio component!"), *(AudioComponent->GetName()));
	}
}


void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component, but no Pressure Plate is set!"), *GetOwner()->GetName());
	}
}

