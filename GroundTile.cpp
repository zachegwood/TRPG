// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundTile.h"

// Sets default values
AGroundTile::AGroundTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialize
	AR_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("AR_Mesh");
	Ground_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Ground_Mesh");

	//Setup Hierarchy
	RootComponent = Ground_Mesh;
	AR_Mesh->SetupAttachment(Ground_Mesh);
}

// Called when the game starts or when spawned
void AGroundTile::BeginPlay()
{
	Super::BeginPlay();	

	//Setup MouseOver events for cursor
	AR_Mesh->OnBeginCursorOver.AddDynamic(this, &AGroundTile::CustomOnBeginMouseOver);
	AR_Mesh->OnEndCursorOver.AddDynamic(this, &AGroundTile::CustomOnEndMouseOver);

	//Set tiles to their default colors based on Editor-selected groundType
	ToggleTile(true, false);
}

// Called every frame
void AGroundTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGroundTile::CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent)
{	
	//Set up logic for different scenarios. 
	// Ex,  1) Trying to walk to a space, is this walkable and in range?
	//		2) Trying to attack, is this obstructed and in range?

	if (tileIsOn == true)
	{
		if (groundType == Walkable)
		{
			// "Can we move to this square?"

			//Change Color of Tile
			dynamicMat->SetVectorParameterValue(FName("AR_color"), walk_highlight);
		}
	}
}

void AGroundTile::CustomOnEndMouseOver(UPrimitiveComponent* TouchedComponent)
{
	//Reset color of tile after we're not mouse-overing it anymore

	if (tileIsOn == true)
	{
		if (groundType == Walkable)
		{
			dynamicMat->SetVectorParameterValue(FName("AR_color"), walkable_color);
		}
	}
}

GroundType AGroundTile::EvaluateTile()
{
	return groundType;
}

//When a tile changes state. 
//ie, A player moves onto it, or an enemy moves off of it
void AGroundTile::ChangeTileType(TEnumAsByte<GroundType> newType) 
{
	groundType = newType;

	//Setup Dynamic Instance of Mat for color change
	dynamicMat = AR_Mesh->CreateAndSetMaterialInstanceDynamic(0);

	if (dynamicMat != nullptr) {	//Error prevention C++

		//Change color based on the tile's state
		switch (groundType) {

		case GroundType::Walkable:
			dynamicMat->SetVectorParameterValue(FName("AR_color"), walkable_color);
			break;
		case GroundType::Blocked:
			dynamicMat->SetVectorParameterValue(FName("AR_color"), blocked_color);
			break;
		case GroundType::Obstructed:
			dynamicMat->SetVectorParameterValue(FName("AR_color"), obstructed_color);
			break;
		default:
			break;
		}
	}
}

//Make the tile visible or invisible
void AGroundTile::ToggleTile(bool changeState, bool turnOn)
{
	//Change Color based on the tile's current condition
	if (changeState == true)
	{
		//Setup for color change
		dynamicMat = AR_Mesh->CreateAndSetMaterialInstanceDynamic(0);

		if (dynamicMat != nullptr) {//Error prevention C++

			//Change color based on the tile's state
			switch (groundType) {

			case GroundType::Walkable:
				dynamicMat->SetVectorParameterValue(FName("AR_color"), walkable_color);
				break;
			case GroundType::Blocked:
				dynamicMat->SetVectorParameterValue(FName("AR_color"), blocked_color);
				break;
			case GroundType::Obstructed:
				dynamicMat->SetVectorParameterValue(FName("AR_color"), obstructed_color);
				break;
			default:
				break;
			}
		}
	}

	//Change Visibility
	if (turnOn == true)
	{
		AR_Mesh->SetVisibility(true);
		tileIsOn = true;
	}
	else 
	{
		AR_Mesh->SetVisibility(false);
		tileIsOn = false;
	}
		
}
