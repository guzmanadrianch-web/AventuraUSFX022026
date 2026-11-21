// Copyright Epic Games, Inc. All Rights Reserved.

#include "AventuraUSFX022026Pawn.h"
#include "AventuraUSFX022026Projectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "EstadoDisparo.h"
#include "EstadoDisparoNormal.h"
#include "EstadoDisparoDoble.h"
#include "EstadoDisparoLaser.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

const FName AAventuraUSFX022026Pawn::MoveForwardBinding("MoveForward");
const FName AAventuraUSFX022026Pawn::MoveRightBinding("MoveRight");
const FName AAventuraUSFX022026Pawn::FireForwardBinding("FireForward");
const FName AAventuraUSFX022026Pawn::FireRightBinding("FireRight");

AAventuraUSFX022026Pawn::AAventuraUSFX022026Pawn()
{
	ProyectilActivo = nullptr;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("StaticMesh'/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO'"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMeshComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f)); //esto modifica el mesh 
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;

	//Inventory
    MyInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("MyInventory"));
}

void AAventuraUSFX022026Pawn::BeginPlay()
{
    Super::BeginPlay();
    EstadoNormal = NewObject<UEstadoDisparoNormal>(this);
    EstadoDoble = NewObject<UEstadoDisparoDoble>(this);
    EstadoLaser = NewObject<UEstadoDisparoLaser>(this);
    CambiarEstadoDisparo(EstadoNormal);
}

void AAventuraUSFX022026Pawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UWorld* Mundo = GetWorld())
        Mundo->GetTimerManager().ClearTimer(TimerHandle_ShotTimerExpired);

    for (AAventuraUSFX022026Projectile* Proyectil : ProyectilesActivos)
    {
        if (IsValid(Proyectil))
            Proyectil->OnDestroyed.RemoveDynamic(this, &AAventuraUSFX022026Pawn::AlDestruirProyectil);
    }
    ProyectilesActivos.Empty();
    ProyectilActivo = nullptr;
    Super::EndPlay(EndPlayReason);
}

void AAventuraUSFX022026Pawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void AAventuraUSFX022026Pawn::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	AInventoryActor* InventoryItem = Cast<AInventoryActor>(Other);
	if (InventoryItem != nullptr) { TakeItem(InventoryItem); }
}

void AAventuraUSFX022026Pawn::DropItem()
{
	if (MyInventory->CurrentInventory.Num() == 0) { return; }
	AInventoryActor* Item = MyInventory->CurrentInventory.Last();
	MyInventory->RemoveFromInventory(Item);
	FVector ItemOrigin;
	FVector ItemBounds;
	Item->GetActorBounds(false, ItemOrigin, ItemBounds);

	FTransform PutDownLocation = GetTransform() + FTransform(RootComponent->GetForwardVector() * ItemBounds.GetMax());
	Item->PutDown(PutDownLocation);
}

void AAventuraUSFX022026Pawn::TakeItem(AInventoryActor* InventoryItem)
{
	InventoryItem->PickUp();
	MyInventory->AddToInventory(InventoryItem);
}

void AAventuraUSFX022026Pawn::Tick(float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	//const FVector MoveDirection = FVector(0.f, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}

	// Create fire direction vector
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

	// Try and fire a shot
	FireShot(FireDirection);
}

void AAventuraUSFX022026Pawn::FireShot(FVector FireDirection)
{
    if (!bCanFire || FireDirection.IsNearlyZero() || !IsValid(EstadoDisparoActual))
        return;

    if (!EstadoDisparoActual->EjecutarDisparo(this, FireDirection)) return;

    bCanFire = false;
    if (UWorld* Mundo = GetWorld())
    {
        Mundo->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired,
            this, &AAventuraUSFX022026Pawn::ShotTimerExpired, FireRate, false);
    }
    if (FireSound)
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
}

bool AAventuraUSFX022026Pawn::PuedeCrearProyectiles(int32 Cantidad) const
{
    return Cantidad > 0 && ProyectilesActivos.Num() + Cantidad <= MaxProyectilesActivos;
}

AAventuraUSFX022026Projectile* AAventuraUSFX022026Pawn::GenerarProyectil(
    const FVector& Direccion, const FVector& OffsetAdicional)
{
    if (Direccion.IsNearlyZero() || !PuedeCrearProyectiles(1)) return nullptr;
    UWorld* Mundo = GetWorld();
    if (!Mundo) return nullptr;

    const FRotator Rotacion = Direccion.Rotation();
    const FVector Posicion = GetActorLocation() +
        Rotacion.RotateVector(GunOffset * 2.f + OffsetAdicional);

    FActorSpawnParameters Parametros;
    Parametros.Owner = this;
    Parametros.Instigator = this;
    Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AAventuraUSFX022026Projectile* Nuevo = Mundo->SpawnActor<AAventuraUSFX022026Projectile>(
        Posicion, Rotacion, Parametros);
    if (!IsValid(Nuevo)) return nullptr;

    Nuevo->GetProjectileMesh()->IgnoreActorWhenMoving(this, true);
    Nuevo->OnDestroyed.AddUniqueDynamic(this, &AAventuraUSFX022026Pawn::AlDestruirProyectil);
    ProyectilesActivos.Add(Nuevo);
    ProyectilActivo = Nuevo;
    return Nuevo;
}

bool AAventuraUSFX022026Pawn::CrearProyectil(
    const FVector& Direccion, const FVector& OffsetAdicional)
{
    return IsValid(GenerarProyectil(Direccion, OffsetAdicional));
}

bool AAventuraUSFX022026Pawn::CrearProyectilLaser(const FVector& Direccion)
{
    AAventuraUSFX022026Projectile* Laser = GenerarProyectil(Direccion, FVector::ZeroVector);
    if (!IsValid(Laser)) return false;

    // Mismo proyectil del proyecto; version alargada y rapida de Examen.rar.
    Laser->SetActorScale3D(FVector(3.f, 0.25f, 0.25f));
    if (UProjectileMovementComponent* Movimiento = Laser->GetProjectileMovement())
    {
        Movimiento->InitialSpeed = 5000.f;
        Movimiento->MaxSpeed = 5000.f;
        Movimiento->bShouldBounce = false;
        Movimiento->Velocity = Direccion.GetSafeNormal2D() * 5000.f;
    }
    return true;
}

void AAventuraUSFX022026Pawn::CambiarEstadoDisparo(UEstadoDisparo* NuevoEstado)
{
    if (IsValid(NuevoEstado)) EstadoDisparoActual = NuevoEstado;
}

void AAventuraUSFX022026Pawn::ActivarDisparoNormal()
{
    CambiarEstadoDisparo(EstadoNormal);
}

void AAventuraUSFX022026Pawn::ActivarDisparoDoble()
{
    CambiarEstadoDisparo(EstadoDoble);
}

void AAventuraUSFX022026Pawn::ActivarDisparoLaser()
{
    CambiarEstadoDisparo(EstadoLaser);
}

void AAventuraUSFX022026Pawn::CambiarEstadoDisparoAleatorio()
{
    if (!IsValid(EstadoNormal) || !IsValid(EstadoDoble) || !IsValid(EstadoLaser)) return;

    TArray<UEstadoDisparo*> Opciones;
    Opciones.Add(EstadoNormal);
    Opciones.Add(EstadoDoble);
    Opciones.Add(EstadoLaser);
    Opciones.Remove(EstadoDisparoActual); // no repetir el estado actual
    if (Opciones.Num() == 0) return;

    CambiarEstadoDisparo(Opciones[FMath::RandRange(0, Opciones.Num() - 1)]);
    const TCHAR* Nombre = EstadoDisparoActual == EstadoNormal ? TEXT("NORMAL") :
        EstadoDisparoActual == EstadoDoble ? TEXT("DOBLE") : TEXT("LASER");
    UE_LOG(LogTemp, Warning, TEXT("State: nuevo disparo %s"), Nombre);
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Cyan,
            FString::Printf(TEXT("STATE - DISPARO: %s"), Nombre));
}

void AAventuraUSFX022026Pawn::AlDestruirProyectil(AActor* ActorDestruido)
{
    AAventuraUSFX022026Projectile* Proyectil = Cast<AAventuraUSFX022026Projectile>(ActorDestruido);
    ProyectilesActivos.Remove(Proyectil);
    if (ProyectilActivo == Proyectil)
        ProyectilActivo = ProyectilesActivos.Num() ? ProyectilesActivos.Last() : nullptr;
}

void AAventuraUSFX022026Pawn::ShotTimerExpired()
{
    bCanFire = true;
}
