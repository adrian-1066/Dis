// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"

#include <SceneExport.h>
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
	RoomAdjacentCheck();
	MinRoomAdjacentCleanUp();
	SetRoomPos();
	//UE_LOG(LogTemp, Warning, TEXT("There are : %d"),RoomsInGrid);
	//UE_LOG(LogTemp, Warning, TEXT("There are this many rooms : %d"),DungeonRooms.Num());
	//UE_LOG(LogTemp, Warning, TEXT("these many rooms have merged : %d"),RoomsMerged);
	PickStartAndEndRooms();
	RoomNeighbourUpdate();
	for(int i = 0; i < DungeonRooms.Num(); i++)
	{
		RoomMergedAdjChec(i);
		UE_LOG(LogTemp, Warning, TEXT("Room: %d has: %d adjacent rooms"),i, DungeonRooms[i].NumOfAdjacentRooms);
		
	}
	RoomNeighbourUpdate();
/*
	for(int x = 0; x < TheGrid.Num(); x++)
	{
		for (int y = 0; y < TheGrid.Num(); y++)
		{
			for(int i = 0; i < TheGrid[x][y].Neighbours.Num(); i++)
			{
				UE_LOG(LogTemp, Warning, TEXT("Cell %d %d Has the roomID of %d its listed Neighbour %d has the RoomID %d while the Grid version has the ID of %d "),x,y,TheGrid[x][y].RoomID,i,TheGrid[x][y].Neighbours[i].RoomID, TheGrid[TheGrid[x][y].Neighbours[i].CellPos.X][TheGrid[x][y].Neighbours[i].CellPos.Y].RoomID);
			}
			
		}
	}*/
	SpawnCubes();
	
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
		}
	}
}

void ADungeonGenerator::SetCellStrAndType()
{
	for(int x = 0; x < GridSize; x++)
	{
		for(int y = 0; y < GridSize; y++)
		{
			
			TheGrid[x][y].CellStrength = GetRandomIntInRange(1, 20);
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
						for(int q = 0; q < RoomsInGrid; q++)
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
	if(DungeonRooms.Num() <= 3)
	{
		return;
	}
	for(int r = 0; r < DungeonRooms.Num(); r++)
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
					RoomMerge(OtherRoomIndex, r);
					r--;
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
	UE_LOG(LogTemp, Warning, TEXT("room: %d seen rooms inital set to: %d "),roomIndex, SeenRoomIDs.Num());
	SeenRoomIDs.Empty();
	for(int i = 0; i < DungeonRooms[roomIndex].Room.Num(); i++)
	{
		int CurrentRoomID = TheGrid[DungeonRooms[roomIndex].Room[i].CellPos.X][DungeonRooms[roomIndex].Room[i].CellPos.Y].RoomID;
		for(int a = 0; a < DungeonRooms[roomIndex].Room[i].Neighbours.Num(); a++)
		{
			
			int AdjRoomID = TheGrid[DungeonRooms[roomIndex].Room[i].Neighbours[a].CellPos.X][DungeonRooms[roomIndex].Room[i].Neighbours[a].CellPos.Y].RoomID;
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
				}
			}
		}
	}
	DungeonRooms[roomIndex].NumOfAdjacentRooms = SeenRoomIDs.Num();
	UE_LOG(LogTemp, Warning, TEXT("room: %d seen rooms end set to: %d "),roomIndex, SeenRoomIDs.Num());
			
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
	StartRoomID = GetRandomIntInRange(0,DungeonRooms.Num()-1);
	int StartRoomIndex = 0;
	for(int i = 0; i < DungeonRooms.Num(); i++)
	{
		if(DungeonRooms[i].RoomID == StartRoomID)
		{
			StartRoomIndex = i;
			//FDungeonRoom &StartRoom = DungeonRooms[i];
			break;
		}
	}
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

	EndRoomID = RoomsAllowed[GetRandomIntInRange(0, RoomsAllowed.Num()-1)];
	

	
}

void ADungeonGenerator::StartPathFinding()
{
	
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
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		for(int i = 0; i < DungeonRooms.Num();i++)
		{
			for(int o = 0; o < DungeonRooms[i].Room.Num();o++)
			{
				int x = DungeonRooms[i].Room[o].CellPos.X;
				int y = DungeonRooms[i].Room[o].CellPos.Y;
				FVector SpawnLocation = FVector(x*100,y*100 ,10*i);
				FRotator SpawnRotation = FRotator(0.0f,0.0f,0.0f);
				World->SpawnActor<AActor>(CubeList[DungeonRooms[i].RoomType-1], SpawnLocation, SpawnRotation, SpawnParams);
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
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("yeah we got a seed my boi"));
		
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

	if(temp < Min)
	{
		//UE_LOG(LogTemp, Warning, TEXT("bruh that number was out of range : %d"), temp);
		//temp = GetRandomIntInRange(Min, Max);
	}
	return temp;
}

