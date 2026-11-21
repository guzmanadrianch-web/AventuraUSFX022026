#include "EstrategiaKamikaze.h"
#include "Enemigo.h"
#include "DrawDebugHelpers.h"

void UEstrategiaKamikaze::Ejecutar(AEnemigo* Enemigo, AActor* Objetivo, float DeltaTime) const
{
    if (!IsValid(Enemigo) || !IsValid(Objetivo)) return;

    const FVector HaciaJugador =
        (Objetivo->GetActorLocation() - Enemigo->GetActorLocation()).GetSafeNormal2D();
    const float Distancia = FVector::Dist2D(
        Enemigo->GetActorLocation(), Objetivo->GetActorLocation());

    if (Distancia <= 200.f)
    {
        // El Pawn original no tiene salud: efecto visual y empujon de contacto.
        DrawDebugSphere(Enemigo->GetWorld(), Enemigo->GetActorLocation(),
            180.f, 20, FColor::Red, false, 2.f, 0, 4.f);
        Objetivo->AddActorWorldOffset(HaciaJugador * 200.f, true);
        Enemigo->AutodestruirPorKamikaze();
        return;
    }

    Enemigo->MoverEnDireccion(HaciaJugador, 550.f);
}
