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

static uint16_t id_count = 1;

class Portfolio {
public:
	Portfolio(const std::optional<uint16_t>& id = std::nullopt);
	~Portfolio();
	
	void Update();
	void Print() noexcept;
	void Add(const Stock& s);
	void Add(const FX& fx);
	void Simulate(uint32_t period);
	void Reset();
	void ShowParams(const std::optional<std::string>& p = std::nullopt);
	void Info() noexcept;

	std::vector<Stock> stocks;
	std::vector<FX> fxs;

	uint16_t id;

	float stock_stddev = DEFAULT_STOCK_STDDEV_VALUE;
	float fx_stddev = DEFAULT_FX_STDDEV_VALUE;

	void WriteToFile();
	void ReadFromFile();
	void PurgeSession();

private:
	std::default_random_engine generator;

	
};

