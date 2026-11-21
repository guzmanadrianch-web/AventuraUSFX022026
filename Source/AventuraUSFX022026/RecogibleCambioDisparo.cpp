#include "RecogibleCambioDisparo.h"
#include "AventuraUSFX022026Pawn.h"
#include "Components/StaticMeshComponent.h"

ARecogibleCambioDisparo::ARecogibleCambioDisparo()
{
    Malla->SetRelativeScale3D(FVector(0.8f));
}

bool ARecogibleCambioDisparo::AplicarEfecto(AAventuraUSFX022026Pawn* Jugador)
{
    if (!IsValid(Jugador)) return false;
    Jugador->CambiarEstadoDisparoAleatorio();
    return true;
}
