#pragma once

#include "CoreMinimal.h"
#include "Estrategia.h"
#include "EstrategiaKamikaze.generated.h"

UCLASS()
class AVENTURAUSFX022026_API UEstrategiaKamikaze : public UEstrategia
{
    GENERATED_BODY()

public:
    virtual void Ejecutar(AEnemigo* Enemigo, AActor* Objetivo, float DeltaTime) const override;
};
