#include "Stock.h"


Stock::Stock(const std::string& name, float initial_value) : name(name), value(initial_value) {}

Stock::~Stock() {}

void Stock::UpdateValue(float change) {
	value += change;
}

void Stock::Print() {
	std::cout << *this;
}

std::ostream& operator<<(std::ostream& s, const Stock& st) {
	s << st.name << " - " << st.value << std::endl;
	return s;
}
