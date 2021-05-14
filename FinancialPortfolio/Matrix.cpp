#include "Matrix.h"

template<typename... Args>
Matrix::Matrix(float first, Args&&... args) {
	matrix.push_back(first);
	using expander = float[];
	(void) expander {
		matrix.push_back(std::forward<Args>(args), void(), 0)...
	};

	dim = sqrt(matrix.size());

}

Matrix::~Matrix() {
	matrix = std::vector<float>();
}

std::ostream& operator<<(const Matrix& m, std::ostream& os) {
	uint32_t c = 0;
	for (auto i = m.matrix.begin(); i != m.matrix.end(); i++) {
		if (c % m.dim == 0 && c != 0) {
			os << std::endl;
		}
		os << *i;
	}

	return os;
}