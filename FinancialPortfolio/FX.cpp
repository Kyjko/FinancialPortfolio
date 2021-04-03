#include "FX.h"

FX::FX(const std::string& name, float rate) : name(name), rate(rate) {
	original_rate = rate;
}

FX::~FX() {}

void FX::UpdateValue(float change) {
	rate += change;
}

void FX::Print() {
	std::cout << *this;
}

void FX::Reset(const std::optional<float>& val) {
	if (val) {
		rate = *val;
	}
	else {
		rate = original_rate;
	}
}

float FX::GetReturn() {
	return rate / original_rate;
}

double FX::GetLogReturn() {
	return log(rate / original_rate);
}

std::ostream& operator<<(std::ostream& s, const FX& fx) {
	s << fx.name << " - " << fx.rate << std::endl;
	return s;
}