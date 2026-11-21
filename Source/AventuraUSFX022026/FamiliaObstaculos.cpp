#include "FamiliaObstaculos.h"
#include "Compuerta.h"
#include "Muro.h"
#include "PlataformaSolida.h"
#include "PlataformaDestruible.h"
#include "MuroSolido.h"
#include "MuroDestruible.h"
#include "Engine/World.h"

namespace
{
    FActorSpawnParameters ParametrosObstaculo()
    {
        FActorSpawnParameters Parametros;
        Parametros.SpawnCollisionHandlingOverride =
            ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        return Parametros;
    }
}

ACompuerta* UFabricaFamiliaObstaculos::CrearPlataforma(
    UWorld* Mundo, const FVector& Posicion, const FRotator& Rotacion) const
{
    return nullptr; // Clase base abstracta; las hijas definen el producto.
}

AMuro* UFabricaFamiliaObstaculos::CrearMuro(
    UWorld* Mundo, const FVector& Posicion, const FRotator& Rotacion) const
{
    return nullptr;
}

ACompuerta* UFabricaFamiliaSolida::CrearPlataforma(
    UWorld* Mundo, const FVector& Posicion, const FRotator& Rotacion) const
{
    return Mundo ? Mundo->SpawnActor<APlataformaSolida>(Posicion, Rotacion, ParametrosObstaculo()) : nullptr;
}

AMuro* UFabricaFamiliaSolida::CrearMuro(
    UWorld* Mundo, const FVector& Posicion, const FRotator& Rotacion) const
{
    return Mundo ? Mundo->SpawnActor<AMuroSolido>(Posicion, Rotacion, ParametrosObstaculo()) : nullptr;
}

ACompuerta* UFabricaFamiliaDestruible::CrearPlataforma(
    UWorld* Mundo, const FVector& Posicion, const FRotator& Rotacion) const
{
    return Mundo ? Mundo->SpawnActor<APlataformaDestruible>(Posicion, Rotacion, ParametrosObstaculo()) : nullptr;
}

AMuro* UFabricaFamiliaDestruible::CrearMuro(
    UWorld* Mundo, const FVector& Posicion, const FRotator& Rotacion) const
{
    return Mundo ? Mundo->SpawnActor<AMuroDestruible>(Posicion, Rotacion, ParametrosObstaculo()) : nullptr;
}
