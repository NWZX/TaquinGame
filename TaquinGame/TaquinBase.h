#pragma once

#define _CRTDBG_MAP_ALLOC

int** tab2D_init(int x, int y);
void tab2D_free(int** tabXY, int x);
int** newPlat(int sizeX, int sizeY, int random);
void newScreen(int cursX, int cursY, int* cursX2, int* cursY2, int** plat, int size);
int interceptKey(int* cursX, int* cursY);
void validateMove(int cursX, int cursY, int* cursX2, int* cursY2, int** plat);