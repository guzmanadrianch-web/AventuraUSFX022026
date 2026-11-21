#include "ConstructorOleada.h"
#include "Oleada.h"
#include "Enemigo.h"
#include "EnemigoJefe.h"
#include "Engine/World.h"

void UConstructorOleada::AgregarEnemigos(TSubclassOf<AEnemigo> Clase, int32 Cantidad)
{
    if (!Clase) return;
    for (int32 I = 0; I < FMath::Clamp(Cantidad, 0, 50); ++I)
    {
        ClasesEnemigos.Add(Clase);
    }
}

void UConstructorOleada::EstablecerJefe(TSubclassOf<AEnemigoJefe> Clase)
{
    ClaseJefe = Clase;
}

void UConstructorOleada::EstablecerRadio(float NuevoRadio)
{
    Radio = FMath::Max(200.f, NuevoRadio);
}

void UConstructorOleada::Reiniciar()
{
    ClasesEnemigos.Empty();
    ClaseJefe = nullptr;
    Radio = 500.f;
}

AOleada* UConstructorOleada::Construir(UWorld* Mundo, const FVector& Centro)
{
    if (!Mundo || !ClaseJefe || ClasesEnemigos.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Builder: falta mundo, jefe o enemigos"));
        return nullptr;
    }

    FActorSpawnParameters Parametros;
    Parametros.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AOleada* NuevaOleada = Mundo->SpawnActor<AOleada>(
        AOleada::StaticClass(), Centro, FRotator::ZeroRotator, Parametros);
    if (!IsValid(NuevaOleada)) return nullptr;
    NuevaOleada->EstablecerRadio(Radio);

    // Construir el producto: el jefe y cada enemigo pertenecen a AOleada.
    const FVector PosicionJefe = Centro + FVector(Radio * 1.6f, 0.f, 0.f);
    AEnemigoJefe* Jefe = Mundo->SpawnActor<AEnemigoJefe>(
        ClaseJefe.Get(), PosicionJefe, FRotator::ZeroRotator, Parametros);
    if (!IsValid(Jefe))
    {
        NuevaOleada->Cancelar();
        NuevaOleada->Destroy();
        return nullptr;
    }
    if (!NuevaOleada->RegistrarJefe(Jefe))
    {
        Jefe->Destroy();
        NuevaOleada->Cancelar();
        NuevaOleada->Destroy();
        return nullptr;
    }

    const int32 Total = ClasesEnemigos.Num();
    for (int32 I = 0; I < Total; ++I)
    {
        const float Angulo = (2.f * PI * static_cast<float>(I)) / Total;
        const FVector Posicion = Centro + FVector(
            FMath::Cos(Angulo) * Radio, FMath::Sin(Angulo) * Radio, 0.f);
        AEnemigo* Creado = Mundo->SpawnActor<AEnemigo>(
            ClasesEnemigos[I].Get(), Posicion, FRotator::ZeroRotator, Parametros);
        if (!IsValid(Creado))
        {
            NuevaOleada->Cancelar();
            NuevaOleada->Destroy();
            return nullptr;
        }
        if (!NuevaOleada->RegistrarEnemigo(Creado))
        {
            Creado->Destroy();
            NuevaOleada->Cancelar();
            NuevaOleada->Destroy();
            return nullptr;
        }
    }

    if (!NuevaOleada->Activar())
    {
        NuevaOleada->Cancelar();
        NuevaOleada->Destroy();
        return nullptr;
    }

    UE_LOG(LogTemp, Warning, TEXT("Builder: AOleada completo construido con %d enemigos"), Total);
    return NuevaOleada;
}
