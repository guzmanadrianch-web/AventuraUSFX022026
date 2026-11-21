#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GestorOleadas.generated.h"

class AOleada;
class AEnemigoJefe;

// Observer: administra una oleada YA CONSTRUIDA y escucha al jefe.
// No fabrica enemigos, ni configura el Builder.
UCLASS()
class AVENTURAUSFX022026_API AGestorOleadas : public AActor
{
    GENERATED_BODY()

public:
    AGestorOleadas();

    bool IniciarOleada(AOleada* NuevaOleada);
    void DetenerOleada();
    bool HayOleadaActiva() const;

    bool SuscribirseAlJefe(AEnemigoJefe* NuevoJefe);
    void DesuscribirseDelJefe();

    UFUNCTION()
    void AlMorirJefe(AEnemigoJefe* JefeMuerto);

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UPROPERTY()
    AOleada* OleadaActual;

    UPROPERTY()
    AEnemigoJefe* JefeObservado;
};
