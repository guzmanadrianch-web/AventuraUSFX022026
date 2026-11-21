#include "GestorJuegoFacade.h"
#include "GestorOleadas.h"
#include "ConstructorOleada.h"
#include "Oleada.h"
#include "FamiliaObstaculos.h"
#include "FabricaRecogibles.h"
#include "ObjetoRecogible.h"
#include "Compuerta.h"
#include "Muro.h"
#include "EnemigoOfensivo.h"
#include "EnemigoDefensivo.h"
#include "EnemigoTerrestre.h"
#include "EnemigoJefe.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

AGestorJuegoFacade::AGestorJuegoFacade()
    : GestorOleadas(nullptr)
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGestorJuegoFacade::IniciarDemostracion()
{
    APawn* Jugador = UGameplayStatics::GetPlayerPawn(this, 0);
    const FVector Centro = IsValid(Jugador) ? Jugador->GetActorLocation() :
        FVector(0.f, 0.f, 200.f);

    GenerarObstaculos(Centro);  // Abstract Factory
    GenerarRecogibles(Centro);  // Factory Method independiente
    IniciarOleada(Centro);      // Builder + Observer
}

void AGestorJuegoFacade::CrearParejaObstaculos(
    const UFabricaFamiliaObstaculos* Fabrica,
    const FVector& Centro, float DireccionLateral)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo || !IsValid(Fabrica)) return;

    const FVector PosicionPlataforma = Centro +
        FVector(-250.f, DireccionLateral * 380.f, -65.f);
    const FVector PosicionMuro = Centro +
        FVector(500.f, DireccionLateral * 390.f, 0.f);

    ACompuerta* Plataforma = Fabrica->CrearPlataforma(
        Mundo, PosicionPlataforma, FRotator::ZeroRotator);
    AMuro* Muro = Fabrica->CrearMuro(
        Mundo, PosicionMuro, FRotator::ZeroRotator);

    // Garantizamos que se crea la familia completa o ninguna.
    if (!IsValid(Plataforma) || !IsValid(Muro))
    {
        if (IsValid(Plataforma)) Plataforma->Destroy();
        if (IsValid(Muro)) Muro->Destroy();
        UE_LOG(LogTemp, Warning, TEXT("Abstract Factory: fallo creando la pareja"));
        return;
    }

    ObstaculosGenerados.Add(Plataforma);
    ObstaculosGenerados.Add(Muro);
}

void AGestorJuegoFacade::DestruirObstaculos()
{
    for (AActor* Obstaculo : ObstaculosGenerados)
    {
        if (IsValid(Obstaculo)) Obstaculo->Destroy();
    }
    ObstaculosGenerados.Empty();
}

void AGestorJuegoFacade::GenerarObstaculos(const FVector& Centro)
{
    DestruirObstaculos();

    UFabricaFamiliaSolida* Solidos = NewObject<UFabricaFamiliaSolida>(this);
    UFabricaFamiliaDestruible* Destruibles = NewObject<UFabricaFamiliaDestruible>(this);

    CrearParejaObstaculos(Solidos, Centro, -1.f);
    CrearParejaObstaculos(Destruibles, Centro, 1.f);
    UE_LOG(LogTemp, Warning, TEXT("Abstract Factory: %d obstaculos"),
        ObstaculosGenerados.Num());
}

void AGestorJuegoFacade::DestruirRecogibles()
{
    for (AObjetoRecogible* Recogible : RecogiblesGenerados)
    {
        if (IsValid(Recogible)) Recogible->Destroy();
    }
    RecogiblesGenerados.Empty();
}

void AGestorJuegoFacade::GenerarRecogibles(const FVector& Centro)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    DestruirRecogibles();
    UFabricaRecogibleVelocidad* FabricaVelocidad = NewObject<UFabricaRecogibleVelocidad>(this);
    UFabricaRecogibleCadencia* FabricaCadencia = NewObject<UFabricaRecogibleCadencia>(this);

    UFabricaRecogibleCambioDisparo* FabricaCambio = NewObject<UFabricaRecogibleCambioDisparo>(this);

    // El algoritmo CrearRecogible es comun; cada subclase sobrescribe el metodo fabrica.
    AObjetoRecogible* Velocidad = FabricaVelocidad->CrearRecogible(
        Mundo, Centro + FVector(-200.f, -750.f, 0.f), FRotator::ZeroRotator);
    AObjetoRecogible* Cadencia = FabricaCadencia->CrearRecogible(
        Mundo, Centro + FVector(-200.f, 750.f, 0.f), FRotator::ZeroRotator);

    // Recogible que cambia entre Normal, Doble y Laser (patron State).
    AObjetoRecogible* CambioDisparo = FabricaCambio->CrearRecogible(
        Mundo, Centro + FVector(350.f, 0.f, 0.f), FRotator::ZeroRotator);

    if (IsValid(Velocidad)) RecogiblesGenerados.Add(Velocidad);
    if (IsValid(Cadencia)) RecogiblesGenerados.Add(Cadencia);
    if (IsValid(CambioDisparo)) RecogiblesGenerados.Add(CambioDisparo);
    UE_LOG(LogTemp, Warning, TEXT("Factory Method: %d recogibles creados"),
        RecogiblesGenerados.Num());
}

void AGestorJuegoFacade::IniciarOleada(const FVector& Centro)
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    if (!IsValid(GestorOleadas))
    {
        GestorOleadas = Mundo->SpawnActor<AGestorOleadas>();
    }
    if (!IsValid(GestorOleadas) || GestorOleadas->HayOleadaActiva()) return;

    UConstructorOleada* Constructor = NewObject<UConstructorOleada>(this);
    Constructor->AgregarEnemigos(AEnemigoOfensivo::StaticClass(), 3);
    Constructor->AgregarEnemigos(AEnemigoDefensivo::StaticClass(), 2);
    Constructor->AgregarEnemigos(AEnemigoTerrestre::StaticClass(), 1);
    Constructor->EstablecerJefe(AEnemigoJefe::StaticClass());
    Constructor->EstablecerRadio(560.f);

    AOleada* NuevaOleada = Constructor->Construir(Mundo, Centro);
    if (!IsValid(NuevaOleada))
    {
        UE_LOG(LogTemp, Error, TEXT("Facade: Builder no pudo construir AOleada"));
        return;
    }

    if (!GestorOleadas->IniciarOleada(NuevaOleada))
    {
        NuevaOleada->Cancelar();
        NuevaOleada->Destroy();
        UE_LOG(LogTemp, Error, TEXT("Facade: no se pudo activar Observer"));
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("Facade: oleada construida y observada (Strategy en enemigos)"));
}

void AGestorJuegoFacade::FinalizarOleada()
{
    if (IsValid(GestorOleadas)) GestorOleadas->DetenerOleada();
}

void AGestorJuegoFacade::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    FinalizarOleada();
    if (IsValid(GestorOleadas)) GestorOleadas->Destroy();
    GestorOleadas = nullptr;
    DestruirObstaculos();
    DestruirRecogibles();
    Super::EndPlay(EndPlayReason);
}
