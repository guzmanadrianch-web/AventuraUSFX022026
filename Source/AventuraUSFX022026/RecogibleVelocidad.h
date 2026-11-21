#pragma once

#include "CoreMinimal.h"
#include "ObjetoRecogible.h"
#include "RecogibleVelocidad.generated.h"

UCLASS()
class AVENTURAUSFX022026_API ARecogibleVelocidad : public AObjetoRecogible
{
    GENERATED_BODY()

public:
    ARecogibleVelocidad();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Recogible|Efecto")
    float VelocidadExtra;

    virtual bool AplicarEfecto(AAventuraUSFX022026Pawn* Jugador) override;
};
