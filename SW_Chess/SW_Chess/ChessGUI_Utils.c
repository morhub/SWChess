#include "ChessGUI_Utils.h"
#include "ChessGUI_MainWindow.h"
#include "ChessGUI_GameWindow.h"
#include "ChessGUI_LoadWindow.h"
#include "ChessGUI_SettingsWindow.h"
#include "Button.h"

ChessWindow* createWindow(WINDOW_TYPE wintype, Uint32 winMode, chessGame* game)
{
	ChessWindow* res = NULL;
	if (wintype == CHESS_LOAD_WINDOW)
		res = createLoadWindow(winMode, game);
	else if (wintype == CHESS_MAIN_WINDOW)
		res = createMainWindow(winMode, game);
	else if (wintype == CHESS_GAME_WINDOW)
		res = createGameWindow(winMode, game);
	else if (wintype == CHESS_SETTINGS_WINDOW)
		res = createSettingsWindow(winMode, game);
//	res->prevWindow = NULL;
	return res;
}

//
//ChessWindow* copyWindow(ChessWindow* src)
//{
//	ChessWindow* res = NULL;
//
//	res = createWindow(src->type, SDL_WINDOW_HIDDEN);
//
//
//	///////how to copy the above ????? 
//	//res->data = src->data;//////////////////////////////////
//	//res->destroyWindow = src->destroyWindow;
//	//res->drawWindow = src->drawWindow;
//	//res->handleEventWindow = src->handleEventWindow;
//	//res->type = src->type;
//
//	return res;
//}




ChessWindow* swapWindows(ChessWindow* oldWindow, WINDOW_TYPE type, chessGame* game)
{
//	chessGame* newGame = copyChessGame(game);
	destroyWindow(oldWindow);
	ChessWindow* newWindow = NULL;
	newWindow = createWindow(type, SDL_WINDOW_OPENGL, game);
	//game = newGame; //update our main game pointer
	if (newWindow == NULL)
	{
		SDL_Quit();
		return NULL;
	}
	newWindow->drawWindow(newWindow);
	return newWindow;
}

void initGameGUIBoard(chessGame* game)
{
	int leftUpCornerX = 800;
	int leftUpCornerY = 520;
	int width = 60;
	int height = 60;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			SDL_Rect currentRect = { .x = -(j * width)  + leftUpCornerX
									,.y = -(i * height) + leftUpCornerY
									,.h = 60
									,.w = 60 };
			game->gameGUIBoard[i][j] = currentRect;
		}
	}
	
}

void drawGameBoard(chessGameWindow* win, chessGame* game)
{
	SDL_Surface* loadingSurface;
	SDL_Texture* squareTexture;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			
			if (SDL_RenderDrawRect(win->windowRenderer, &game->gameGUIBoard[i][j]) < 0)
				printf("ERROR: unable to draw rect: %s\n", SDL_GetError());
			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0))
			{
				loadingSurface = SDL_LoadBMP("./white_sq.bmp"); //We use the surface as a temp var;
				squareTexture =  SDL_CreateTextureFromSurface(win->windowRenderer, loadingSurface);
				//SDL_SetRenderDrawColor(win->windowRenderer, 0, 255, 0, 255);
			}
			else
			{
				loadingSurface = SDL_LoadBMP("./black_sq.bmp"); //We use the surface as a temp var;
				squareTexture =  SDL_CreateTextureFromSurface(win->windowRenderer, loadingSurface);
			}
						
			if (loadingSurface == NULL || squareTexture == NULL)
			{
				SDL_FreeSurface(loadingSurface); 
				SDL_DestroyTexture(squareTexture);
			}
			else if (SDL_RenderCopy(win->windowRenderer, squareTexture, NULL, &game->gameGUIBoard[i][j]) != 0)
			{
				printf("ERROR: unable to draw the square texture: %s\n", SDL_GetError());
				return;
			}
		}
	}
}
	

bool updateConsoleBoardIfValid(int x, int y, chessGame* game, Widget* currentWidget)
{
	SDL_Point point;
	point.x = x;
	point.y = y;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (SDL_PointInRect(&point, &(game->gameGUIBoard[i][j])))
			{
				if (setChessMove(game, currentWidget->row, currentWidget->coll, i, j, true, true))
					return true;  
				return false;
			}
		}
	}
	return false;  //not in the board 
}

void setButtonPlace(int* newX, int* newY, chessGame* game, int locX, int locY, Widget* src)
{
	SDL_Point point;
	point.x = locX;
	point.y = locY;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (SDL_PointInRect(&point, &(game->gameGUIBoard[i][j])))
			{
				*newX = game->gameGUIBoard[i][j].x;
				*newY = game->gameGUIBoard[i][j].y;
				src->row = i;
				src->coll = j;
			}
		}
	}
}
	/*
rect origin = button location;
drag&drop - mouse_motion => piece.i += event.motion.xrel , j+=y.rel (update button location)
mouse_up-> 
for(i)
for (j)
if pointinrect(button->location, src[i][j])
	button location.x = scr[i].x
	.y =.y 
else
	button location = origin 
}*/

void widgetUpdates(Widget* src, int x, int y, int row, int coll)
{
	src->isVisible = true;
	updateButtonLocation(src, x, y);
	src->row = row;
	src->coll = coll;
}

void setBoardPieces(ChessWindow* src, chessGameWindow* data)
{
	int kStart, kEnd, kInc;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			switch (src->game->gameBoard[i][j])
			{
			case PAWN_WHITE:
				kStart = PAWN_WHITE1;
				kEnd   = PAWN_BLACK8-1;
				kInc   = 2;
				break;
			case PAWN_BLACK:
				kStart = PAWN_BLACK1;
				kEnd = PAWN_BLACK8 - 1;
				kInc = 2;
				break;
			case BISHOP_WHITE:
				kStart = BISHOP_WHITE1;
				kEnd = BISHOP_WHITE2;
				kInc = 1;
				break;
			case BISHOP_BLACK:
				kStart = BISHOP_BLACK1;
				kEnd = BISHOP_BLACK2;
				kInc = 1;
				break;
			case ROOK_WHITE:
				kStart = ROOK_WHITE1;
				kEnd =   ROOK_WHITE2;
				kInc = 1;
				break;
			case ROOK_BLACK:
				kStart = ROOK_BLACK1;
				kEnd =   ROOK_BLACK2;
				kInc = 1;
				break;
			case KNIGHT_WHITE:
				kStart = KNIGHT_WHITE1;
				kEnd =   KNIGHT_WHITE2;
				kInc = 1;
				break;
			case KNIGHT_BLACK:
				kStart = KNIGHT_BLACK1;
				kEnd = KNIGHT_BLACK2;
				kInc = 1;
				break;
			case QUEEN_WHITE:
				kStart = QUEEN_WHITE1;
				kEnd = QUEEN_WHITE1;
				kInc = 1;
				break;
			case QUEEN_BLACK:
				kStart = QUEEN_BLACK1;
				kEnd = QUEEN_BLACK1;
				kInc = 1;
				break;
			case KING_WHITE:
				kStart = KING_WHITE1;
				kEnd = KING_WHITE1;
				kInc = 1;
				break;
			case KING_BLACK:
				kStart = KING_BLACK1;
				kEnd = KING_BLACK1;
				kInc = 1;
				break;
			case EMPTY_BOARD_POS:
			default:
				break;
			}
			
			for (int k = kStart; k <= kEnd; k = k + kInc)
			{
				if (!data->widgets[k]->isVisible)
				{
					widgetUpdates(data->widgets[k],
						src->game->gameGUIBoard[i][j].x, src->game->gameGUIBoard[i][j].y,
						i, j);
					break;
				}
			}
		}
	}
}

void GUIMove(ChessWindow* src, Widget* currentwidget, SDL_Event* event)
{
	int newX, newY;
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	
	//check valid move + update location - origin or event 
	if (updateConsoleBoardIfValid(mouseX, mouseY, src->game, currentwidget))
	{
		setButtonPlace(&newX, &newY, src->game, event->button.x, event->button.y, currentwidget);
		updateButtonLocation(currentwidget, newX, newY);
	}
	else
	{
		updateButtonLocation(currentwidget
			, src->game->gameGUIBoard[currentwidget->row][currentwidget->coll].x
			, src->game->gameGUIBoard[currentwidget->row][currentwidget->coll].y);
		return;
	}
		
	//if we are here - the move was valid 
	if (src->game->gameMode == ONE_PLAYER)
	{
		//compMove - alex functions
		//update widgets 
		//
	}
	else if (src->game->gameMode == TWO_PLAYERS)
	{
		return; //turn was switched in the setMove function from Chess_gameUtils.c 
	}
	//if 1p - comp move - another function :
	//do the console part , update the screen (how? with the function I wrote - widgetsUpdate)

}
