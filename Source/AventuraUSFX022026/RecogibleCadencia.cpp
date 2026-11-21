#include "RecogibleCadencia.h"
#include "AventuraUSFX022026Pawn.h"
#include "Components/StaticMeshComponent.h"

ARecogibleCadencia::ARecogibleCadencia()
    : ReduccionSegundos(0.025f)
{
    Malla->SetRelativeScale3D(FVector(0.4f));
}

bool ARecogibleCadencia::AplicarEfecto(AAventuraUSFX022026Pawn* Jugador)
{
    if (!IsValid(Jugador)) return false;
    Jugador->FireRate = FMath::Max(0.05f, Jugador->FireRate - ReduccionSegundos);
    UE_LOG(LogTemp, Warning, TEXT("Factory Method: cadencia nueva %.3f segundos"), Jugador->FireRate);
    return true;
}
