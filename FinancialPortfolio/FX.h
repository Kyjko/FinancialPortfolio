#pragma once
#include <optional>
#include <string>
#include "common.h"
#include <iostream>

class FX {
public:
	FX();
	FX(const std::string& name, float rate, float weight);
	~FX();
	void UpdateValue(float change);
	void Print();

	friend std::ostream& operator<<(std::ostream& s, const FX& fx);

	void Reset(const std::optional<float>& val = std::nullopt);

	float GetReturn() const noexcept;
	double GetLogReturn() const noexcept;

	std::string name;
	float rate;
	float original_rate;
	float weight;
};

