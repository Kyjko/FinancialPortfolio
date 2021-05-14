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
#include <thread>

static uint16_t id_count = 1;

class Portfolio {
public:
	enum class DistributionTypes {
		NormalDistribution,
		CauchyDistribution,
		LognormalDistribution
	};

	Portfolio(const DistributionTypes& dist_type, const std::optional<uint16_t>& id = std::nullopt);
	~Portfolio();
	
	void Update();
	void Print() noexcept;
	void Add(const Stock& s);
	void Add(const FX& fx);
	void Simulate(uint32_t period);
	void Reset();
	void ShowParams(const std::optional<std::string>& p = std::nullopt);
	void PrintReturnAndStddev() const noexcept;
	void Info() noexcept;
	void Graph(const std::string& name, const std::optional<uint32_t>& tp = std::nullopt);
	void ShowDistributionType();

	// MPT
	std::pair<float, float> GetReturn() const noexcept;
	std::pair<float, float> GetStddev() const noexcept;

	void AdjustWeights() noexcept;

	void SetDistribution(const DistributionTypes& dist_type);

	std::vector<Stock> stocks;
	std::vector<FX> fxs;

	DistributionTypes random_distribution;

	uint16_t id;

	float stock_stddev = DEFAULT_STOCK_STDDEV_VALUE;
	float fx_stddev = DEFAULT_FX_STDDEV_VALUE;

	void WriteToFile(const std::vector<Stock>& stocks, const std::vector<FX>& fxs);
	void ReadFromFile(std::vector<Stock>& stocks, std::vector<FX>& fxs);
	void PurgeSession();

private:
	std::default_random_engine generator;

	
};

