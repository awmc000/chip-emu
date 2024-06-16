#include "chip8.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320
#define PIXEL_LENGTH 10

using std::ios_base;

byte * loadFileBuf(const std::string &filename) {
	byte * fileBuf = new byte[CHIP8_ROM_BYTES]; 
	std::ifstream in(filename, ios_base::in | ios_base::binary);

	do {
		in.read((char *) fileBuf, CHIP8_ROM_BYTES);
	} while (in.gcount() > 0);

	return fileBuf;
}


void configurePixels(SDL_Rect pixels[32][64], int pixLength)
{
    for (int y = 0; y < CHIP8_SCREEN_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_SCREEN_WIDTH; x++) {
            pixels[y][x] = {x * pixLength, y * pixLength, pixLength, pixLength};
        }
    }
}

void drawFromChip(Chip8 *sys, SDL_Surface *surface, SDL_Rect pixels[32][64])
{
    sys->draw = false;

    for (int y = 0; y < CHIP8_SCREEN_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_SCREEN_WIDTH; x++) {
            byte draw = sys->displayBuffer[y][x];

            if (draw == 0) {
                SDL_FillRect(surface,
                    &pixels[y][x],
                    SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
            } else {
                SDL_FillRect(surface,
                    &pixels[y][x],
                    SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
            }
		}
    }
}

void emulate(SDL_Window * window, SDL_Surface * surface, const char * filename) {
	SDL_UpdateWindowSurface(window);

	SDL_Event e;
	bool running = true;
	auto last = std::chrono::high_resolution_clock::now();
	auto now = last;

	Chip8 * sys = new Chip8();
	SDL_Rect pixels[CHIP8_SCREEN_HEIGHT][CHIP8_SCREEN_WIDTH];

	sys->load(loadFileBuf(filename));

	int pixLength = 10;
	
	// Set up pixel rectangles: 64x32 10x10px rectangles.
    configurePixels(pixels, pixLength);

    while (running) {
		while (SDL_PollEvent(&e)) {
			// Check for quit event
			if (e.type == SDL_QUIT) {
				running = false;
			}

			// Check time
			auto elapsed = std::chrono::high_resolution_clock::now() - last;
			auto elapsedMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
			last = std::chrono::high_resolution_clock::now();
			
			// If a 700th of a second has passed, run next cycle on Chip8
			if (elapsedMicroseconds > 1429LL) {

				sys->cycle();

				// If the sound flag is set, play a sound then unset it
				if (sys->sound) {
					// TODO: Play sound
					sys->sound = false;
				}
				
				// If the draw flag is set, draw, then unset it
                if (sys->draw) {
					drawFromChip(sys, surface, pixels);
	                SDL_UpdateWindowSurface(window);
				}
			}
		}
	}

	delete sys;
}

int main(int argc, char ** argv)
{
	if (argc < 2) {
		std::cout << "Include the filename of a Chip8 ROM." << std::endl;
		exit(0);
	}

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
	
			std::string fn = argv[1];
			emulate(window, surface, fn.c_str());
		}

	}
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
