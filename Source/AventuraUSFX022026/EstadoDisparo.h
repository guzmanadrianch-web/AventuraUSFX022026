#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EstadoDisparo.generated.h"

class AAventuraUSFX022026Pawn;

// STATE: comportamiento intercambiable del arma del Pawn.
// UObject en vez de AActor: no necesita posicion ni Tick en el escenario.
UCLASS(Abstract)
class AVENTURAUSFX022026_API UEstadoDisparo : public UObject
{
    GENERATED_BODY()

public:
    // true si se creo al menos un proyectil y debe comenzar el enfriamiento.
    virtual bool EjecutarDisparo(AAventuraUSFX022026Pawn* Nave,
        const FVector& Direccion) const;
};
