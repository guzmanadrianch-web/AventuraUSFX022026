#include "GestorOleadas.h"
#include "Oleada.h"
#include "EnemigoJefe.h"

AGestorOleadas::AGestorOleadas()
    : OleadaActual(nullptr), JefeObservado(nullptr)
{
    PrimaryActorTick.bCanEverTick = false;
}

bool AGestorOleadas::HayOleadaActiva() const
{
    return IsValid(OleadaActual) && OleadaActual->ObtenerEstado() == EEstadoOleada::Activa;
}

bool AGestorOleadas::SuscribirseAlJefe(AEnemigoJefe* NuevoJefe)
{
    // Es posible cancelar la suscripcion incluso si el jefe no ha muerto.
    if (!HayOleadaActiva() || !IsValid(NuevoJefe) ||
        OleadaActual->ObtenerJefe() != NuevoJefe)
    {
        return false;
    }

    DesuscribirseDelJefe();
    JefeObservado = NuevoJefe;
    JefeObservado->OnJefeMuerto.AddUniqueDynamic(this, &AGestorOleadas::AlMorirJefe);
    UE_LOG(LogTemp, Log, TEXT("Observer: suscrito a %s"), *NuevoJefe->GetName());
    return true;
}

void AGestorOleadas::DesuscribirseDelJefe()
{
    if (IsValid(JefeObservado))
    {
        JefeObservado->OnJefeMuerto.RemoveDynamic(this, &AGestorOleadas::AlMorirJefe);
        UE_LOG(LogTemp, Log, TEXT("Observer: suscripcion cancelada"));
    }
    JefeObservado = nullptr;
}

bool AGestorOleadas::IniciarOleada(AOleada* NuevaOleada)
{
    if (HayOleadaActiva() || !IsValid(NuevaOleada) ||
        NuevaOleada->ObtenerEstado() != EEstadoOleada::Activa)
    {
        return false;
    }

    AEnemigoJefe* Jefe = NuevaOleada->ObtenerJefe();
    if (!IsValid(Jefe)) return false;

    // Se puede iniciar otra oleada tras finalizar la anterior.
    DetenerOleada();
    OleadaActual = NuevaOleada;
    if (!SuscribirseAlJefe(Jefe))
    {
        OleadaActual = nullptr;
        return false;
    }
    return true;
}

void AGestorOleadas::AlMorirJefe(AEnemigoJefe* JefeMuerto)
{
    if (!HayOleadaActiva() || JefeMuerto != JefeObservado) return;

    // Desuscripcion explicita ANTES de limpiar la oleada.
    DesuscribirseDelJefe();
    UE_LOG(LogTemp, Warning, TEXT("Observer: jefe muerto; finalizando oleada"));
    AOleada* OleadaTerminada = OleadaActual;
    OleadaActual = nullptr;
    OleadaTerminada->Finalizar();
    OleadaTerminada->Destroy();
}

void AGestorOleadas::DetenerOleada()
{
    DesuscribirseDelJefe();
    if (IsValid(OleadaActual))
    {
        OleadaActual->Cancelar();
        OleadaActual->Destroy();
    }
    OleadaActual = nullptr;
}

void AGestorOleadas::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    DetenerOleada();
    Super::EndPlay(EndPlayReason);
}
