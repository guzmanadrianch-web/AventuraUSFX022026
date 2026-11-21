// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemigoJefe.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AEnemigoJefe::AEnemigoJefe()
{
    VidaMaxima = 180.f; // Permite visualizar las fases de Strategy.
    velocidad = 300.f;
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MallaJefe(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Melee_Dusk.Minion_Lane_Melee_Dusk'"));

    if (MallaJefe.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(MallaJefe.Object);
    }
}

void AEnemigoJefe::Morir()
{
    UE_LOG(LogTemp, Warning, TEXT("Observer: murio el jefe; notificando suscriptores"));
	OnJefeMuerto.Broadcast(this);
    Super::Morir();
}
