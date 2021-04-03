#pragma once
#include <string>
#include <math.h>
#include <iostream>
#include <optional>

class Stock {
public:
	Stock();
	Stock(const std::string& name, float initial_value);
	~Stock();
	void UpdateValue(float change);
	void Print();

	friend std::ostream& operator<<(std::ostream& s, const Stock& st);

	std::string name;
	float value;
	float original_value;

	float GetReturn() const noexcept;
	double GetLogReturn() const noexcept;
	void Reset(const std::optional<float>& val = std::nullopt);
};



