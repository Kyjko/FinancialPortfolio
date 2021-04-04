#pragma once
#include <vector>
#include "Stock.h"
#include "FX.h"
#include <random>
#include <stdint.h>
#include <fstream>
#include "common.h"
#include <sstream>
#include <optional>
#include <string>

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
	void ShowParams(const std::optional<std::string>& p = std::nullopt);

	std::vector<Stock> stocks;
	std::vector<FX> fxs;

	float stock_stddev = DEFAULT_STOCK_STDDEV_VALUE;
	float fx_stddev = DEFAULT_FX_STDDEV_VALUE;

	void WriteToFile();
	void ReadFromFile();
	void PurgeSession();

private:
	std::default_random_engine generator;

	
};

