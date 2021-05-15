#include <iostream>
#include <SDL.h>
//#include <SDL_image.h>

void destroyResource();

typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
} App;

App app;

SDL_Rect rect;
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
}

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
			printf("Clicked on - X: %d\n, Y: %d\n", row + 1, column + 1);
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
SDL_Surface* img;
SDL_Texture* img_txt;
void loadResource() {
	//img = IMG_Load("C:\\Users\\NTB-XX\\source\\repos\\SDL2_Start\\Debug\\kocicka.bmp");
	//img = SDL_LoadBMP("C:\\Users\\NTB-XX\\source\\repos\\SDL2_Start\\Debug\\kocicka.bmp");
	img_txt = SDL_CreateTextureFromSurface(app.renderer, img);
}
void destroyResource() {
	SDL_FreeSurface(img);
	SDL_DestroyTexture(img_txt);
}

void prepareScene()
{
	SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
	SDL_RenderClear(app.renderer);

	bool light = true;
	for (size_t x = 0; x < 8; x++)
	{
		light = !light;
		for (size_t y = 0; y < 8; y++)
		{
			rect.x = x * rect.w;
			rect.y = y * rect.h;
			if (light) {
				SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
			}
			else {
				SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
			}

			SDL_RenderFillRect(app.renderer, &rect);

			SDL_RenderCopy(app.renderer, img_txt, NULL, &rect);
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