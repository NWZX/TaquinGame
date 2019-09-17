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
#include "Taquin_Game_UI.h"
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include <crtdbg.h>


int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //Check memory leaks

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stdout, "Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
		return -1;
	}
	if (TTF_Init() < 0) {
		exit(0);
	}

	SDL_Window* pWindow = NULL;
	pWindow = SDL_CreateWindow("TaquinGame", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800,
		600,
		SDL_WINDOW_OPENGL);

	if (pWindow)
	{
		newGameBoard(4, 800, 600, 0, pWindow);

		SDL_DestroyWindow(pWindow);
	}

	TTF_Quit();
	SDL_Quit();

	//int** plat = NULL;
	//plat = newPlat(4, 4, 0);

	//int cursX = 0, cursY = 0;
	//int* cursX2 = NULL, * cursY2 = NULL;

	//while (1)
	//{
	//	newScreen(cursX, cursY, cursX2, cursY2, plat, 4);
	//	int result = interceptKey(&cursX, &cursY);
	//	if (result == 1)
	//	{
	//		if (cursX2 == NULL && cursY2 == NULL)
	//		{
	//			cursX2 = malloc(sizeof(int));
	//			cursY2 = malloc(sizeof(int));
	//			if (cursX2 == NULL || cursY2 == NULL)
	//			{
	//				exit(0);
	//			}

	//			*cursX2 = cursX;
	//			*cursY2 = cursY;
	//		}
	//		else
	//		{
	//			validateMove(cursX, cursY, cursX2, cursY2, plat);
	//			free(cursX2);
	//			free(cursY2);
	//			cursX2 = NULL;
	//			cursY2 = NULL;
	//		}
	//	}
	//	if (result == -1)
	//	{
	//		if (cursX2 == NULL)
	//		{
	//			free(cursX2);
	//		}
	//		if (cursY2 == NULL)
	//		{
	//			free(cursY2);
	//		}
	//		tab2D_free(plat, 4);
	//		break;
	//	}
	//}
	return 0;
}