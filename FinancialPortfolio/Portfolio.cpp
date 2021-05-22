#include "Portfolio.h"

extern "C" {
	#include "display.h"
}

Portfolio::Portfolio(const DistributionTypes& dist_type, const std::optional<uint16_t>& id) {
	random_distribution = dist_type;
	if (id) {
		this->id = *id;
		id_count = *id + 1;
	}
	else {
		this->id = id_count++;
	}
}

Portfolio::~Portfolio() {
	stocks = std::vector<Stock>();
	fxs = std::vector<FX>();
	std::ifstream f("graphdata.fp");
	if (f.good()) {
		f.close();
		if (std::remove("graphdata.fp") != 0) {
			FILE_REMOVE_ERR("unknown error");
		}
		
	}
	std::ifstream f2("graphdata_ma12.fp");
	if (f2.good()) {
		f2.close();
		if (std::remove("graphdata_ma12.fp") != 0) {
			FILE_REMOVE_ERR("unknown error");
		}

	}
	std::ifstream f3("graphdata_ma26.fp");
	if (f3.good()) {
		f3.close();
		if (std::remove("graphdata_ma26.fp") != 0) {
			FILE_REMOVE_ERR("unknown error");
		}

	}
	std::cout << "Exited portfolio (id: " << id << ")" << std::endl;
}

void Portfolio::SetDistribution(const DistributionTypes& dist_type) {
	random_distribution = dist_type;
}

void Portfolio::Update() {
	switch(random_distribution) {
	case DistributionTypes::NormalDistribution: {
		std::normal_distribution<float> ndist(0.0f, stock_stddev);
		for (auto i = stocks.begin(); i != stocks.end(); i++) {
			i->UpdateValue(ndist(generator));
		}
		ndist = std::normal_distribution<float>(0.0f, fx_stddev);
		for (auto i = fxs.begin(); i != fxs.end(); i++) {
			i->UpdateValue(ndist(generator));
		}
		break;
	}
	case DistributionTypes::LognormalDistribution: {
		std::lognormal_distribution<float> ndist(0.0f, stock_stddev);
		for (auto i = stocks.begin(); i != stocks.end(); i++) {
			i->UpdateValue(ndist(generator));
		}
		ndist = std::lognormal_distribution<float>(0.0f, fx_stddev);
		for (auto i = fxs.begin(); i != fxs.end(); i++) {
			i->UpdateValue(ndist(generator));
		}
		break;
	}
	case DistributionTypes::CauchyDistribution: {
		std::cauchy_distribution<float> ndist(0.0f, stock_stddev);
		for (auto i = stocks.begin(); i != stocks.end(); i++) {
			i->UpdateValue(ndist(generator));
		}
		ndist = std::cauchy_distribution<float>(0.0f, fx_stddev);
		for (auto i = fxs.begin(); i != fxs.end(); i++) {
			i->UpdateValue(ndist(generator));
		}
		break;
	}
	default: {break; }
	}
}

void Portfolio::ShowDistributionType() {
	switch (random_distribution) {
	case DistributionTypes::NormalDistribution: {
		std::cout << "distribution: normal distribution" << std::endl;
		break;
	}
	case DistributionTypes::LognormalDistribution: {
		std::cout << "distribution: lognormal distribution" << std::endl;
		break;
	}
	case DistributionTypes::CauchyDistribution: {
		std::cout << "distribution: cauchy distribution" << std::endl;
		break;
	}
	default: {break; }
	}
}

void Portfolio::Reset() {
	stock_stddev = DEFAULT_STOCK_STDDEV_VALUE;
	fx_stddev = DEFAULT_FX_STDDEV_VALUE;
	random_distribution = DistributionTypes::NormalDistribution;
	for (auto i = stocks.begin(); i != stocks.end(); i++) {
		i->Reset();
	}
	for (auto i = fxs.begin(); i != fxs.end(); i++) {
		i->Reset();
	}
}

void Portfolio::Print() noexcept {
	std::cout << "-----Portfolio:-----" << std::endl;
	PrintReturnAndStddev();
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

void Portfolio::PrintReturnAndStddev() const noexcept {
	std::pair<float, float> s = GetStddev();
	std::pair<float, float> r = GetReturn();
	std::cout << "---Stddev of Stocks---" << std::endl;
	std::cout << s.first << std::endl;
	std::cout << "---Stddev of FXs---" << std::endl;
	std::cout << s.second << std::endl;
	std::cout << "---Return of Stocks---" << std::endl;
	std::cout << r.first << std::endl;
	std::cout << "---Return of FXs---" << std::endl;
	std::cout << r.second << std::endl;
	std::cout << "---Stddev of All---" << std::endl;
	std::cout << (s.first + s.second) * 0.5f << std::endl;
	std::cout << "----Return of All----" << std::endl;
	std::cout << (r.first + r.second) * 0.5f << std::endl;
}

void Portfolio::Add(const Stock& s) {
	stocks.push_back(s);
	AdjustWeights();
}

void Portfolio::Add(const FX& fx) {
	fxs.push_back(fx);
	AdjustWeights();
}
 
void Portfolio::Simulate(uint32_t period) {
	for (uint32_t i = 0; i < period; i++) {
		Update();
	}
}

void Portfolio::Info() noexcept {
	std::cout << "=====|Portfolio information|=====" << std::endl;
	std::cout << "id: " << id << std::endl;
	ShowDistributionType();
	ShowParams();
	Print();
	std::cout << "=================================" << std::endl;
}

void Portfolio::Graph(const std::string& name, const std::optional<uint32_t>& tp) {
	bool stockfound = false;
	size_t stockpos = 0;
	bool fxfound = false;
	size_t fxpos = 0;
	for (auto i = stocks.begin(); i != stocks.end(); i++) {
		if ((i->name).compare(name) == 0) {
			stockfound = true;
			stockpos = i - stocks.begin();
			break;
		}
	}
	for (auto i = fxs.begin(); i != fxs.end(); i++) {
		if ((i->name).compare(name) == 0) {
			fxfound = true;
			fxpos = i - fxs.begin();
			break;
		}
	}
	if (!stockfound && !fxfound) {
		std::cout << "Couldn't find product with name '" << name << "'!" << std::endl;
		return;
	}
	uint32_t p;
	if (tp) {
		p = *tp;
	}
	else {
		p = DEFAULT_SIMULATION_TIMEPERIOD_VALUE;
	}

	std::cout << "time period: " << p << std::endl;

	std::vector<float> values;

	if (stockfound) {
		std::cout << "plotting " << name << " (stock)" << std::endl;
		Stock s = stocks[stockpos];

		switch (random_distribution) {
		case DistributionTypes::NormalDistribution: {
			std::normal_distribution<float> ndist(0.0f, stock_stddev);
			for (uint32_t i = 0; i < p; i++) {
				values.push_back(s.value);
				s.UpdateValue(ndist(generator));
			}
			break;
		}
		case DistributionTypes::LognormalDistribution: {
			std::lognormal_distribution<float> ndist(0.0f, stock_stddev);
			for (uint32_t i = 0; i < p; i++) {
				values.push_back(s.value);
				s.UpdateValue(ndist(generator));
			}
			break;
		}
		case DistributionTypes::CauchyDistribution: {
			std::cauchy_distribution<float> ndist(0.0f, stock_stddev);
			for (uint32_t i = 0; i < p; i++) {
				values.push_back(s.value);
				s.UpdateValue(ndist(generator));
			}
			break;
		}
		default: {}
		}
	}
	else if (fxfound) {
		std::cout << "plotting " << name << " (fx) " << std::endl;
		std::cout << "plotting fx" << std::endl;
		FX fx = fxs[fxpos];

		switch (random_distribution) {
		case DistributionTypes::NormalDistribution: {
			std::normal_distribution<float> ndist(0.0f, fx_stddev);
			for (uint32_t i = 0; i < p; i++) {
				values.push_back(fx.rate);
				fx.UpdateValue(ndist(generator));
			}
			break;
		}
		case DistributionTypes::LognormalDistribution: {
			std::lognormal_distribution<float> ndist(0.0f, fx_stddev);
			for (uint32_t i = 0; i < p; i++) {
				values.push_back(fx.rate);
				fx.UpdateValue(ndist(generator));
			}
			break;
		}
		case DistributionTypes::CauchyDistribution: {
			std::cauchy_distribution<float> ndist(0.0f, fx_stddev);
			for (uint32_t i = 0; i < p; i++) {
				values.push_back(fx.rate);
				fx.UpdateValue(ndist(generator));
			}
			break;
		}
		default: {break; }
		}
	}

	std::ofstream ofile("graphdata.fp", std::ios::trunc);
	if (ofile.good()) {
		for (auto i = values.begin(); i != values.end(); i++) {
			ofile << *i;
			ofile << '\n';
		}
		ofile.close();
	}
	else {
		FILE_OPEN_ERR("open error");
	}

	std::ofstream ofile2("graphdata_ma12.fp", std::ios::trunc);
	if (ofile2.good()) {
		uint32_t idx = 0;
		uint32_t c = 0;
		for (auto i = values.begin(); i != values.end(); i++) {
			float r = 0;
			if (idx >= 12) {
				for(int k = c; k < idx; k++) {
					r += values[k];
				}
				r /= 12;
				ofile2 << r;
				ofile2 << '\n';
				c++;
			}
			idx++;
		}
		ofile2.close();
	}
	else {
		FILE_OPEN_ERR("open error");
	}

	std::ofstream ofile3("graphdata_ma26.fp", std::ios::trunc);
	if (ofile3.good()) {
		uint32_t idx = 0;
		uint32_t c = 0;
		for (auto i = values.begin(); i != values.end(); i++) {
			float r = 0;
			if (idx >= 26) {
				for (int k = c; k < idx; k++) {
					r += values[k];
				}
				r /= 26;
				ofile3 << r;
				ofile3 << '\n';
				c++;
			}
			idx++;
		}
		ofile3.close();
	}
	else {
		FILE_OPEN_ERR("open error");
	}

	/*for (auto i = values.begin(); i != values.end(); i++) {
		std::cout << *i << "\n";
	}*/

	_Display(values.data(), values.size(), DEFAULT_FIGUREWINDOW_WIDTH, DEFAULT_FIGUREWINDOW_HEIGHT, 1);
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

void Portfolio::WriteToFile(const std::vector<Stock>& stocks, const std::vector<FX>& fxs) {
	std::ofstream ofile("stocks.fp", std::ios::trunc);
	std::ofstream ofile2("fxs.fp", std::ios::trunc);

	std::thread s_thread([&ofile, stocks] {
		if (ofile.good()) {
			for (auto i = stocks.begin(); i != stocks.end(); i++) {
				ofile << i->name;
				ofile << ',';
				ofile << i->value;
				ofile << ",\n";
			}
			ofile.close();
		}
		else {
			FILE_OPEN_ERR("open error");
		}
	});
	
	std::thread f_thread([&ofile2, fxs] {
		if (ofile2.good()) {
			for (auto i = fxs.begin(); i != fxs.end(); i++) {
				ofile2 << i->name;
				ofile2 << ',';
				ofile2 << i->rate;
				ofile2 << ",\n";
			}
			ofile2.close();
		}
		else {
			FILE_OPEN_ERR("open error");
		}
	});
	
	s_thread.join();
	f_thread.join();
	
}

void Portfolio::ReadFromFile(std::vector<Stock>& stocks, std::vector<FX>& fxs) {

	stocks.clear();
	fxs.clear();

	std::ifstream ifile("stocks.fp");
	std::ifstream ifile2("fxs.fp");

	std::thread s_thread([&ifile, &stocks] {
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
				stocks.push_back(Stock(name, value, 1));
			}

			ifile.close();
		}
		else {
			FILE_OPEN_ERR("not found");
		}
	});

	std::thread f_thread([&ifile2, &fxs] {
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
				fxs.push_back(FX(name, rate, 1));
			}

			ifile2.close();
		}
		else {
			FILE_OPEN_ERR("not found");
		}
	});

	s_thread.join();
	f_thread.join();

	AdjustWeights();
}

void Portfolio::AdjustWeights() noexcept {
	uint32_t stocksize = stocks.size();
	uint32_t fxsize = fxs.size();
	
	if (stocksize > 0) {
		for (auto i = stocks.begin(); i != stocks.end(); i++) {
			i->weight = 1;
			i->weight /= stocksize;
		}
	}
	if (fxsize > 0) {
		for (auto i = fxs.begin(); i != fxs.end(); i++) {
			i->weight = 1;
			i->weight /= fxsize;
		}
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

std::pair<float, float> Portfolio::GetReturn() const noexcept {
	float sr = 0, fr = 0;
	for (auto i = stocks.begin(); i != stocks.end(); i++) {
		sr += i->weight * i->GetReturn();
	}
	for (auto i = fxs.begin(); i != fxs.end(); i++) {
		fr += i->weight * i->GetReturn();
	}

	return std::make_pair(sr, fr);
}
std::pair<float, float> Portfolio::GetStddev() const noexcept {
	// TODO: Covariance matrix!!
	return std::make_pair(0, 0);
}