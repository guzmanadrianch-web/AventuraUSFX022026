#pragma once

#include "CoreMinimal.h"
#include "ObjetoRecogible.h"
#include "RecogibleCadencia.generated.h"

UCLASS()
class AVENTURAUSFX022026_API ARecogibleCadencia : public AObjetoRecogible
{
    GENERATED_BODY()

public:
    ARecogibleCadencia();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Recogible|Efecto")
    float ReduccionSegundos;

    virtual bool AplicarEfecto(AAventuraUSFX022026Pawn* Jugador) override;
};
