#include "Portfolio.h"

Portfolio::Portfolio() {
}

Portfolio::~Portfolio() {
	stocks = std::vector<Stock>();
	fxs = std::vector<FX>();
}

void Portfolio::Update() {
	std::normal_distribution<float> ndist(0.0f, 5.0f);
	for (auto i = stocks.begin(); i != stocks.end(); i++) {
		i->UpdateValue(ndist(generator));
	}
	ndist = std::normal_distribution<float>(0.0f, 2.0f);
	for (auto i = fxs.begin(); i != fxs.end(); i++) {
		i->UpdateValue(ndist(generator));
	}
}

void Portfolio::Reset() {
	for (auto i = stocks.begin(); i != stocks.end(); i++) {
		i->Reset();
	}
	for (auto i = fxs.begin(); i != fxs.end(); i++) {
		i->Reset();
	}
}

void Portfolio::Print() {
	std::cout << "----Portfolio:----" << std::endl;
	std::cout << "--Stocks--" << std::endl;
	for (auto i = stocks.begin(); i != stocks.end(); i++) {
		i->Print();
		std::cout << "\tReturn: x" << i->GetReturn() << " (" << i->GetLogReturn()*100.0 << "%)" << std::endl;
	}
	std::cout << "--FX--" << std::endl;
	for (auto i = fxs.begin(); i != fxs.end(); i++) {
		i->Print();
		std::cout << "\tReturn: x" << i->GetReturn() << " (" << i->GetLogReturn()*100.0 << "%)" << std::endl;
	}
}

void Portfolio::Add(const Stock& s) {
	stocks.push_back(s);
}

void Portfolio::Add(const FX& fx) {
	fxs.push_back(fx);
}
 
void Portfolio::Simulate(uint32_t period) {
	for (uint32_t i = 0; i < period; i++) {
		Update();
	}
}