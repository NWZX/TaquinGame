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


//Function for generate random int value 
int randO(int min, int max)
{
	return rand() % (max - min) + min;
}

//Function for generate a new 2D tab of int
int** tab2D_init(int x, int y)
{
	int** tabXY = NULL;
	tabXY = malloc(x * sizeof(int*));
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

//Fuction for convert 2D tab to 1D tab
int* tab2D_convert(int** tabXY, int dimSize)
{
	int* temp = NULL;
	temp = malloc((int)pow(dimSize, 2) * sizeof(int*));
	if (temp == NULL)
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

//Function for delete the 2D tab int
void tab2D_free(int** tabXY, int x)
{
	for (int i = 0; i < x; i++)
	{
		free(tabXY[i]);
	}
	free(tabXY);
}

//This function create a new game tab
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
		cont = 1;
		for (int i = 0; i < sizeX; i++)
		{
			for (int l = 0; l < sizeY; l++)
			{
				plat[i][l] = cont;
				cont++;
			}
		}
		plat[3][3] = 0;
	}

	return plat;
}

//This function validate the move of the player
void validateMove(int cursX, int cursY, int cursX2, int cursY2, int** plat, Item_text* text, Item** surface, int dim)
{
	int test = plat[cursX][cursY] == 0 || plat[cursX2][cursY2] == 0;
	SDL_Texture* tempT = NULL;
	SDL_Texture* tempTas = NULL;
	int tempH = 0, tempW = 0;

	if (abs(cursX - cursX2) == 1 && abs(cursY - cursY2) == 0 && test)
	{
		tempTas = surface[cursX][cursY].texture;
		surface[cursX][cursY].texture = surface[cursX2][cursY2].texture;
		surface[cursX2][cursY2].texture = tempTas;

		int temp = plat[cursX][cursY];
		plat[cursX][cursY] = plat[cursX2][cursY2];
		plat[cursX2][cursY2] = temp;

		tempT = text[cursX2 * dim + cursY2].text;
		text[cursX2 * dim + cursY2].text = text[cursX * dim + cursY].text;
		text[cursX * dim + cursY].text = tempT;

		tempH = text[cursX2 * dim + cursY2].dest.h;
		text[cursX2 * dim + cursY2].dest.h = text[cursX * dim + cursY].dest.h;
		text[cursX * dim + cursY].dest.h = tempH;

		tempW = text[cursX2 * dim + cursY2].dest.w;
		text[cursX2 * dim + cursY2].dest.w = text[cursX * dim + cursY].dest.w;
		text[cursX * dim + cursY].dest.w = tempW;

		text[cursX * dim + cursY].dest.x = (int)(surface[cursX][cursY].dest.x + (surface[cursX][cursY].dest.w / 2) - (text[cursX * dim + cursY].dest.w / 2));
		text[cursX * dim + cursY].dest.y = (int)(surface[cursX][cursY].dest.y + (surface[cursX][cursY].dest.h / 2) - (text[cursX * dim + cursY].dest.h / 2));

		text[cursX2 * dim + cursY2].dest.x = (int)(surface[cursX2][cursY2].dest.x + (surface[cursX2][cursY2].dest.w / 2) - (text[cursX2 * dim + cursY2].dest.w / 2));
		text[cursX2 * dim + cursY2].dest.y = (int)(surface[cursX2][cursY2].dest.y + (surface[cursX2][cursY2].dest.h / 2) - (text[cursX2 * dim + cursY2].dest.h / 2));
	}
	else if (abs(cursX - cursX2) == 0 && abs(cursY - cursY2) == 1 && test)
	{
		tempTas = surface[cursX][cursY].texture;
		surface[cursX][cursY].texture = surface[cursX2][cursY2].texture;
		surface[cursX2][cursY2].texture = tempTas;

		int temp = plat[cursX][cursY];
		plat[cursX][cursY] = plat[cursX2][cursY2];
		plat[cursX2][cursY2] = temp;

		tempT = text[cursX2 * dim + cursY2].text;
		text[cursX2 * dim + cursY2].text = text[cursX * dim + cursY].text;
		text[cursX * dim + cursY].text = tempT;

		tempH = text[cursX2 * dim + cursY2].dest.h;
		text[cursX2 * dim + cursY2].dest.h = text[cursX * dim + cursY].dest.h;
		text[cursX * dim + cursY].dest.h = tempH;

		tempW = text[cursX2 * dim + cursY2].dest.w;
		text[cursX2 * dim + cursY2].dest.w = text[cursX * dim + cursY].dest.w;
		text[cursX * dim + cursY].dest.w = tempW;

		text[cursX * dim + cursY].dest.x = (int)(surface[cursX][cursY].dest.x + (surface[cursX][cursY].dest.w / 2) - (text[cursX * dim + cursY].dest.w / 2));
		text[cursX * dim + cursY].dest.y = (int)(surface[cursX][cursY].dest.y + (surface[cursX][cursY].dest.h / 2) - (text[cursX * dim + cursY].dest.h / 2));

		text[cursX2 * dim + cursY2].dest.x = (int)(surface[cursX2][cursY2].dest.x + (surface[cursX2][cursY2].dest.w / 2) - (text[cursX2 * dim + cursY2].dest.w / 2));
		text[cursX2 * dim + cursY2].dest.y = (int)(surface[cursX2][cursY2].dest.y + (surface[cursX2][cursY2].dest.h / 2) - (text[cursX2 * dim + cursY2].dest.h / 2));
	}
}

//This function check if the tab is in the correct order
int checkWin(int** plat, int dim)
{
	int** checkTab = newPlat(dim, dim, 0);
	for (int i = 0; i < dim; i++)
	{
		for (int l = 0; l < dim; l++)
		{
			if (plat[i][l] != checkTab[i][l])
			{
				tab2D_free(checkTab, dim);
				return 1;
			}
		}
	}

	tab2D_free(checkTab, dim);
	return 0;
}
