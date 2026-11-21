// Fill out your copyright notice in the Description page of Project Settings.


#include "MuroDestruible.h"
#include "AventuraUSFX022026Projectile.h"

AMuroDestruible::AMuroDestruible()
{
	mallaMuro->SetNotifyRigidBodyCollision(true);
	mallaMuro->OnComponentHit.AddDynamic(this, &AMuroDestruible::AlColisionar);

}

void AMuroDestruible::AlColisionar(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AAventuraUSFX022026Projectile* Proyectil =
		Cast<AAventuraUSFX022026Projectile>(OtherActor);

	if (Proyectil != nullptr)
	{
		Destroy();
	}
}
