#include <stdio.h>
#include <stdlib.h>
#include "Chess_gameUtils.h"
#include "Chess_gameUtilsAux.h"
#include "SPMiniMax.h"
#include "Chess_FlowTwoPlayers.h"
#include <stdbool.h>

#include "unitTest.h"
#include "Chess_gameParser.h"
#include "Chess_gameSettingsParser.h"
#include "Chess_ArrayList.h"

/////////////////////////////Testers declerations////////////////////////////////////

/////ALEX/////
//void TestBoard(chessGame* src);

/////MOR/////
static bool spParserCheckIsInt();
static bool spParserCheckParseLine();
static bool spArrayListBasicGetTest();
static bool spArrayListBasicCopyTest();
static bool spArrayListBasicRemoveTest();
static bool spArrayListBasicAddTest();
static bool spArrayListCreateTest();
////////////////////////////////////////////////////////////////////////////////////



#define CAPACITY_SIZE 10


/*
////////////////////////////////////////////////
//////////////////check - ALEX//////////////////
////////////////////////////////////////////////

void TestBoard(chessGame* src) {
for (int i = 0; i<BOARD_SIZE; i++)
for (int j = 0; j <BOARD_SIZE; j++)
src->gameBoard[i][j] = EMPTY_BOARD_POS;


//whites
src->gameBoard[1][5] = KNIGHT_WHITE;
//    src->gameBoard[2][7] = KING_WHITE;
//    src->gameBoard[0][1] = KNIGHT_WHITE;
//    src->gameBoard[0][2] = BISHOP_WHITE;
//    src->gameBoard[0][3] = QUEEN_WHITE;
//    src->gameBoard[0][4] = KING_WHITE;
//    src->gameBoard[0][5] = BISHOP_WHITE;
//    src->gameBoard[0][6] = KNIGHT_WHITE;
//    src->gameBoard[0][7] = ROOK_WHITE;



//blacks
src->gameBoard[2][6] = KING_BLACK;
//src->gameBoard[4][6] = KING_BLACK;
//    src->gameBoard[7][1] = KNIGHT_BLACK;
src->gameBoard[2][7] = BISHOP_BLACK;
//    src->gameBoard[7][3] = QUEEN_BLACK;
//    src->gameBoard[7][4] = KING_BLACK;
//    src->gameBoard[7][5] = BISHOP_BLACK;
//    src->gameBoard[7][6] = KNIGHT_BLACK;
//    src->gameBoard[7][7] = ROOK_BLACK;
}
////////////////////////////////////////////////
//////////////////check - ALEX- end/////////////
////////////////////////////////////////////////

	*/


////////////////////////////////////////////////
//////////////////check - MOR//////////////////
////////////////////////////////////////////////

static bool spParserCheckIsInt()
{
	ASSERT_TRUE(spParserIsInt("1"));
	ASSERT_TRUE(spParserIsInt("11"));
	ASSERT_TRUE(spParserIsInt("111"));
	ASSERT_TRUE(spParserIsInt("-1"));
	ASSERT_TRUE(spParserIsInt("-11"));
	ASSERT_TRUE(spParserIsInt("-111"));
	ASSERT_TRUE(spParserIsInt("01"));
	ASSERT_TRUE(spParserIsInt("011"));
	ASSERT_TRUE(spParserIsInt("0111"));
	ASSERT_TRUE(spParserIsInt("-01"));
	ASSERT_TRUE(spParserIsInt("-011"));
	ASSERT_TRUE(spParserIsInt("-0111"));

	ASSERT_TRUE(spParserIsInt("-0"));
	ASSERT_TRUE(spParserIsInt("00000"));
	//ASSERT_TRUE(spParserIsInt("2.1"));  //this is false, just checked 
	return true;
}

static bool spParserCheckParseLine() {

	////////////////////////////Chess_gameParser test/////////////////////////////
	CHESSCommand comd;
	comd = spParserLine("undo");
	ASSERT_TRUE(comd.cmd == CHESS_UNDO_MOVE);
	comd = spParserLine("move <2,A> to <3,B>");
	ASSERT_TRUE(comd.cmd == CHESS_MOVE);
	ASSERT_TRUE(comd.isValidFirstPair && comd.isValidSecondPair);
	ASSERT_TRUE(comd.sourceRow == 2 && comd.sourceColl == 'A');
	ASSERT_TRUE(comd.targertRow == 3 && comd.targetColl == 'B');
	comd = spParserLine("move <2,A> cxdf <3,B>");
	ASSERT_TRUE(comd.cmd == CHESS_INVALID_LINE && !comd.isValidFirstPair && !comd.isValidSecondPair);
	comd = spParserLine("save");
	ASSERT_TRUE(comd.cmd == CHESS_SAVE);
	
	
	////////////////////////////Chess_gameSettingsParser test/////////////////////////////
	CHESSSettingCommand comdset;
	comdset = spParserSettingLine("    game_mode 2");
	ASSERT_TRUE(comdset.cmd == CHESS_MODE && comdset.isValidArg && comdset.arg == 2);
	comdset = spParserSettingLine("5 diff");
	ASSERT_TRUE(comdset.cmd == CHESS_INVALID_SETTING_LINE && !comdset.isValidArg);
	comdset = spParserSettingLine("	difficulty     5   \n");
	ASSERT_TRUE(comdset.cmd == CHESS_DIFFICULTY && comdset.arg == 5 && comdset.isValidArg);
	comdset = spParserSettingLine("user_color \n 1");
	ASSERT_TRUE(comdset.cmd == CHESS_COLOR && comdset.arg == 1 && comdset.isValidArg);
	comdset = spParserSettingLine("load");
	ASSERT_TRUE(comdset.cmd == CHESS_LOAD);

	return true;
	}
	

////////////////////////////Chess_ArrayList test/////////////////////////////
static bool spArrayListBasicGetTest() {
	SPArrayList* list = spArrayListCreate(CAPACITY_SIZE);
	ASSERT_TRUE(list != NULL);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(spArrayListAddLast(list, i, i,i,i ) == SP_ARRAY_LIST_SUCCESS);
	}
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(spArrayListGetAt(list, i)->current_pos_row == i);
		ASSERT_TRUE(spArrayListGetAt(list, i)->current_pos_col == i);
		ASSERT_TRUE(spArrayListGetAt(list, i)->prev_pos_row == i);
		ASSERT_TRUE(spArrayListGetAt(list, i)->prev_pos_col == i);

	}
	spArrayListDestroy(list);
	return true;
}

static bool spArrayListBasicCopyTest() {
	SPArrayList* list = spArrayListCreate(CAPACITY_SIZE);
	ASSERT_TRUE(list != NULL);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(spArrayListAddFirst(list, i,i,i,i) == SP_ARRAY_LIST_SUCCESS);
	}
	SPArrayList* copyList = spArrayListCopy(list);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(spArrayListGetAt(list, i)->current_pos_row == spArrayListGetAt(list, i)->current_pos_row);
		ASSERT_TRUE(spArrayListGetAt(list, i)->current_pos_col == spArrayListGetAt(list, i)->current_pos_col);
		ASSERT_TRUE(spArrayListGetAt(list, i)->prev_pos_row == spArrayListGetAt(list, i)->prev_pos_row);
		ASSERT_TRUE(spArrayListGetAt(list, i)->prev_pos_col == spArrayListGetAt(list, i)->prev_pos_col);
	}
	spArrayListDestroy(list);
	spArrayListDestroy(copyList);
	return true;
}

static bool spArrayListBasicRemoveTest() {
	SPArrayList* list = spArrayListCreate(CAPACITY_SIZE);
	ASSERT_TRUE(list != NULL);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(spArrayListAddFirst(list, i,i,i,i) == SP_ARRAY_LIST_SUCCESS);
	}
	ASSERT_TRUE(spArrayListSize(list) == CAPACITY_SIZE);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(spArrayListRemoveFirst(list) == SP_ARRAY_LIST_SUCCESS);
		ASSERT_TRUE(spArrayListSize(list) == CAPACITY_SIZE - i - 1);
	}
	ASSERT_TRUE(spArrayListIsEmpty(list));
	spArrayListDestroy(list);
	return true;
}

static bool spArrayListBasicAddTest() {
	SPArrayList* list = spArrayListCreate(CAPACITY_SIZE);
	ASSERT_TRUE(list != NULL);
	for (int i = 0; i < CAPACITY_SIZE; i++) {
		ASSERT_TRUE(spArrayListAddFirst(list, i,i,i,i) == SP_ARRAY_LIST_SUCCESS);
		ASSERT_TRUE(spArrayListSize(list) == i + 1);
	}
	ASSERT_TRUE(spArrayListSize(list) == CAPACITY_SIZE);
	spArrayListDestroy(list);
	return true;
}
static bool spArrayListCreateTest() {
	SPArrayList* list = spArrayListCreate(CAPACITY_SIZE);
	ASSERT_TRUE(list != NULL);
	spArrayListDestroy(list);
	return true;
}
////////////////////////////////////////////////
//////////////////check - MOR - end/////////////
////////////////////////////////////////////////


int main(int argc, const char * argv[]) {
	/*
	////////////////////////////////////////////////
	//////////////////check - ALEX//////////////////
	printf("Hello, World!\n");
	chessGame* src = createChessGame(5, TWO_PLAYERS, 3);
	TestBoard(src);
	chessConsolePrintBoard(src);
	printf("hello\n");
	printf("\n%d\n", isValidMove(src, 2, 6, 4, 6) == true);
	printf("\n%d\n", isValidMove(src, 1, 5, 2, 3) == true);
	printf("\n%d\n", isValidMove(src, 1, 5, 2, 7) == true);


	movesArray* moves = allPossibleMoves(src, 2, 6);
	int counter = 0;
	while (moves->moves[counter][0] != -1) {
		printf("MOVE: (%d,%d) -> (%d,%d) END\n", moves->moves[counter][0], moves->moves[counter][1], moves->moves[counter][2], moves->moves[counter][3]);
		counter++;
	}

	get_moves(src, 2, 6);
	//printf("\n%d\n", saveGame(src,"/Users/alexs/dev/SWChess/test2.xml"));
    */

	///////////////////////////////////////////////////
	///////////////////////////////////////////////////


	///////////////////////////////////////////////
	//////////////////check - MOR//////////////////
//	RUN_TEST(spParserCheckParseLine);
//	RUN_TEST(spParserCheckIsInt);
//
//	RUN_TEST(spArrayListCreateTest);
//	RUN_TEST(spArrayListBasicAddTest);
//	RUN_TEST(spArrayListBasicRemoveTest);
//	RUN_TEST(spArrayListBasicGetTest);
//	RUN_TEST(spArrayListBasicCopyTest);
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
    
    chessGame* src = createChessGame(5, TWO_PLAYERS, 3);
    two_players_flow(src);
	
	return 0;
}




