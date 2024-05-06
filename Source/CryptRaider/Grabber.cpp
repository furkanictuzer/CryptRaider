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

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//Grab();
	// ...
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Release"));
}

void UGrabber::Grab()
{
	UWorld* World=GetWorld();
	
	FHitResult OutHit;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() + Start;

	if (bool IsHit= World->SweepSingleByChannel(
		OutHit,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	))
	{
		FString Name = OutHit.GetActor()->GetName();
		UE_LOG(LogTemp, Display, TEXT("Hit actor: %s"), *Name);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No hit."));
	}
}



