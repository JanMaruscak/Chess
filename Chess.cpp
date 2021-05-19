#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Point.h"
#include <list>

void destroyResource();

typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
} App;
enum Piece
{
	NONE, PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING
};

Piece Index[8][8];
App app;

SDL_Rect rect;
int Board[8][8];
void initSDL(void)
{
	int rendererFlags, windowFlags;

	rendererFlags = SDL_RENDERER_ACCELERATED;

	windowFlags = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	app.window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, windowFlags);

	if (!app.window)
	{
		printf("Failed to open %d x %d window: %s\n", 800, 600, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

	if (!app.renderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	rect.x = 10;
	rect.y = 10;
	rect.w = 50;
	rect.h = 50;

	for (size_t x = 0; x < 8; x++)
	{
		for (size_t y = 0; y < 8; y++)
		{
			Board[x][y] = -1;
		}
	}

	Board[7][6] = 0;
	Board[6][6] = 0;
	Board[5][6] = 0;
	Board[4][6] = 0;
	Board[3][6] = 0;
	Board[2][6] = 0;
	Board[1][6] = 0;
	Board[0][6] = 0;

	Board[7][7] = 3;
	Board[6][7] = 1;
	Board[5][7] = 2;
	Board[4][7] = 5;
	Board[3][7] = 4;
	Board[2][7] = 2;
	Board[1][7] = 1;
	Board[0][7] = 3;

	Board[7][1] = 6;
	Board[6][1] = 6;
	Board[5][1] = 6;
	Board[4][1] = 6;
	Board[3][1] = 6;
	Board[2][1] = 6;
	Board[1][1] = 6;
	Board[0][1] = 6;

	Board[7][0] = 3 + 6;
	Board[6][0] = 1 + 6;
	Board[5][0] = 2 + 6;
	Board[4][0] = 5 + 6;
	Board[3][0] = 4 + 6;
	Board[2][0] = 2 + 6;
	Board[1][0] = 1 + 6;
	Board[0][0] = 3 + 6;
}

bool light = true;
int LastRow = NULL; int LastColumn = NULL;

bool WhiteMove = true;
bool FirstClick = true;
std::list<Point> ValidMoves = {};

bool IsPieceBeetweenXBishop(int row, int column) {
	//int xInc = (row - LastRow) / abs(row - LastRow);
	//int yInc = (column - LastColumn) / abs(column - LastColumn);
	//for (size_t x = LastRow + xInc; x != row; x += xInc)
	//{
	//	for (size_t y = LastColumn + yInc; y != column; y += yInc)
	//	{
	//		if (Board[x][y] != -1) return true;
	//	}
	//}
	//return false;
	int xInc = (row - LastRow) / (abs(row - LastRow));
	int yInc = (column - LastColumn) / (abs(column - LastColumn));

	for (int i = 1; i < abs(LastRow - row); i++)
	{
		if (Board[LastRow + xInc * i][LastColumn + yInc * i] != -1)
			return true;
	}
	return false;
}
bool IsPieceBeetweenX(int row, int column) {
	int xInc = (row - LastRow) / abs(row - LastRow);
	for (size_t x = LastRow + xInc; x != row; x += xInc)
	{
		if (Board[x][LastColumn] != -1) return true;
	}
	return false;
}
bool IsPieceBeetweenY(int row, int column) {
	int yInc = (column - LastColumn) / abs(column - LastColumn);
	for (size_t y = LastColumn + yInc; y != column; y += yInc)
	{
		if (Board[LastRow][y] != -1) return true;
	}
	return false;
}
//Point* SetValidMoves() {
//	for (size_t x = 0; x < 8; x++)
//	{
//		light = !light;
//		for (size_t y = 0; y < 8; y++)
//		{
//			IsValidMove()
//		}
//	}
//}

bool IsValidMove(int row, int column)
{
	int oldPiece = Board[LastRow][LastColumn];
	int newPiece = Board[row][column];
	bool newIsBlack = newPiece > 5;
	bool newIsWhite = newPiece > -1 && newPiece <= 5;

	if (LastRow == row && column == LastColumn) return false;

	switch (oldPiece)
	{
	case 0: // PAWN
		if (row == LastRow && LastColumn - column == 1 && !IsPieceBeetweenY(row, column)) {
		}
		else {
			return false;
		}
		break;
	case 6:
		if (row == LastRow && column - LastColumn == 1 && !IsPieceBeetweenY(row, column)) {
		}
		else {
			return false;
		}
		break;
	case 1: // KNIGHT
	case 7:
		if ((abs(row - LastRow) == 2 && abs(column - LastColumn) == 1) || (abs(row - LastRow) == 1 && abs(column - LastColumn) == 2)) {
		}
		else {
			return false;
		}
		break;
	case 2: // BISHOP
	case 8:
		if (abs(column - LastColumn) == abs(row - LastRow) && !IsPieceBeetweenXBishop(row, column)) {
		}
		else {
			return false;
		}
		break;
	case 3: // ROOK
	case 9:
		if (column == LastColumn && !IsPieceBeetweenX(row, column)) {
		}
		else if (row == LastRow && !IsPieceBeetweenY(row, column)) {
		}
		else {
			return false;
		}
		break;
	case 4: // QUEEN
	case 10:
		if (column == LastColumn && !IsPieceBeetweenX(row, column)) {
		}
		else if (row == LastRow && !IsPieceBeetweenY(row, column)) {
		}
		else if (abs(column - LastColumn) == abs(row - LastRow) && !IsPieceBeetweenXBishop(row, column)) {
		}
		else {
			return false;
		}
		break;
		break;
	case 5: // KING
	case 11:
		if (abs(column - LastColumn) <= 1 && abs(row - LastRow) <= 1) {
		}
		else {
			return false;
		}
		break;

	default:
		break;
	}
	return (!FirstClick && (!(newIsBlack && !WhiteMove) && !(newIsWhite && WhiteMove)));
}
bool IsValidChoice(int row, int column)
{
	int newPiece = Board[row][column];
	return (FirstClick && newPiece != -1 && ((newPiece > 5 && !WhiteMove) || (newPiece > -1 && newPiece <= 5 && WhiteMove)));
}
//bool ValidPieceMove(int row, int column) {
//	int oldPiece = Board[LastRow][LastColumn];
//	int newPiece = Board[row][column];
//
//	switch (oldPiece)
//	{
//	case 0:
//		if(Row - LastRow == 1 && column)
//		break;
//	default:
//		break;
//	}
//	if (oldPiece == 0 || oldPiece == 5) {
//
//	}
//}
void doInput(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		int x = 0, y = 0, column = 0, row = 0;

		switch (event.type)
		{
		case SDL_QUIT:
			exit(0);
			break;
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&x, &y);
			row = x / 50;
			column = y / 50;

			if (IsValidChoice(row, column) && FirstClick) {
				LastRow = row;
				LastColumn = column;
				FirstClick = false;
			}
			else if (!IsValidMove(row, column)) {
				LastRow = NULL;
				LastColumn = NULL;
				FirstClick = true;
			}
			else if (!FirstClick) {
				int newNum = Board[LastRow][LastColumn];

				Board[row][column] = newNum;
				Board[LastRow][LastColumn] = -1;
				LastRow = NULL;
				LastColumn = NULL;
				WhiteMove = !WhiteMove;
				FirstClick = true;
				printf("Clicked on X: %d, Y: %d\n", row + 1, column + 1);
				//printf("Clicked on X: %d, Y: %d\n", row + 1, column + 1);
			}

			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.scancode)
			{
			case 79: // key_right
				rect.x += 1;
				break;
			default:
				printf("%d\n", event.key.keysym.scancode);
				break;
			}
		default:
			break;
		}
	}
}
bool myExitFlag = false;
void cleanupSDL() {
	if (myExitFlag == false) {
		myExitFlag = true;
		destroyResource();
		SDL_DestroyWindow(app.window);
		SDL_DestroyRenderer(app.renderer);
		SDL_Quit();
	}
}

SDL_Surface* piece_B_img[6];
SDL_Texture* piece_B_txt[6];

SDL_Surface* piece_W_img[6];
SDL_Texture* piece_W_txt[6];

std::string pieceNames[6] = { "pawn", "knight", "bishop", "rook", "queen", "king" };

void loadResource() {
	//for (size_t i = 0; i < std::size(pieceNames); i++)
	//{
	//	std::string name = pieceNames[i];
	//	std::string location = "C:\\dev\\Chess\\Chess\\Pieces\\256px\\b_%s_png_shadow_256px.png", name;
	//	piece_B_img[i] = IMG_Load(location.c_str());
	//}
	//for (size_t i = 0; i < std::size(pieceNames); i++)
	//{
	//	std::string name = pieceNames[i];
	//	std::string location = "C:\\dev\\Chess\\Chess\\Pieces\\256px\\w_%s_png_shadow_256px.png", name;
	//	piece_W_img[i] = IMG_Load(location.c_str());
	//}

	piece_B_img[0] = IMG_Load("C:\\dev\\Chess\\Chess\\Pieces\\256px\\b_pawn_png_shadow_256px.png");
	piece_B_img[1] = IMG_Load("C:\\dev\\Chess\\Chess\\Pieces\\256px\\b_knight_png_shadow_256px.png");
	piece_B_img[2] = IMG_Load("C:\\dev\\Chess\\Chess\\Pieces\\256px\\b_bishop_png_shadow_256px.png");
	piece_B_img[3] = IMG_Load("C:\\dev\\Chess\\Chess\\Pieces\\256px\\b_rook_png_shadow_256px.png");
	piece_B_img[4] = IMG_Load("C:\\dev\\Chess\\Chess\\Pieces\\256px\\b_queen_png_shadow_256px.png");
	piece_B_img[5] = IMG_Load("C:\\dev\\Chess\\Chess\\Pieces\\256px\\b_king_png_shadow_256px.png");

	piece_W_img[0] = IMG_Load("C:\\dev\\Chess\\Chess\\Pieces\\256px\\w_pawn_png_shadow_256px.png");
	piece_W_img[1] = IMG_Load("C:\\dev\\Chess\\Chess\\Pieces\\256px\\w_knight_png_shadow_256px.png");
	piece_W_img[2] = IMG_Load("C:\\dev\\Chess\\Chess\\Pieces\\256px\\w_bishop_png_shadow_256px.png");
	piece_W_img[3] = IMG_Load("C:\\dev\\Chess\\Chess\\Pieces\\256px\\w_rook_png_shadow_256px.png");
	piece_W_img[4] = IMG_Load("C:\\dev\\Chess\\Chess\\Pieces\\256px\\w_queen_png_shadow_256px.png");
	piece_W_img[5] = IMG_Load("C:\\dev\\Chess\\Chess\\Pieces\\256px\\w_king_png_shadow_256px.png");

	for (size_t i = 0; i < 6; i++)
	{
		piece_W_txt[i] = SDL_CreateTextureFromSurface(app.renderer, piece_W_img[i]);
	}
	for (size_t i = 0; i < 6; i++)
	{
		piece_B_txt[i] = SDL_CreateTextureFromSurface(app.renderer, piece_B_img[i]);
	}
}
void destroyResource() {
	for (size_t i = 0; i < 6; i++)
	{
		SDL_DestroyTexture(piece_W_txt[i]);
	}
	for (size_t i = 0; i < 6; i++)
	{
		SDL_DestroyTexture(piece_B_txt[i]);
	}
	for (size_t i = 0; i < 6; i++)
	{
		SDL_FreeSurface(piece_W_img[i]);
	}
	for (size_t i = 0; i < 6; i++)
	{
		SDL_FreeSurface(piece_B_img[i]);
	}
}

void prepareScene()
{
	SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
	SDL_RenderClear(app.renderer);

	for (size_t x = 0; x < 8; x++)
	{
		light = !light;
		for (size_t y = 0; y < 8; y++)
		{
			rect.x = x * rect.w;
			rect.y = y * rect.h;
			if (light) {
				if (!FirstClick && IsValidMove(x, y)) {
					SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
				}
				else {
					SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
				}
			}
			else {
				if (!FirstClick && IsValidMove(x, y)) {
					SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
				}
				else {
					SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
				}
			}

			SDL_RenderFillRect(app.renderer, &rect);

			if (Board[x][y] != -1) {
				if (Board[x][y] > 5) {
					SDL_RenderCopy(app.renderer, piece_B_txt[Board[x][y] - 6], NULL, &rect);
				}
				else {
					SDL_RenderCopy(app.renderer, piece_W_txt[Board[x][y]], NULL, &rect);
				}
			}
			light = !light;
		}
	}
}

void presentScene() {
	SDL_RenderPresent(app.renderer);
}

int main(int argc, char* argv[])
{
	initSDL();
	loadResource();
	atexit(cleanupSDL);
	while (true)
	{
		prepareScene(); //clear atd
		doInput(); // process input event
		presentScene(); // show
		SDL_Delay(10);
	}
	return 0;
}