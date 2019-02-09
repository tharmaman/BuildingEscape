// Copyright Daniel Tharmathurai 2016

#include "Grabber.h"

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
	UGrabber::FindPhysicsHandleComponent();
	UGrabber::SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

//	// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
				// annotating OUT
				OUT PlayerViewPointLocation,
				OUT PlayerViewPointRotation
		);


	// straight out line
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// if the physics handle is attached
	if (PhysicsHandle && PhysicsHandle -> GrabbedComponent)
	{
		// move the object that we're holding
		 PhysicsHandle -> SetTargetLocation(LineTraceEnd);
	}
}

/**
 * Middleware starts here
 */
void UGrabber::FindPhysicsHandleComponent()
{
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

}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("%s is grabbing"), *(GetOwner()->GetName()));

	/// Line trace and see if we reach any actors with physics body collision channel set
	auto HitResult = UGrabber::GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit != nullptr)
	{
		/// attach physics handle
		PhysicsHandle->GrabComponentAtLocation(
					ComponentToGrab,
					NAME_None,
					ComponentToGrab -> GetOwner() -> GetActorLocation()
			);
	}
}

// :: means we already know at compile time
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
	// TODO release physics handle
	PhysicsHandle->ReleaseComponent();

}

void UGrabber::SetupInputComponent()
{
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

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(
				// annotating OUT
				OUT PlayerViewPointLocation,
				OUT PlayerViewPointRotation
		);


	// straight out line
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	// Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld() -> LineTraceSingleByObjectType(
				OUT Hit,
				PlayerViewPointLocation,
				LineTraceEnd,
				FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
				TraceParameters
		);

	// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
	}

	return Hit;
}


