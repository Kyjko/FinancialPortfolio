#include <iostream>
#include "common.h"
#include "Portfolio.h"
#include "FX.h"
#include <stdint.h>

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

void AddInitialEntries(Portfolio& p) {
	p.stocks.push_back(Stock("STOCK1", 50));
	p.stocks.push_back(Stock("STOCK2", 150));
	p.fxs.push_back(FX("EURUSD", 1.21f));
	p.fxs.push_back(FX("EURHUF", 360.54f));
}

void PrintHelp() noexcept {
	std::cout << "FinancialPortfolio v0.1" << std::endl;
	std::cout << "Commands:" << std::endl;
	std::cout << "\tls - list portfolio" << std::endl;
	std::cout << "\tclear - clear console" << std::endl;
	std::cout << "\tupdate - update portfolio once" << std::endl;
	std::cout << "\tsimulate - enter simulation preparation mode" << std::endl;
	std::cout << "\t\t<time period> - number of updates to be applied" << std::endl;
	std::cout << "\tadd stock - enter add stock mode" << std::endl;
	std::cout << "\t\t<name> - name of stock" << std::endl;
	std::cout << "\t\t<value> - initial value of stock" << std::endl;
	std::cout << "\tadd fx - enter add fx mode" << std::endl;
	std::cout << "\t\t<name> - name of fx" << std::endl;
	std::cout << "\t\t<rate> - initial rate of fx" << std::endl;
	std::cout << "\treset - resets all stock and fx values to their original value" << std::endl;
	std::cout << "\tsave - saves session data" << std::endl;
	std::cout << "\tget - imports session data" << std::endl;
	std::cout << "\tpurge - deletes saved session data" << std::endl;
	std::cout << "\texit - exits FinancialPortfolio, all unsaved data will be lost - use 'save' to save your session data" << std::endl;
}

int main(int argc, char** argv) {
	Portfolio p;
	bool quit = false;
	AddInitialEntries(p);
	
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
		case Str2Int("help"): {
			PrintHelp();
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
				if (val <= 0) {
					std::cout << "stock value cannot be zero or negative!" << std::endl;
					break;
				}
				else {
					p.Add(Stock(name, val));
				}
			}
			
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
				if (val <= 0) {
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