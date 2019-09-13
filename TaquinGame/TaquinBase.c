#include "TaquinBase.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

int** tab2D_init(int x, int y)
{
	int** tabXY = NULL;
	tabXY = malloc(x * 8 * sizeof(int*));
	if (tabXY == NULL)
	{
		exit(0);
	}

	for (int i = 0; i < x; i++)
	{
		tabXY[i] = malloc(y * sizeof(int));
		if (tabXY[i] == NULL)
		{
			exit(0);
		}
	}

	return tabXY;
}

void tab2D_free(int** tabXY, int x)
{
	for (int i = 0; i < x; i++)
	{
		free(tabXY[i]);
	}
	free(tabXY);
}

int** newPlat(int sizeX, int sizeY, int random)
{
	int** plat = tab2D_init(sizeX, sizeY);
	int cont = 0;

	if (random)
	{
	}
	else
	{
		for (int i = 0; i < sizeX; i++)
		{
			for (int l = 0; l < sizeY; l++)
			{
				plat[i][l] = cont;
				cont++;
			}
		}
	}

	return plat;
}

void newScreen(int cursX, int cursY, int* cursX2, int* cursY2, int** plat, int size)
{
	system("cls");
	for (int i = 0; i < size; i++)
	{
		for (int l = 0; l < size; l++)
		{
			if (cursX2 != NULL && cursY2 != NULL && i == *cursX2 && l == *cursY2)
			{
				printf("|+%02d", plat[i][l]);
			}
			else if (i == cursX && l == cursY)
			{
				printf("|*%02d", plat[i][l]);
			}
			else
				printf("| %02d", plat[i][l]);
		}
		printf("|\n");
	}
}

int interceptKey(int* cursX, int* cursY)
{
	int key = _getch();
	if (key == 72 && *cursX > 0) // UP
		* cursX -= 1;
	else if (key == 80 && *cursX < 3) // DOWN
		* cursX += 1;
	else if (key == 75 && *cursY > 0) // LEFT
		* cursY -= 1;
	else if (key == 77 && *cursY < 3) // RIGHT
		* cursY += 1;
	else if (key == 13) // Enter
		return 1;
	else if (key == 113) // Q
		return -1;

	return 0;
}

void validateMove(int cursX, int cursY, int* cursX2, int* cursY2, int** plat)
{
	if (cursX2 == NULL || cursY2 == NULL)
		exit(1);

	int test = plat[cursX][cursY] == 0 || plat[*cursX2][*cursY2] == 0;

	if (abs(cursX - *cursX2) == 1 && abs(cursY - *cursY2) == 0 && test)
	{
		int temp = plat[cursX][cursY];
		plat[cursX][cursY] = plat[*cursX2][*cursY2];
		plat[*cursX2][*cursY2] = temp;
	}
	else if (abs(cursX - *cursX2) == 0 && abs(cursY - *cursY2) == 1 && test)
	{
		int temp = plat[cursX][cursY];
		plat[cursX][cursY] = plat[*cursX2][*cursY2];
		plat[*cursX2][*cursY2] = temp;
	}

	free(cursX2);
	free(cursY2);
}