// Fill out your copyright notice in the Description page of Project Settings.

/*
*	This is the glowing red/green/blue square that hovers above the board in RTS games,
*	indicating how many blocks away your movement or attack can be.
*
*	Includes Cursor events and Enum indicating tile state (blocked, walkable, etc)
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h" //static mesh
#include "GameFramework/Actor.h"
//#include "ToolsHeader.h"
#include "GroundTile.generated.h"

UENUM(BlueprintType)
enum GroundType
{
	Walkable,
	Blocked,
	Obstructed
};

UCLASS()
class RTS_API AGroundTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGroundTile();

	//The actual ground block itself. What the player sees in game.
	UStaticMeshComponent* Ground_Mesh;

	//AR Mesh. Invisible to player unless trying to move or attack
	//The block will be red or green depending on if you can move to this space
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* AR_Mesh;

	//how high above the actor should the hidden AR block be
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		float AR_Height = 110;

	//Used for changing the color of the AR_Mesh
	UPROPERTY()
		UMaterialInstanceDynamic* dynamicMat;

	//set up the options for color change here
	//Colors are picked in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Tile Settings")
		FLinearColor walkable_color = FLinearColor(0,0,0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Tile Settings")
		FLinearColor walk_highlight = FLinearColor(0, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Tile Settings")
		FLinearColor blocked_color = FLinearColor(0, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Tile Settings")
		FLinearColor obstructed_color = FLinearColor(0, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Tile Settings")
		FLinearColor attack_color = FLinearColor(0, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Tile Settings")
		FLinearColor outOfBounds_color = FLinearColor(0, 0, 0);

	//Define this ground tile (walkable, obstructed, etc)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GroundType)
		TEnumAsByte<GroundType> groundType;

	//AR Mesh is invisible by default
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool tileIsOn = false;

	// Used by Dijkstra's algorithm
	// tracks where the tile is, if we've considered it, etc
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GroundType) //has been added to queue
		bool addedToEvalQueue = false;  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GroundType) //already went through the queue
		bool evaluated = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GroundType) //place in xy grid
		int posInArray=0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GroundType) //how many spaces from player
		int distFromPlayerTile = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GroundType)
		AGroundTile* parentTile = nullptr; //for algorithm, which tile am I evaluated from


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Change the tile color and its visibility
	void ToggleTile(bool changeState, bool turnOn);

	//Change the ground type
	void ChangeTileType(TEnumAsByte<GroundType> newType);

	//called during grid algorithm
	GroundType EvaluateTile();

	//Mouse Over events
	UFUNCTION()
		void CustomOnBeginMouseOver(UPrimitiveComponent* TouchedComponent);
	UFUNCTION()
		void CustomOnEndMouseOver(UPrimitiveComponent* TouchedComponent);
};
