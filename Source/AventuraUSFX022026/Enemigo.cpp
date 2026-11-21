#include "Enemigo.h"
#include "Estrategia.h"
#include "EstrategiaAtaque.h"
#include "EstrategiaHuida.h"
#include "EstrategiaKamikaze.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimInstance.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AEnemigo::AEnemigo()
{
    PrimaryActorTick.bCanEverTick = true;
    velocidad = 180.f;
    // La vida original (1) no permitia observar los cambios de Strategy.
    VidaMaxima = 90.f;
    VidaActual = VidaMaxima;
    bPerseguirJugador = true;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MallaEnemigo(
        TEXT("SkeletalMesh'/Game/ROG_Creatures/Stickman/Meshes/SK_Stickman.SK_Stickman'"));
    if (MallaEnemigo.Succeeded()) GetMesh()->SetSkeletalMesh(MallaEnemigo.Object);

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimacionEnemigo(
        TEXT("/Game/ROG_Creatures/Stickman/Animations/ABP_Stickman"));
    if (AnimacionEnemigo.Succeeded())
    {
        GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
        GetMesh()->SetAnimInstanceClass(AnimacionEnemigo.Class);
    }
    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    GetCharacterMovement()->MaxWalkSpeed = velocidad;
    GetCharacterMovement()->GravityScale = 0.f;
    GetCharacterMovement()->bRunPhysicsWithNoController = true;
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
}

void AEnemigo::BeginPlay()
{
    Super::BeginPlay();
    VidaActual = VidaMaxima;
    GetCharacterMovement()->SetMovementMode(MOVE_Flying);
    SeleccionarEstrategia();
}

void AEnemigo::Morir()
{
    OnEnemigoMuerto.Broadcast(this);
    Destroy();
}

float AEnemigo::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    const float Danio = FMath::Max(0.f, DamageAmount);
    if (VidaActual <= 0.f || Danio <= 0.f) return 0.f;

    VidaActual = FMath::Max(0.f, VidaActual - Danio);
    UE_LOG(LogTemp, Log, TEXT("%s: vida restante %.1f"), *GetName(), VidaActual);
    if (VidaActual <= 0.f)
    {
        Morir(); // Respeta el override del jefe (Observer).
    }
    else
    {
        SeleccionarEstrategia();
    }
    return Danio;
}

void AEnemigo::SeleccionarEstrategia()
{
    if (VidaMaxima <= 0.f) return;
    TSubclassOf<UEstrategia> NuevaClase;

    if (VidaActual <= VidaMaxima / 3.f)
        NuevaClase = UEstrategiaKamikaze::StaticClass();
    else if (VidaActual <= VidaMaxima / 2.f)
        NuevaClase = UEstrategiaHuida::StaticClass();
    else
        NuevaClase = UEstrategiaAtaque::StaticClass();

    if (ClaseEstrategiaActual.Get() == NuevaClase.Get()) return;
    ClaseEstrategiaActual = NuevaClase;

    UE_LOG(LogTemp, Warning, TEXT("Strategy: %s cambia a %s (%.0f/%.0f)"),
        *GetName(), *GetNameSafe(NuevaClase.Get()), VidaActual, VidaMaxima);
}

void AEnemigo::MoverEnDireccion(const FVector& Direccion, float Rapidez)
{
    if (Direccion.IsNearlyZero()) return;
    if (UCharacterMovementComponent* Movimiento = GetCharacterMovement())
    {
        Movimiento->MaxFlySpeed = Rapidez;
        const FVector Normal = Direccion.GetSafeNormal2D();
        AddMovementInput(Normal, 1.f);
        SetActorRotation(Normal.Rotation());
    }
}

bool AEnemigo::PuedeDisparar()
{
    UWorld* Mundo = GetWorld();
    if (!Mundo) return false;
    if (Mundo->GetTimeSeconds() < SiguienteDisparo) return false;
    SiguienteDisparo = Mundo->GetTimeSeconds() + 1.f;
    return true;
}

void AEnemigo::AutodestruirPorKamikaze()
{
    if (VidaActual <= 0.f) return;
    VidaActual = 0.f;
    Morir(); // llamada virtual: AEnemigoJefe notifica por Observer.
}

void AEnemigo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!bPerseguirJugador || VidaActual <= 0.f) return;

    APawn* Jugador = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!IsValid(Jugador) || !ClaseEstrategiaActual) return;

    // Usamos el Class Default Object compartido; ningun enemigo crea 3 UObjects propios.
    const UEstrategia* Estrategia = ClaseEstrategiaActual.GetDefaultObject();
    if (Estrategia) Estrategia->Ejecutar(this, Jugador, DeltaTime);

    if (bMostrarEstrategia && !IsPendingKill())
    {
        const UClass* Tipo = ClaseEstrategiaActual.Get();
        const TCHAR* Nombre = Tipo == UEstrategiaAtaque::StaticClass() ? TEXT("ATAQUE") :
            Tipo == UEstrategiaHuida::StaticClass() ? TEXT("HUIDA") : TEXT("KAMIKAZE");
        DrawDebugString(GetWorld(), GetActorLocation() + FVector(0.f, 0.f, 120.f),
            FString::Printf(TEXT("%.0f / %.0f | %s"), VidaActual, VidaMaxima, Nombre),
            nullptr, FColor::Yellow, 0.f, true);
    }
}

void AEnemigo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
