#include "EstadoDisparoNormal.h"
#include "AventuraUSFX022026Pawn.h"

bool UEstadoDisparoNormal::EjecutarDisparo(AAventuraUSFX022026Pawn* Nave,
    const FVector& Direccion) const
{
    return IsValid(Nave) && Nave->CrearProyectil(Direccion);
}
