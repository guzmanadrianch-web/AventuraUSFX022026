// Fill out your copyright notice in the Description page of Project Settings.


#include "Compuerta.h"

// Sets default values
ACompuerta::ACompuerta()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CompuertaMallaAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));

	// Create mesh component for the projectile sphere
	mallaCompuerta = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompuertaMalla01"));
	mallaCompuerta->SetStaticMesh(CompuertaMallaAsset.Object);
	mallaCompuerta->SetupAttachment(RootComponent);
	RootComponent = mallaCompuerta;

	PosicionActual = FVector(0.0f, 0.0f, 0.0f);
    PosicionMinima = FVector(200.0f, 200.0f, 200.0f);
    PosicionMaxima = FVector(600.0f, 600.0f, 600.0f);
    Incremento = FVector(10.0f, 10.0f, 10.0f);
    Velocidad = 50.0f;
	VelocidadRotacion = 50.0f;
    comportamiento = 10;
    SignoIncremento = 1;

}

// Called when the game starts or when spawned
void ACompuerta::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACompuerta::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    
	PosicionActual = this->GetActorLocation();
    
    if (PosicionActual.Z >= PosicionMaxima.Z)
        SignoIncremento = -1;
    if (PosicionActual.Z <= PosicionMinima.Z)
        SignoIncremento = 1;

    FVector NuevaPosicion = PosicionActual;
    if (comportamiento == 0) // Arriba
    {
        NuevaPosicion.Z += Velocidad * DeltaTime;
    }
    else if (comportamiento == 1) // Abajo
    {
        NuevaPosicion.Z -= Velocidad * DeltaTime;
    }
    else if (comportamiento == 2) // Izquierda
    {
        NuevaPosicion.Y -= Velocidad * DeltaTime;
    }
    else if (comportamiento == 3) // Derecha
    {
        NuevaPosicion.Y += Velocidad * DeltaTime;
    }
    else if (comportamiento == 4) // Adelante
    {
        NuevaPosicion.X += Velocidad * DeltaTime;
    }
    else if (comportamiento == 5) // Atras
    {
        NuevaPosicion.X -= Velocidad * DeltaTime;
    }
    else if (comportamiento == 6) //RotarAdelante
    {
        FRotator RotacionActual = this->GetActorRotation();
        RotacionActual.Pitch += SignoIncremento * VelocidadRotacion * DeltaTime;
        this->SetActorRotation(RotacionActual);
    }
    else if (comportamiento == 7) //RotarAtras
    {
        FRotator RotacionActual = this->GetActorRotation();
        RotacionActual.Pitch -= SignoIncremento * VelocidadRotacion * DeltaTime;
        this->SetActorRotation(RotacionActual);
    }
    else if (comportamiento == 8) //RotarIzquierda
    {
        FRotator RotacionActual = this->GetActorRotation();
        RotacionActual.Yaw -= SignoIncremento * VelocidadRotacion * DeltaTime;
        this->SetActorRotation(RotacionActual);
    }
    else if (comportamiento == 9) //RotarDerecha
    {
        FRotator RotacionActual = this->GetActorRotation();
        RotacionActual.Yaw += SignoIncremento * VelocidadRotacion * DeltaTime;
        this->SetActorRotation(RotacionActual);
	}
    //NuevaPosicion.Y = NuevaPosicion.Y + (SignoIncremento * Incremento.Y * DeltaTime);
    //llamar a la funcion arriba
    //NuevaPosicion.Z = NuevaPosicion.Z + (SignoIncremento * Incremento.Z * DeltaTime);
    
    SetActorLocation(NuevaPosicion);

    /*
    // Crear una dirección aleatoria
    FVector DireccionAleatoria(
        FMath::FRandRange(-50.0f, 50.0f),
        FMath::FRandRange(-50.0f, 50.0f),
        0.0f
    );

    // Normalizar para mantener una velocidad aproximadamente constante
    DireccionAleatoria.Normalize();

    // Calcular nueva posición
    FVector NuevaPosicion =
        PosicionActual +
        DireccionAleatoria * Velocidad * DeltaTime;

    // Mover el actor
    SetActorLocation(NuevaPosicion);
    */
}

FVector ACompuerta::MArribaAbajo(FVector NuevaPosicion,float DeltaTime)
{
    NuevaPosicion.Z = NuevaPosicion.Z + (SignoIncremento * Incremento.Z * DeltaTime);
    return NuevaPosicion;
}

FVector ACompuerta::MDerechaIzquierda(FVector NuevaPosicion, float DeltaTime)
{
    NuevaPosicion.Y = NuevaPosicion.Y + (SignoIncremento * Incremento.Y * DeltaTime); //
    return NuevaPosicion;
}

FVector ACompuerta::MAdelanteAtras(FVector NuevaPosicion, float DeltaTime)
{
    NuevaPosicion.X = NuevaPosicion.X + (SignoIncremento * Incremento.X * DeltaTime); //
    return NuevaPosicion;
}

