#include "AventuraUSFX022026Projectile.h"
#include "Enemigo.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

AAventuraUSFX022026Projectile::AAventuraUSFX022026Projectile()
{
    PrimaryActorTick.bCanEverTick = false;
    Danio = 15.f; // 6 impactos contra un enemigo normal (90 puntos de vida).

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(
        TEXT("StaticMesh'/Game/TwinStick/Meshes/TwinStickProjectile.TwinStickProjectile'"));

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
    RootComponent = ProjectileMesh;
    if (ProjectileMeshAsset.Succeeded())
    {
        ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
    }
    ProjectileMesh->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ProjectileMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    ProjectileMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    ProjectileMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
    ProjectileMesh->SetNotifyRigidBodyCollision(true);
    ProjectileMesh->OnComponentHit.AddDynamic(this,
        &AAventuraUSFX022026Projectile::OnHit);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
    ProjectileMovement->UpdatedComponent = ProjectileMesh;
    ProjectileMovement->InitialSpeed = 1000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 1.0f;
    ProjectileMovement->ProjectileGravityScale = 0.f;
    ProjectileMovement->bConstrainToPlane = true;
    ProjectileMovement->SetPlaneConstraintNormal(FVector(0.f, 0.f, 1.f));

    InitialLifeSpan = 3.f;
}

void AAventuraUSFX022026Projectile::OnHit(UPrimitiveComponent* HitComp,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    FVector NormalImpulse, const FHitResult& Hit)
{
    if (!IsValid(OtherActor) || OtherActor == this || OtherActor == GetOwner()) return;

    const bool bDisparoEnemigo = Cast<AEnemigo>(GetOwner()) != nullptr;
    AEnemigo* Enemigo = Cast<AEnemigo>(OtherActor);
    if (bDisparoEnemigo)
    {
        // Los enemigos no se atacan entre si. El Pawn aun no tiene puntos de vida.
        if (!IsValid(Enemigo))
            UE_LOG(LogTemp, Verbose, TEXT("Strategy: proyectil enemigo impacto %s"), *OtherActor->GetName());
        Destroy();
        return;
    }

    if (IsValid(Enemigo) && !EnemigosImpactados.Contains(OtherActor))
    {
        EnemigosImpactados.Add(OtherActor);
        UGameplayStatics::ApplyDamage(Enemigo, Danio, GetInstigatorController(),
            this, UDamageType::StaticClass());
    }

    if (OtherComp && OtherComp->IsSimulatingPhysics())
        OtherComp->AddImpulseAtLocation(GetVelocity() * 20.f, GetActorLocation());
}
