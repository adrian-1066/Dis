// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"

#include <SceneExport.h>

#include "EngineUtils.h"
//#include "Math/UnrealMathUtility.h"
// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	RoomsInGrid = 0;
	RoomsMerged = 0;
	BasePuzzleChance = 30.0f;
	BaseTreasureChance = 15.0f;
	BaseFightChance = 55.0f;

	CurrentPuzzleChance = BasePuzzleChance;
	CurrentTreasureChance = BaseTreasureChance;
	CurrentFightChance = BaseFightChance;
	Super::BeginPlay();
	SetGridUp();
	SetCellStrAndType();
	SetCellhood();
	InitialStrCheck();
	SecondStrCheck();
	//InitialStrCheck();
	CreateRooms();
	RoomCleanUp();
	SmallRoomCleanUp();
	//RoomCleanUp();
	RoomAdjacentCheck();
	RoomNeighbourUpdate();
	MinRoomAdjacentCleanUp();
	RoomNeighbourUpdate();
	RoomAdjacentCheck();
	//ReveseMinRoomCleanUp();
	RoomNeighbourUpdate();

	RoomWallSet();
	AddRoomsToIgnoreList();
	SetRoomPos();
	//UE_LOG(LogTemp, Warning, TEXT("There are : %d"),RoomsInGrid);
	//UE_LOG(LogTemp, Warning, TEXT("There are this many rooms : %d"),DungeonRooms.Num());
	//UE_LOG(LogTemp, Warning, TEXT("these many rooms have merged : %d"),RoomsMerged);
	PickStartAndEndRooms();
	StartPathFinding();
	for(int i  = 0; i < DungeonRooms.Num();i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Room at index %d has this many adj rooms %d"),i, DungeonRooms[i].AdjRoomIDs.Num());
	}
	
	/*
	for(int i = 0; i < DungeonRooms.Num(); i++)
	{
		RoomMergedAdjChec(i);
		UE_LOG(LogTemp, Warning, TEXT("Room: %d has: %d adjacent rooms"),i, DungeonRooms[i].NumOfAdjacentRooms);
		
	}*/
	//RoomNeighbourUpdate();
/*
	for(int x = 0; x < TheGrid.Num(); x++)
	{
		for (int y = 0; y < TheGrid.Num(); y++)
		{
			for(int i = 0; i < TheGrid[x][y].Neighbours.Num(); i++)
			{
				if(TheGrid[x][y].Neighbours[i].RoomID != TheGrid[TheGrid[x][y].Neighbours[i].CellPos.X][TheGrid[x][y].Neighbours[i].CellPos.Y].RoomID)
				{
					UE_LOG(LogTemp, Warning, TEXT("Cell %d %d Has the roomID of %d its listed Neighbour %d has the RoomID %d while the Grid version has the ID of %d "),x,y,TheGrid[x][y].RoomID,i,TheGrid[x][y].Neighbours[i].RoomID, TheGrid[TheGrid[x][y].Neighbours[i].CellPos.X][TheGrid[x][y].Neighbours[i].CellPos.Y].RoomID);
				}
			}
			
		}
	}*/
/*
	for(int i = 0; i < DungeonRooms.Num(); i++)
	{
		for(int q = 0; q < DungeonRooms[i].Room.Num();q++)
		{
			if(TheGrid[DungeonRooms[i].Room[q].CellPos.X][DungeonRooms[i].Room[q].CellPos.Y].RoomID != DungeonRooms[i].RoomID)
			{
				UE_LOG(LogTemp, Warning, TEXT("cell %d %d does have the same room Id as room %d"),DungeonRooms[i].Room[q].CellPos.X,DungeonRooms[i].Room[q].CellPos.Y,i); 
			}
		}
	}*/
	/*
	for(int i = 0; i < DungeonRooms.Num(); i++)
	{
		for(int p  = 0; p < DungeonRooms[i].Room.Num(); p++)
		{
			if(DungeonRooms[i].Room[p].IsWall)
			{
				UE_LOG(LogTemp, Warning, TEXT("room %d Cell %d is a wall"),i, p); 
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("room %d Cell %d is NOT wall"),i, p); 
			}
		}
	}*/
	CleanWAllsBetweenRooms();
	SetRoomTypes();
	SpawnCubes();
	SpawnWalls();
	UE_LOG(LogTemp, Warning, TEXT("the test number is saying that there should be %d number of rooms in the stack, there are %d in the stack"), TestNumber, PathStack.Num());
	
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADungeonGenerator::SetGridUp()
{
	TheGrid.SetNum(GridSize);
	for(int i = 0; i < GridSize; i++)
	{
		TheGrid[i].SetNum(GridSize);
		
	}

	for(int x = 0; x < GridSize;x++)
	{
		for(int y = 0; y < GridSize; y++)
		{
			TheGrid[x][y].CellStrength = 0;
			TheGrid[x][y].CellRoomType = 0;
			TheGrid[x][y].IsInRoom = false;
			TheGrid[x][y].IsWall = false;
		}
	}
}

void ADungeonGenerator::SetCellStrAndType()
{
	for(int x = 0; x < GridSize; x++)
	{
		for(int y = 0; y < GridSize; y++)
		{
			
			TheGrid[x][y].CellStrength = GetRandomIntInRange(6, 26);
			TheGrid[x][y].CellRoomType = GetRandomIntInRange(1,8);
			TheGrid[x][y].CellPos = FIntVector2(x,y);
			//UE_LOG(LogTemp, Warning, TEXT("the room str is: %d"), TheGrid[x][y].CellStrength);
		}
	}
}

void ADungeonGenerator::SetCellhood()
{
	int TempAmount = 0;
	for(int x = 0; x < GridSize; x++)
	{
		for(int y = 0; y < GridSize; y++)
		{
			TempAmount = 0;
			if(y - 1 >= 0)
			{
				TempAmount++;
				//TheGrid[x][y].Neighbours.Add(TheGrid[x][y-1]);
			}
			
			if(y + 1 <= GridSize-1)
			{
				TempAmount++;
				//TheGrid[x][y].Neighbours.Add(TheGrid[x][y+1]);
			}
		

			if(x - 1 >= 0)
			{
				TempAmount++;
				//TheGrid[x][y].Neighbours.Add(TheGrid[x-1][y]);
			}
			

			if(x + 1 <= GridSize-1)
			{
				TempAmount++;
				//TheGrid[x][y].Neighbours.Add(TheGrid[x+1][y]);
			}
			
			SetNeighborhood(TempAmount,FIntVector2(x,y));
		}
	}
}

void ADungeonGenerator::SetNeighborhood(int Value,  FIntVector2 Cell)
{
	//TheGrid[Cell.X][Cell.Y].Neighbours.SetNum(0);
	//UE_LOG(LogTemp, Warning, TEXT("Array size at x y is : %d %d %d"),Cell.X, Cell.Y, TheGrid[Cell.X][Cell.Y].Neighbours.Num());
	//TheGrid[Cell.X][Cell.Y].Neighbours.SetNum(Value);
	FDungeonCellNeighb TempOne;
	FDungeonCellNeighb TempTwo;
	FDungeonCellNeighb TempThree;
	FDungeonCellNeighb TempFour;
	int index = 0;

	//UE_LOG(LogTemp, Warning, TEXT("Value at cell is: %d %d %d"),Value ,Cell.X, Cell.Y);
	if(Cell.X == 0 && Cell.Y == 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Array size at x+1 y+1 is : %d %d %d"),Cell.X+1, Cell.Y+1, TheGrid[Cell.X][Cell.Y].Neighbours.Num());
	}
	
	if(Cell.Y - 1 >= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("y - 1"));
		TempOne.CellPos.X = Cell.X;
		TempOne.CellPos.Y = Cell.Y - 1;
		TempOne.CellStrength = TheGrid[Cell.X][Cell.Y-1].CellStrength;
		TempOne.CellRoomType = TheGrid[Cell.X][Cell.Y-1].CellRoomType;
		TempOne.IsWall = TheGrid[Cell.X][Cell.Y-1].IsWall;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Insert(TempOne, index);
		TheGrid[Cell.X][Cell.Y].Neighbours.Add(TempOne);
		index++;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Add(TheGrid[Cell.X][Cell.Y-1]);
	}
	if(Cell.Y + 1 <= GridSize-1)
	{
		//UE_LOG(LogTemp, Warning, TEXT("y + 1"));
		TempTwo.CellPos.X = Cell.X;
		TempTwo.CellPos.Y = Cell.Y + 1;
		TempTwo.CellStrength = TheGrid[Cell.X][Cell.Y+1].CellStrength;
		TempTwo.CellRoomType = TheGrid[Cell.X][Cell.Y+1].CellRoomType;
		TempTwo.IsWall = TheGrid[Cell.X][Cell.Y+1].IsWall;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Insert(TempTwo, index);
		TheGrid[Cell.X][Cell.Y].Neighbours.Add(TempTwo);
		index++;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Add(TheGrid[Cell.X][Cell.Y+1]);
	}
	if(Cell.X - 1 >= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("x - 1"));
		TempThree.CellPos.X = Cell.X-1;
		TempThree.CellPos.Y = Cell.Y;
		TempThree.CellStrength = TheGrid[Cell.X-1][Cell.Y].CellStrength;
		TempThree.CellRoomType = TheGrid[Cell.X-1][Cell.Y].CellRoomType;
		TempThree.IsWall = TheGrid[Cell.X-1][Cell.Y].IsWall;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Insert(TempThree, index);
		TheGrid[Cell.X][Cell.Y].Neighbours.Add(TempThree);
		index++;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Add(TheGrid[Cell.X-1][Cell.Y]);
	}
	if(Cell.X + 1 <= GridSize-1)
	{
		//UE_LOG(LogTemp, Warning, TEXT("x + 1"));
		TempFour.CellPos.X = Cell.X+1;
		TempFour.CellPos.Y = Cell.Y;
		TempFour.CellStrength = TheGrid[Cell.X+1][Cell.Y].CellStrength;
		TempFour.CellRoomType = TheGrid[Cell.X+1][Cell.Y].CellRoomType;
		TempFour.IsWall = TheGrid[Cell.X+1][Cell.Y].IsWall;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Insert(TempFour, index);
		TheGrid[Cell.X][Cell.Y].Neighbours.Add(TempFour);
		index++;
		//TheGrid[Cell.X][Cell.Y].Neighbours.Add(TheGrid[Cell.X+1][Cell.Y]);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Array size is now at : %d"), TheGrid[Cell.X][Cell.Y].Neighbours.Num());
}

void ADungeonGenerator::InitialStrCheck()
{
	for(int x = 0; x < GridSize; x++)
	{
		for(int y = 0; y < GridSize; y++)
		{
			for(int i = 0; i < TheGrid[x][y].Neighbours.Num();i++)
			{
				//UE_LOG(LogTemp, Warning, TEXT("x y has neighb : %d %d %d"),x,y,TheGrid[x][y].Neighbours.Num());
				int tempX = TheGrid[x][y].Neighbours[i].CellPos.X;
				int tempY = TheGrid[x][y].Neighbours[i].CellPos.Y;
				//UE_LOG(LogTemp, Warning, TEXT("str check at x y neighb : %d %d %d"), x, y, i);
				//UE_LOG(LogTemp, Warning, TEXT("x y tempX temp Y : %d %d %d %d"), x, y, tempX, tempY);
				//bool IsStrong = StrCheck(x,y,tempX,tempY);

				if(TheGrid[x][y].CellStrength >= TheGrid[x][y].Neighbours[i].CellStrength)
				{
					TheGrid[tempX][tempY].CellRoomType = TheGrid[x][y].CellRoomType;
					TheGrid[tempX][tempY].CellStrength = TheGrid[x][y].CellStrength - 1;
				}
				else
				{
					TheGrid[x][y].CellRoomType = TheGrid[tempX][tempY].CellRoomType;
					TheGrid[x][y].CellStrength = TheGrid[tempX][tempY].CellStrength - 1;
				}
			}
				

			
		}
	}
}

void ADungeonGenerator::SecondStrCheck()
{
	for(int x = GridSize-1; x >= 0; x--)
	{
		for(int y = GridSize-1; y >= 0; y--)
		{
			for(int i = 0; i < TheGrid[x][y].Neighbours.Num();i++)
			{
				//UE_LOG(LogTemp, Warning, TEXT("x y has neighb : %d %d %d"),x,y,TheGrid[x][y].Neighbours.Num());
				int tempX = TheGrid[x][y].Neighbours[i].CellPos.X;
				int tempY = TheGrid[x][y].Neighbours[i].CellPos.Y;
				//UE_LOG(LogTemp, Warning, TEXT("str check at x y neighb : %d %d %d"), x, y, i);
				//UE_LOG(LogTemp, Warning, TEXT("x y tempX temp Y : %d %d %d %d"), x, y, tempX, tempY);
				//bool IsStrong = StrCheck(x,y,tempX,tempY);

				if(TheGrid[x][y].CellStrength >= TheGrid[x][y].Neighbours[i].CellStrength)
				{
					TheGrid[tempX][tempY].CellRoomType = TheGrid[x][y].CellRoomType;
					TheGrid[tempX][tempY].CellStrength = TheGrid[x][y].CellStrength - 1;
				}
				else
				{
					TheGrid[x][y].CellRoomType = TheGrid[tempX][tempY].CellRoomType;
					TheGrid[x][y].CellStrength = TheGrid[tempX][tempY].CellStrength - 1;
				}
			}
				

			
		}
	}
}

void ADungeonGenerator::CreateRooms()
{
	
	for(int x = 0; x < GridSize; x++)
	{
		for(int y = 0; y < GridSize; y++)
		{
			
			if(RoomsInGrid == 0)
			{
				
				CreateNewRoom(&TheGrid[x][y]);
				TheGrid[x][y].IsInRoom = true;
				//UE_LOG(LogTemp, Warning, TEXT("creaing new room for as there are no rooms : %d %d "),x, y);
				continue;
			}

			for(int i = 0; i < TheGrid[x][y].Neighbours.Num(); i++)
			{
				int tempX = TheGrid[x][y].Neighbours[i].CellPos.X;
				int tempY = TheGrid[x][y].Neighbours[i].CellPos.Y;
				if(TheGrid[tempX][tempY].IsInRoom)
				{
					if(TheGrid[tempX][tempY].CellRoomType == TheGrid[x][y].CellRoomType)
					{
						AddCellToRoom(TheGrid[tempX][tempY].RoomID, &TheGrid[x][y]);
						TheGrid[x][y].RoomID = TheGrid[tempX][tempY].RoomID;
						//UE_LOG(LogTemp, Warning, TEXT("adding cell to room : %d %d %d"),x, y,TheGrid[tempX][tempY].RoomID);
						TheGrid[x][y].IsInRoom = true;
						break;
					}
				}
			}

			if(!TheGrid[x][y].IsInRoom)
			{
				//create room
				CreateNewRoom(&TheGrid[x][y]);
				TheGrid[x][y].IsInRoom = true;
				//UE_LOG(LogTemp, Warning, TEXT("creaing new room for : %d %d "),x, y);
			}
		}
	}
}

void ADungeonGenerator::AddCellToRoom(int RoomIndex, FDungeonCell* CellToAdd)
{
	for(int i = 0; i < RoomsInGrid; i++)
	{
		if(DungeonRooms[i].RoomID == RoomIndex)
		{
			DungeonRooms[i].Room.Add(*CellToAdd);
			TheGrid[CellToAdd->CellPos.X][CellToAdd->CellPos.Y].RoomID = RoomIndex;
			break;
		}
	}
}

void ADungeonGenerator::CreateNewRoom(FDungeonCell* Cell)
{
	FDungeonRoom TempRoom;
	/*
	if(DungeonRooms.Num() == 0)
	{
		TempRoom.RoomID = 0;
	}
	else
	{
		TempRoom.RoomID = DungeonRooms.Num();
	}*/
	TempRoom.RoomID = RoomsInGrid;
	RoomsInGrid++;
	
	TempRoom.Room.Add(*Cell);
	TempRoom.RoomType = Cell->CellRoomType;
	TheGrid[Cell->CellPos.X][Cell->CellPos.Y].RoomID = TempRoom.RoomID;
	TempRoom.NumOfUsableCells = 0;
	TempRoom.NumOfWallCells = 0;
	DungeonRooms.Add(TempRoom);
}

void ADungeonGenerator::RoomCleanUp()
{
	for(int x = 0; x < GridSize; x++ )
	{
		for(int y = 0; y < GridSize; y++)
		{
			for(int i = 0; i < TheGrid[x][y].Neighbours.Num(); i++)
			{
				int tempX = TheGrid[x][y].Neighbours[i].CellPos.X;
				int tempY = TheGrid[x][y].Neighbours[i].CellPos.Y;
				if(TheGrid[x][y].CellRoomType == TheGrid[tempX][tempY].CellRoomType)
				{
					if(TheGrid[x][y].RoomID != TheGrid[tempX][tempY].RoomID)
					{
						int RoomOneCount;
						int RoomTwoCount;
						int RoomOneIndex;
						int RoomTwoIndex;
						for(int q = 0; q < DungeonRooms.Num(); q++)
						{
							if(DungeonRooms[q].RoomID == TheGrid[x][y].RoomID)
							{
								RoomOneIndex = q;
								RoomOneCount = DungeonRooms[q].Room.Num();
							}
							
							if(DungeonRooms[q].RoomID ==TheGrid[tempX][tempY].RoomID)
							{
								RoomTwoIndex = q;
								RoomTwoCount = DungeonRooms[q].Room.Num();
							}

							
						}
						if(RoomOneCount >= RoomTwoCount)
						{
							RoomMerge(RoomOneIndex, RoomTwoIndex);
						}
						else
						{
							RoomMerge(RoomTwoIndex,RoomOneIndex);
						}
					
						
					
					}
				}
			}
		}
	}
}

void ADungeonGenerator::SmallRoomCleanUp()
{
	for(int x = 0; x < GridSize; x++ )
	{
		for(int y = 0; y < GridSize; y++)
		{
			for(int i = 0; i < TheGrid[x][y].Neighbours.Num(); i++)
			{
				int tempX = TheGrid[x][y].Neighbours[i].CellPos.X;
				int tempY = TheGrid[x][y].Neighbours[i].CellPos.Y;
				int RoomOneID = TheGrid[x][y].RoomID;
				int RoomOneIndex;
				int RoomTwoIndex;
				
				int RoomTwoID = TheGrid[tempX][tempY].RoomID;
				if(RoomOneID != RoomTwoID)
				{
					for(int q = 0; q < DungeonRooms.Num();q++)
					{
						if(DungeonRooms[q].RoomID == RoomOneID)
						{
							RoomOneIndex = q;
						}
						else if(DungeonRooms[q].RoomID == RoomTwoID)
						{
							RoomTwoIndex = q;
						}
					}

					if(RoomOneIndex == RoomTwoIndex)
					{
						continue;
					}
					FDungeonRoom &RoomOneRef = DungeonRooms[RoomOneIndex];
					FDungeonRoom &RoomTwoRef = DungeonRooms[RoomTwoIndex];

					if(RoomOneRef.Room.Num() < MinRoomSizeByCell || RoomTwoRef.Room.Num() < MinRoomSizeByCell)
					{
						if(RoomOneRef.Room.Num() >= RoomTwoRef.Room.Num())
						{
							RoomMerge(RoomOneIndex,RoomTwoIndex);
						}
						else
						{
							RoomMerge(RoomTwoIndex,RoomOneIndex);
						}/*
						if(RoomOneRef.Room.Num() < MaxRoomSizeByCell && RoomTwoRef.Room.Num() < MaxRoomSizeByCell)
						{
							if(RoomOneRef.Room.Num() >= RoomTwoRef.Room.Num())
							{
								RoomMerge(RoomOneIndex,RoomTwoIndex);
							}
							else
							{
								RoomMerge(RoomTwoIndex,RoomOneIndex);
							}
						}*/
					}

					
				}

				
			}
		}
	}
}

void ADungeonGenerator::RoomAdjacentCheck()
{
	for(int r = 0; r < DungeonRooms.Num(); r++)
	{
		RoomMergedAdjChec(r);
	}
}

void ADungeonGenerator::MinRoomAdjacentCleanUp()
{
	/*if(DungeonRooms.Num() <= 3)
	{
		return;
	}*/
	int test = 0;
	for(int r = 0; r < DungeonRooms.Num(); r++)
	{
		RoomMergedAdjChec(r);
		//UE_LOG(LogTemp, Warning, TEXT("room %d has %d adjacent rooms this is itteration %d the current itteration is %d "),DungeonRooms[r].RoomID, DungeonRooms[r].NumOfAdjacentRooms,test, r);
		test++;
		if(DungeonRooms[r].NumOfAdjacentRooms <= 2)
		{
			int RoomID = DungeonRooms[r].RoomID;
			for(int c = 0; c < DungeonRooms[r].Room.Num(); c++)
			{
				//RoomID = TheGrid[DungeonRooms[r].Room[c].CellPos.X][DungeonRooms[r].Room[c].CellPos.Y].RoomID;
				int OtherRoomIndex;
				bool RoomFound = false;
				int OtherRoomID;
				for(int a = 0; a < DungeonRooms[r].Room[c].Neighbours.Num(); a++)
				{
					OtherRoomID = TheGrid[DungeonRooms[r].Room[c].CellPos.X][DungeonRooms[r].Room[c].CellPos.Y].Neighbours[a].RoomID;//TheGrid[DungeonRooms[r].Room[c].Neighbours[a].CellPos.X][DungeonRooms[r].Room[c].Neighbours[a].CellPos.X].RoomID;
					if(RoomID != OtherRoomID)
					{
						//OtherRoomID = DungeonRooms[r].Room[c].Neighbours[a].RoomID;
											
						for(int q = 0; q < DungeonRooms.Num(); q++)
						{
							if(DungeonRooms[q].RoomID == OtherRoomID)
							{
								OtherRoomIndex = q;
								RoomFound = true;
								break;
							}
						}
						break;
						
						
					}
				}
				//
				if(RoomFound)
				{
					RoomMerge(OtherRoomIndex, r);
					//DungeonRooms[r].RoomType = 10;
					UE_LOG(LogTemp, Warning, TEXT("room %d got set to 10 "),r);
					r = 0;/*
					for(int h = 0; h < DungeonRooms.Num(); h++)
					{
						if(DungeonRooms[h].RoomID == OtherRoomID)
						{
							RoomMergedAdjChec(h);
						}
					}*/
					break;
				}
			}
		}
	}
}

void ADungeonGenerator::ReveseMinRoomCleanUp()
{
	if(DungeonRooms.Num() <= 3)
	{
		return;
	}
	for(int r = DungeonRooms.Num()-1; r >= 0; r--)
	{
		if(DungeonRooms[r].NumOfAdjacentRooms <= 2)
		{
			int RoomID;// = DungeonRooms[r].RoomID;
			for(int c = 0; c < DungeonRooms[r].Room.Num(); c++)
			{
				RoomID = TheGrid[DungeonRooms[r].Room[c].CellPos.X][DungeonRooms[r].Room[c].CellPos.Y].RoomID;
				int OtherRoomIndex;
				bool RoomFound = false;
				int OtherRoomID;
				for(int a = 0; a < DungeonRooms[r].Room[c].Neighbours.Num(); a++)
				{
					OtherRoomID = TheGrid[DungeonRooms[r].Room[c].Neighbours[a].CellPos.X][DungeonRooms[r].Room[c].Neighbours[a].CellPos.X].RoomID;
					if(RoomID != OtherRoomID)
					{
						//OtherRoomID = DungeonRooms[r].Room[c].Neighbours[a].RoomID;
											
						for(int q = 0; q < DungeonRooms.Num(); q++)
						{
							if(DungeonRooms[q].RoomID == OtherRoomID)
							{
								OtherRoomIndex = q;
								RoomFound = true;
								break;
							}
						}
						break;
						
						
					}
				}
				//
				if(RoomFound)
				{
					UE_LOG(LogTemp, Warning, TEXT("room %d got merged into %d "),r, OtherRoomIndex);
					RoomMerge(OtherRoomIndex, r);
					r = 0;
					for(int h = 0; h < DungeonRooms.Num(); h++)
					{
						if(DungeonRooms[h].RoomID == OtherRoomID)
						{
							RoomMergedAdjChec(h);
						}
					}
					break;
				}
			}
		}
	}
}

void ADungeonGenerator::RoomMergedAdjChec(int roomIndex)
{
	//int CurrentRoomID = TheGrid[]
		//DungeonRooms[roomIndex].RoomID;
	TArray<int> SeenRoomIDs;
	//UE_LOG(LogTemp, Warning, TEXT("room: %d seen rooms inital set to: %d "),roomIndex, SeenRoomIDs.Num());
	SeenRoomIDs.Empty();
	DungeonRooms[roomIndex].NumOfAdjacentRooms = 0;
	DungeonRooms[roomIndex].AdjRoomIDs.Empty();
	for(int i = 0; i < DungeonRooms[roomIndex].Room.Num(); i++)
	{
		int CurrentRoomID = DungeonRooms[roomIndex].RoomID; //TheGrid[DungeonRooms[roomIndex].Room[i].CellPos.X][DungeonRooms[roomIndex].Room[i].CellPos.Y].RoomID;
		for(int a = 0; a < DungeonRooms[roomIndex].Room[i].Neighbours.Num(); a++)
		{
			
			int AdjRoomID = TheGrid[DungeonRooms[roomIndex].Room[i].Neighbours[a].CellPos.X][DungeonRooms[roomIndex].Room[i].Neighbours[a].CellPos.Y].RoomID;
			if(CurrentRoomID == AdjRoomID)
			{
				continue;
			}
				//DungeonRooms[roomIndex].Room[i].Neighbours[a].RoomID;
			if(AdjRoomID != CurrentRoomID)
			{
				bool RoomSeen = false;

				for(int k = 0; k < SeenRoomIDs.Num(); k++)
				{
					if(SeenRoomIDs[k] == AdjRoomID)
					{
						RoomSeen = true;
					}
				}
				if(!RoomSeen)
				{
					SeenRoomIDs.Add(AdjRoomID);
					DungeonRooms[roomIndex].AdjRoomIDs.Add(AdjRoomID);
				}
			}
		}
	}
	DungeonRooms[roomIndex].NumOfAdjacentRooms = SeenRoomIDs.Num();
	//UE_LOG(LogTemp, Warning, TEXT("room: %d seen rooms end set to: %d "),roomIndex, SeenRoomIDs.Num());
			
}

void ADungeonGenerator::RoomNeighbourUpdate()
{
	for(int x = 0; x < TheGrid.Num(); x++)
	{
		for(int y = 0; y < TheGrid.Num(); y++)
		{
			for(int i = 0; i < TheGrid[x][y].Neighbours.Num();i++)
			{
				int Nx = TheGrid[x][y].Neighbours[i].CellPos.X;
				int Ny = TheGrid[x][y].Neighbours[i].CellPos.Y;
				TheGrid[x][y].Neighbours[i].RoomID = TheGrid[Nx][Ny].RoomID;
				TheGrid[x][y].Neighbours[i].CellRoomType = TheGrid[Nx][Ny].CellRoomType;
			}
		}
	}
}

void ADungeonGenerator::RoomWallSet()
{
	for(int i = 0; i < DungeonRooms.Num(); i++)
	{
		//int CurrentRoomID = DungeonRooms[i].RoomID;
		for(int p = 0; p < DungeonRooms[i].Room.Num(); p++)
		{
			for(int n = 0; n < DungeonRooms[i].Room[p].Neighbours.Num();n++)
			{
				int CurrentRoomID = TheGrid[DungeonRooms[i].Room[p].CellPos.X][DungeonRooms[i].Room[p].CellPos.Y].RoomID; //DungeonRooms[i].Room[p].RoomID;
				int NeigbRoomID = TheGrid[DungeonRooms[i].Room[p].Neighbours[n].CellPos.X][DungeonRooms[i].Room[p].Neighbours[n].CellPos.Y].RoomID; //DungeonRooms[i].Room[p].Neighbours[n].RoomID;
				if(CurrentRoomID != NeigbRoomID)
				{
					DungeonRooms[i].Room[p].IsWall = true;
					TheGrid[DungeonRooms[i].Room[p].CellPos.X][DungeonRooms[i].Room[p].CellPos.Y].IsWall = true;
					float TempX = (DungeonRooms[i].Room[p].CellPos.X + DungeonRooms[i].Room[p].Neighbours[n].CellPos.X)/2.0f;
					float TempY = (DungeonRooms[i].Room[p].CellPos.Y + DungeonRooms[i].Room[p].Neighbours[n].CellPos.Y)/2.0f;
					float Rot;
					if(DungeonRooms[i].Room[p].CellPos.X == DungeonRooms[i].Room[p].Neighbours[n].CellPos.X)
					{
						Rot = 90;
						UE_LOG(LogTemp,Warning,TEXT("Bro get rotated"));
					}
					else
					{
						Rot = 0;
						UE_LOG(LogTemp,Warning,TEXT("NO ROTATION"));
					}
					DungeonRooms[i].WallLocAndRot.Add(FVector(TempX,TempY, Rot));
				}
				
			}

			if(DungeonRooms[i].Room[p].IsWall)
			{
				DungeonRooms[i].NumOfWallCells++;
				//p = 0;
			}
			else
			{
				DungeonRooms[i].NumOfUsableCells++;
			}
			
			
		}
	}

	/*for(int i = 0; i < DungeonRooms.Num();i++)
	{
		for(int p = 0; p < DungeonRooms[i].Room.Num(); p++)
		{
			if(DungeonRooms[i].Room[p].IsWall)
			{
				continue;
			}
			int NeighboursThatAreWalls = 0;
			for(int n = 0; n < DungeonRooms[i].Room[p].Neighbours.Num(); n++)
			{
				if(TheGrid[DungeonRooms[i].Room[p].Neighbours[n].CellPos.X][DungeonRooms[i].Room[p].Neighbours[n].CellPos.Y].IsWall)
				{
					NeighboursThatAreWalls++;
				}
			}

			if(NeighboursThatAreWalls >= 2)
			{
				DungeonRooms[i].Room[p].IsWall = true;
				TheGrid[DungeonRooms[i].Room[p].CellPos.X][DungeonRooms[i].Room[p].CellPos.Y].IsWall = true;
			}


			if(DungeonRooms[i].Room[p].IsWall)
			{
				DungeonRooms[i].NumOfWallCells++;
				//p = 0;
			}
			else
			{
				DungeonRooms[i].NumOfUsableCells++;
			}
			//p = 0;
		}
	}*/
	
}

void ADungeonGenerator::AddRoomsToIgnoreList()
{
	for(int i = 0; i < DungeonRooms.Num(); i++)
	{
		if( DungeonRooms[i].Room.Num() - DungeonRooms[i].NumOfWallCells < MinRoomSizeWithWall )
		{
			RoomsToIgnoreForObj.Add(DungeonRooms[i].RoomID);
			continue;
		}

		for(int p = 0; p < DungeonRooms[i].Room.Num(); p++)
		{
			if(DungeonRooms[i].Room[p].CellPos.X == 0 || DungeonRooms[i].Room[p].CellPos.X == GridSize-1)
			{
				//RoomsToIgnoreForObj.Add(DungeonRooms[i].RoomID);
				RoomsToIgnore.Add(DungeonRooms[i].RoomID);
				break;
			}

			if(DungeonRooms[i].Room[p].CellPos.Y == 0 || DungeonRooms[i].Room[p].CellPos.Y == GridSize-1)
			{
				//RoomsToIgnoreForObj.Add(DungeonRooms[i].RoomID);
				RoomsToIgnore.Add(DungeonRooms[i].RoomID);
				break;
			}
		}
	}
}

void ADungeonGenerator::SetRoomPos()
{
	
	for(int i = 0; i < DungeonRooms.Num();i++)
	{
		float AveX = 0;
		float AveY = 0;
		int AveDiv = 0;
		for(int p = 0; p < DungeonRooms[i].Room.Num(); p++)
		{
			AveDiv++;
			AveX += DungeonRooms[i].Room[p].CellPos.X;
			AveY += DungeonRooms[i].Room[p].CellPos.Y;
		}

		DungeonRooms[i].AverageRoomPos = FVector2D(AveX/AveDiv, AveY/AveDiv);
	}
}

void ADungeonGenerator::RoomMerge(int MainRoomIndex, int SmallRoomIndex)
{
	TArray<FDungeonCell> TempRoomList;
	for(int i = 0; i < DungeonRooms[SmallRoomIndex].Room.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("i SmallRoomIndex : %d %d "),i, SmallRoomIndex);
		//DungeonRooms[MainRoomIndex].Room.Add(DungeonRooms[SmallRoomIndex].Room[i]);
		TempRoomList.Add(DungeonRooms[SmallRoomIndex].Room[i]);
	}

	for(int i = 0; i < DungeonRooms[MainRoomIndex].Room.Num(); i++)
	{
		TempRoomList.Add(DungeonRooms[MainRoomIndex].Room[i]);
	}

	for(int i = 0; i < TempRoomList.Num(); i++)
	{
		TheGrid[TempRoomList[i].CellPos.X][TempRoomList[i].CellPos.Y].RoomID = DungeonRooms[MainRoomIndex].RoomID;
		TheGrid[TempRoomList[i].CellPos.X][TempRoomList[i].CellPos.Y].CellRoomType = DungeonRooms[MainRoomIndex].RoomType;
		for(int q = 0; q < TheGrid[TempRoomList[i].CellPos.X][TempRoomList[i].CellPos.Y].Neighbours.Num(); q++)
		{
			TheGrid[TempRoomList[i].CellPos.X][TempRoomList[i].CellPos.Y].Neighbours[q].RoomID = TheGrid[TheGrid[TempRoomList[i].CellPos.X][TempRoomList[i].CellPos.Y].Neighbours[q].CellPos.X][TheGrid[TempRoomList[i].CellPos.X][TempRoomList[i].CellPos.Y].Neighbours[q].CellPos.Y].RoomID;
			TheGrid[TempRoomList[i].CellPos.X][TempRoomList[i].CellPos.Y].Neighbours[q].CellRoomType = TheGrid[TheGrid[TempRoomList[i].CellPos.X][TempRoomList[i].CellPos.Y].Neighbours[q].CellPos.X][TheGrid[TempRoomList[i].CellPos.X][TempRoomList[i].CellPos.Y].Neighbours[q].CellPos.Y].CellRoomType;
			//TheGrid[TempRoomList[i].CellPos.X][TempRoomList[i].CellPos.Y].Neighbours[q].RoomID = DungeonRooms[MainRoomIndex].RoomID;
			//TheGrid[TempRoomList[i].CellPos.X][TempRoomList[i].CellPos.Y].Neighbours[q].CellRoomType = DungeonRooms[MainRoomIndex].RoomType;

			
		}
	}

	
	RoomsInGrid--;
	RoomsMerged++;
	DungeonRooms[MainRoomIndex].Room = TempRoomList;
	DungeonRooms.RemoveAt(SmallRoomIndex);
	RoomNeighbourUpdate();
}

void ADungeonGenerator::PickStartAndEndRooms()
{
	TestNumber = 0;
	int StartRoomIndex = GetRandomIntInRange(0,DungeonRooms.Num()-1);
	StartRoomID = DungeonRooms[StartRoomIndex].RoomID;
	FDungeonRoom &StartRoom = DungeonRooms[StartRoomIndex];
	FVector2D StartPos = StartRoom.AverageRoomPos;
	//FDungeonRoom &StartRoom = DungeonRooms[]
	TArray<int> RoomsAllowed;

	for(int i = 0; i < DungeonRooms.Num(); i++)
	{
		float Dis = FVector2D::Distance(StartPos,DungeonRooms[i].AverageRoomPos);
		if(Dis >= MinDistBetweenStartAndEnd)
		{
			RoomsAllowed.Add(i);
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("the number of rooms allowed to be the end point are : %d"),RoomsAllowed.Num());

	EndRoomID = DungeonRooms[RoomsAllowed[GetRandomIntInRange(0, RoomsAllowed.Num()-1)]].RoomID;
	

	
}

void ADungeonGenerator::StartPathFinding()
{
	PathAtEndPoint = false;
	int CurrentRoomID = StartRoomID;
	PathStack.Push(StartRoomID);
	TestNumber++;
	while (!PathAtEndPoint)
	{
		int NextRoomID = NextRoomInPath(CurrentRoomID);
		if(NextRoomID == -1)
		{
			//go back my guy
			PathStack.Pop();
			TestNumber--;
			RoomsToIgnore.Add(CurrentRoomID);
			CurrentRoomID = PathStack.Top();
		}
		else
		{
			CurrentRoomID = NextRoomID;
			PathStack.Push(CurrentRoomID);
			TestNumber++;
			if(NextRoomID == EndRoomID)
			{
				PathAtEndPoint = true;
			}
		}
		
	}
	
}

int ADungeonGenerator::NextRoomInPath(int CurrentRoomID)
{
	bool NextRoomAllowed = false;
	int CurrentRoomIndex = -1;
	for(int i = 0; i < DungeonRooms.Num();i++)
	{
		if(DungeonRooms[i].RoomID == CurrentRoomID)
		{
			CurrentRoomIndex = i;
		}
	}

	UE_LOG(LogTemp,Warning,TEXT("current room index is %d for room ID %d"), CurrentRoomIndex, CurrentRoomID);

	if(DungeonRooms[CurrentRoomIndex].AdjRoomIDs.Num() <= 0)
	{
		return -1;
	}

	TArray<int> AllowedRooms;

	for(int i = 0; i < DungeonRooms[CurrentRoomIndex].AdjRoomIDs.Num(); i++)
	{
		int RoomIDIndex;
		for(int x = 0; x <DungeonRooms.Num();x++)
		{
			if(DungeonRooms[x].RoomID == DungeonRooms[CurrentRoomIndex].AdjRoomIDs[i])
			{
				
				RoomIDIndex = x;
			}
		}
		if(IsNextRoomAllowed(RoomIDIndex))
		{
			if(DungeonRooms[RoomIDIndex].RoomID != CurrentRoomID)
			{
				AllowedRooms.Add(DungeonRooms[CurrentRoomIndex].AdjRoomIDs[i]);
			}
		}
	}
	if(AllowedRooms.Num() > 0)
	{
		int NextRoom = GetRandomIntInRange(1, AllowedRooms.Num()) - 1;
		
		return AllowedRooms[NextRoom];
	}
	//int NextRoom = GetRandomIntInRange(1, AllowedRooms.Num()) - 1;
	RoomsToIgnore.Add(CurrentRoomID);
	return -1;
}

bool ADungeonGenerator::IsNextRoomAllowed(int NextRoomIndex)
{
	if(DungeonRooms[NextRoomIndex].RoomID == EndRoomID)
	{
		if(NumOfRoomsInCurrentPath >= MinNumberOfRoomsAllowedInPath)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	

	for(int i = 0; i < RoomsToIgnore.Num();i++)
	{
		if(DungeonRooms[NextRoomIndex].RoomID == RoomsToIgnore[i])
		{
			return false;
		}
	}

	for(int i = 0; i < PathStack.Num(); i++)
	{
		if(DungeonRooms[NextRoomIndex].RoomID == PathStack[i])
		{
			return false;
		}
	}
	return true;
}

void ADungeonGenerator::CleanWAllsBetweenRooms()
{
	for(int i = 0; i < PathStack.Num()-1;i++)
	{
		int CurrentRoomIndex;
		//int CurrentRoomID;
		int NextRoomIndex;
		for(int x = 0; x < DungeonRooms.Num(); x++)
		{
			if(DungeonRooms[x].RoomID == PathStack[i])
			{
				CurrentRoomIndex = x;
			}

			if(DungeonRooms[x].RoomID == PathStack[i+1])
			{
				NextRoomIndex = x;
			}
		}
		for(int x = 0; x < DungeonRooms[NextRoomIndex].Room.Num(); x++)
		{
			for(int z = 0; z < DungeonRooms[NextRoomIndex].Room[x].Neighbours.Num(); z++)
			{
				if(TheGrid[DungeonRooms[NextRoomIndex].Room[x].Neighbours[z].CellPos.X][DungeonRooms[NextRoomIndex].Room[x].Neighbours[z].CellPos.Y].RoomID == PathStack[i])
				{
					FVector temp;
					float TempX = (DungeonRooms[NextRoomIndex].Room[x].CellPos.X + DungeonRooms[NextRoomIndex].Room[x].Neighbours[z].CellPos.X)/2.0f;
					float TempY = (DungeonRooms[NextRoomIndex].Room[x].CellPos.Y + DungeonRooms[NextRoomIndex].Room[x].Neighbours[z].CellPos.Y)/2.0f;
					float TempRot;
					if(DungeonRooms[NextRoomIndex].Room[x].CellPos.X == DungeonRooms[NextRoomIndex].Room[x].Neighbours[z].CellPos.X )
					{
						TempRot = 90;
					}
					else
					{
						TempRot = 0;
					}

					temp = FVector(TempX,TempY,TempRot);

					for(int k = 0; k < DungeonRooms[NextRoomIndex].WallLocAndRot.Num(); k++)
					{
						if(DungeonRooms[NextRoomIndex].WallLocAndRot[k] == temp)
						{
							DungeonRooms[NextRoomIndex].WallLocAndRot.RemoveAt(k);
							
							break;
						}
					}
					//DungeonRooms[NextRoomIndex].Room[x].IsWall = false;
					//TheGrid[DungeonRooms[NextRoomIndex].Room[x].Neighbours[z].CellPos.X][DungeonRooms[NextRoomIndex].Room[x].Neighbours[z].CellPos.Y].IsWall = false;
				}
			}
		}
		bool ClearRoom = false;
		for(int f = 0; f < RoomsToIgnoreForObj.Num();f++)
		{
			if(DungeonRooms[CurrentRoomIndex].RoomID == RoomsToIgnoreForObj[f])
			{
				ClearRoom = true;
				break;
			}
		}

		for(int x = 0; x < DungeonRooms[CurrentRoomIndex].Room.Num(); x++)
		{
			if(ClearRoom)
			{
				DungeonRooms[CurrentRoomIndex].WallLocAndRot.Empty();
				//DungeonRooms[CurrentRoomIndex].Room[x].IsWall = false;
				//TheGrid[DungeonRooms[CurrentRoomIndex].Room[x].CellPos.X][DungeonRooms[CurrentRoomIndex].Room[x].CellPos.Y].IsWall = false;
				continue;
			}
			int NCellsThatAreNotWalls = 0;
			int NCellsThatAreWalls = 0;
			for(int z = 0; z < DungeonRooms[CurrentRoomIndex].Room[x].Neighbours.Num(); z++)
			{
				if(TheGrid[DungeonRooms[CurrentRoomIndex].Room[x].Neighbours[z].CellPos.X][DungeonRooms[CurrentRoomIndex].Room[x].Neighbours[z].CellPos.Y].RoomID == PathStack[i] && !DungeonRooms[CurrentRoomIndex].Room[x].Neighbours[z].IsWall)//!DungeonRooms[CurrentRoomIndex].Room[x].Neighbours[z].IsWall && 
				{
					NCellsThatAreNotWalls++;
				}
				else if(TheGrid[DungeonRooms[CurrentRoomIndex].Room[x].Neighbours[z].CellPos.X][DungeonRooms[CurrentRoomIndex].Room[x].Neighbours[z].CellPos.Y].RoomID == PathStack[i] && DungeonRooms[CurrentRoomIndex].Room[x].Neighbours[z].IsWall)
				{
					NCellsThatAreWalls++;
				}
				if(TheGrid[DungeonRooms[CurrentRoomIndex].Room[x].Neighbours[z].CellPos.X][DungeonRooms[CurrentRoomIndex].Room[x].Neighbours[z].CellPos.Y].RoomID == PathStack[i+1])
				{
					FVector temp;
					float TempX = (DungeonRooms[CurrentRoomIndex].Room[x].CellPos.X + DungeonRooms[CurrentRoomIndex].Room[x].Neighbours[z].CellPos.X)/2.0f;
					float TempY = (DungeonRooms[CurrentRoomIndex].Room[x].CellPos.Y + DungeonRooms[CurrentRoomIndex].Room[x].Neighbours[z].CellPos.Y)/2.0f;
					float TempRot;
					if(DungeonRooms[CurrentRoomIndex].Room[x].CellPos.X ==DungeonRooms[CurrentRoomIndex].Room[x].Neighbours[z].CellPos.X )
					{
						TempRot = 90;
					}
					else
					{
						TempRot = 0;
					}

					temp = FVector(TempX,TempY,TempRot);

					for(int k = 0; k < DungeonRooms[CurrentRoomIndex].WallLocAndRot.Num(); k++)
					{
						if(DungeonRooms[CurrentRoomIndex].WallLocAndRot[k] == temp)
						{
							DungeonRooms[CurrentRoomIndex].WallLocAndRot.RemoveAt(k);
							break;
						}
					}
					//DungeonRooms[CurrentRoomIndex].Room[x].IsWall = false;
					//TheGrid[DungeonRooms[CurrentRoomIndex].Room[x].CellPos.X][DungeonRooms[CurrentRoomIndex].Room[x].CellPos.Y].IsWall = false;
				}
				
			}
			/*if(NCellsThatAreNotWalls == 1 && NCellsThatAreWalls >= 0)// && DungeonRooms[CurrentRoomIndex].Room[x].IsWall)
			{
				DungeonRooms[CurrentRoomIndex].Room[x].IsWall = false;
				TheGrid[DungeonRooms[CurrentRoomIndex].Room[x].CellPos.X][DungeonRooms[CurrentRoomIndex].Room[x].CellPos.Y].IsWall = false;
				DungeonRooms[CurrentRoomIndex].NumOfWallCells--;
				//i = 0;
				//break;
				//x = 0;
			}*/
		}

		
	}
}

void ADungeonGenerator::SetRoomTypes()
{
	for(int i = 0; i < DungeonRooms.Num(); i++)
	{
		if(DungeonRooms[i].RoomID == PathStack[0])
		{
			DungeonRooms[i].RoomType = 0;
		}
		else if(DungeonRooms[i].RoomID == PathStack[PathStack.Num()-1])
		{
			DungeonRooms[i].RoomType = 4;
		}
		else if(DungeonRooms[i].RoomID == PathStack[PathStack.Num()-2])
		{
			DungeonRooms[i].RoomType = 3;
		}
	}

	for(int i = 1; i < PathStack.Num()-2;i++)
	{
		int RoomIndex;
		for(int p = 0; p < DungeonRooms.Num();p++)
		{
			if(DungeonRooms[p].RoomID == PathStack[i])
			{
				RoomIndex = p;
				break;
			}
		}
		if(DungeonRooms[RoomIndex].WallLocAndRot.Num() == 0)
		{
			DungeonRooms[RoomIndex].RoomType = 5;
			continue;
		}

		float RandomRoomChance = GetRandomIntInRange(0,100);
		if(RandomRoomChance < CurrentFightChance)
		{
			//set it to fight
			ResetRoomChance();
			CurrentFightChance -= 10.0f;
			CurrentPuzzleChance += 5.0f;
			CurrentTreasureChance += 5.0f;
			DungeonRooms[RoomIndex].RoomType = 1;
		}
		else if(RandomRoomChance < CurrentFightChance + CurrentPuzzleChance && RandomRoomChance >= CurrentFightChance)
		{
			//set it to puzzle chance
			ResetRoomChance();
			CurrentFightChance += 5.0f;
			CurrentPuzzleChance -= 10.0f;
			CurrentTreasureChance += 5.0f;
			DungeonRooms[RoomIndex].RoomType = 2;
		}
		else if(RandomRoomChance < 100 && RandomRoomChance >= 100 - CurrentTreasureChance)
		{
			//set it to treasure
			ResetRoomChance();
			CurrentFightChance += 5.0f;
			CurrentPuzzleChance += 5.0f;
			CurrentTreasureChance -= 10.0f;
			DungeonRooms[RoomIndex].RoomType = 3;
		}
	}

	
}

bool ADungeonGenerator::StrCheck(int x, int y, int Nx, int Ny)
{
	if(TheGrid[x][y].CellStrength >= TheGrid[Nx][Ny].CellStrength)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void ADungeonGenerator::SpawnCubes()
{
	
	UWorld* const World = GetWorld();
	if(World)
	{
		int CurrentRoomInPath = 0;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		for(int i = 0; i < DungeonRooms.Num();i++)
		{
			bool CanBuildRoom = false;
			
			for(int t = 0; t < PathStack.Num(); t++)
			{
				if(DungeonRooms[i].RoomID == PathStack[t])
				{
					CanBuildRoom = true;
					CurrentRoomInPath = t;
					UE_LOG(LogTemp, Warning, TEXT("building room %d in path"), t);
					break;
				}
			}
			if(CanBuildRoom)
			{
				for(int o = 0; o < DungeonRooms[i].Room.Num();o++)
				{
				
					int x = DungeonRooms[i].Room[o].CellPos.X;
					int y = DungeonRooms[i].Room[o].CellPos.Y;
					float tempZ = 0;
					if(CurrentRoomInPath == 0)
					{
						tempZ = 100;
						UE_LOG(LogTemp, Warning, TEXT("building room zero"));
					}
					FVector SpawnLocation = FVector(x*100,y*100 ,0*CurrentRoomInPath);
					
					FRotator SpawnRotation = FRotator(0.0f,0.0f,0.0f);

					World->SpawnActor<AActor>(CubeList[DungeonRooms[i].RoomType], SpawnLocation, SpawnRotation, SpawnParams);
				/*
					if(DungeonRooms[i].Room[o].IsWall)
					{
						World->SpawnActor<AActor>(WallList[0], SpawnLocation, SpawnRotation, SpawnParams);
					}
					else
					{
						World->SpawnActor<AActor>(CubeList[DungeonRooms[i].RoomType-1], SpawnLocation, SpawnRotation, SpawnParams);
					}*/
					//World->SpawnActor<AActor>(CubeList[DungeonRooms[i].RoomType-1], SpawnLocation, SpawnRotation, SpawnParams);
					/*
					if(DungeonRooms[i].RoomType == 10)
					{
						//UE_LOG(LogTemp, Warning, TEXT("the number of rooms allowed to be the end point are : %d"),RoomsAllowed.Num());
					}
					else
					{
						World->SpawnActor<AActor>(CubeList[DungeonRooms[i].RoomType-1], SpawnLocation, SpawnRotation, SpawnParams);
					}*/
					//World->SpawnActor<AActor>(CubeList[DungeonRooms[i].RoomType-1], SpawnLocation, SpawnRotation, SpawnParams);
				}
			}
		}
		
	}
}

void ADungeonGenerator::SpawnWalls()
{
	UWorld* const World = GetWorld();
	if(World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		for(int i = 0; i < DungeonRooms.Num(); i++)
		{
			bool CanBuildRoom = false;
			
			for(int t = 0; t < PathStack.Num(); t++)
			{
				if(DungeonRooms[i].RoomID == PathStack[t])
				{
					CanBuildRoom = true;
					break;
				}
			}
			if(CanBuildRoom)
			{
				for(int k = 0; k < DungeonRooms[i].WallLocAndRot.Num(); k++)
				{
					FVector SpawnLocation = FVector(DungeonRooms[i].WallLocAndRot[k].X * 100.0f,DungeonRooms[i].WallLocAndRot[k].Y*100.0f ,0);
					
					FRotator SpawnRotation = FRotator(0.0f,DungeonRooms[i].WallLocAndRot[k].Z,0.0f);

					World->SpawnActor<AActor>(WallList[0], SpawnLocation, SpawnRotation, SpawnParams);
				}
			}
		}
	}
}


void ADungeonGenerator::checkIfThereIsASeed()
{
	if(GlobalSeed == 0)
	{
		GlobalSeed = FMath::RandRange(1000000,9999999);
	}
	
}

float ADungeonGenerator::GetRandomFloat()
{
	GlobalSeed = (GlobalSeed * 196314165) + 907633515;
	float temp = FMath::Fmod(static_cast<float>(GlobalSeed), MAX_int32) / MAX_int32;
	if(temp < 0)
	{
		temp += 1.0f;
	}
	temp = FMath::Clamp(temp, 0.0f, 1.0f);  // Ensure the value is within [0, 1)
	//UE_LOG(LogTemp, Warning, TEXT("random float is: %f"), temp);
	return temp;
}	

int ADungeonGenerator::GetRandomIntInRange(int Min, int Max)
{
	float tempRandFloat = GetRandomFloat();
	int temp = FMath::RoundToInt((tempRandFloat * (Max - Min)) + Min);
	return temp;
}

void ADungeonGenerator::ResetRoomChance()
{
	CurrentPuzzleChance = BasePuzzleChance;
	CurrentTreasureChance = BaseTreasureChance;
	CurrentFightChance = BaseFightChance;
}

