#pragma once

#include "CoreMinimal.h"
#include "EstadoDisparo.h"
#include "EstadoDisparoNormal.generated.h"

UCLASS()
class AVENTURAUSFX022026_API UEstadoDisparoNormal : public UEstadoDisparo
{
    GENERATED_BODY()

public:
    virtual bool EjecutarDisparo(AAventuraUSFX022026Pawn* Nave,
        const FVector& Direccion) const override;
};
