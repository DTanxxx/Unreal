// Copyright David Tan 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void SetupAudioComponent();
	void FindPressurePlate();

private:
	float InitialYaw;
	float CurrentYaw;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.0f;

	float DoorLastOpened = 0.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.5f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 8.0f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;	

	UPROPERTY(EditAnywhere)
	float PressurePlateActivationMass = 60.0f;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	bool bOpenDoorSoundPlayed = true;
	bool bCloseDoorSoundPlayed = true;

	// UPROPERTY(EditAnywhere)
	// AActor* ActorThatOpens = nullptr;
};

