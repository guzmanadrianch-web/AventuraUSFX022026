// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Compuerta.h"
#include "PlataformaTerrestre.generated.h"

/**
 * 
 */
class UBoxComponent;

UCLASS()
class AVENTURAUSFX022026_API APlataformaTerrestre : public ACompuerta
{
	GENERATED_BODY()
public:
	APlataformaTerrestre();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* ColisionBlock;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* ZonaOverLap;

    UFUNCTION()
    void AlEntrarZona(
        UPrimitiveComponent* componente,
        AActor* OtroActor,
        UPrimitiveComponent* OtroComponente,
        int32 OtroIndice,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    UFUNCTION()
    void AlSalirZona(
        UPrimitiveComponent* componente,
        AActor* OtroActor,
        UPrimitiveComponent* OtroComponente,
        int32 OtroIndice
    );
};
