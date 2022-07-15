#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FVector StartLine { GetComponentLocation() };
	const FVector EndLine { StartLine + GetForwardVector() * MaxGrabDistance };
	DrawDebugLine(GetWorld(), StartLine, EndLine, FColor::Cyan);

	FCollisionShape Sphere { FCollisionShape::MakeSphere(GrabRadius) };
	FHitResult HitResult {};
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLine, EndLine,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere);
}


