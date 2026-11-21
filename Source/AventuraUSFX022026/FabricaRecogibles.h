#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "FabricaRecogibles.generated.h"

class AObjetoRecogible;
class UWorld;

// Creator GoF: el algoritmo de creacion es comun; el FACTORY METHOD
// ObtenerClaseRecogible() es sobrescrito por cada creador concreto.
UCLASS(Abstract)
class AVENTURAUSFX022026_API UFabricaRecogible : public UObject
{
    GENERATED_BODY()

public:
    AObjetoRecogible* CrearRecogible(UWorld* Mundo, const FVector& Posicion,
        const FRotator& Rotacion) const;

protected:
    virtual TSubclassOf<AObjetoRecogible> ObtenerClaseRecogible() const;
};

UCLASS()
class AVENTURAUSFX022026_API UFabricaRecogibleVelocidad : public UFabricaRecogible
{
    GENERATED_BODY()

protected:
    virtual TSubclassOf<AObjetoRecogible> ObtenerClaseRecogible() const override;
};

UCLASS()
class AVENTURAUSFX022026_API UFabricaRecogibleCadencia : public UFabricaRecogible
{
    GENERATED_BODY()

protected:
    virtual TSubclassOf<AObjetoRecogible> ObtenerClaseRecogible() const override;
};

// Nuevo creador concreto: el power-up State es un recogible de Factory Method.
UCLASS()
class AVENTURAUSFX022026_API UFabricaRecogibleCambioDisparo : public UFabricaRecogible
{
    GENERATED_BODY()

protected:
    virtual TSubclassOf<AObjetoRecogible> ObtenerClaseRecogible() const override;
};
