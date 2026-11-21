#include "ObjetoRecogible.h"
#include "AventuraUSFX022026Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AObjetoRecogible::AObjetoRecogible()
{
    PrimaryActorTick.bCanEverTick = false;

    ZonaRecogida = CreateDefaultSubobject<USphereComponent>(TEXT("ZonaRecogida"));
    RootComponent = ZonaRecogida;
    ZonaRecogida->InitSphereRadius(70.f);
    ZonaRecogida->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ZonaRecogida->SetCollisionObjectType(ECC_WorldDynamic);
    ZonaRecogida->SetCollisionResponseToAllChannels(ECR_Ignore);
    ZonaRecogida->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    ZonaRecogida->SetGenerateOverlapEvents(true);
    ZonaRecogida->OnComponentBeginOverlap.AddDynamic(this, &AObjetoRecogible::AlEntrarJugador);

    Malla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaRecogible"));
    Malla->SetupAttachment(ZonaRecogida);
    Malla->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Malla->SetRelativeScale3D(FVector(0.6f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Esfera(
        TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (Esfera.Succeeded()) Malla->SetStaticMesh(Esfera.Object);
}

bool AObjetoRecogible::AplicarEfecto(AAventuraUSFX022026Pawn* Jugador)
{
    return false; // Debe implementarse en las subclases concretas.
}

void AObjetoRecogible::AlEntrarJugador(UPrimitiveComponent* ComponenteSolapado,
    AActor* OtroActor, UPrimitiveComponent* OtroComponente,
    int32 IndiceCuerpo, bool bDesdeBarrido, const FHitResult& Barrido)
{
    AAventuraUSFX022026Pawn* Jugador = Cast<AAventuraUSFX022026Pawn>(OtroActor);
    if (IsValid(Jugador) && AplicarEfecto(Jugador))
    {
        Destroy();
    }
}
