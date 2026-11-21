// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Compuerta.h"
#include "PlataformaDestruible.generated.h"


class AAventuraUSFX022026Projectile;
/**
 * 
 */
UCLASS()
class AVENTURAUSFX022026_API APlataformaDestruible : public ACompuerta
{
	GENERATED_BODY()

public:
	APlataformaDestruible();

	UFUNCTION()
	void PlataformaAlColisionar(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
	
};
