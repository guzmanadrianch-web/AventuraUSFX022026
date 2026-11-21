#pragma once

#include "CoreMinimal.h"
#include "EstadoDisparo.h"
#include "EstadoDisparoLaser.generated.h"

UCLASS()
class AVENTURAUSFX022026_API UEstadoDisparoLaser : public UEstadoDisparo
{
    GENERATED_BODY()

public:
    virtual bool EjecutarDisparo(AAventuraUSFX022026Pawn* Nave,
        const FVector& Direccion) const override;
};
