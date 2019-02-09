// Copyright Daniel Tharmathurai 2016

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

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

	// Look for attached Physics Handle
	PhysicsHandle = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// physics handle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *(GetOwner()->GetName()));
	}

	// Look for attached Input Component
	InputComponent = GetOwner() -> FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found"));

		// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *(GetOwner()->GetName()));
	}

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
				// annotating OUT
				OUT PlayerViewPointLocation,
				OUT PlayerViewPointRotation
			);

	// Log out to test
//	UE_LOG(LogTemp, Warning, TEXT("Locations: %s, Position: %s"),
//                *PlayerViewPointLocation.ToString(),
//                *PlayerViewPointRotation.ToString()
//	        );

	/// straight out line
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// Draw a red trace in the world to visualize
	DrawDebugLine(
	        GetWorld(),
	        PlayerViewPointLocation,
	        LineTraceEnd,
	        FColor(255, 0, 0),
	        false,
	        0.f,
	        0.f,
	        10.f
        );

	// Setup query parameters
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	// Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld() -> LineTraceSingleByObjectType(
				OUT Hit,
				PlayerViewPointLocation,
				LineTraceEnd,
				FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
				TraceParams
			);

	// See what we hit
	// Pointer
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
	}

}

// middleware here
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("%s is grabbing"), *(GetOwner()->GetName()));
}

// :: means we already know at compile time
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));

}


