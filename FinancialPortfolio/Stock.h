#pragma once
#include <string>
#include <iostream>

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
};



