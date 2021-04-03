#pragma once
#include <optional>
#include <string>
#include <iostream>

class FX {
public:
	FX();
	FX(const std::string& name, float rate);
	~FX();
	void UpdateValue(float change);
	void Print();

	friend std::ostream& operator<<(std::ostream& s, const FX& fx);

	void Reset(const std::optional<float>& val = std::nullopt);

	float GetReturn();
	double GetLogReturn();

	std::string name;
	float rate;
	float original_rate;
};

