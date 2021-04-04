#pragma once
#include <stdio.h>
#include <SDL.h>

#define W 1000
#define H 1000

__declspec(noalias) void _Display(const float* __restrict data, const size_t len);