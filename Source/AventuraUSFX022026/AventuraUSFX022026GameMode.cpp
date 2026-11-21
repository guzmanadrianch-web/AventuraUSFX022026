#include "AventuraUSFX022026GameMode.h"
#include "AventuraUSFX022026Pawn.h"
#include "Compuerta.h"
#include "CustomHUD.h"
#include "MuroDestruible.h"
#include "GestorJuegoFacade.h"
#include "Engine/World.h"
#include "TimerManager.h"

AAventuraUSFX022026GameMode::AAventuraUSFX022026GameMode()
{
    DefaultPawnClass = AAventuraUSFX022026Pawn::StaticClass();
    HUDClass = ACustomHUD::StaticClass();
    bActivarDemostracionPatrones = true;
    Facade = nullptr;
    objetoSpawneado = nullptr;
    muroDestruido = nullptr;
}

void AAventuraUSFX022026GameMode::DestruirCompuerta()
{
    if (IsValid(objetoSpawneado)) objetoSpawneado->Destroy();
    objetoSpawneado = nullptr;
}

void AAventuraUSFX022026GameMode::EliminarPlataforma()
{
    if (Plataformas.Num() == 0) return;
    const int32 I = FMath::RandRange(0, Plataformas.Num() - 1);
    if (IsValid(Plataformas[I])) Plataformas[I]->Destroy();
    Plataformas.RemoveAt(I);
}

void AAventuraUSFX022026GameMode::EmpezarMovimiento()
{
    for (ACompuerta* Plataforma : Plataformas)
    {
        if (IsValid(Plataforma)) Plataforma->comportamiento = FMath::RandRange(0, 9);
    }
}

void AAventuraUSFX022026GameMode::DetenerMovimiento()
{
    for (ACompuerta* Plataforma : Plataformas)
    {
        if (IsValid(Plataforma)) Plataforma->comportamiento = 10;
    }
}

void AAventuraUSFX022026GameMode::BeginPlay()
{
    Super::BeginPlay();
    UWorld* Mundo = GetWorld();
    if (!Mundo) return;

    if (bActivarDemostracionPatrones)
    {
        
        GetWorldTimerManager().SetTimerForNextTick(
            this, &AAventuraUSFX022026GameMode::IniciarSistemaPatrones);
        return;
    }

    
    muroDestruido = Mundo->SpawnActor<AMuroDestruible>(
        FVector(-950.f, 150.f, 214.f), FRotator::ZeroRotator);
}

void AAventuraUSFX022026GameMode::IniciarSistemaPatrones()
{
    UWorld* Mundo = GetWorld();
    if (!Mundo || IsValid(Facade)) return;

    Facade = Mundo->SpawnActor<AGestorJuegoFacade>();
    if (!IsValid(Facade))
    {
        UE_LOG(LogTemp, Error, TEXT("GameMode: no se pudo crear GestorJuegoFacade"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("GameMode: iniciando Builder + Factory Method + Abstract Factory + Observer mediante Facade"));
    Facade->IniciarDemostracion();
}
