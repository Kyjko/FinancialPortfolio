#include "Portfolio.h"

Portfolio::Portfolio() {
}

Portfolio::~Portfolio() {
	stocks = std::vector<Stock>();
	fxs = std::vector<FX>();
}

void Portfolio::Update() {
	std::normal_distribution<float> ndist(0.0f, 5.0f);
	for (auto i = stocks.begin(); i != stocks.end(); i++) {
		i->UpdateValue(ndist(generator));
	}
	ndist = std::normal_distribution<float>(0.0f, 2.0f);
	for (auto i = fxs.begin(); i != fxs.end(); i++) {
		i->UpdateValue(ndist(generator));
	}
}

void Portfolio::Reset() {
	for (auto i = stocks.begin(); i != stocks.end(); i++) {
		i->Reset();
	}
	for (auto i = fxs.begin(); i != fxs.end(); i++) {
		i->Reset();
	}
}

void Portfolio::Print() noexcept {
	std::cout << "----Portfolio:----" << std::endl;
	std::cout << "--Stocks--" << std::endl;
	for (auto i = stocks.begin(); i != stocks.end(); i++) {
		i->Print();
		std::cout << "\tReturn: x" << i->GetReturn() << " (" << i->GetLogReturn()*100.0 << "%)" << std::endl;
	}
	std::cout << "--FX--" << std::endl;
	for (auto i = fxs.begin(); i != fxs.end(); i++) {
		i->Print();
		std::cout << "\tReturn: x" << i->GetReturn() << " (" << i->GetLogReturn()*100.0 << "%)" << std::endl;
	}
}

void Portfolio::Add(const Stock& s) {
	stocks.push_back(s);
}

void Portfolio::Add(const FX& fx) {
	fxs.push_back(fx);
}
 
void Portfolio::Simulate(uint32_t period) {
	for (uint32_t i = 0; i < period; i++) {
		Update();
	}
}

void Portfolio::WriteToFile() {
	std::ofstream ofile("stocks.fp", std::ios::trunc);
	if (ofile.good()) {
		for (auto i = stocks.begin(); i != stocks.end(); i++) {
			ofile.write((const char*)&(i->value), sizeof(float));
		}
		ofile.close();
	}
	else {
		FILE_OPEN_ERR("data");
	}

	std::ofstream nameofile("stocksname.fp", std::ios::trunc);
	if (nameofile.good()) {
		for (auto i = stocks.begin(); i != stocks.end(); i++) {
			nameofile << i->name;
		}
		nameofile.close();
	}
	else {
		FILE_OPEN_ERR("data");
	}

	std::ofstream ofile2("fxs.fp", std::ios::trunc);
	if (ofile2.good()) {
		for (auto i = fxs.begin(); i != fxs.end(); i++) {
			ofile2.write((const char*)&(i->rate), sizeof(float));
		}
		ofile2.close();
	}
	else {
		FILE_OPEN_ERR("data");
	}

	std::ofstream nameofile2("fxsname.fp", std::ios::trunc);
	if (nameofile2.good()) {
		for (auto i = fxs.begin(); i != fxs.end(); i++) {
			nameofile2 << i->name;
		}
		nameofile2.close();
	}
	else {
		FILE_OPEN_ERR("data");
	}
	
}

void Portfolio::ReadFromFile() {
	std::ifstream ifile("stocks.fp");
	if (ifile.good()) {
		for (auto i = stocks.begin(); i != stocks.end(); i++) {
			float ival;
			ifile.read((char*)&ival, sizeof(float));
			i->value = ival;
		}

		ifile.close();
	}
	else {
		FILE_OPEN_ERR("data");
	}

	std::ifstream ifile2("fxs.fp");
	if (ifile2.good()) {
		for (auto i = fxs.begin(); i != fxs.end(); i++) {
			float ival;
			ifile2.read((char*)&ival, sizeof(float));
			i->rate = ival;
		}

		ifile2.close();
	}
	else {
		FILE_OPEN_ERR("data");
	}
}