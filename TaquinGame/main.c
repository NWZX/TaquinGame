#include "SDL.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout,"�chec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

	SDL_Window* window = SDL_CreateWindow
	("TaquinGame", // window's title
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, // coordinates on the screen, in pixels, of the window's upper left corner
		640, 480, // window's length and height in pixels  
		SDL_WINDOW_OPENGL);

	if (window)
	{
		SDL_Delay(3000); /* Attendre trois secondes, que l'utilisateur voie la fen�tre */

		SDL_DestroyWindow(window);
	}
	else
	{
		fprintf(stderr, "Erreur de cr�ation de la fen�tre: %s\n", SDL_GetError());
	}

	SDL_Quit();
	return 0;
}