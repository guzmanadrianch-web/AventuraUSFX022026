// Fill out your copyright notice in the Description page of Project Settings.


#include "Muro.h"

// Sets default values
AMuro::AMuro()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MuroMallaAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MuroMaterial(
		TEXT("Material'/Game/StarterContent/Materials/M_Basic_Floor.M_Basic_Floor'")
	);

	// Create mesh component for the projectile sphere
	mallaMuro = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MuroMalla01"));
	mallaMuro->SetRelativeScale3D(FVector(5.0f, 0.3f, 1.0f));
	mallaMuro->SetStaticMesh(MuroMallaAsset.Object);
	mallaMuro->SetMaterial(0, MuroMaterial.Object);
	mallaMuro->SetupAttachment(RootComponent);
	RootComponent = mallaMuro;

}

// Called when the game starts or when spawned
void AMuro::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMuro::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

