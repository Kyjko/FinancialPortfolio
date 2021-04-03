#pragma once

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

	std::string name;
	float rate;
};

