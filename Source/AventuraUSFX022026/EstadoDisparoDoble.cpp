#include "EstadoDisparoDoble.h"
#include "AventuraUSFX022026Pawn.h"

bool UEstadoDisparoDoble::EjecutarDisparo(AAventuraUSFX022026Pawn* Nave,
    const FVector& Direccion) const
{
    if (!IsValid(Nave) || !Nave->PuedeCrearProyectiles(2)) return false;

    const bool Primero = Nave->CrearProyectil(Direccion, FVector(0.f, -25.f, 0.f));
    const bool Segundo = Nave->CrearProyectil(Direccion, FVector(0.f, 25.f, 0.f));
    return Primero || Segundo;
}
