#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AventuraUSFX022026GameMode.generated.h"

class ACompuerta;
class AMuroDestruible;
class AGestorJuegoFacade;

UCLASS(MinimalAPI)
class AAventuraUSFX022026GameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AAventuraUSFX022026GameMode();

    
    UPROPERTY(EditDefaultsOnly, Category = "Patrones de diseno")
    bool bActivarDemostracionPatrones;

    UPROPERTY()
    ACompuerta* objetoSpawneado;

    UPROPERTY()
    AMuroDestruible* muroDestruido;

    UPROPERTY()
    TArray<ACompuerta*> Plataformas;

    void DestruirCompuerta();
    void EliminarPlataforma();
    void EmpezarMovimiento();
    void DetenerMovimiento();

protected:
    virtual void BeginPlay() override;

    void IniciarSistemaPatrones();

private:
    UPROPERTY()
    AGestorJuegoFacade* Facade;
};
