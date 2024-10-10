// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"

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

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;

		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Release"));

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		
		PhysicsHandle->ReleaseComponent();
		GrabbedActor->Tags.Remove("Grabbed");
	}
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	
	FHitResult HitResult;
	
	bool IsHit = GetGrabbableInReach(HitResult);
	
	if (IsHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();

		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		
		AActor* HitActor = HitResult.GetActor();
		
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
			);
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	
	if (result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requires UPhysicsHandleComponent"));
	}
	
	return result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	UWorld* World = GetWorld();
	
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	DrawDebugLine(World, Start, End, FColor::Red);
	DrawDebugSphere(World, End, 10,10 , FColor::Blue, false, 5);

	return World->SweepSingleByChannel(
			OutHitResult,
			Start,
			End,
			FQuat::Identity,
			ECC_GameTraceChannel2,
			Sphere);	
}



