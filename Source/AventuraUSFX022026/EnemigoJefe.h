// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemigo.h"
#include "EnemigoJefe.generated.h"

class AEnemigoJefe;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventoJefeMuerto, AEnemigoJefe*, JefeMuerto);
UCLASS()
class AVENTURAUSFX022026_API AEnemigoJefe : public AEnemigo
{
	GENERATED_BODY()
	
public:
    AEnemigoJefe();

	UPROPERTY(BlueprintAssignable, Category = "Observer")
	FEventoJefeMuerto OnJefeMuerto;

protected:
    virtual void Morir() override;
};
