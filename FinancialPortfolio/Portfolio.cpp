#include "Portfolio.h"

Portfolio::Portfolio() {
}

Portfolio::~Portfolio() {
	stocks = std::vector<Stock>();
	fxs = std::vector<FX>();
}

void Portfolio::Update() {
	std::normal_distribution<float> ndist(0.0f, stock_stddev);
	for (auto i = stocks.begin(); i != stocks.end(); i++) {
		i->UpdateValue(ndist(generator));
	}
	ndist = std::normal_distribution<float>(0.0f, fx_stddev);
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

void Portfolio::ShowParams(const std::optional<std::string>& p) {
	if (p) {
		std::string p1("stock stddev");
		std::string p2("fx stddev");
		if (p1.compare(*p)) {
			std::cout << *p << " = " << stock_stddev << std::endl;
		}
		else if (p2.compare(*p)) {
			std::cout << *p << " = " << fx_stddev << std::endl;
		}
		else {
			std::cout << "unknown parameter!" << std::endl;
		}
	}
	else {
		std::cout << "Parameters:" << std::endl;
		std::cout << "\tstock stddev = " << stock_stddev << std::endl;
		std::cout << "\tfx stddev = " << fx_stddev << std::endl;
	}
}

void Portfolio::WriteToFile() {
	std::ofstream ofile("stocks.fp", std::ios::trunc);
	if (ofile.good()) {
		for (auto i = stocks.begin(); i != stocks.end(); i++) {
			ofile << i->name;
			ofile << ',';
			//ofile.write((const char*)&(i->value), sizeof(float));
			ofile << i->value;
			ofile << ",\n";

		}
		ofile.close();
	}
	else {
		FILE_OPEN_ERR("open error");
	}

	std::ofstream ofile2("fxs.fp", std::ios::trunc);
	if (ofile2.good()) {
		for (auto i = fxs.begin(); i != fxs.end(); i++) {
			ofile2 << i->name;
			ofile2 << ',';
			ofile2 << i->rate;
			//ofile2.write((const char*)&(i->rate), sizeof(float));
			ofile2 << ",\n";
		}
		ofile2.close();
	}
	else {
		FILE_OPEN_ERR("open error");
	}
	
}

void Portfolio::ReadFromFile() {

	stocks.clear();
	fxs.clear();

	std::ifstream ifile("stocks.fp");
	if (ifile.good()) {		
		std::string line;
		while (std::getline(ifile, line)) {
			std::stringstream ss(line);
			std::string name, svalue;
			std::getline(ss, name, ',');
			std::getline(ss, svalue, ',');
			float value = 1.0f;
			try {
				value = std::stof(svalue);
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what();
				std::cout << "error in file format, float value will be set to default (1.0)" << std::endl;
			}
			stocks.push_back(Stock(name, value));
		}

		ifile.close();
	}
	else {
		FILE_OPEN_ERR("not found");
	}

	std::ifstream ifile2("fxs.fp");
	if (ifile2.good()) {
		std::string line;
		while (std::getline(ifile2, line)) {
			std::stringstream ss(line);
			std::string name, srate;
			std::getline(ss, name, ',');
			std::getline(ss, srate, ',');
			float rate = 1.0f;
			try {
				rate = std::stof(srate);
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what();
				std::cout << "error in file format, float value will be set to default (1.0)" << std::endl;
			}
			fxs.push_back(FX(name, rate));
		}

		ifile2.close();
	}
	else {
		FILE_OPEN_ERR("not found");
	}
}

void Portfolio::PurgeSession() {
	if (std::remove("stocks.fp") != 0 || std::remove("fxs.fp") != 0) {
		FILE_REMOVE_ERR("unknown cause");
	}
	else {
		std::cout << "session data deleted!" << std::endl;
	}
}