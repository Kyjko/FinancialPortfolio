#include "display.h"

__declspec(noalias) void _Display(const float* __restrict data, const size_t len) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return;
	}
	SDL_Window* w = SDL_CreateWindow("Figure", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, 0);
	if (w == NULL) {
		return;
	}
	SDL_Renderer* r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
	if (r == NULL) {
		return;
	}

	short quit = 0;
	while (quit != 1) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != NULL) {
			switch (e.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			}
		}
		SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r, 70, 255, 0, 255);
		for (size_t i = 0; i < len-2; i++) {
			SDL_RenderDrawLine(r, i * W / len, H / 2 - (data[i+1] - data[i])*50, (i + 1) * W / len, H / 2 - (data[i+2] - data[i+1])*50);
		}

		SDL_RenderPresent(r);
	}

	SDL_DestroyWindow(w);
	SDL_DestroyRenderer(r);
	SDL_Quit();

	w = NULL;
	r = NULL;
	
}