#include "EstrategiaAtaque.h"
#include "Enemigo.h"
#include "AventuraUSFX022026Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

void UEstrategiaAtaque::Ejecutar(AEnemigo* Enemigo, AActor* Objetivo, float DeltaTime) const
{
    if (!IsValid(Enemigo) || !IsValid(Objetivo)) return;

    const FVector Direccion =
        (Objetivo->GetActorLocation() - Enemigo->GetActorLocation()).GetSafeNormal2D();
    if (Direccion.IsNearlyZero()) return;

    if (FVector::Dist2D(Objetivo->GetActorLocation(), Enemigo->GetActorLocation()) > 650.f)
    {
        Enemigo->MoverEnDireccion(Direccion, 200.f);
        return;
    }

    // El enfriamiento pertenece a cada AEnemigo; nunca al CDO compartido.
    if (!Enemigo->PuedeDisparar()) return;

    UWorld* Mundo = Enemigo->GetWorld();
    if (!Mundo) return;

    FVector Posicion = Enemigo->GetActorLocation() + Direccion * 115.f;
    Posicion.Z = Objetivo->GetActorLocation().Z;

    FActorSpawnParameters Parametros;
    Parametros.Owner = Enemigo;
    Parametros.Instigator = Enemigo;
    Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AAventuraUSFX022026Projectile* Bala = Mundo->SpawnActor<AAventuraUSFX022026Projectile>(
        Posicion, Direccion.Rotation(), Parametros);
    if (!IsValid(Bala)) return;

    Bala->GetProjectileMesh()->IgnoreActorWhenMoving(Enemigo, true);
    if (UProjectileMovementComponent* Movimiento = Bala->GetProjectileMovement())
    {
        Movimiento->bShouldBounce = false;
        Movimiento->Velocity = Direccion * 1000.f;
    }
}
