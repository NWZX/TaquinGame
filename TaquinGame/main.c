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

#include "Taquin_Game_UI.h"
#include "main.h"

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //Check memory leaks

	srand(time(NULL));

	//Init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		fprintf(stdout, "Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
		return -1;
	}

	//Init SDL_TTF
	if (TTF_Init() < 0) {
		exit(0);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
	{
		printf("%s", Mix_GetError());
	}
	Mix_AllocateChannels(32);
	Mix_Volume(-1, 128);

	Mix_Music* musique;
	musique = Mix_LoadMUS("res/musique.mp3");

	Mix_Music* Hmusique;
	Hmusique = Mix_LoadMUS("res/menu.mp3");
	

	//Make a new window
	SDL_Window* pWindow = NULL;
	pWindow = SDL_CreateWindow("TaquinGame", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800,
		600,
		SDL_WINDOW_OPENGL);

	if (pWindow)
	{
		Mix_PlayMusic(Hmusique, -1);

		SDL_Renderer* render = NULL;
		render = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
		if (render == NULL)
		{
			exit(0);
		}
		SDL_SetRenderDrawColor(render, 255, 171, 76, 255);

		while (1)
		{
			if (newMenu(render) == 1)
			{
				Mix_PausedMusic();

				Mix_PlayMusic(musique, -1);
				newGameBoard(4, 800, 600, 0, render);

				Mix_PausedMusic();

				Mix_PlayMusic(Hmusique, -1);
			}
			else
			{
				break;
			}
		}

		SDL_DestroyRenderer(render);
		SDL_DestroyWindow(pWindow);
	}
	Mix_FreeMusic(Hmusique);
	Mix_FreeMusic(musique);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
	return 0;
}