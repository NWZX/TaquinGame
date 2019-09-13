#include "SDL.h"
#include "TaquinBase.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	int** plat = NULL;
	plat = newPlat(4, 4, 0);

	int cursX = 0, cursY = 0;
	int* cursX2 = NULL, * cursY2 = NULL;

	while (1)
	{
		newScreen(cursX, cursY, cursX2, cursY2, plat, 4);
		int result = interceptKey(&cursX, &cursY);
		if (result == 1)
		{
			if (cursX2 == NULL && cursY2 == NULL)
			{
				cursX2 = malloc(sizeof(int));
				cursY2 = malloc(sizeof(int));
				if (cursX2 == NULL || cursY2 == NULL)
				{
					exit(0);
				}

				*cursX2 = cursX;
				*cursY2 = cursY;
			}
			else
			{
				validateMove(cursX, cursY, cursX2, cursY2, plat);
				cursX2 = NULL;
				cursY2 = NULL;
			}
		}
		if (result == -1)
		{
			if (cursX2 == NULL)
			{
				free(cursX2);
			}
			if (cursY2 == NULL)
			{
				free(cursY2);
			}
			tab2D_free(plat, 4);
			break;
		}
	}
	return 0;
}