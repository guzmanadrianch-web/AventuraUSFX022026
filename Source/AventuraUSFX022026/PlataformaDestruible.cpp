// Fill out your copyright notice in the Description page of Project Settings.


#include "PlataformaDestruible.h"
#include "AventuraUSFX022026Projectile.h"

APlataformaDestruible::APlataformaDestruible()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CompuertaMallaAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Trim.Shape_Trim'"));
	mallaCompuerta->SetStaticMesh(CompuertaMallaAsset.Object);
	mallaCompuerta->SetNotifyRigidBodyCollision(true);
	mallaCompuerta->OnComponentHit.AddDynamic(this, &APlataformaDestruible::PlataformaAlColisionar);
}

void APlataformaDestruible::PlataformaAlColisionar(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AAventuraUSFX022026Projectile* Proyectil =
		Cast<AAventuraUSFX022026Projectile>(OtherActor);

	if (Proyectil != nullptr)
	{
		Destroy();
	}
}
