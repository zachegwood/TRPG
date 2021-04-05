// Fill out your copyright notice in the Description page of Project Settings.

/*

	Uses a custom Dijkstra's algorithm
	To evaluate grid movement and pathfinding
	On a 2-D grid built from a 1-D array

	For testing, builds a randomized grid of tiles,
	25% obstructed


*/

#include "Battle_Director.h"

// Sets default values
ABattle_Director::ABattle_Director()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABattle_Director::BeginPlay()
{
	Super::BeginPlay();

	SpawnTilesInGrid();  //Also created the grid array

	SpawnPlayer();

	ShowMovementTiles();  // as if player just clicked "move"	
}

// Called every frame
void ABattle_Director::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


	/*

		RULES FOR MOVING IN THE GRID PATTERN THROUGH THIS 1D ARRAY

		UNREAL ENGINE : X = FORWARDS / BACK, Y = SIDE / SIDE

		Assume Player is on piece "n"

		MOVE UP : (Y_gridSize*x)+n
		MOVE DOWN : n-(Y_gridSize*x)
		MOVE RIGHT : n + (1*x)
		MOVE LEFT : n-(x*1)

	*/

void ABattle_Director::ShowMovementTiles()
{
	//tell the system that the player is currently in control
	isPlayerTurn = true;

	//How far can we move? IE, how many MovePts do we have?
	int movementPointsCurr = playerMovementMax; //from header file

	//Tiles used in the Queue below
	AGroundTile* queueTile = nullptr; //the tile we pull out of the queue
	AGroundTile* childTile = nullptr; //the tile we're evaluating atm
	AGroundTile* playerTile = arrayOfTiles[(playerTileLoc.X * Y_gridSize) + playerTileLoc.Y];  //what tile is the player on?
	playerTile->parentTile = playerTile;  //set as own parent
	playerTile->distFromPlayerTile = 0;

	//==================================== THE TILE QUEUE - EVALUATING NEIGHBORS =====================

	TQueue<AGroundTile*> tileQueue;
	tileQueue.Enqueue(playerTile); //begin queue with only Player Tile

	int queueCount = 1;

	int pN = (((playerTile->GetActorLocation().X / 100) * Y_gridSize) +
		(playerTile->GetActorLocation().Y / 100));

	while (tileQueue.IsEmpty() == false)
	{
		/*
			queueTile = the tile we pulled out of the Queue
			parentTile = the tile that originally put this tile into the queue
			childTile = the tile we're evaluating and might put into the queue
		*/

		//Pull out the next tile and update it's variables
		tileQueue.Dequeue(queueTile);
		movementPointsCurr = playerMovementMax - queueTile->distFromPlayerTile; //this tile costs one more movement pt than its parent
		queueTile->evaluated = true;
		queueCount--;

		//n is now using the /100 amount, so the xy is in grid not meters
		int n = (((queueTile->GetActorLocation().X / 100) * Y_gridSize) +
			(queueTile->GetActorLocation().Y / 100));  //math rep of this tile loc in array, used below

		 //don't turn the player tile green
		if (queueTile != playerTile)
			queueTile->ToggleTile(true, true);

		if (queueTile == playerTile)
			queueTile->ChangeTileType(Walkable);

		//If this is an obstructed tile
		if (queueTile->groundType == Blocked ||
			queueTile->groundType == Obstructed)
		{
			queueTile->ChangeTileType(Obstructed);
			queueTile->ToggleTile(true, true);
			movementPointsCurr = 0; //this prevents the queue from examining neighbors of obstructed tiles
		}

		//If we can afford to move beyond this tile,
		//Look at its neighbors. If they're available, add them to the queue.
		if (movementPointsCurr > 0)
		{
			//moving to the right = n+  and y+

			//== RIGHT ==
			if ((n + 1) < (gridSize) &&
				((n + 1) % Y_gridSize) != 0) { //Error prevention -- Array Edition

				//Evaluate Right
				childTile = arrayOfTiles[n + (1)];

				//we haven't tried to or succeeded at considering this tile
				if (childTile->evaluated == false &&
					childTile->addedToEvalQueue == false) {

					//set the parent 
					childTile->parentTile = queueTile;
					childTile->distFromPlayerTile = queueTile->distFromPlayerTile + 1;

					childTile->addedToEvalQueue = true;
					tileQueue.Enqueue(childTile);

					queueCount++;
				}
			}

			//== LEFT ==
			if (n - 1 >= 0 &&
				((n) % Y_gridSize) != 0) { //Error prevention -- Array Edition

				//Evaluate Left
				childTile = arrayOfTiles[n - (1)];

				//we haven't tried to or succeeded at considering this tile
				if (childTile->evaluated == false &&
					childTile->addedToEvalQueue == false) {

					//set the parent 
					childTile->parentTile = queueTile;
					childTile->distFromPlayerTile = queueTile->distFromPlayerTile + 1;

					childTile->addedToEvalQueue = true;
					tileQueue.Enqueue(childTile);

					queueCount++;
				}
			}

			//== UP ==
			if ((n + Y_gridSize) <= (gridSize)) {  //Error prevention -- Array Edition

				//Evaluate Above
				childTile = arrayOfTiles[(Y_gridSize)+n];

				//we haven't tried to or succeeded at considering this tile
				if (childTile->evaluated == false &&
					childTile->addedToEvalQueue == false) {

					//set the parent 
					childTile->parentTile = queueTile;
					childTile->distFromPlayerTile = queueTile->distFromPlayerTile + 1;

					childTile->addedToEvalQueue = true;
					tileQueue.Enqueue(childTile);

					queueCount++;
				}
			}

			//== DOWN ==
			if ((n - Y_gridSize) >= 0) {  //Error prevention -- Array Edition

				//Evaluate Below
				childTile = arrayOfTiles[n - (Y_gridSize)];

				//we haven't tried to or succeeded at considering this tile
				if (childTile->evaluated == false &&
					childTile->addedToEvalQueue == false) {

					//set the parent 
					childTile->parentTile = queueTile;
					childTile->distFromPlayerTile = queueTile->distFromPlayerTile + 1;

					childTile->addedToEvalQueue = true;
					tileQueue.Enqueue(childTile);

					queueCount++;
				}
			}
		}
	}
}
	

//====================  SCENE SETUP FOR TEST LEVEL  ======================\\


void ABattle_Director::SpawnTilesInGrid() 
{
	//Array that holds our Ground Tiles.
	//Size is hard-coded. If our ground tiles count is larger than the array,
	//the loop below will break and print a warning message.
	//If we didn't have that break, UE4 would crash

	arrayOfTiles.Init(nullptr, 1000);  //fill with null pointers

	//Error Prevention
	gridSize = (X_gridSize * Y_gridSize); //for error checking
	bool breakThisLoop = false; //to break loop if array is too small	

	//Error prevention
	if (gridSize >= arrayOfTiles.Num()) {

		breakThisLoop = true;

		UE_LOG(LogTemp, Error, TEXT("Grid array is smaller than requested map size. Breaking map-spawning loop to prevent UE4 crash."));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Grid Array too small!!!!!!!!!!!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Breaking Loop to prevent UE4 crash."));
		return;
	}	

	//NOTE IN UNREAL ENGINE:
	//X is forwards/backwards, Y is right/left. 

	for (int x = 0; x < X_gridSize; x++)
	{
		//error prevention
		if (breakThisLoop == true)
			break;

		for (int y = 0; y < Y_gridSize; y++)
		{
			//Allows our 1D array to act like a 2D array
			int currentGridIndex = (x * Y_gridSize) + y;

			//For naming the spawned actor later
			FString xName = FString::FromInt(x);
			FString yName = FString::FromInt(y);
			FString tileName = "Tile_x" + xName + "_y" + yName;

			//find coordinates to spawn based on x and y of for loop
			//the location to spawn is the float-converted x variable times 100 (1 unit)
			float spawnX = float(x) * 100;
			float spawnY = float(y) * 100;

			FVector Location(spawnX, spawnY, -floorHeight); //center lowered by 100, so a 100m actor spawns flush with floor
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			FActorSpawnParameters spawnParams;

			//Spawn here 
			AGroundTile* thisTile = GetWorld()->SpawnActor<AGroundTile>(groundTileObj, Location, Rotation, spawnParams);

			//arrayOfTiles[currentGridIndex].Add(thisTile);
			arrayOfTiles[currentGridIndex] = thisTile;

			//Random chance to set this tile as obstructed, just for testing.
			float chance = FMath::RandRange(0, 100);
			if (chance > 80)
				thisTile->ChangeTileType(Obstructed);

			//Organize World outliner
			thisTile->SetFolderPath("myTiles");
		}
	}
	
	//if we didn't force the loop to break, print that the tiles are placed.
	if (breakThisLoop == false)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Map spawned"));	
}

void ABattle_Director::SpawnPlayer()
{
	if (ourPawn)
	{
		//multiply grid placement by grid tile size so we place the player at the right spot
		FVector playerSpawnMeters(playerSpawnXY.X * 100, playerSpawnXY.Y * 100, playerSpawnXY.Z);
		ourPawn->SetActorLocation(playerSpawnMeters);

		playerTileLoc = FVector(playerSpawnXY.X, playerSpawnXY.Y, floorHeight);
	}
}