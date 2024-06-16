#include "chip8.hpp"
#include <SDL2/SDL.h>
#include <iostream>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 240

int main(void) {
	
	SDL_Window * window = nullptr;
	SDL_Surface * surface = nullptr;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL init failed: " << SDL_GetError() << std::endl;
	} else {
		window = SDL_CreateWindow("chip-emu",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				SDL_WINDOW_SHOWN);
	
		if (window == nullptr) {
			std::cout << "Window creation failed:" << SDL_GetError() << std::endl;	
		} else {
			surface = SDL_GetWindowSurface(window);
			SDL_FillRect(surface,
				NULL,
				SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF)
			);
	
			SDL_UpdateWindowSurface(window);

			SDL_Event e;
			bool running = true;
			while (running) {
				while (SDL_PollEvent(&e)) {
					if (e.type==SDL_QUIT)
						running = false;
				}
			}
		}

	}
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
