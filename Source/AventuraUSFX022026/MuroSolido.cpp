// Fill out your copyright notice in the Description page of Project Settings.


#include "MuroSolido.h"

AMuroSolido::AMuroSolido()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MuroMallaAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	mallaMuro->SetRelativeScale3D(FVector(3.0f, 0.5f, 1.0f));
	mallaMuro->SetStaticMesh(MuroMallaAsset.Object);
}
