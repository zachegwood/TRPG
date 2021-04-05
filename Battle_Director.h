// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GroundTile.h"
#include "Battle_Director.generated.h"

UCLASS()
class RTS_API ABattle_Director : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattle_Director();

	//What Tile object are we spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Tiles")
		TSubclassOf<AGroundTile> groundTileObj;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Tiles")
	int gridSize = 0;

	//How large is the grid we'll be spawning on?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Tiles")
		int X_gridSize = 1;
	//	int FMath::Clamp(X_gridSize, 1, 100);
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground Tiles")
		int Y_gridSize = 1;
	//	FMath::Clamp(int Y_gridSize, 1, 100);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ground Tiles")
		float floorHeight = 100;  //this value is hard-coded 

	//Player stuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		FVector playerSpawnXY = FVector(2, 2, 0); // CAN be zero z bc we set floor height! Will be converted to meters in cpp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		AActor* ourPawn = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		bool isPlayerTurn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		int playerMovementMax;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		FVector playerTileLoc;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		TArray<AGroundTile*> arrayOfTiles;

	void SpawnTilesInGrid(); //setup
	void SpawnPlayer();      //setup

	void ShowMovementTiles();  //actual gameplay; player turn begins	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;	
};
