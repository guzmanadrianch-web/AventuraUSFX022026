// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Muro.h"
#include "MuroDestruible.generated.h"


class AAventuraUSFX022026Projectile;
/**
 * 
 */
UCLASS()
class AVENTURAUSFX022026_API AMuroDestruible : public AMuro
{
	GENERATED_BODY()

public:
	AMuroDestruible();

	UFUNCTION()
	void AlColisionar(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
	
};
