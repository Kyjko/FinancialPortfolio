#pragma once

#define DEFAULT_STOCK_VALUE 10.0f
#define DEFAULT_FX_VALUE 1.0f
#define DEFAULT_SIMULATION_TIMEPERIOD_VALUE 10
#define STOCKS_NAME_MAX_LENGTH 100
#define FXS_NAME_MAX_LENGTH 100

constexpr unsigned int Str2Int(const char* str, int h = 0);
void PrintHelp() noexcept;
void Clear() noexcept;

#define FILE_OPEN_ERR(type) std::cout << "Error: couldn't open datafile - " << type << std::endl
#define FILE_REMOVE_ERR(type) std::cout << "Error: couldn't delete datafile - " << type << std::endl