#include "FabricaRecogibles.h"
#include "ObjetoRecogible.h"
#include "RecogibleVelocidad.h"
#include "RecogibleCadencia.h"
#include "RecogibleCambioDisparo.h"
#include "Engine/World.h"

AObjetoRecogible* UFabricaRecogible::CrearRecogible(
    UWorld* Mundo, const FVector& Posicion, const FRotator& Rotacion) const
{
    TSubclassOf<AObjetoRecogible> ClaseProducto = ObtenerClaseRecogible();
    if (!Mundo || !ClaseProducto) return nullptr;

    FActorSpawnParameters Parametros;
    Parametros.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    return Mundo->SpawnActor<AObjetoRecogible>(
        ClaseProducto.Get(), Posicion, Rotacion, Parametros);
}

TSubclassOf<AObjetoRecogible> UFabricaRecogible::ObtenerClaseRecogible() const
{
    return nullptr;
}

TSubclassOf<AObjetoRecogible> UFabricaRecogibleVelocidad::ObtenerClaseRecogible() const
{
    return ARecogibleVelocidad::StaticClass();
}

TSubclassOf<AObjetoRecogible> UFabricaRecogibleCadencia::ObtenerClaseRecogible() const
{
    return ARecogibleCadencia::StaticClass();
}

TSubclassOf<AObjetoRecogible> UFabricaRecogibleCambioDisparo::ObtenerClaseRecogible() const
{
    return ARecogibleCambioDisparo::StaticClass();
}
