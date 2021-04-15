#include<stdio.h>
#include<windows.h>
#include<time.h>
#include"block.h"

#define LEFT 75
#define RIGHT 77
#define UP 72
#define SPACE 32

//Size of gameboard
#define GBOARD_WIDTH 10
#define GBOARD_HEIGHT 20

//Starting point of gameboard
#define GBOARD_ORIGIN_X 5
#define GBOARD_ORIGIN_Y 3

int level = 0;
int up = 0;
int score = 0;
int block_id;
int block_id_next;
int speed = 15;
int score_speed = 10;
int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];

COORD curPos = { GBOARD_ORIGIN_X + GBOARD_WIDTH, GBOARD_ORIGIN_Y / 2 };

void SetCurrentCursorPos(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//Ŀ�� �ش� ��ġ�� �ֱ�

COORD GetCurrentCursorPos(void)
{
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;
	return curPoint;
}
//Ŀ�� ���

void RemoveCursor(void)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
//Ŀ�� �����

void ShowBlock(char blockInfo[4][4])
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			if (blockInfo[y][x] == 1)
				printf("■");
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}
//���� ���

void DeleteBlock(char blockInfo[4][4])
{
	int x, y;
	COORD curPos = GetCurrentCursorPos();
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			if (blockInfo[y][x] == 1)
				printf(" ");
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}
//���� ����

int DetectCollision(int posX, int posY, char blockModel[4][4])
{
	int x, y;
	int arrX = (posX - GBOARD_ORIGIN_X) / 2;
	int arrY = posY - GBOARD_ORIGIN_Y;

	for (y = 0; y < 4; y++)
		for (x = 0; x < 4; x++)
			if (blockModel[y][x] == 1 && gameBoardInfo[arrY + y][arrX + x] == 1)
				return 0;
	return 1;
}
//���ϰ� �� �浹üũ

int ShiftRight()
{
	if (!DetectCollision(curPos.X + 2, curPos.Y, blockModel[block_id]))
		return 0;
	DeleteBlock(blockModel[block_id]);
	curPos.X += 2;
	SetCurrentCursorPos(curPos.X, curPos.Y);
	ShowBlock(blockModel[block_id]);
}
//������ ���������� �����̰Բ�

int ShiftLeft()
{
	if (!DetectCollision(curPos.X - 2, curPos.Y, blockModel[block_id]))
		return 0;
	DeleteBlock(blockModel[block_id]);
	curPos.X -= 2;
	SetCurrentCursorPos(curPos.X, curPos.Y);
	ShowBlock(blockModel[block_id]);
}
//������ �������� �����̰Բ�

void ShiftUp()
{
	DeleteBlock(blockModel[block_id]);
	curPos.Y -= 2;
	SetCurrentCursorPos(curPos.X, curPos.Y);
	ShowBlock(blockModel[block_id]);
}
//������ ���� ���� ���Բ�

int BlockDown()
{
	if (DetectCollision(curPos.X, curPos.Y + 1, blockModel[block_id]) == 0) 
		return 0;
	DeleteBlock(blockModel[block_id]);
	curPos.Y++;
	SetCurrentCursorPos(curPos.X, curPos.Y);
	ShowBlock(blockModel[block_id]);
	return 1;
}
//������ �Ʒ��� �����̰Բ�

void SpaceDown()
{
	while (BlockDown());
}
//�����̽��ٸ� ����� �ѹ��� ������ ��

void RotateBlock()
{
	int block_base = block_id - block_id % 4;
	int block_rotated = block_base + (block_id + 1) % 4;
	if (!DetectCollision(curPos.X, curPos.Y, blockModel[block_rotated])) 
		return;
	DeleteBlock(blockModel[block_id]);
	block_id = block_rotated;
	ShowBlock(blockModel[block_id]);
}
//������ ��׸�����

void ProcessKeyInput()
{
	int i, key, y;
	for (i = 0; i < 20; i++)//�� blockDown�ϰ� i�� 20�� ��ٷ��� �ϴ°�?
	{                  //->1000ms�� 20������ ���� ���� �ǹ���
		if (exit() != 0)  //���� ���� ������?: ������ �ִ� �� 1�ʰ� �߻��ص� ó���ؾ� �ϹǷ�
		{
			key = getch();
			switch (key)
			{
			case LEFT:
				ShiftLeft();
				break;
			case RIGHT:
				ShiftRight();
				break;
			case UP:
				RotateBlock();
				break;
			case SPACE:
				SpaceDown();
				break;
			}
		}
		Sleep(speed);
	}
}
//Ű���� ����Ű ���� ������

void DrawGameBoard()
{
	int x, y;
	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT) printf("L");
		else printf("|");
	}
	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT) printf("J");
		else printf("|");
	}
	for (x = 0 ; x<GBOARD_WIDTH; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2 + 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		printf("-");
	}

	//Collision Check
	for (y = 0; y<GBOARD_HEIGHT; y++) {
		gameBoardInfo[y][0] = 1;
		gameBoardInfo[y][GBOARD_WIDTH + 1] = 1;
	}
	for (x = -1; x<GBOARD_WIDTH; x++)
		gameBoardInfo[GBOARD_HEIGHT][x + 1] = 1;
}
//������ Ʋ(������ ��� ����)

void AddBlockToBoard(int shape)
{
	int x, y, arrCurX, arrCurY;
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
		/*	if (shape >= 28 && !DetectCollision(curPos.X + 2, curPos.Y, blockModel[shape]))
			{
				arrCurX = (curPos.X + 1 - GBOARD_ORIGIN_X) / 2;
				arrCurY = curPos.Y - GBOARD_ORIGIN_Y;
				gameBoardInfo[arrCurY + y][arrCurX + x] = 0;
			}
			if (shape >= 28 && !DetectCollision(curPos.X - 2, curPos.Y, blockModel[shape]))
			{
				arrCurX = (curPos.X - 1 - GBOARD_ORIGIN_X) / 2;
				arrCurY = curPos.Y - GBOARD_ORIGIN_Y;
				gameBoardInfo[arrCurY + y][arrCurX + x] = 0;
			}
			if (shape >= 28 && !DetectCollision(curPos.X, curPos.Y - 1, blockModel[shape]))
			{
				arrCurX = (curPos.X - GBOARD_ORIGIN_X) / 2;
				arrCurY = curPos.Y - GBOARD_ORIGIN_Y - 1;
				gameBoardInfo[arrCurY + y][arrCurX + x] = 0;
			}
			if (shape >= 28 && !DetectCollision(curPos.X, curPos.Y + 1, blockModel[shape]))
			{
				arrCurX = (curPos.X - GBOARD_ORIGIN_X) / 2;
				arrCurY = curPos.Y - GBOARD_ORIGIN_Y + 1;
				gameBoardInfo[arrCurY + y][arrCurX + x] = 0;
			}*/
			//Convert X,Y cordinate to array index
			arrCurX = (curPos.X - GBOARD_ORIGIN_X) / 2;
			arrCurY = curPos.Y - GBOARD_ORIGIN_Y;

			if (blockModel[block_id][y][x] == 1)
				gameBoardInfo[arrCurY + y][arrCurX + x] = 1;
		}
	}
}
//���� ���̸� ��Ŀ� �߰�

void RedrawBlocks()
{
	int x, y;
	int cursX, cursY;
	for (y = 0; y<GBOARD_HEIGHT; y++)
	{
		for (x = 1; x<GBOARD_WIDTH + 1; x++)
		{
			cursX = x * 2 + GBOARD_ORIGIN_X;
			cursY = y + GBOARD_ORIGIN_Y;
			SetCurrentCursorPos(cursX, cursY);
			if (gameBoardInfo[y][x] == 1)
				printf("■");
			else 
				printf(" ");
		}
	}
}
//������ �������� 

void RemoveFillUpLine()
{
	int line, x, y;
	for (y = GBOARD_HEIGHT - 1; y>0; y--) 
	{
		for (x = 1; x<GBOARD_WIDTH + 1; x++) 
		{
			if (gameBoardInfo[y][x] != 1)
				break;
		}
		if (x == (GBOARD_WIDTH + 1))
		{
			for (line = 0; y - line>0; line++) 
			{
				memcpy(&gameBoardInfo[y - line][1],
					&gameBoardInfo[(y - line) - 1][1], GBOARD_WIDTH * sizeof(int));
			}
			y++;
			score += 10;
		}
	}
	RedrawBlocks();
	SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
	printf("J");
}
//�� �� ä������ �����

void RemoveLineBlock()
{
	int line, x, y;
	for (y = GBOARD_HEIGHT - 1; y>0; y--)
	{
		for (line = 0; y - line>0; line++)
		{
			memcpy(&gameBoardInfo[y - line][1],
				&gameBoardInfo[(y - line) - 1][1], GBOARD_WIDTH * sizeof(int));
		}
		y++;
	}
	score += 10;
	RedrawBlocks();
	SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
	printf("J");
}
//block���� �� �����(�� �� ���� ��ä������ ����)

void ScoreBoard()
{
	level = score / 20 + 1;
	SetCurrentCursorPos(30, 15);
	printf("Level : ");
	printf("%d", level);
	SetCurrentCursorPos(30, 16);
	printf("Score : ");
	printf("%d", score);
	SetCurrentCursorPos(30, 17);
	printf("Speed :");
	printf("%d", score_speed);
}
//������ Ʋ(����, ����, ���ǵ� ���)

void lineup() 
{
	srand((unsigned int)time(NULL));
	int tt = 0;
	int vacant = 0;
	for (int c = 0; c<GBOARD_HEIGHT; c++)
	{
		memcpy(&gameBoardInfo[c - 1][1], &gameBoardInfo[c][1], GBOARD_WIDTH * sizeof(int));
		RedrawBlocks();
	}
	for (int x = 1; x<GBOARD_WIDTH; x++) {
		tt = rand() % 100;
		if (tt == 0)
		{
			gameBoardInfo[GBOARD_HEIGHT - 1][x] = 0;
			RedrawBlocks();
		}
		else {
			gameBoardInfo[GBOARD_HEIGHT - 1][x] = 1;
			RedrawBlocks();
		}
	}
	vacant = rand() % GBOARD_WIDTH;
	vacant += 1;
	gameBoardInfo[GBOARD_HEIGHT - 1][vacant] = 0;
	RedrawBlocks();
}
//�ؿ��� �� ����

void Control_level() 
{   
	switch (level)
	{
		case 2:
		{
			score_speed = 20;	
			speed = 10;
			up = 1;
			break;
		}
		case 3:
		{
			score_speed = 30;
			speed = 8;
			break;
		}
		case 4:
		{
			score_speed = 40;
			speed = 5;
			break;
		}
		case 5:
		{
			score_speed = 50;
			speed = 3;
			break;
		}
	}
}
//����, ���ǵ� ����

int IsGameOver()
{
	if (!DetectCollision(curPos.X, curPos.Y, blockModel[block_id])) return 1;
	return 0;
}
//���ӿ��� �� 

void Show_next_block(int shape)
{
	SetCurrentCursorPos(30, 7);
	ShowBlock(blockModel[shape]);
}
//���� ���� �����ֱ�

void Delete_next_block(char blockInfo[4][4])
{
	int x, y;
	SetCurrentCursorPos(30, 7);
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			SetCurrentCursorPos(30 + (x * 2), 7 + y);
			if (blockInfo[y][x] == 1)
				printf(" ");
		}
	}
}
//���� ���� â �����

int main()
{
	{
		int line_cnt = 0;
		RemoveCursor();
		DrawGameBoard();
		SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH, 0);
		srand((unsigned int)time(NULL));
		//block_id = (rand() % 7) * 4;
		//block_id_next = (rand() % 7) * 4;

		//���찳 ���ϱ���
		block_id = (rand() % 8) * 4;
		block_id_next = (rand() % 8) * 4;

		Show_next_block(block_id_next);
	}
	//�⺻����(Ŀ�������, �����Ǳ׸���, Ŀ����ġ, ���Ϸ�������)
	//line_cnt=���� ���� �ؿ� ���� ���� ����
	while (1)
	{
		curPos.X = GBOARD_ORIGIN_X + GBOARD_WIDTH;
		curPos.Y = 0;
		ScoreBoard();
		if (IsGameOver()) break;
		while (1)
		{
			if (BlockDown() == 0)
			{
				if (block_id >= 28) RemoveLineBlock();
				else AddBlockToBoard(block_id);
				RedrawBlocks();
				if (up)
				{
					lineup();
					RedrawBlocks();
				}
				//���� �ؿ��� �ö���°�
				break;
			}
			Control_level(); //���� ����
			ProcessKeyInput();
		}
		RemoveFillUpLine(); //���� ä������ �����
		{
			block_id = block_id_next;
			Delete_next_block(blockModel[block_id]);
			//block_id_next = (rand() % 7) * 4;
			block_id_next = (rand() % 8) * 4;
			Show_next_block(block_id_next);
		}
		//���� �̸����� ����
	}
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH, 0);
		printf("Game Over!!");
	}
	//���ӿ��� ���
	return 0;
}