#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GestorJuegoFacade.generated.h"

class AGestorOleadas;
class UFabricaFamiliaObstaculos;
class AObjetoRecogible;

// Facade: coordina subsistemas pero NO implementa su logica interna.
UCLASS()
class AVENTURAUSFX022026_API AGestorJuegoFacade : public AActor
{
    GENERATED_BODY()

public:
    AGestorJuegoFacade();

    void IniciarDemostracion();
    void GenerarObstaculos(const FVector& Centro);
    void GenerarRecogibles(const FVector& Centro);
    void IniciarOleada(const FVector& Centro);
    void FinalizarOleada();

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UPROPERTY()
    AGestorOleadas* GestorOleadas;

    UPROPERTY()
    TArray<AActor*> ObstaculosGenerados;

    UPROPERTY()
    TArray<AObjetoRecogible*> RecogiblesGenerados;

    void CrearParejaObstaculos(const UFabricaFamiliaObstaculos* Fabrica,
        const FVector& Centro, float DireccionLateral);
    void DestruirObstaculos();
    void DestruirRecogibles();
};
