#include <iostream>
#include "common.h"
#include "Portfolio.h"
#include "FX.h"
#include <stdint.h>

const char* BANNER =
"_____________________\n"
"|  ________________  |\n"
"| | Financial      | |\n"
"| |__Portfolio v0.1| |\n"
"|  ___ ___ ___  ____ |\n"
"| | 7 | 8 | 9 | | +| |\n"
"| |___|___|___| |__| |\n"
"| | 4 | 5 | 6 | | -| |\n"
"| |___|___|___| |__| |\n"
"| | 1 | 2 | 3 | | x| |\n"
"| |___|___|___| |__| |\n"
"| | . | 0 | = | | /| |\n"
"| |___|___|___| |__| |\n"
"|____________________|\n";

constexpr unsigned int Str2Int(const char* str, int h) {
	return !str[h] ? 5381 : (Str2Int(str, h + 1) * 33) ^ str[h];
}

void Clear() noexcept {
#ifdef __unix__
	system("clear");
#elif defined(_WIN32) || defined(_WIN64)
	system("cls");
#endif
}

void PrintBanner() noexcept {
	std::cout << BANNER << std::endl;
}

void AddInitialEntries(Portfolio& p) {
	p.stocks.push_back(Stock("STOCK1", 50));
	p.stocks.push_back(Stock("STOCK2", 150));
	p.fxs.push_back(FX("EURUSD", 1.21f));
	p.fxs.push_back(FX("EURHUF", 360.54f));
}

void PrintHelp() noexcept {
	std::cout << "FinancialPortfolio v0.1" << std::endl;
	std::cout << "Commands:" << std::endl;
	std::cout << "\tls - list portfolio contents" << std::endl;
	std::cout << "\tclear - clear console" << std::endl;
	std::cout << "\tset - enter 'set parameters' mode" << std::endl;
	std::cout << "\tshow params - show parameters and their values" << std::endl;
	std::cout << "\tbanner - show banner" << std::endl;
	std::cout << "\tinfo - display portfolio information" << std::endl;
	std::cout << "\tupdate - update portfolio once" << std::endl;
	std::cout << "\tplot - enters graph plot mode" << std::endl;
	std::cout << "\t\t<name> - name of product to be plotted" << std::endl;
	std::cout << "\tsimulate - enter 'simulation preparation' mode" << std::endl;
	std::cout << "\t\t<time period> - number of updates to be applied" << std::endl;
	std::cout << "\tadd stock - enter 'add stock' mode" << std::endl;
	std::cout << "\t\t<name> - name of stock" << std::endl;
	std::cout << "\t\t<value> - initial value of stock" << std::endl;
	std::cout << "\tadd fx - enter 'add fx' mode" << std::endl;
	std::cout << "\t\t<name> - name of fx" << std::endl;
	std::cout << "\t\t<rate> - initial rate of fx" << std::endl;
	std::cout << "\treset - resets all stock and fx values to their original value" << std::endl;
	std::cout << "\tsave - saves session data" << std::endl;
	std::cout << "\tget - imports session data" << std::endl;
	std::cout << "\tpurge - deletes saved session data" << std::endl;
	std::cout << "\texit - exits FinancialPortfolio, all unsaved data will be lost - use 'save' to save your session data" << std::endl;
}

int main(int argc, char** argv) {
	bool quit = false;

	Portfolio p;
	AddInitialEntries(p);
	PrintBanner();
	std::string cmd;
	while (!quit) {
		std::cout << "> ";
		std::getline(std::cin, cmd);
		switch (Str2Int(cmd.c_str())) {
		case Str2Int("exit"): {
			std::cout << "Bye" << std::endl;
			quit = true;
			break;
		}
		case Str2Int("ls"): {
			p.Print();
			break;
		}
		case Str2Int("clear"): {
			Clear();
			break;
		}
		case Str2Int("update"): {
			p.Update();
			break;
		}
		case Str2Int("info"): {
			p.Info();
			break;
		}
		case Str2Int("help"): {
			PrintHelp();
			break;
		}
		case Str2Int("banner"): {
			PrintBanner();
			break;
		}
		case Str2Int("show params"): {
			p.ShowParams();
			break;
		}
		case Str2Int("simulate"): {
			std::cout << "Time period:" << std::endl;
			std::string s;
			std::getline(std::cin, s);
			uint32_t pr = 10;
			try {
				pr = std::stoi(s);
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what() << std::endl;
				std::cout << "value set to default (" << DEFAULT_SIMULATION_TIMEPERIOD_VALUE << ")" << std::endl;
			}
			p.Simulate(pr);
			break;
		}
		case Str2Int("plot"): {
			std::cout << "name: " << std::endl;
			std::string s;
			std::getline(std::cin, s);
			p.Graph(s);
			break;
		}
		case Str2Int("reset"): {
			p.Reset();
			break;
		}
		case Str2Int("save"): {
			p.WriteToFile();
			break;
		}
		case Str2Int("get"): {
			p.ReadFromFile();
			break;
		}
		case Str2Int("purge"): {
			p.PurgeSession();
			break;
		}
		case Str2Int("add stock"): {
			std::cout << "(<name> <value>)" << std::endl;
			std::string s;
			std::getline(std::cin, s);

			uint16_t args_count = 1;
			for (const char& c : s) {
				if (isspace(c)) {
					args_count++;
				}
			}
			if (args_count != 2) {
				std::cout << "Invalid syntax!" << std::endl;
			}
			else {
				size_t pos = s.find(" ");
				std::string name = s.substr(0, pos);
				float val = DEFAULT_STOCK_VALUE;
				try {
					val = std::stof(s.substr(pos, s.length()));
				}
				catch (const std::invalid_argument& e) {
					std::cout << e.what() << std::endl;
					std::cout << "value set to default (" << DEFAULT_STOCK_VALUE << ")" << std::endl;
				}
				if (val < MINIMAL_FLOAT_VALUE) {
					std::cout << "stock value cannot be zero or negative!" << std::endl;
					break;
				}
				else {
					p.Add(Stock(name, val));
				}
			}
			
			break;
		}
		case Str2Int("set"): {
			std::cout << "stock stddev:" << std::endl;
			std::string s;
			std::getline(std::cin, s);
			float val = DEFAULT_STOCK_STDDEV_VALUE;
			try {
				val = std::stof(s);
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what() << std::endl;
				std::cout << "value set to default (" << DEFAULT_STOCK_STDDEV_VALUE << ")" << std::endl;
			}
			if (val < MINIMAL_FLOAT_VALUE) {
				std::cout << "standard deviation can only be positive, value set to minimal float value" << std::endl;
				val = MINIMAL_FLOAT_VALUE;
			}
			p.stock_stddev = val;
			std::cout << "fx stddev:" << std::endl;
			std::string s2;
			std::getline(std::cin, s2);
			float val2 = DEFAULT_FX_STDDEV_VALUE;
			try {
				val2 = std::stof(s2);
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what() << std::endl;
				std::cout << "value set to default (" << DEFAULT_FX_STDDEV_VALUE << ")" << std::endl;
			}
			if (val2 < MINIMAL_FLOAT_VALUE) {
				std::cout << "standard deviation can only be positive, value set to minimal float value" << std::endl;
				val2 = MINIMAL_FLOAT_VALUE;
			}
			p.fx_stddev = val2;
			break;
		}
		case Str2Int("add fx"): {
			std::cout << "(<name> <rate>)" << std::endl;
			std::string s;
			std::getline(std::cin, s);

			uint16_t args_count = 1;
			for (const char& c : s) {
				if (isspace(c)) {
					args_count++;
				}
			}
			if (args_count != 2) {
				std::cout << "Invalid syntax!" << std::endl;
			}
			else {
				size_t pos = s.find(" ");
				std::string name = s.substr(0, pos);
				float val = DEFAULT_FX_VALUE;
				try {
					val = std::stof(s.substr(pos, s.length()));
				}
				catch (const std::invalid_argument& e) {
					std::cout << e.what() << std::endl;
					std::cout << "value set to default (" << DEFAULT_FX_VALUE << ")" << std::endl;
				}
				if (val < MINIMAL_FLOAT_VALUE) {
					std::cout << "fx rate cannot be zero or negative!" << std::endl;
					break;
				}
				else {
					p.Add(FX(name, val));
				}
			}
			break;
		}
		default: {
			std::cout << "Unknown command!" << std::endl;
		}
		}
	}

	std::cout << "Finished" << std::endl;
	
	return 0;
}