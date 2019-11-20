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

//Function for generate a new 2D tab of Item
Item** tab2D_initG(SDL_Renderer* render, int dim, int windowX, int windowY)
{
	Item** items = NULL;
	items = malloc(dim * 8 * sizeof(Item*));
	if (items == NULL)
	{
		exit(0);
	}
	for (int i = 0; i < dim; i++)
	{
		items[i] = malloc(dim * sizeof(Item));
		if (items[i] == NULL)
		{
			exit(0);
		}
	}

	int trueBoardX = (int)((windowX/2 - 640/2));
	int trueBoardY = (int)((windowY/2 - 480/2));

	SDL_Surface* cube_sprite = NULL;
	cube_sprite = IMG_Load("res/cube.png");
	if (cube_sprite == NULL)
		exit(0);

	int spaceX = (640 - cube_sprite->w * dim) / dim;
	int spaceY = (480 - cube_sprite->h * dim) / dim;

	for (int i = 0; i < dim; i++)
	{
		for (int l = 0; l < dim; l++)
		{
			items[i][l].texture = SDL_CreateTextureFromSurface(render, cube_sprite);
			items[i][l].dest.x = (trueBoardX+spaceX/2) + (cube_sprite->w+spaceX)*l;
			items[i][l].dest.y = (trueBoardY+spaceY/2) + (cube_sprite->h +spaceY)*i;
			items[i][l].dest.h = cube_sprite->h;
			items[i][l].dest.w = cube_sprite->w;
		}
	}

	SDL_FreeSurface(cube_sprite);

	return items;
}

//Function for delete the 2D tab of Item
void tab2D_freeG(Item** items, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		for (int l = 0; l < dim; l++)
		{
			SDL_DestroyTexture(items[i][l].texture);
		}
		free(items[i]);
	}
	free(items);
}

//This function check and process the action if the player press a key
int interceptKeyG(Mix_Chunk* move, SDL_Event* even, int* cursX, int* cursY, int dim)
{
	if (SDL_PollEvent(even))
	{
		if (even->window.event == SDL_WINDOWEVENT_CLOSE) // Fermeture de la fenêtre
		{
			return -1;
		}
		if (even->type == SDL_KEYUP)
		{
			if (even->key.keysym.sym == SDLK_ESCAPE)
			{
				return -1;
			}
			else if (even->key.keysym.sym == SDLK_UP && *cursX > 0)
			{
				*cursX -= 1;
				Mix_PlayChannel(-1, move, 0);
				
			}
			else if (even->key.keysym.sym == SDLK_DOWN && *cursX < dim-1)
			{
				*cursX += 1;
				Mix_PlayChannel(-1, move, 0);
			}
			else if (even->key.keysym.sym == SDLK_RIGHT && *cursY < dim-1)
			{
				*cursY += 1;
				Mix_PlayChannel(-1, move, 0);
			}
			else if (even->key.keysym.sym == SDLK_LEFT && *cursY > 0)
			{
				*cursY -= 1;
				Mix_PlayChannel(-1, move, 0);
			}
			else
			{
				return 1;
			}
		}
	}
	return 0;
}

Uint32 trick(Uint32 intervalle, void* parametre)
{
	int* temps = parametre;
	*temps += 1;
	//printf("Temps : %d:%d\n", *temps / 60, *temps % 60);
	return intervalle;
}

void newGameBoard(int boardDim, int screenSizeX, int screenSizeY, int rand, SDL_Renderer* render)
{
	int cursX = 0, cursY = 0;
	int cursX2 = 0, cursY2 = 0;

	//Initiate window
	//-------------------------------------------------------
	SDL_Event event;
	Item** plat = NULL;

	SDL_RenderClear(render);

	plat = tab2D_initG(render, boardDim, screenSizeX, screenSizeY);
	if (plat == NULL)
	{
		exit(0);
	}

	// Make board texture
	//--------------------------------------------------------

	SDL_Surface* board_sprite = NULL;
	board_sprite = IMG_Load("res/back_fond.jpg");
	if (board_sprite == NULL)
		exit(0);

	SDL_Texture* board = SDL_CreateTextureFromSurface(render, board_sprite);
	SDL_FreeSurface(board_sprite);

	SDL_Rect dest = { 0, 0, 800, 600 };
	//SDL_Rect dest = { screenSizeX / 2 - board_sprite->w / 2, screenSizeY / 2 - board_sprite->h / 2, board_sprite->w, board_sprite->h };

	//Make hover texture
	//-------------------------------------------------

	SDL_Surface* hover_sprite = NULL;
	hover_sprite = IMG_Load("res/hover.png");
	if (hover_sprite == NULL)
		exit(0);

	SDL_Texture* hover = SDL_CreateTextureFromSurface(render, hover_sprite);
	SDL_Rect hover_dest = { 
		plat[cursX][cursY].dest.x - ((hover_sprite->w - plat[cursX][cursY].dest.w) / 2), 
		plat[cursX][cursY].dest.y - ((hover_sprite->h - plat[cursX][cursY].dest.h) / 2),
		hover_sprite->w,
		hover_sprite->h
	};
	SDL_FreeSurface(hover_sprite);

	//Make select texture
	//-------------------------------------------------

	SDL_Surface* select_sprite = NULL;
	select_sprite = IMG_Load("res/select.png");
	if (select_sprite == NULL)
		exit(0);

	SDL_Texture* select = SDL_CreateTextureFromSurface(render, select_sprite);
	SDL_Rect select_dest;
	select_dest.h = select_sprite->h;
	select_dest.w = select_sprite->w;
	SDL_FreeSurface(select_sprite);

	//------------------------------
	Mix_Chunk* move = NULL;
	move = Mix_LoadWAV("res/move.wav");

	Mix_Chunk* win = NULL;
	win = Mix_LoadWAV("res/win.wav");

	//Make text texture
	//--------------------------------------------------

	Item_text* text = malloc((int)pow(boardDim, 2) *40* sizeof(Item_text));
	SDL_Color color = { 0,0,0 };
	TTF_Font* font = TTF_OpenFont("res/arial.ttf", 25);
	if (text == NULL)
	{
		exit(0);
	}

	char buff[4] = "";
	SDL_Surface* temp = NULL;
	int* vir_inf_plat = NULL;
	int** virtual_plat = NULL;

	virtual_plat = newPlat(boardDim, boardDim, 1);
	vir_inf_plat = tab2D_convert(virtual_plat, boardDim);
	if (vir_inf_plat == NULL)
	{
		exit(0);
	}

	for (int i = 0; i < (int)pow(boardDim, 2); i++)
	{
		sprintf_s(buff, 4, "%d", vir_inf_plat[i]);
		temp = TTF_RenderText_Blended(font, buff, color);
		if (temp == NULL)
		{
			printf("Error : %s", TTF_GetError());
			exit(0);
		}
		text[i].dest.h = temp->h;
		text[i].dest.w = temp->w;
		text[i].dest.x = (int)(plat[i / boardDim][i % boardDim].dest.x + (plat[i / boardDim][i % boardDim].dest.w / 2) - (temp->w / 2));
		text[i].dest.y = (int)(plat[i / boardDim][i % boardDim].dest.y + (plat[i / boardDim][i % boardDim].dest.h / 2) - (temp->h / 2));
		text[i].text = SDL_CreateTextureFromSurface(render, temp);
		SDL_FreeSurface(temp);
	}

	free(vir_inf_plat);

	int temps = 0;
	SDL_TimerID timer = SDL_AddTimer(1000, trick, &temps);
	SDL_Surface* text_s = NULL;
	Item_text text_timer;
	char buff_text[20];

	while (1)
	{
		int result = interceptKeyG(move, &event, &cursX, &cursY, boardDim);
		if (result == 0)
		{
			validateMove(cursX, cursY, cursX2, cursY2, virtual_plat, text, plat, boardDim);
			if (checkWin(virtual_plat, boardDim) == 0)
			{
				Mix_PlayChannel(-1, win, 0);
				break;
			}

			cursX2 = cursX;
			cursY2 = cursY;
		}
		else if (result == -1)
		{
			break;
		}

		SDL_RenderClear(render);
		SDL_RenderCopy(render, board, NULL, &dest);

		hover_dest.x = plat[cursX][cursY].dest.x - ((hover_dest.w - plat[cursX][cursY].dest.w) / 2);
		hover_dest.y = plat[cursX][cursY].dest.y - ((hover_dest.h - plat[cursX][cursY].dest.h) / 2);
		SDL_RenderCopy(render, hover, NULL, &hover_dest);

		//if (cursX2 != NULL && cursY2 != NULL)
		//{
		//	select_dest.x = plat[cursX2][cursY2].dest.x - ((select_dest.w - plat[cursX2][cursY2].dest.w) / 2);
		//	select_dest.y = plat[cursX2][cursY2].dest.y - ((select_dest.h - plat[cursX2][cursY2].dest.h) / 2);
		//	SDL_RenderCopy(render, select, NULL, &select_dest);
		//}

		for (int i = 0; i < boardDim; i++)
		{
			for (int l = 0; l < boardDim; l++)
			{
				SDL_RenderCopy(render, plat[i][l].texture, NULL, &plat[i][l].dest);
				SDL_RenderCopy(render, text[i * boardDim + l].text, NULL, &text[i * boardDim +  l].dest);
			}
		}

		sprintf_s(buff_text, 20, "Temps : %d:%d", temps / 60, temps % 60);
		text_s = TTF_RenderText_Solid(font, buff_text, color);
		text_timer.dest.h = text_s->h;
		text_timer.dest.w = text_s->w;
		text_timer.dest.x = (screenSizeX / 2) - (text_s->w) / 2;
		text_timer.dest.y = 0;
		text_timer.text = SDL_CreateTextureFromSurface(render, text_s);
		SDL_FreeSurface(text_s);
		SDL_RenderCopy(render, text_timer.text, NULL, &text_timer.dest);
		SDL_DestroyTexture(text_timer.text);

		SDL_RenderPresent(render);
		SDL_Delay(1); //Reduce CPU activity
	}

	Mix_FreeChunk(move);
	Mix_FreeChunk(win);
	tab2D_freeG(plat, boardDim);

	Mix_HaltChannel(-1);
	SDL_RemoveTimer(timer);

	for (int i = 0; i < (int)pow(boardDim, 2); i++)
	{
		SDL_DestroyTexture(text[i].text);
	}

	TTF_CloseFont(font);

	tab2D_free(virtual_plat, boardDim);
	free(text);

	SDL_DestroyTexture(board); // Destroy texture

	//SDL_DestroyRenderer(render); //

}

int interceptMouse(Mix_Chunk* select, Mix_Chunk* selectc, SDL_Event* even, int hover)
{
	int cube1IX = 244, cube1IY = 206;
	int cube1SX = 286, cube1SY = 110;
	int cube2IX = 244, cube2IY = 357;
	int cube2SX = 286, cube2SY = 118;

	if (SDL_PollEvent(even))
	{
		if (even->window.event == SDL_WINDOWEVENT_CLOSE) // Fermeture de la fenêtre
		{
			return -1;
		}
		if (even->type == SDL_MOUSEBUTTONDOWN)
		{
			if (even->button.x > cube1IX&& even->button.x < cube1IX + cube1SX && even->button.y > cube1IY&& even->button.y < cube1IY + cube1SY)
			{
				Mix_PlayChannel(-1, selectc, 0);
				return 1;
			}
			if (even->button.x > cube2IX&& even->button.x < cube2IX + cube2SX && even->button.y > cube2IY&& even->button.y < cube2IY + cube2SY)
			{
				Mix_PlayChannel(-1, selectc, 0);
				return 2;
			}
		}
		else if (even->type == SDL_MOUSEMOTION)
		{
			if (even->button.x > cube1IX&& even->button.x < cube1IX + cube1SX && even->button.y > cube1IY&& even->button.y < cube1IY + cube1SY)
			{
				if(hover == 0)
					Mix_PlayChannel(-1, select, 0);

				return 3;
			}
			if (even->button.x > cube2IX&& even->button.x < cube2IX + cube2SX && even->button.y > cube2IY&& even->button.y < cube2IY + cube2SY)
			{
				if(hover == 0)
					Mix_PlayChannel(-1, select, 0);

				return 3;
			}
			else
			{
				return 4;
			}
		}
	}
	return 0;
}

int newMenu(SDL_Renderer* render)
{
	SDL_Event even;
	int result = 0;
	int hover = 0;

	SDL_RenderClear(render);

	SDL_Surface* board_sprite = NULL;
	board_sprite = IMG_Load("res/menu.png");
	if (board_sprite == NULL)
		exit(0);

	SDL_Texture* board = SDL_CreateTextureFromSurface(render, board_sprite);
	SDL_FreeSurface(board_sprite);

	SDL_Rect dest = { 0, 0, board_sprite->w, board_sprite->h };

	Mix_Chunk* select = NULL;
	select = Mix_LoadWAV("res/select.wav");

	Mix_Chunk* selectc = NULL;
	selectc = Mix_LoadWAV("res/selectAM.wav");

	while (1)
	{
		result = interceptMouse(select,selectc, &even, hover);
		if(result != 0)
		{
			if (result == 1) {
				SDL_Delay(250);
				break;
			}
			else if (result == 3)
				hover = 1;
			else if (result == 4)
				hover = 0;
			else {
				SDL_Delay(250);
				break;
			}
		}
		SDL_RenderCopy(render, board, NULL, &dest);
		SDL_RenderPresent(render);
		SDL_Delay(1); //Reduce CPU activity
	}

	Mix_FreeChunk(select);
	Mix_FreeChunk(selectc);
	SDL_DestroyTexture(board); // Destroy texture
	return result;
}