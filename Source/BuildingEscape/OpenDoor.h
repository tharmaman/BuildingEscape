// Copyright Daniel Tharmathurai 2016

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameframework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
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

private:
    UPROPERTY(EditAnywhere)
    float OpenAngle = 0.0f;

	UPROPERTY(EditAnywhere)
	float ClosedAngle = -90.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

//	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens; // remember pawn inherits from actor

	AActor* Owner;	// owner of door

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.7f;

	float LastDoorOpenTime;

	// middleware
	void OpenDoor();
	void CloseDoor();
};