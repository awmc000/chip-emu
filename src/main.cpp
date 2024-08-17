#include "chip8.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

#define SCREEN_WIDTH 		640
#define SCREEN_HEIGHT 		320
#define SIDEBAR_WIDTH 		200
#define PIXEL_LENGTH 		10
#define CYCLE_MICROSECONDS 	1429
#define OFF_COLOUR			0x00, 0x00, 0x00
#define ON_COLOUR			0x00, 0xFF, 0x33

using std::ios_base;

struct ChipFrontend {
	bool active;
	Mix_Chunk * beep_sfx;
	SDL_Rect pixels[32][64];
};

ChipFrontend fe {
	false,
	nullptr,
};

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
                    SDL_MapRGB(surface->format, OFF_COLOUR));
            } else {
                SDL_FillRect(surface,
                    &pixels[y][x],
                    SDL_MapRGB(surface->format, ON_COLOUR));
            }
		}
    }
}


void printCurrentInstruction(Chip8 *sys)
{
    byte instr_top = sys->ram[sys->programCounter];
    byte instr_bot = sys->ram[sys->programCounter + 1];
    word instr = (instr_top << 8) | instr_bot;
    std::cerr << std::hex << instr << std::endl;
}

void handleKeyDown(SDL_Event * e, Chip8 * sys) {

	if (sys->blockingForKey)
		sys->lastKeyFromBlock = true;

	switch (e->key.keysym.sym) {

		// Row 1:  1234 -> 123C

		case SDLK_1:
			sys->keyState[0x1] = 1;
			sys->lastKey = 0x1;
			break;
		case SDLK_2:
			sys->keyState[0x2] = 1;
			sys->lastKey = 0x2;
			break;
		case SDLK_3:
			sys->keyState[0x3] = 1;
			sys->lastKey = 0x3;
			break;
		case SDLK_4:
			sys->keyState[0xC] = 1;
			sys->lastKey = 0xC;
			break;

		// Row 2: QWER -> 456D

		case SDLK_q:
			sys->keyState[0x4] = 1;
			sys->lastKey = 0x4;
			break;
		case SDLK_w:
			sys->keyState[0x5] = 1;
			sys->lastKey = 0x5;
			break;
		case SDLK_e:
			sys->keyState[0x6] = 1;
			sys->lastKey = 0x6;
			break;
		case SDLK_r:
			sys->keyState[0xD] = 1;
			sys->lastKey = 0xD;
			break;

		// Row 3: ASDF -> 789E

		case SDLK_a:
			sys->keyState[0x7] = 1;
			sys->lastKey = 0x7;
			break;
		case SDLK_s:
			sys->keyState[0x8] = 1;
			sys->lastKey = 0x8;
			break;
		case SDLK_d:
			sys->keyState[0x9] = 1;
			sys->lastKey = 0x9;
			break;
		case SDLK_f:
			sys->keyState[0xE] = 1;
			sys->lastKey = 0xE;
			break;

		// Row 4: ZXCV -> A0BF

		case SDLK_z:
			sys->keyState[0xA] = 1;
			sys->lastKey = 0xA;
			break;
		case SDLK_x:
			sys->keyState[0x0] = 1;
			sys->lastKey = 0x0;
			break;
		case SDLK_c:
			sys->keyState[0xB] = 1;
			sys->lastKey = 0xB;
			break;
		case SDLK_v:
			sys->keyState[0xF] = 1;
			sys->lastKey = 0xF;
			break;
		default:
			// The user pressed a key not on the Chip8 keypad
			// So we don't track it for GETKEY purposes
			sys->lastKeyFromBlock = false;
			break;
	}
	
	// Interpreter frontend controls
	switch (e->key.keysym.sym) {
		case SDLK_SPACE:
			fe.active ^= 1;
			break;
		case SDLK_PERIOD:
            printCurrentInstruction(sys);
			sys->cycle();
			break;
		case SDLK_ESCAPE:
			exit(0);
			break;
	}
}


void handleKeyUp(SDL_Event * e, Chip8 * sys) {
	switch (e->key.keysym.sym) {
		case SDLK_1:
			sys->keyState[0x1] = 0;
			break;
		case SDLK_2:
			sys->keyState[0x2] = 0;
			break;
		case SDLK_3:
			sys->keyState[0x3] = 0;
			break;
		case SDLK_4:
			sys->keyState[0xC]= 0;
			break;

		case SDLK_q:
			sys->keyState[0x4] = 0;
			break;
		case SDLK_w:
			sys->keyState[0x5] = 0;
			break;
		case SDLK_e:
			sys->keyState[0x6] = 0;
			break;
		case SDLK_r:
			sys->keyState[0xD] = 0;
			break;

		case SDLK_a:
			sys->keyState[0x7] = 0;
			break;
		case SDLK_s:
			sys->keyState[0x8] = 0;
			break;
		case SDLK_d:
			sys->keyState[0x9] = 0;
			break;
		case SDLK_f:
			sys->keyState[0xE] = 0;
			break;

		case SDLK_z:
			sys->keyState[0xA] = 0;
			break;
		case SDLK_x:
			sys->keyState[0x0] = 0;
			break;
		case SDLK_c:
			sys->keyState[0xB] = 0;
			break;
		case SDLK_v:
			sys->keyState[0xF] = 0;
			break;
		default:
			break;
	}
}

void emulate(SDL_Window * window, SDL_Surface * surface, const char * filename) {
	SDL_UpdateWindowSurface(window);

	SDL_Event e;
	bool running = true;
	auto last = std::chrono::high_resolution_clock::now();
	auto now = last;

	Chip8 * sys = new Chip8();
	sys->load(loadFileBuf(filename));

	int pixLength = 10;

	// Set up pixel rectangles: 64x32 10x10px rectangles.
    configurePixels(fe.pixels, pixLength);

    while (running) {

		SDL_PollEvent(&e);

		if (e.type == SDL_QUIT) {
			running = false;
		}

		if (e.type == SDL_KEYDOWN) {
			handleKeyDown(&e, sys);
		}

		if (e.type == SDL_KEYUP) {
			handleKeyUp(&e, sys);
		}

		// Check time
		auto elapsed = std::chrono::high_resolution_clock::now() - last;
		auto elapsedMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

		// If unpaused and a 700th of a second has passed, run next cycle on Chip8
		if (fe.active && elapsedMicroseconds > CYCLE_MICROSECONDS) {
			last = std::chrono::high_resolution_clock::now();

			sys->cycle();

			// If the sound flag is set, play a sound then unset it
			if (sys->sound) {
				Mix_PlayChannel(-1, fe.beep_sfx, 0);
				sys->sound = false;
			}
		}

		// If the draw flag is set, draw, then unset it
        if (sys->draw) {
			drawFromChip(sys, surface, fe.pixels);
	        SDL_UpdateWindowSurface(window);
		}
	}
	delete sys;
}

void printInstructions() {
	std::cout << "Hit [Space] to pause/unpause.\n"
	"The interpreter is paused when opened.\n"
	"When paused, hit [.] to step through instructions one at a time.\n"
	"Hit [Escape] at any time to close the interpreter.\n"
	<< std::endl;
}

void setupAudio()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        exit(1);
    }
}

int main(int argc, char ** argv)
{
	if (argc < 2) {
		std::cout << "Include the filename of a Chip8 ROM." << std::endl;
		exit(0);
	}

    SDL_Window * window = nullptr;
	SDL_Surface * surface = nullptr;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL init failed: " << SDL_GetError() << std::endl;
		exit(1);
	}

    setupAudio();

    fe.beep_sfx = Mix_LoadWAV("beep.wav");

	if (fe.beep_sfx == nullptr) {
		printf("Failed to load beep wav! SDL_mixer error: %s\n", Mix_GetError());
		exit(1);
	}

	window = SDL_CreateWindow("chip-emu",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		std::cout << "Window creation failed:" << SDL_GetError() << std::endl;
		exit(1);
	} 

	surface = SDL_GetWindowSurface(window);

	SDL_FillRect(surface,
		NULL,
		SDL_MapRGB(surface->format, OFF_COLOUR)
	);

	std::string fn = argv[1];

	printInstructions();

	emulate(window, surface, fn.c_str());

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
