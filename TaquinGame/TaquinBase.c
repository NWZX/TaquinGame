/*
MIT License

Copyright (c) 2019 NWZX

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "TaquinBase.h"



int randO(int min, int max)
{
	return rand() % (max - min) + min;
}

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

int* tab2D_convert(int** tabXY, int dimSize)
{
	int* temp = NULL;
	temp = malloc((int)pow(dimSize, 2) * 8 * sizeof(int*));
	if (tabXY == NULL)
	{
		exit(0);
	}

	int m = 0;
	for (int i = 0; i < dimSize; i++)
	{
		for (int l = 0; l < dimSize; l++)
		{
			temp[m] = tabXY[i][l];
			m++;
		}
	}

	return temp;
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
		int rand1 = 0;
		int rand2 = 0;
		int temp = 0;

		for (int i = 0; i < sizeX; i++)
		{
			for (int l = 0; l < sizeY; l++)
			{
				plat[i][l] = cont;
				cont++;
			}
		}

		for (int i = 0; i < sizeX; i++)
		{
			for (int l = 0; l < sizeY; l++)
			{
				rand1 = randO(1, (sizeX * sizeY));
				rand2 = randO(1, (sizeX * sizeY));
				temp = plat[rand1 / sizeY][rand1 % sizeX];
				plat[rand1/sizeY][rand1%sizeX] = plat[rand2/sizeY][rand2%sizeX];
				plat[rand2 / sizeY][rand2 % sizeX] = temp;
			}
		}
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

void validateMove(int cursX, int cursY, int* cursX2, int* cursY2, int** plat, Item_text* text)
{
	if (cursX2 == NULL || cursY2 == NULL)
		exit(1);

	int test = plat[cursX][cursY] == 0 || plat[*cursX2][*cursY2] == 0;
	SDL_Texture* tempT = NULL;

	if (abs(cursX - *cursX2) == 1 && abs(cursY - *cursY2) == 0 && test)
	{
		int temp = plat[cursX][cursY];
		plat[cursX][cursY] = plat[*cursX2][*cursY2];
		plat[*cursX2][*cursY2] = temp;

		tempT = text[*cursX2 * 4 + *cursY2].text;
		text[*cursX2 * 4 + *cursY2].text = text[cursX * 4 + cursY].text;
		text[cursX * 4 + cursY].text = tempT;

		//Fix for non convervative REC
	}
	else if (abs(cursX - *cursX2) == 0 && abs(cursY - *cursY2) == 1 && test)
	{
		int temp = plat[cursX][cursY];
		plat[cursX][cursY] = plat[*cursX2][*cursY2];
		plat[*cursX2][*cursY2] = temp;

		tempT = text[*cursX2 * 4 + *cursY2].text;
		text[*cursX2 * 4 + *cursY2].text = text[cursX * 4 + cursY].text;
		text[cursX * 4 + cursY].text = tempT;
	}
}

//int checkWin(int** plat, int dim)
//{
//	for (int i = 0; i < pow(dim, 2); i++)
//	{
//		if (i != pow(dim, 2) - 1)
//			if (plat[i / dim][i % dim] != i)
//				return 0;
//			else
//				return 1;
//		else
//
//
//			
//	}
//}
