#include "RecogibleVelocidad.h"
#include "AventuraUSFX022026Pawn.h"
#include "Components/StaticMeshComponent.h"

ARecogibleVelocidad::ARecogibleVelocidad()
    : VelocidadExtra(200.f)
{
    Malla->SetRelativeScale3D(FVector(0.6f));
}

bool ARecogibleVelocidad::AplicarEfecto(AAventuraUSFX022026Pawn* Jugador)
{
    if (!IsValid(Jugador)) return false;
    Jugador->MoveSpeed += VelocidadExtra;
    UE_LOG(LogTemp, Warning, TEXT("Factory Method: recogible velocidad: %.0f"), Jugador->MoveSpeed);
    return true;
}
