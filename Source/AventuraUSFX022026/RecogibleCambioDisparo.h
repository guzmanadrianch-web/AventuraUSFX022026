#pragma once

#include "CoreMinimal.h"
#include "ObjetoRecogible.h"
#include "RecogibleCambioDisparo.generated.h"

// Producto Factory Method: adapta el power-up de Examen.rar
// a la interfaz AObjetoRecogible que ya existe en este proyecto.
UCLASS()
class AVENTURAUSFX022026_API ARecogibleCambioDisparo : public AObjetoRecogible
{
    GENERATED_BODY()

public:
    ARecogibleCambioDisparo();

protected:
    virtual bool AplicarEfecto(AAventuraUSFX022026Pawn* Jugador) override;
};
