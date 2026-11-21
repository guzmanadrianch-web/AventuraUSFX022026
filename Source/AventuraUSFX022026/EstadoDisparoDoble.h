#pragma once

#include "CoreMinimal.h"
#include "EstadoDisparo.h"
#include "EstadoDisparoDoble.generated.h"

UCLASS()
class AVENTURAUSFX022026_API UEstadoDisparoDoble : public UEstadoDisparo
{
    GENERATED_BODY()

public:
    virtual bool EjecutarDisparo(AAventuraUSFX022026Pawn* Nave,
        const FVector& Direccion) const override;
};
