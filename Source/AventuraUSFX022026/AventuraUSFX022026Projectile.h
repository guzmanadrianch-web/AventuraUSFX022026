#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AventuraUSFX022026Projectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS(config = Game)
class AAventuraUSFX022026Projectile : public AActor
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile,
        meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement,
        meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent* ProjectileMovement;

    // Evita multiples danos al mismo enemigo por rebotes de un unico disparo.
    UPROPERTY()
    TArray<AActor*> EnemigosImpactados;

public:
    AAventuraUSFX022026Projectile();

    UPROPERTY(EditDefaultsOnly, Category = "Combate")
    float Danio;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }
    FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
