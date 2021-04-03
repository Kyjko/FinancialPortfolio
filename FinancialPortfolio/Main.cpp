#include <iostream>
#include "Portfolio.h"
#include "FX.h"

constexpr unsigned int Str2Int(const char* str, int h = 0) {
	return !str[h] ? 5381 : (Str2Int(str, h + 1) * 33) ^ str[h];
}

int main(int argc, char** argv) {
	Portfolio p;
	bool quit = false;
	p.stocks.push_back(Stock("STOCK_1", 50));
	p.stocks.push_back(Stock("STOCK_2", 150));
	
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
			system("cls");
			break;
		}
		case Str2Int("update"): {
			p.Update();
			break;
		}
		case Str2Int("add stock"): {
			std::string n;
			std::cout << "\t| ";
			std::cin >> n;
			std::string v;
			std::cout << "\t| ";
			std::cin >> v;
			p.Add(Stock(n, stof(v)));
			
			break;
			
		}
		case Str2Int("add fx"): {
			std::string n;
			std::cout << "\t| ";
			std::cin >> n;
			std::string r;
			std::cout << "\t| ";
			std::cin >> r;
			p.Add(FX(n, stof(r)));

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