#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Oleada.generated.h"

class AEnemigo;
class AEnemigoJefe;
class USceneComponent;

// Producto del patron Builder: una oleada es un Actor que posee su composicion.
UENUM(BlueprintType)
enum class EEstadoOleada : uint8
{
    EnConstruccion,
    Activa,
    Finalizada,
    Cancelada
};

UCLASS()
class AVENTURAUSFX022026_API AOleada : public AActor
{
    GENERATED_BODY()

public:
    AOleada();

    // Solo el constructor de oleadas debe llamar estos metodos antes de Activar.
    bool RegistrarEnemigo(AEnemigo* Enemigo);
    bool RegistrarJefe(AEnemigoJefe* NuevoJefe);
    void EstablecerRadio(float NuevoRadio);
    bool Activar();

    // Finalizar: el jefe ya esta muriendo; elimina los enemigos restantes.
    void Finalizar();
    // Cancelar: destruye tanto al jefe como a los enemigos restantes.
    void Cancelar();

    AEnemigoJefe* ObtenerJefe() const { return Jefe; }
    const TArray<AEnemigo*>& ObtenerEnemigos() const { return Enemigos; }
    EEstadoOleada ObtenerEstado() const { return Estado; }

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Oleada")
    USceneComponent* Raiz;

    UPROPERTY(VisibleAnywhere, Category = "Oleada")
    TArray<AEnemigo*> Enemigos;

    UPROPERTY(VisibleAnywhere, Category = "Oleada")
    AEnemigoJefe* Jefe;

    UPROPERTY(VisibleAnywhere, Category = "Oleada")
    float Radio;

    UPROPERTY(VisibleAnywhere, Category = "Oleada")
    EEstadoOleada Estado;

    void DestruirEnemigos();
    void DestruirJefe();
};
