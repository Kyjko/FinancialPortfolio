#pragma once

#define DEFAULT_STOCK_VALUE 10.0f
#define DEFAULT_FX_VALUE 1.0f
#define DEFAULT_SIMULATION_TIMEPERIOD_VALUE 50
#define DEFAULT_STOCK_STDDEV_VALUE 1.5f
#define DEFAULT_FX_STDDEV_VALUE 0.5f

#define MINIMAL_FLOAT_VALUE 0.000000001f

#define DEFAULT_FIGUREWINDOW_WIDTH 1000
#define DEFAULT_FIGUREWINDOW_HEIGHT 1000

constexpr unsigned int Str2Int(const char* str, int h = 0);
void PrintHelp() noexcept;
void Clear() noexcept;
void PrintBanner() noexcept;

#define FILE_OPEN_ERR(type) std::cout << "Error: couldn't open datafile - " << type << std::endl
#define FILE_REMOVE_ERR(type) std::cout << "Error: couldn't delete datafile - " << type << std::endl