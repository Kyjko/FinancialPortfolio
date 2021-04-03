#include "FX.h"

FX::FX(const std::string& name, float rate) : name(name), rate(rate) {}

FX::~FX() {}

void FX::UpdateValue(float change) {
	rate += change;
}

void FX::Print() {
	std::cout << *this;
}

std::ostream& operator<<(std::ostream& s, const FX& fx) {
	s << fx.name << " - " << fx.rate << std::endl;
	return s;
}