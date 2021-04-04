#pragma once
#include <vector>
#include "Stock.h"
#include "FX.h"
#include <random>
#include <stdint.h>
#include <fstream>
#include "common.h"
#include <sstream>

class Portfolio {
public:
	Portfolio();
	~Portfolio();
	
	void Update();
	void Print() noexcept;
	void Add(const Stock& s);
	void Add(const FX& fx);
	void Simulate(uint32_t period);
	void Reset();

	std::vector<Stock> stocks;
	std::vector<FX> fxs;

	void WriteToFile();
	void ReadFromFile();

private:
	std::default_random_engine generator;
	
};

