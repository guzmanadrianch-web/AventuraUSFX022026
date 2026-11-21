// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Compuerta.generated.h"

class UStaticMeshComponent;

UCLASS()
class AVENTURAUSFX022026_API ACompuerta : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACompuerta();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FVector MArribaAbajo(FVector NuevaPosicion,float DeltaTime);
	FVector MDerechaIzquierda(FVector NuevaPosicion,float DeltaTime);
	FVector MAdelanteAtras(FVector NuevaPosicion, float DeltaTime);
	FVector MAtras(FVector NuevaPosicion, float DeltaTime);
	

	UStaticMeshComponent* mallaCompuerta;

	FVector PosicionActual;
	FVector PosicionMaxima;
	FVector PosicionMinima;
	FVector Incremento;

	int32 comportamiento=10; // 0: Arriba, 1: Abajo, 2: Izquierda, 3: Derecha, 4: Adelante, 5: Atras, 6: RotarAdelante, 7: RotarAtras, 8: RotarIzquierda, 9: RotarDerecha
	
	float Velocidad;
	float VelocidadRotacion;

	float SignoIncremento;

};
