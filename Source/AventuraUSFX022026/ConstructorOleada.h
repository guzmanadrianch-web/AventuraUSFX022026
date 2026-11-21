#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "ConstructorOleada.generated.h"

class AEnemigo;
class AEnemigoJefe;
class AOleada;
class UWorld;

// Builder de Unreal: configura y devuelve un AOleada completo.
// No depende de ninguna otra fabrica o patron.
UCLASS()
class AVENTURAUSFX022026_API UConstructorOleada : public UObject
{
    GENERATED_BODY()

public:
    void AgregarEnemigos(TSubclassOf<AEnemigo> Clase, int32 Cantidad);
    void EstablecerJefe(TSubclassOf<AEnemigoJefe> Clase);
    void EstablecerRadio(float NuevoRadio);
    AOleada* Construir(UWorld* Mundo, const FVector& Centro);
    void Reiniciar();

private:
    UPROPERTY()
    TArray<TSubclassOf<AEnemigo>> ClasesEnemigos;

    UPROPERTY()
    TSubclassOf<AEnemigoJefe> ClaseJefe;

    UPROPERTY()
    float Radio = 500.f;
};
