#include "EstrategiaHuida.h"
#include "Enemigo.h"

void UEstrategiaHuida::Ejecutar(AEnemigo* Enemigo, AActor* Objetivo, float DeltaTime) const
{
    if (!IsValid(Enemigo) || !IsValid(Objetivo)) return;
    const FVector Alejarse =
        (Enemigo->GetActorLocation() - Objetivo->GetActorLocation()).GetSafeNormal2D();
    Enemigo->MoverEnDireccion(Alejarse, 250.f);
}
