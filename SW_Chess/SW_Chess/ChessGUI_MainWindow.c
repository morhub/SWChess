#include <stdlib.h>
#include "ChessGUI_MainWindow.h"
#include "Button.h"

static const int width = 450;
static const int height = 700; 

//Helper function to create buttons in the simple window;
Widget** createMainWindowWidgets(SDL_Renderer* renderer) 
{
    
	if (renderer == NULL ) {
        printf("EROR SDL-6; eror creating a SDL window\n");
		return NULL ;
	}
	Widget** widgets = malloc(sizeof(Widget*) * 3);
	if (widgets == NULL ) 
	{
        printf("EROR SDL-5; eror creating a SDL window\n");
		return NULL ;
	}
	
	SDL_Rect newGame =	{ .x = 125, .y = 70, .h = 80, .w = 200 };
	SDL_Rect loadGame = { .x = 125, .y = 190, .h = 80, .w = 200 };
	SDL_Rect quit =		{ .x = 125, .y = 310, .h = 80, .w = 200 };

	widgets[0] = createButton(renderer, &newGame,		"./start_active.bmp", CHESS_NEWGAME_BUTTON);
	widgets[1] = createButton(renderer, &loadGame,		"./load_active.bmp" , CHESS_LOAD_BUTTON);
	widgets[2] = createButton(renderer, &quit,			"./exit_active.bmp" , CHESS_QUIT_BUTTON);

	if ((widgets[0] == NULL) || (widgets[1] == NULL) || (widgets[2] == NULL))
	{
        printf("EROR SDL-4; eror creating a SDL window\n");
		destroyWidget(widgets[0]); //NULL SAFE
		destroyWidget(widgets[1]); //NULL SAFE
		destroyWidget(widgets[2]); //NULL SAFE
		free(widgets);
		return NULL ;
	}

	return widgets;
}


ChessWindow* createMainWindow(Uint32 winMode, chessGame* game)
{
	ChessWindow* res = malloc(sizeof(ChessWindow));
	chessMainWindow* data = malloc(sizeof(chessMainWindow));

	SDL_Window* window = SDL_CreateWindow("CHESS!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, winMode);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Widget** widgets = createMainWindowWidgets(renderer);

	if ((res == NULL) || (data == NULL) || (window == NULL) || (renderer == NULL) || (widgets == NULL))
	{
        printf("EROR SDL-1; eror creating a SDL window\n");
		free(res);
		free(data);
		free(widgets);
		SDL_DestroyRenderer(renderer); //NULL safe
		SDL_DestroyWindow(window);	  //NULL safe
		return NULL ;
	}
	
	data->widgets = widgets;
	data->numOfWidgets = 3;
	data->window = window;
	data->windowRenderer = renderer;

	for (int i = 0; i < data->numOfWidgets; i++)
	{
		data->widgets[i]->isDragLegal = false;
	}

	res->data = (void*) data;
	res->destroyWindow = destroyMainWindow;
	res->drawWindow = drawMainWindow;
	res->handleEventWindow = handleEventMainWindow;
	res->type = CHESS_MAIN_WINDOW;
	res->game = game;

	return res;
}


void destroyMainWindow(ChessWindow* src) 
{
	if (src == NULL ) {
		return;
	}
	chessMainWindow* data = (chessMainWindow*) src->data;
	for (int i = 0;  i < data->numOfWidgets; i++) 
	{
		destroyWidget(data->widgets[i]);
	}
	free(data->widgets);
	SDL_DestroyRenderer(data->windowRenderer);
	SDL_DestroyWindow(data->window);
	free(data);
	free(src);
}


void drawMainWindow(ChessWindow* src)
{
	if (src == NULL ) 
		return;
    SDL_Delay(100);

	chessMainWindow* data = (chessMainWindow*) src->data;
	SDL_RenderClear(data->windowRenderer);
    SDL_Delay(100);

	//draw background
	SDL_Surface* surf = SDL_LoadBMP("./main_background.bmp");
	SDL_Texture * background = SDL_CreateTextureFromSurface(data->windowRenderer, surf);
	if ((surf == NULL) || (background == NULL))
	{
		free(surf);
		SDL_DestroyTexture(background);
		return;
	}
	SDL_FreeSurface(surf);
	if (SDL_SetTextureBlendMode(background, SDL_BLENDMODE_NONE) != 0)
	{
		printf("ERROR: unable to blend background texture: %s\n", SDL_GetError());
	}

	SDL_RenderCopy(data->windowRenderer, background, NULL, NULL);
	SDL_DestroyTexture(background);
	
	//Draw window
	for (int i = 0; i < data->numOfWidgets; i++) 
	{
		if (data->widgets[i]->isVisible)
			data->widgets[i]->drawWidget(data->widgets[i]);
	}
	SDL_Delay(100);
	SDL_RenderPresent(data->windowRenderer);
}


WINDOW_EVENT handleEventMainWindow(ChessWindow* src, SDL_Event* event)
{
	if((src == NULL) || (event==NULL))
		return CHESS_EMPTY_WINDOWEVENT;

	chessMainWindow* windata = (chessMainWindow*)src->data;

	if (!((event->type == SDL_MOUSEBUTTONDOWN)  && (event->button.button == SDL_BUTTON_LEFT)) &&
		!((event->type == SDL_MOUSEBUTTONUP) && (event->button.button == SDL_BUTTON_LEFT)))
		return CHESS_EMPTY_WINDOWEVENT;
	
	for(int i=0; i<windata->numOfWidgets; i++)
	{
		windata->widgets[i]->handleEvent(windata->widgets[i], event);
		SDL_RenderPresent(windata->windowRenderer);

		if (windata->widgets[i]->isActive)
		{
			switch (windata->widgets[i]->widget_type)
			{
			case CHESS_EMPTY_BUTTON:
				return CHESS_EMPTY_WINDOWEVENT;
			case CHESS_NEWGAME_BUTTON:
				windata->widgets[i]->isActive = false;
			    return CHESS_SETTINGS_WINDOWEVENT;
			case CHESS_LOAD_BUTTON:
				windata->widgets[i]->isActive = false;
				return CHESS_LOAD_SCREEN_WINDOWEVENT;
			case CHESS_QUIT_BUTTON:
				windata->widgets[i]->isActive = false;
				return CHESS_QUIT_WINDOWEVENT;
			default:
				return CHESS_EMPTY_WINDOWEVENT;
			}
		}
		
		
	}
	return CHESS_EMPTY_WINDOWEVENT;   //return error!!!!!!!!!!!!!! 
}
