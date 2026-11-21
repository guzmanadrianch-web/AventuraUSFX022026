#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjetoRecogible.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;
class AAventuraUSFX022026Pawn;

// Producto base de Factory Method. El Pawn del proyecto tiene MoveSpeed y FireRate.
UCLASS(Abstract)
class AVENTURAUSFX022026_API AObjetoRecogible : public AActor
{
    GENERATED_BODY()

public:
    AObjetoRecogible();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Recogible")
    USphereComponent* ZonaRecogida;

    UPROPERTY(VisibleAnywhere, Category = "Recogible")
    UStaticMeshComponent* Malla;

    // Efecto especializado por producto concreto.
    virtual bool AplicarEfecto(AAventuraUSFX022026Pawn* Jugador);

    UFUNCTION()
    void AlEntrarJugador(UPrimitiveComponent* ComponenteSolapado,
        AActor* OtroActor, UPrimitiveComponent* OtroComponente,
        int32 IndiceCuerpo, bool bDesdeBarrido, const FHitResult& Barrido);
};
