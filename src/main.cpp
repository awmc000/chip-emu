#include "chip8.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320

using std::ios_base;

byte * loadFileBuf(const std::string &filename) {
	byte * fileBuf = new byte[CHIP8_ROM_BYTES]; 
	std::ifstream in(filename, ios_base::in | ios_base::binary);

	do {
		in.read((char *) fileBuf, CHIP8_ROM_BYTES);
	} while (in.gcount() > 0);

	return fileBuf;
}

void emulate(SDL_Window * window, SDL_Surface * surface) {
	SDL_UpdateWindowSurface(window);

	SDL_Event e;
	bool running = true;
	clock_t last = clock();
	clock_t now = 0;

	Chip8 * sys = new Chip8();
	SDL_Rect pixels[CHIP8_SCREEN_HEIGHT][CHIP8_SCREEN_WIDTH];

	sys->load(loadFileBuf("IBM Logo.ch8"));
	
	int pixLength = 10;
	
	for (int y = 0; y < CHIP8_SCREEN_HEIGHT; y++) {
		for (int x = 0; x < CHIP8_SCREEN_WIDTH; x++) {
			pixels[y][x] = {x, y, pixLength, pixLength};
		}
	}

	while (running) {
		while (SDL_PollEvent(&e)) {
			// Check for quit event
			if (e.type == SDL_QUIT) {
				running = false;
			}

			// Check time
			now = clock();
			// If a 700th of a second has passed, run next cycle on Chip8
			// Dividing clock ticks per second by 1000 gives clock ticks per millisecond.
			// Cycle the Chip8 if 2 ms worth of ticks have passed.
			if (1) {
				std::cout << "draw" << std::endl;
				last = now;

				sys->cycle();

				// If the sound flag is set, play a sound then unset it
				if (sys->sound) {
					// TODO: Play sound
					sys->sound = false;
				}
				
				// If the draw flag is set, draw, then unset it
				if (sys->draw) {
					sys->draw = false;

					for (int y = 0; y < CHIP8_SCREEN_HEIGHT; y++) {
						for (int x = 0; x < CHIP8_SCREEN_WIDTH; x++) {
							byte draw = sys->displayBuffer[y][x];

							if (draw == 1) {
								SDL_FillRect(surface,
									&pixels[y][x],
									SDL_MapRGB(surface->format, 0x00, 0x00, 0x00)
								);
								std::cout << "pix" << std::endl;
							} else {
								SDL_FillRect(surface,
									&pixels[y][x],
									SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF)
								);
							}
						}
					}

					SDL_UpdateWindowSurface(window);
				}
			}
		}
	}

	delete sys;
}

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
	
			emulate(window, surface);
		}

	}
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
