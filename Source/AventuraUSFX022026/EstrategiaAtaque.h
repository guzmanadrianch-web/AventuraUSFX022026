#pragma once

#include "CoreMinimal.h"
#include "Estrategia.h"
#include "EstrategiaAtaque.generated.h"

// Persigue hasta alcanzar una distancia de ataque, entonces dispara.
UCLASS()
class AVENTURAUSFX022026_API UEstrategiaAtaque : public UEstrategia
{
    GENERATED_BODY()

public:
    virtual void Ejecutar(AEnemigo* Enemigo, AActor* Objetivo, float DeltaTime) const override;
};
