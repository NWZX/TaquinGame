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

#include <SDL.h>
#include <SDL_ttf.h>	
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <crtdbg.h>

#pragma once

#define _CRTDBG_MAP_ALLOC

typedef struct Item Item;
struct Item
{
	SDL_Texture* texture;
	SDL_Rect dest;
};

typedef struct Item_text Item_text;
struct Item_text
{
	SDL_Texture* text;
	SDL_Rect dest;
};

int randO(int min, int max);
int** tab2D_init(int x, int y);
int* tab2D_convert(int** tabXY, int dimSize);
void tab2D_free(int** tabXY, int x);
int** newPlat(int sizeX, int sizeY, int random);
void validateMove(int cursX, int cursY, int* cursX2, int* cursY2, int** plat, Item_text* text, Item** surface, int dim);
int checkWin(int** plat, int dim);