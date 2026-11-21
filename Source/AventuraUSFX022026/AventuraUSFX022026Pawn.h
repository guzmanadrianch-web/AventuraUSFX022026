// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InventoryComponent.h"
#include "InventoryActor.h"
#include "AventuraUSFX022026Pawn.generated.h"

class AAventuraUSFX022026Projectile;
class UEstadoDisparo;
class UEstadoDisparoNormal;
class UEstadoDisparoDoble;
class UEstadoDisparoLaser;

UCLASS(Blueprintable)
class AAventuraUSFX022026Pawn : public APawn
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	/** The camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY()
	UInventoryComponent* MyInventory;
	UFUNCTION()
	void DropItem();
	UFUNCTION()
	void TakeItem(AInventoryActor* InventoryItem);
	UFUNCTION()
	virtual void NotifyHit(
		class UPrimitiveComponent* MyComp,
		AActor* Other, class UPrimitiveComponent* OtherComp,
		bool bSelfMoved, FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit
	) override;

public:
    AAventuraUSFX022026Pawn();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	FVector GunOffset;

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	/* Fire a shot in the specified direction */
    void FireShot(FVector FireDirection);

    // STATE: la nave delega el tipo de disparo al estado actual.
    bool CrearProyectil(const FVector& Direccion,
        const FVector& OffsetAdicional = FVector::ZeroVector);
    bool CrearProyectilLaser(const FVector& Direccion);
    bool PuedeCrearProyectiles(int32 Cantidad) const;

    void CambiarEstadoDisparo(UEstadoDisparo* NuevoEstado);
    void CambiarEstadoDisparoAleatorio();
    void ActivarDisparoNormal();
    void ActivarDisparoDoble();
    void ActivarDisparoLaser();

    UPROPERTY(EditAnywhere, Category = "State|Disparo", meta = (ClampMin = "2", UIMin = "2"))
    int32 MaxProyectilesActivos = 24;

	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

	UFUNCTION()
	void AlDestruirProyectil(AActor* ActorDestruido);

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;

private:

	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
    FTimerHandle TimerHandle_ShotTimerExpired;

    UPROPERTY(Transient)
    UEstadoDisparo* EstadoDisparoActual = nullptr;
    UPROPERTY(Transient)
    UEstadoDisparoNormal* EstadoNormal = nullptr;
    UPROPERTY(Transient)
    UEstadoDisparoDoble* EstadoDoble = nullptr;
    UPROPERTY(Transient)
    UEstadoDisparoLaser* EstadoLaser = nullptr;

    // Admite mas de una bala por disparo (Doble) y elimina referencias al destruirse.
    UPROPERTY(Transient)
    TArray<AAventuraUSFX022026Projectile*> ProyectilesActivos;

    AAventuraUSFX022026Projectile* GenerarProyectil(
        const FVector& Direccion, const FVector& OffsetAdicional);

public:
	/** Returns ShipMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

public:
	UPROPERTY()
	AAventuraUSFX022026Projectile* ProyectilActivo;
};
