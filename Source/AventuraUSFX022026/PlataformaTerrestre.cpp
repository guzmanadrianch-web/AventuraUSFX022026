// Fill out your copyright notice in the Description page of Project Settings.


#include "PlataformaTerrestre.h"
#include "Components/BoxComponent.h"

APlataformaTerrestre::APlataformaTerrestre()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CompuertaMallaAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	mallaCompuerta->SetStaticMesh(CompuertaMallaAsset.Object);
	// Create mesh component for the projectile sphere
	PosicionActual = FVector(0.0f, 0.0f, 0.0f);
	PosicionMinima = FVector(200.0f, 200.0f, 200.0f);
	PosicionMaxima = FVector(600.0f, 600.0f, 600.0f);
	Incremento = FVector(10.0f, 10.0f, 10.0f);
	Velocidad = 50.0f;
	VelocidadRotacion = 50.0f;
	comportamiento = 10;
	SignoIncremento = 1;

	ColisionBlock = CreateDefaultSubobject<UBoxComponent>(TEXT("ColisionBlock"));
	ColisionBlock->SetupAttachment(mallaCompuerta);
	ColisionBlock->SetBoxExtent(FVector(60.0f, 60.0f, 60.0f));
	ColisionBlock->SetCollisionProfileName(TEXT("BlockAll"));

	ZonaOverLap = CreateDefaultSubobject<UBoxComponent>(TEXT("ZonaOverLap"));
	ZonaOverLap->SetupAttachment(mallaCompuerta);
	ZonaOverLap->SetBoxExtent(FVector(300.0f, 300.0f, 300.0f));
	ZonaOverLap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ZonaOverLap->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ZonaOverLap->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ZonaOverLap->OnComponentBeginOverlap.AddDynamic(this, &APlataformaTerrestre::AlEntrarZona);
	ZonaOverLap->OnComponentEndOverlap.AddDynamic(this, &APlataformaTerrestre::AlSalirZona);

}

void APlataformaTerrestre::AlEntrarZona(UPrimitiveComponent* componente, AActor* OtroActor, UPrimitiveComponent* OtroComponente, int32 OtroIndice, bool bFromSweep, const FHitResult& SweepResult)
{
}

void APlataformaTerrestre::AlSalirZona(UPrimitiveComponent* componente, AActor* OtroActor, UPrimitiveComponent* OtroComponente, int32 OtroIndice)
{
}


