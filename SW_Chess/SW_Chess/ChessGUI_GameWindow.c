#include <stdlib.h>
#include "ChessGUI_GameWindow.h"
#include "Button.h"
#include "ChessGUI_Utils.h"
#include "Chess_gameUtils.h"
#include "Chess_FlowOnePlayer.h"

static const int game_width = 900;
static const int game_height = 700;
bool steadyBoard = true;

/////////////////for the exit message box////////////////// 
const SDL_MessageBoxButtonData buttons[] = {
	{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,   0,    "yes" },
	{ 0,										 1,     "no" },
	{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,   2, "cancel" },
}; 
const SDL_MessageBoxColorScheme colorScheme = {
	{ /* .colors (.r, .g, .b) */
	  /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
		{ 255,   0,   0 },
		/* [SDL_MESSAGEBOX_COLOR_TEXT] */
		{ 0, 255,   0 },
		/* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
		{ 255, 255,   0 },
		/* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
		{ 0,   0, 255 },
		/* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
		{ 255,   0, 255 }
	}
};

const SDL_MessageBoxData messageboxdata = {
	SDL_MESSAGEBOX_INFORMATION,
	NULL,
	"Chess!",
	"Do you want to save changes?",
	SDL_arraysize(buttons),
	buttons,
	&colorScheme
};

int buttonid = -1;

WINDOW_EVENT showSavingMessage()
{
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0)
	{
		printf("ERROR: error displaying message box: %s\n", SDL_GetError());
		return CHESS_EMPTY_WINDOWEVENT;
	}
	else if (buttonid == 0) // yes
		return CHESS_SAVE_WINDOWEVENT;

	else if (buttonid == 1) // no
		return CHESS_DONT_SAVE_WINDOWEVENT;

	else if (buttonid == 2) //cancel
		return CHESS_CANCEL_WINDOWEVENT;
	
	printf("message box: no selection\n");
	return CHESS_EMPTY_WINDOWEVENT;
}

//Helper function to create buttons in the simple window;
Widget** createGameWindowWidgets(SDL_Renderer* renderer, ChessWindow* window)
{	
	if ((!renderer) || (!window))
		return NULL;
	
	Widget** widgets = malloc(sizeof(Widget*) * 38);
	if (!widgets) 
		return NULL;
	
	memset(widgets, 0, 38);

	SDL_Rect restart =  { .x = 80,.y = 70, .h = 72,.w = 180 };
	SDL_Rect save    =  { .x = 80,.y = 162,.h = 72,.w = 180 };
	SDL_Rect load	 =  { .x = 80,.y = 254,.h = 72,.w = 180 };
	SDL_Rect undo	 =  { .x = 80,.y = 346,.h = 72,.w = 180 };
	SDL_Rect home	 =  { .x = 80,.y = 438,.h = 72,.w = 180 };
	SDL_Rect exit	 =  { .x = 80,.y = 530,.h = 72,.w = 180 };
    
	SDL_Rect rookWhite =	{.x = window->game->gameGUIBoard[0][0].x,.y = window->game->gameGUIBoard[0][0].y,
							 .h = window->game->gameGUIBoard[0][0].h,.w = window->game->gameGUIBoard[0][0].w };

	SDL_Rect knightWhite =  {.x = window->game->gameGUIBoard[0][1].x,.y = window->game->gameGUIBoard[0][1].y,
							 .h = window->game->gameGUIBoard[0][1].h,.w = window->game->gameGUIBoard[0][1].w };

	SDL_Rect bishopWhite =  {.x = window->game->gameGUIBoard[0][2].x,.y = window->game->gameGUIBoard[0][2].y,
							 .h = window->game->gameGUIBoard[0][2].h,.w = window->game->gameGUIBoard[0][2].w };

	SDL_Rect queenWhite =   {.x = window->game->gameGUIBoard[0][3].x,.y = window->game->gameGUIBoard[0][3].y,
							 .h = window->game->gameGUIBoard[0][3].h,.w = window->game->gameGUIBoard[0][3].w };

	SDL_Rect kingWhite =    {.x = window->game->gameGUIBoard[0][4].x,.y = window->game->gameGUIBoard[0][4].y,
						     .h = window->game->gameGUIBoard[0][4].h,.w = window->game->gameGUIBoard[0][4].w };

	SDL_Rect bishopWhite2 =  {.x = window->game->gameGUIBoard[0][5].x,.y = window->game->gameGUIBoard[0][5].y,
							  .h = window->game->gameGUIBoard[0][5].h,.w = window->game->gameGUIBoard[0][5].w };

	SDL_Rect knightWhite2 =  {.x = window->game->gameGUIBoard[0][6].x,.y = window->game->gameGUIBoard[0][6].y,
							  .h = window->game->gameGUIBoard[0][6].h,.w = window->game->gameGUIBoard[0][6].w };

	SDL_Rect rookWhite2 =    {.x = window->game->gameGUIBoard[0][7].x,.y = window->game->gameGUIBoard[0][7].y,
							  .h = window->game->gameGUIBoard[0][7].h,.w = window->game->gameGUIBoard[0][7].w };

	SDL_Rect rookBlack =    {.x = window->game->gameGUIBoard[7][0].x,.y = window->game->gameGUIBoard[7][0].y,
							 .h = window->game->gameGUIBoard[7][0].h,.w = window->game->gameGUIBoard[7][0].w };
															 
	SDL_Rect knightBlack =  {.x = window->game->gameGUIBoard[7][1].x,.y = window->game->gameGUIBoard[7][1].y,
							 .h = window->game->gameGUIBoard[7][1].h,.w = window->game->gameGUIBoard[7][1].w };
															 
	SDL_Rect bishopBlack =  {.x = window->game->gameGUIBoard[7][2].x,.y = window->game->gameGUIBoard[7][2].y,
							 .h = window->game->gameGUIBoard[7][2].h,.w = window->game->gameGUIBoard[7][2].w };
															 
	SDL_Rect queenBlack =   {.x = window->game->gameGUIBoard[7][3].x,.y = window->game->gameGUIBoard[7][3].y,
							 .h = window->game->gameGUIBoard[7][3].h,.w = window->game->gameGUIBoard[7][3].w };
															 
	SDL_Rect kingBlack   =  {.x = window->game->gameGUIBoard[7][4].x,.y = window->game->gameGUIBoard[7][4].y,
							 .h = window->game->gameGUIBoard[7][4].h,.w = window->game->gameGUIBoard[7][4].w };
															 
	SDL_Rect bishopBlack2 = {.x = window->game->gameGUIBoard[7][5].x,.y = window->game->gameGUIBoard[7][5].y,
							 .h = window->game->gameGUIBoard[7][5].h,.w = window->game->gameGUIBoard[7][5].w };
															 
	SDL_Rect knightBlack2 = {.x = window->game->gameGUIBoard[7][6].x,.y = window->game->gameGUIBoard[7][6].y,
							 .h = window->game->gameGUIBoard[7][6].h,.w = window->game->gameGUIBoard[7][6].w };

	SDL_Rect rookBlack2 =   {.x = window->game->gameGUIBoard[7][7].x,.y = window->game->gameGUIBoard[7][7].y,
							 .h = window->game->gameGUIBoard[7][7].h,.w = window->game->gameGUIBoard[7][7].w };

	widgets[0]  = createButton(renderer, &restart,     "./restart_active.bmp", CHESS_RESTART_BUTTON);
	widgets[1]  = createButton(renderer, &save,	       "./save_active.bmp", CHESS_SAVE_BUTTON);
	widgets[2]  = createButton(renderer, &load,	       "./load_active.bmp", CHESS_LOAD_BUTTON);
	if ((spArrayListIsEmpty(window->game->historyArray)) || (window->game->gameMode == TWO_PLAYERS))
		widgets[3]  = createButton(renderer, &undo, "./undo_nonactive.bmp", CHESS_UNDO_BUTTON);
	else
		widgets[3] = createButton(renderer, &undo,  "./undo_active.bmp"   , CHESS_UNDO_BUTTON);
	widgets[4]  = createButton(renderer, &home,        "./home_active.bmp", CHESS_HOME_BUTTON);
	widgets[5]  = createButton(renderer, &exit,        "./exit_active.bmp", CHESS_QUIT_BUTTON);
	
	widgets[6] = createButton(renderer, &bishopWhite,  "./bishop_white_active.bmp", CHESS_BISHOP_WHITE_BUTTON);
	widgets[7] = createButton(renderer, &bishopWhite2, "./bishop_white_active.bmp", CHESS_BISHOP_WHITE_BUTTON);

	widgets[8] = createButton(renderer, &bishopBlack,  "./bishop_black_active.bmp", CHESS_BISHOP_BLACK_BUTTON);
	widgets[9] = createButton(renderer, &bishopBlack2, "./bishop_black_active.bmp", CHESS_BISHOP_BLACK_BUTTON);
	
	widgets[10] = createButton(renderer, &rookWhite,  "./rook_white_active.bmp",  CHESS_ROOK_WHITE_BUTTON);
	widgets[11] = createButton(renderer, &rookWhite2, "./rook_white_active.bmp", CHESS_ROOK_WHITE_BUTTON);

	widgets[12] = createButton(renderer, &rookBlack,  "./rook_black_active.bmp",  CHESS_ROOK_BLACK_BUTTON);
	widgets[13] = createButton(renderer, &rookBlack2, "./rook_black_active.bmp", CHESS_ROOK_BLACK_BUTTON);

	widgets[14] = createButton(renderer, &knightWhite,  "./knight_white_active.bmp",  CHESS_KNIGHT_WHITE_BUTTON);
	widgets[15] = createButton(renderer, &knightWhite2, "./knight_white_active.bmp", CHESS_KNIGHT_WHITE_BUTTON);

	widgets[16] = createButton(renderer, &knightBlack,  "./knight_black_active.bmp",  CHESS_KNIGHT_BLACK_BUTTON);
	widgets[17] = createButton(renderer, &knightBlack2, "./knight_black_active.bmp", CHESS_KNIGHT_BLACK_BUTTON);

	widgets[18] = createButton(renderer, &queenWhite, "./queen_white_active.bmp", CHESS_QUEEN_WHITE_BUTTON);
	widgets[19] = createButton(renderer, &queenBlack, "./queen_black_active.bmp", CHESS_QUEEN_BLACK_BUTTON);
	widgets[20] = createButton(renderer, &kingWhite,  "./king_white_active.bmp", CHESS_KING_WHITE_BUTTON);
	widgets[21] = createButton(renderer, &kingBlack,  "./king_black_active.bmp", CHESS_KING_BLACK_BUTTON);

	int place = 22;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		SDL_Rect pawnWhite =  {.x = window->game->gameGUIBoard[1][i].x,.y = window->game->gameGUIBoard[1][i].y,
							   .h = window->game->gameGUIBoard[1][i].h,.w = window->game->gameGUIBoard[1][i].w };
		SDL_Rect pawnBlack =  {.x = window->game->gameGUIBoard[6][i].x,.y = window->game->gameGUIBoard[6][i].y,
							   .h = window->game->gameGUIBoard[6][i].h,.w = window->game->gameGUIBoard[6][i].w };
		widgets[place]   = createButton(renderer, &pawnWhite, "./pawn_white_active.bmp", CHESS_PAWN_WHITE_BUTTON);
		if (!widgets[place])
			break;
		widgets[place]->row = 1;
		widgets[place]->coll = i;
		
		widgets[place+1] = createButton(renderer, &pawnBlack, "./pawn_black_active.bmp", CHESS_PAWN_BLACK_BUTTON);
		if (!widgets[place+1])
			break;
		widgets[place+1]->row = 6;
		widgets[place+1]->coll = i;
		
		place+=2;
	}
	
	for (int i = 0; i < 38; i++)
	{
		if (!widgets[i])
		{
			printf("ERROR SDL: unable to create the window's buttons\n");
			for (int j = 0; j < 38; j++)
			{
				if(widgets[j])
					destroyWidget(widgets[j]);
			}
			free(widgets);
			return NULL;
		}
	}
	
	widgets[6]->row = 0;
	widgets[6]->coll = 2;
	widgets[7]->row = 0;
	widgets[7]->coll = 5;

	widgets[8]->row = 7;
	widgets[8]->coll = 2;
	widgets[9]->row = 7;
	widgets[9]->coll = 5;

	widgets[10]->row = 0;
	widgets[10]->coll = 0;
	widgets[11]->row = 0;
	widgets[11]->coll = 7;

	widgets[12]->coll = 0;
	widgets[12]->row = 7;
	widgets[13]->row = 7;
	widgets[13]->coll = 7;

	widgets[14]->row = 0;
	widgets[14]->coll = 1;
	widgets[15]->row = 0;
	widgets[15]->coll = 6;

	widgets[16]->row = 7;
	widgets[16]->coll = 1;
	widgets[17]->row = 7;

	widgets[17]->coll = 6;
	widgets[18]->row = 0;
	widgets[18]->coll = 3;
	widgets[19]->row = 7;
	widgets[19]->coll = 3;
	widgets[20]->row = 0;
	widgets[20]->coll = 4;
	widgets[21]->row = 7;
	widgets[21]->coll = 4;


	for (int i = 0; i < 38; i++)
	{
		if (i < 6)
		{
			widgets[i]->isDragLegal = false;
			widgets[i]->isVisible = true;
		}
		else
		{
			widgets[i]->isDragLegal = true;
			widgets[i]->isVisible = false; //pieces are unvisible, untill update acording to the game board (Will be update in "draw window" function)
		}
		widgets[i]->isActivateLegal = true; //Will be update in "draw window" function 
		widgets[i]->isMoving = false;
		widgets[i]->endOfDrag = false; 
		widgets[i]->isActive = false;

		//////white pieces///////
		if ( (i == 6) || (i == 7) || (i == 10) || (i == 11) || (i == 14) || (i == 15) || (i == 18) ||
			(i == 20) || (i == 22) || (i == 24) || (i == 26) || (i == 28) || (i == 30) || (i == 32) || (i == 34) || (i == 36))
			widgets[i]->color = 'w';
		else if (i > 6)
		{
			//////Black pieces///////
			widgets[i]->color = 'b';
		}
	}

	return widgets;
}


ChessWindow* createGameWindow(Uint32 winMode, chessGame* game)
{
	if (!game)
		return NULL;

	ChessWindow* res = malloc(sizeof(ChessWindow));
	if (!res)
		return NULL;
	chessGameWindow* data = malloc(sizeof(chessGameWindow));
	if (!data)
	{
		free(res);
		return NULL;
	}
	SDL_Window* window = SDL_CreateWindow("CHESS!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game_width, game_height, winMode);
	if (!window)
	{
		printf("ERROR: unable to create a window: %s\n", SDL_GetError());
		free(res);
		free(data);
		return NULL;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		printf("ERROR: unable to create a renderer: %s\n", SDL_GetError());
		free(res);
		free(data);
		SDL_DestroyWindow(window);	  //NULL safe
		return NULL;
	}
	res->game = game;
	Widget** widgets = createGameWindowWidgets(renderer, res);
	if (!widgets)
	{
		printf("ERROR: unable to create window's buttons: %s\n", SDL_GetError());
		free(res);
		free(data);
		SDL_DestroyRenderer(renderer); //NULL safe
		SDL_DestroyWindow(window);	  //NULL safe
		return NULL;
	}
	data->widgets = widgets;
	data->numOfWidgets = 38;
	data->window = window;
	data->windowRenderer = renderer;

	res->data = (void*)data;
	res->destroyWindow = destroyGameWindow;
	res->drawWindow = drawGameWindow;
	res->handleEventWindow = handleEventGameWindow;
	res->type = CHESS_GAME_WINDOW;

	return res;
}


void destroyGameWindow(ChessWindow* src) 
{
	if (!src)
		return;

	chessGameWindow* data = (chessGameWindow*)src->data;
	for (int i=0; i < data->numOfWidgets; i++)
	{
		destroyWidget(data->widgets[i]);
	}
	free(data->widgets);
	SDL_DestroyRenderer(data->windowRenderer);
	SDL_DestroyWindow(data->window);
	free(data);
	free(src);
}


void drawGameWindow(ChessWindow* src) 
{
	if (!src) 
		return;
	
	chessGameWindow* data = (chessGameWindow*)src->data;
	SDL_RenderClear(data->windowRenderer);

	//draw background
	SDL_Surface* surf = SDL_LoadBMP("./game_background.bmp");
	if (!surf)
	{
		printf("ERROR: unable to load BMP file: %s\n", SDL_GetError());
		return;
	}
	SDL_Texture * background = SDL_CreateTextureFromSurface(data->windowRenderer, surf);
	if (!background)
	{
		SDL_FreeSurface(surf);
		printf("ERROR: unable to create a background: %s\n", SDL_GetError());
		return;
	}
	
	SDL_FreeSurface(surf);
	if (SDL_SetTextureBlendMode(background, SDL_BLENDMODE_NONE) != 0)
	{
		printf("ERROR: unable to blend background texture: %s\n", SDL_GetError());
	}
	if (SDL_RenderCopy(data->windowRenderer, background, NULL, NULL) != 0)
	{
		printf("ERROR: unable to render texture: %s\n", SDL_GetError());
	}

	SDL_DestroyTexture(background);
	
	//send to drawgame in utils - draw the board 
	drawGameBoard(data, src->game);
	
	if (steadyBoard)
		setBoardPieces(src, data);

	//Draw widgets
	for (int i = 0; i < data->numOfWidgets; i++)
	{	
		if (data->widgets[i]->isVisible)
			data->widgets[i]->drawWidget(data->widgets[i]);
		
		if (i == 3) //undo button
		{
			if ((spArrayListIsEmpty(src->game->historyArray)) || (src->game->gameMode == TWO_PLAYERS))
			{
				data->widgets[i]->isActivateLegal = false; 
				updateButtonTexture(data->widgets[i], "./undo_nonactive.bmp");
			}
		}

		//Update according to current turn 
		if ((src->game->currentPlayer == WHITES) && (data->widgets[i]->color == 'b') ||
			(src->game->currentPlayer == BLACKS) && (data->widgets[i]->color == 'w'))
		{
			data->widgets[i]->isActivateLegal = false;
		}
		if ((src->game->currentPlayer == WHITES) && (data->widgets[i]->color == 'w') ||
			(src->game->currentPlayer == BLACKS) && (data->widgets[i]->color == 'b'))
		{
			data->widgets[i]->isActivateLegal = true;
		}
	}

	SDL_RenderPresent(data->windowRenderer);
}

WINDOW_EVENT handleEventGameWindow(ChessWindow* src, SDL_Event* event)
{
	if ((!src) || (!event)) 
		return CHESS_EMPTY_WINDOWEVENT;
	
	chessGameWindow* windata = (chessGameWindow*)src->data;
	int endStatus;
	WINDOW_EVENT savingChoose;

	if (src->game->gameMode == ONE_PLAYER && src->game->humanPlayerColor == BLACKS && src->game->currentPlayer == WHITES)
	{
		compMove(src->game);
		steadyBoard = true;
		drawGameWindow(src);
	}
	
	while (1) 
	{
	begin:
		while (SDL_PollEvent(event))
		{
			if (!(event->type == SDL_MOUSEBUTTONDOWN  && event->button.button == SDL_BUTTON_LEFT)&&
				!(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)&&
				!((event->type == SDL_MOUSEMOTION &&
					SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))))
					goto begin;

			for (int i = 0; i < windata->numOfWidgets; i++) 
			{
				windata->widgets[i]->handleEvent(windata->widgets[i], event);
				SDL_RenderPresent(windata->windowRenderer);

				if (windata->widgets[i]->isActive && windata->widgets[i]->isActivateLegal)
				{
					switch (windata->widgets[i]->widget_type)
					{
					case CHESS_EMPTY_BUTTON:
						return CHESS_EMPTY_WINDOWEVENT;
					
					case CHESS_RESTART_BUTTON: 
						windata->widgets[i]->isActive = false;
						return CHESS_RESTART_WINDOWEVENT;
					
					case CHESS_SAVE_BUTTON:
						windata->widgets[i]->isActive = false;
						saveGameInLastestSlot(src->game);
						updateButtonTexture(windata->widgets[i], "./save_active.bmp");
						drawGameWindow(src);
						goto begin;

					case CHESS_LOAD_BUTTON:
						windata->widgets[i]->isActive = false;
						return CHESS_LOAD_SCREEN_WINDOWEVENT;
					
					case CHESS_UNDO_BUTTON:
						undoGui(src->game);
						undoGui(src->game);
						updateButtonTexture(windata->widgets[i], "./undo_active.bmp");
						steadyBoard = true;
						drawGameWindow(src);
						windata->widgets[i]->isActive = false;
						goto begin;
				
					case CHESS_HOME_BUTTON:
						savingChoose = showSavingMessage(windata->widgets[i]->widget_type);
						if (savingChoose == CHESS_CANCEL_WINDOWEVENT)
						{
							updateButtonTexture(windata->widgets[4], "./home_active.bmp");
							windata->widgets[i]->isActive = false;
							steadyBoard = false;
							drawGameWindow(src);
							steadyBoard = true;
							goto begin;
						}
					    if (savingChoose == CHESS_SAVE_WINDOWEVENT)	//"yes" pressed
							saveGameInLastestSlot(src->game);
						
						windata->widgets[i]->isActive = false;
						return CHESS_HOME_WINDOWEVENT;
					
					case CHESS_QUIT_BUTTON:
						savingChoose = showSavingMessage(windata->widgets[i]->widget_type);
						if (savingChoose == CHESS_CANCEL_WINDOWEVENT)
						{
							updateButtonTexture(windata->widgets[5], "./exit_active.bmp");
							windata->widgets[i]->isActive = false;
							steadyBoard = false;
							drawGameWindow(src);
							steadyBoard = true;
							goto begin;
						}
						
						if (savingChoose == CHESS_SAVE_WINDOWEVENT)	//"yes" pressed
							saveGameInLastestSlot(src->game);

						windata->widgets[i]->isActive = false;
						return CHESS_QUIT_WINDOWEVENT;
					
					case CHESS_PAWN_BLACK_BUTTON:
					case CHESS_PAWN_WHITE_BUTTON:
					case CHESS_BISHOP_BLACK_BUTTON:
					case CHESS_BISHOP_WHITE_BUTTON:
					case CHESS_KNIGHT_BLACK_BUTTON:
					case CHESS_KNIGHT_WHITE_BUTTON:
					case CHESS_ROOK_BLACK_BUTTON:
					case CHESS_ROOK_WHITE_BUTTON:
					case CHESS_QUEEN_BLACK_BUTTON:
					case CHESS_QUEEN_WHITE_BUTTON:
					case CHESS_KING_WHITE_BUTTON:
					case CHESS_KING_BLACK_BUTTON:
						/////bonus - start writing 
						/*SDL_SetRenderDrawColor(windata->windowRenderer, 100,10, 150, 255);
						if (SDL_RenderFillRect(windata->windowRenderer, &src->game->gameGUIBoard[4][5]) < 0)
							printf("ERROR: unable to fill rect: %s\n", SDL_GetError());
						SDL_RenderPresent(windata->windowRenderer);*/
						if (windata->widgets[i]->endOfDrag)
						{
							steadyBoard = true;

							if (GUIMove(src, windata->widgets[i], event, windata) == false)
							{
								windata->widgets[i]->endOfDrag = false;
								windata->widgets[i]->isActive = false;
								drawGameWindow(src);
								goto begin;
							}
							
							windata->widgets[i]->endOfDrag = false;
							windata->widgets[i]->isActive = false;

							drawGameWindow(src);
							SDL_Delay(50);
							endStatus = checkGuiGameEnd(src);
							if ((endStatus == STALEMATE) || (endStatus == CHECKMATE))
								return CHESS_QUIT_WINDOWEVENT;

							//if we are here - the move was valid 
							if (src->game->gameMode == ONE_PLAYER)
							{
								if (windata->widgets[3]->isActivateLegal == false)
								{
									windata->widgets[3]->isActivateLegal = true;
									updateButtonTexture(windata->widgets[3], "./undo_active.bmp");
								}
								compMove(src->game);
								drawGameWindow(src);
								//SDL_Delay(50);
								endStatus = checkGuiGameEnd(src);
								if ((endStatus == STALEMATE) || (endStatus == CHECKMATE))
									return CHESS_QUIT_WINDOWEVENT;
								goto begin;
							}
							else if (src->game->gameMode == TWO_PLAYERS)
							{
								goto begin;
							} //turn was switched in the setMove function from Chess_gameUtils.c 
						}
						else
						{
							steadyBoard = false;
							drawGameWindow(src);
						}

						goto begin;

					default:
						return CHESS_EMPTY_WINDOWEVENT;
						break;
					}
				}
			}
		}
}
	return CHESS_EMPTY_WINDOWEVENT;
}


