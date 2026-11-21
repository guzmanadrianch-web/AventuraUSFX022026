#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Estrategia.generated.h"

class AEnemigo;
class AActor;

// STRATEGY: contrato comun de los comportamientos de un enemigo.
// Las estrategias no guardan estado de cada actor: se usan como CDO de Unreal.
UCLASS(Abstract)
class AVENTURAUSFX022026_API UEstrategia : public UObject
{
    GENERATED_BODY()

public:
    virtual void Ejecutar(AEnemigo* Enemigo, AActor* Objetivo, float DeltaTime) const;
};
