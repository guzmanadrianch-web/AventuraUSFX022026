#include "EstadoDisparoLaser.h"
#include "AventuraUSFX022026Pawn.h"

bool UEstadoDisparoLaser::EjecutarDisparo(AAventuraUSFX022026Pawn* Nave,
    const FVector& Direccion) const
{
    return IsValid(Nave) && Nave->CrearProyectilLaser(Direccion);
}
