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
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle { GetPhysicsHandle() };
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation { GetComponentLocation() + GetForwardVector() * HoldDistance };
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}	
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle { GetPhysicsHandle() };
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	
	FVector StartLine { GetComponentLocation() };
	FVector EndLine { StartLine + GetForwardVector() * MaxGrabDistance };
	DrawDebugLine(GetWorld(), StartLine, EndLine, FColor::Red);
	DrawDebugSphere(GetWorld(), EndLine, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere { FCollisionShape::MakeSphere(GrabRadius) };
	FHitResult HitResult {};
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLine, EndLine,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere);

	if (HasHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green, false, 5);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);
		UPrimitiveComponent* HitComponent { HitResult.GetComponent() };
		HitComponent->WakeAllRigidBodies();
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()			
		);
	}
	UE_LOG(LogTemp, Display, TEXT("Grab Grabber"));
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle { GetPhysicsHandle() };
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		PhysicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result { GetOwner()->FindComponentByClass<UPhysicsHandleComponent>() };
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	return Result;
}


