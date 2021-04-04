#include "display.h"

#pragma warning(disable : 6386)
#pragma warning(disable : 6385)

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

	float* normalized_data = (float*)malloc(sizeof(float) * len);
	if (normalized_data == NULL) {
		return;
	}
	float max = 0;
	for (size_t i = 0; i < len; i++) {
		if (data[i] >= max) {
			max = data[i];
		}
	}
	
	for (size_t i = 0; i < len; i++) {
		normalized_data[i] = data[i] / max;
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
		
		SDL_SetRenderDrawColor(r, 180, 255, 0, 255);
		for (size_t i = 0; i < len - 1; i++) {
			SDL_RenderDrawLine(r, i * W / len, H / 2 - (normalized_data[i])*100, (i + 1) * W / len, H / 2 - (normalized_data[i+1])*100);
		}

		SDL_SetRenderDrawColor(r, 255, 180, 0, 255);
		for (size_t i = 0; i < len - 2; i++) {
			SDL_RenderDrawLine(r, i * W / len, 3 * H / 4 - (data[i + 1] - data[i]) * 50, (i + 1) * W / len, 3 * H / 4 - (data[i + 2] - data[i + 1]) * 50);
		}
		SDL_RenderPresent(r);
	}

	free(normalized_data);
	SDL_DestroyWindow(w);
	SDL_DestroyRenderer(r);
	SDL_Quit();

	w = NULL;
	r = NULL;
	
}