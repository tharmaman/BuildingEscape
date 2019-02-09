// Copyright Daniel Tharmathurai 2016

#include "OpenDoor.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// setting private variables
    Owner = GetOwner();
	ActorThatOpens = GetWorld() -> GetFirstPlayerController() -> GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// Poll the Trigger Volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
    {
        OpenDoor();
        LastDoorOpenTime = GetWorld() -> GetTimeSeconds();
    }

	// Check if it's time to close the door
	if (GetWorld() -> GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) CloseDoor();
}

// middleware
void UOpenDoor::OpenDoor(){
	// set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor(){
	// set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}



