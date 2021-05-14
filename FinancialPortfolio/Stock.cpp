#include "Stock.h"

Stock::Stock(const std::string& name, float initial_value, float weight) : name(name), value(initial_value), weight(weight) {
	original_value = value;
}

Stock::~Stock() {}

void Stock::UpdateValue(float change) {
	value += change;
	if (value < MINIMAL_FLOAT_VALUE) {
		value = MINIMAL_FLOAT_VALUE;
	}
}

void Stock::Print() {
	std::cout << *this;
}

float Stock::GetReturn() const noexcept { 
	return value / original_value;
}

double Stock::GetLogReturn() const noexcept {
	return log(value / original_value);
}

void Stock::Reset(const std::optional<float>& val) {
	if (val) {
		value = *val;
	}
	else {
		value = original_value;
	}
}

std::ostream& operator<<(std::ostream& s, const Stock& st) {
	s << st.name << " - " << st.value << std::endl;
	return s;
}
