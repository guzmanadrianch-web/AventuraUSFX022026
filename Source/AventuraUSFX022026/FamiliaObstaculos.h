#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FamiliaObstaculos.generated.h"

class UWorld;
class ACompuerta;
class AMuro;

// Abstract Factory independiente: NUNCA usa las fabricas de Factory Method.
UCLASS(Abstract)
class AVENTURAUSFX022026_API UFabricaFamiliaObstaculos : public UObject
{
    GENERATED_BODY()

public:
    virtual ACompuerta* CrearPlataforma(UWorld* Mundo, const FVector& Posicion,
        const FRotator& Rotacion) const;
    virtual AMuro* CrearMuro(UWorld* Mundo, const FVector& Posicion,
        const FRotator& Rotacion) const;
};

UCLASS()
class AVENTURAUSFX022026_API UFabricaFamiliaSolida : public UFabricaFamiliaObstaculos
{
    GENERATED_BODY()

public:
    virtual ACompuerta* CrearPlataforma(UWorld* Mundo, const FVector& Posicion,
        const FRotator& Rotacion) const override;
    virtual AMuro* CrearMuro(UWorld* Mundo, const FVector& Posicion,
        const FRotator& Rotacion) const override;
};

UCLASS()
class AVENTURAUSFX022026_API UFabricaFamiliaDestruible : public UFabricaFamiliaObstaculos
{
    GENERATED_BODY()

public:
    virtual ACompuerta* CrearPlataforma(UWorld* Mundo, const FVector& Posicion,
        const FRotator& Rotacion) const override;
    virtual AMuro* CrearMuro(UWorld* Mundo, const FVector& Posicion,
        const FRotator& Rotacion) const override;
};
