#include "display.h"

#pragma warning(disable : 6386)
#pragma warning(disable : 6385)


__declspec(noalias) void RenderNormal(SDL_Renderer* r, const size_t len, const float* normalized_data, const float* data, const int W, const int H) {
	SDL_SetRenderDrawColor(r, 180, 255, 0, 255);
	for (size_t i = 0; i < len - 1; i++) {
		SDL_RenderDrawLine(r, i * W / len, H / 2 - (normalized_data[i]) * H / 4, (i + 1) * W / len, H / 2 - (normalized_data[i + 1]) * H / 4);
	}

	SDL_SetRenderDrawColor(r, 255, 180, 0, 255);
	for (size_t i = 0; i < len - 2; i++) {
		SDL_RenderDrawLine(r, i * W / len, 3 * H / 4 - (data[i + 1] - data[i]) * 50, (i + 1) * W / len, 3 * H / 4 - (data[i + 2] - data[i + 1]) * 50);
	}
}

__declspec(noalias) void RenderAdvanced(SDL_Renderer* r, int lines, const double* data, const double* data_ma12, const double* data_ma26, float pan_x, float pan_y, float s, int W, int H) {
	float max1 = 0;
	for (int i = lines - 500; i < lines + 500; i++) {
		if (data[i] >= max1) {
			max1 = data[i];
		}
	}

	int liness = 1000;

	int sc = 1;

	for (int i = lines - 500; i < lines + 500; i++) {

		float datapos1 = H / s - data[i]*sc / max1 * (700 + pan_y) / s;
		float datapos2 = H / s - data[i + 1]*sc / max1 * (700 + pan_y) / s;
		SDL_SetRenderDrawColor(r, data[i]*sc > data[i + 1]*sc ? 170 : 0, data[i]*sc > data[i + 1]*sc ? 0 : 170, 30, 255);
		SDL_RenderDrawLine(r, i * W / liness + pan_x, datapos1, (i + 1) * W / liness + pan_x, datapos2);
		if (data_ma12 != NULL) {
			if (i > 12) {
				SDL_SetRenderDrawColor(r, 150, 140, 30, 255);
				float dataposma121 = H / s - data_ma12[i]*sc / max1 * (700 + pan_y) / s;
				float dataposma122 = H / s - data_ma12[i + 1]*sc / max1 * (700 + pan_y) / s;
				SDL_RenderDrawLine(r, (i + 12) * W / liness + pan_x, dataposma121, (i + 1 + 12) * W / liness + pan_x, dataposma122);
			}
		}
		if (data_ma26 != NULL) {
			if (i > 26) {
				SDL_SetRenderDrawColor(r, 30, 170, 200, 255);
				float dataposma261 = H / s - data_ma26[i]*sc / max1 * (700 + pan_y) / s;
				float dataposma262 = H / s - data_ma26[i + 1]*sc / max1 * (700 + pan_y) / s;
				SDL_RenderDrawLine(r, (i + 26) * W / liness + pan_x, dataposma261, (i + 1 + 26) * W / liness + pan_x, dataposma262);
				//SDL_RenderDrawLine(r, (i+26) * W / lines, H * 3*(1000-lines/2)/1000*2 - data_ma26[i] * 10*(1000-lines/2)/1000*2, (i + 1+26) * W / lines, H * 3*(1000-lines/2)/1000*2 - data_ma26[i + 1] * 10*(1000-lines/2)/1000*2);
			}
		}

	}
	SDL_SetRenderDrawColor(r, 40, 40, 40, 255);
	SDL_RenderSetScale(r, 10, 10);
	SDL_RenderDrawLine(r, 0, 0, 0, H / 10);
	SDL_RenderDrawLine(r, 0, H / 10 - 10, W / 10, H / 10 - 10);
	SDL_RenderSetScale(r, 1, 1);
}

void _Display(const float* data, const float* data_ma12, const float* data_ma26, const size_t len, const int W, const int H, const short is_advanced) {

	int lines = LINES;

	float pan_x = 0;
	float pan_y = 0;

	float s = 0.5;

	/*for (int i = 0; i < lines; i++) {
		printf("%f\n", data[i]);
	}*/
	
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
	float max = 0.0f;
	float min = 999999999.0f;
	for (size_t i = 0; i < len; i++) {
		if (data[i] >= max) {
			max = data[i];
		}
		if (data[i] <= min) {
			min = data[i];
		}
	}
	
	for (size_t i = 0; i < len; i++) {
		normalized_data[i] = data[i] / max;
	}

	short quit = 0;
	int drag = 0;
	int zoom = 0;
	while (quit != 1) {
		SDL_Event e;

		while (SDL_PollEvent(&e) != NULL) {

			switch (e.type) {
			case SDL_QUIT: {
				quit = 1;
				break;
			}
			case SDL_KEYDOWN: {
				if (e.key.keysym.sym == SDLK_LCTRL) {
					zoom = 1;
					break;
				}
			}
			case SDL_KEYUP: {
				if (e.key.keysym.sym == SDLK_LCTRL) {
					zoom = 0;
					break;
				}
			}
			case SDL_MOUSEWHEEL: {
				if (e.wheel.y > 0) {
					if (zoom == 1) {
						s -= 0.05;
					}
					else {
						lines += 25;
						pan_x -= 50;
					}
				}
				else {
					if (zoom == 1) {
						s += 0.05;
					}
					else {
						lines -= 30;
						pan_x += 50;
					}
				}
			}
			case SDL_MOUSEBUTTONDOWN: {
				if (e.button.button == SDL_BUTTON_LEFT) {
					drag = 1;

					break;
				}
			}
			case SDL_MOUSEBUTTONUP: {
				if (e.button.button == SDL_BUTTON_LEFT) {
					drag = 0;

					break;
				}
			}
			case SDL_MOUSEMOTION: {

				if (drag == 1) {
					pan_x += 0.4 * e.motion.xrel;
					pan_y -= 0.4 * e.motion.yrel;
				}
			}
			}
		}
		SDL_SetRenderDrawColor(r, 10, 10, 10, 255);
		SDL_RenderClear(r);
		if (is_advanced == 0) {
			RenderNormal(r, len, normalized_data, data, W, H);
		}
		else {
			RenderAdvanced(r, lines, data, data_ma12, data_ma26, pan_x, pan_y, s, W, H);
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