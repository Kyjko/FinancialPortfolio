#pragma once
#include <vector>
#include "Stock.h"
#include "FX.h"
#include <random>

class Portfolio {
public:
	Portfolio();
	~Portfolio();
	
	void Update();
	void Print();
	void Add(const Stock& s);
	void Add(const FX& fx);

	std::vector<Stock> stocks;
	std::vector<FX> fxs;

private:
	std::default_random_engine generator;
	
};

