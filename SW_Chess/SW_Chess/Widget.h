#ifndef WIDGET_H_
#define WIDGET_H_
#include <SDL.h>
#include <SDL_video.h>
#include <stdbool.h>


typedef enum {
	CHESS_EMPTY_BUTTON,
	CHESS_NEWGAME_BUTTON,		//goes to setting stage
	CHESS_LOAD_BUTTON,
	CHESS_LOADER_INSIDE_BUTTON,
	CHESS_QUIT_BUTTON,
	CHESS_BACK_BUTTON,
	CHESS_NEXT_BUTTON,
	CHESS_START_BUTTON,
	CHESS_RESTART_BUTTON,
	CHESS_HOME_BUTTON,
	CHESS_SAVE_BUTTON,
	CHESS_ONEPLAYER_BUTTON,
	CHESS_TWOPLAYERS_BUTTON,
	CHESS_NOOB_BUTTON,
	CHESS_EASY_BUTTON,
	CHESS_MODERATE_BUTTON,
	CHESS_HARD_BUTTON,
	CHESS_WHITE_BUTTON,
	CHESS_BLACK_BUTTON,
	CHESS_SLOT1_BUTTON,
	CHESS_SLOT2_BUTTON,
	CHESS_SLOT3_BUTTON,
	CHESS_SLOT4_BUTTON,
	CHESS_SLOT5_BUTTON,
	CHESS_UNDO_BUTTON,
	CHESS_PAWN_BLACK,
	CHESS_PAWN_WHITE,
	CHESS_BISHOP_BLACK,
	CHESS_BISHOP_WHITE,
	CHESS_KNIGHT_BLACK,
	CHESS_KNIGHT_WHITE,
	CHESS_ROOK_BLACK,
	CHESS_ROOK_WHITE,
	CHESS_QUEEN_BLACK,
	CHESS_QUEEN_WHITE,
	CHESS_KING_WHITE,
	CHESS_KING_BLACK,
} WIDGET_TYPE;


typedef struct widget_t Widget;
struct widget_t {
	void (*drawWidget)(Widget*);
	void (*handleEvent)(Widget*, SDL_Event*);
//	void (*handleEvent)(Widget*, SDL_Event*);
	void (*destroyWidget)(Widget*);
	void* data;
	WIDGET_TYPE widget_type; 
	bool isActivateLegal; //always true, except load button in some cases and back button on the first turn of the game 
	bool isActive;		 // is pressed or draged at the moment 
	bool isVisible;
};

//This function would be usefull for NULL safe desetroy
void destroyWidget(Widget* src);
#endif