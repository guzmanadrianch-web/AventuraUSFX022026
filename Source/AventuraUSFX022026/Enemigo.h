#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Templates/SubclassOf.h"
#include "Enemigo.generated.h"

class UEstrategia;
class AEnemigo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventoEnemigoMuerto, AEnemigo*, EnemigoMuerto);

// Personaje original conservado; ahora es el Contexto del patron Strategy.
UCLASS()
class AVENTURAUSFX022026_API AEnemigo : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemigo();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combate")
    float VidaMaxima;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combate")
    float VidaActual;

    UPROPERTY(EditAnywhere, Category = "Combate")
    bool bPerseguirJugador;

    UPROPERTY(BlueprintAssignable, Category = "Observer")
    FEventoEnemigoMuerto OnEnemigoMuerto;

    virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
        AController* EventInstigator, AActor* DamageCauser) override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    // Operaciones del contexto que utilizan las estrategias.
    void MoverEnDireccion(const FVector& Direccion, float Rapidez);
    bool PuedeDisparar();
    void AutodestruirPorKamikaze();
    void SeleccionarEstrategia();

    UFUNCTION(BlueprintPure, Category = "Strategy")
    TSubclassOf<UEstrategia> ObtenerEstrategiaActual() const { return ClaseEstrategiaActual; }

protected:
    virtual void BeginPlay() override;
    virtual void Morir();

    float velocidad;

private:
    UPROPERTY(VisibleInstanceOnly, Category = "Strategy")
    TSubclassOf<UEstrategia> ClaseEstrategiaActual;

    UPROPERTY(EditAnywhere, Category = "Strategy|Depuracion")
    bool bMostrarEstrategia = true;

    // El tiempo entre ataques es individual y nunca se almacena en el CDO.
    float SiguienteDisparo = 0.f;
};
