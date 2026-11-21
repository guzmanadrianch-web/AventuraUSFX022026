#include "Oleada.h"
#include "Enemigo.h"
#include "EnemigoJefe.h"
#include "Components/SceneComponent.h"

AOleada::AOleada()
    : Jefe(nullptr), Radio(500.f), Estado(EEstadoOleada::EnConstruccion)
{
    PrimaryActorTick.bCanEverTick = false;
    Raiz = CreateDefaultSubobject<USceneComponent>(TEXT("RaizOleada"));
    RootComponent = Raiz;
}

bool AOleada::RegistrarEnemigo(AEnemigo* Enemigo)
{
    if (Estado != EEstadoOleada::EnConstruccion || !IsValid(Enemigo) || Enemigos.Contains(Enemigo))
    {
        return false;
    }
    Enemigos.Add(Enemigo);
    return true;
}

bool AOleada::RegistrarJefe(AEnemigoJefe* NuevoJefe)
{
    if (Estado != EEstadoOleada::EnConstruccion || !IsValid(NuevoJefe) || IsValid(Jefe))
    {
        return false;
    }
    Jefe = NuevoJefe;
    return true;
}

void AOleada::EstablecerRadio(float NuevoRadio)
{
    if (Estado == EEstadoOleada::EnConstruccion)
    {
        Radio = FMath::Max(200.f, NuevoRadio);
    }
}

bool AOleada::Activar()
{
    if (Estado != EEstadoOleada::EnConstruccion || !IsValid(Jefe) || Enemigos.Num() == 0)
    {
        return false;
    }
    for (AEnemigo* Enemigo : Enemigos)
    {
        if (!IsValid(Enemigo)) return false;
    }

    Estado = EEstadoOleada::Activa;
    UE_LOG(LogTemp, Log, TEXT("Oleada activada: %d enemigos, un jefe, radio %.0f"),
        Enemigos.Num(), Radio);
    return true;
}

void AOleada::DestruirEnemigos()
{
    for (AEnemigo* Enemigo : Enemigos)
    {
        if (IsValid(Enemigo)) Enemigo->Destroy();
    }
    Enemigos.Empty();
}

void AOleada::DestruirJefe()
{
    if (IsValid(Jefe)) Jefe->Destroy();
    Jefe = nullptr;
}

void AOleada::Finalizar()
{
    if (Estado != EEstadoOleada::Activa) return;
    Estado = EEstadoOleada::Finalizada;
    // El jefe emite OnJefeMuerto antes de ejecutar Super::Morir().
    // No se destruye aqui para dejar terminar su propio metodo Morir().
    Jefe = nullptr;
    DestruirEnemigos();
}

void AOleada::Cancelar()
{
    if (Estado == EEstadoOleada::Cancelada) return;
    Estado = EEstadoOleada::Cancelada;
    DestruirEnemigos();
    DestruirJefe();
}

void AOleada::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Tambien cubre destruccion inesperada de AOleada o cambio de nivel.
    if (Estado == EEstadoOleada::Activa || Estado == EEstadoOleada::EnConstruccion)
    {
        Cancelar();
    }
    Super::EndPlay(EndPlayReason);
}
