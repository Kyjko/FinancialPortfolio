#pragma once

#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <memory.h>

#define LINES 500

// Display API

void _Display(const float* data, const float* data_ma12, const float* data_ma26, const size_t len, const int W, const int H, const short is_advanced);