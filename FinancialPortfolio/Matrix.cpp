#include "Matrix.h"

template<typename... Args>
Matrix::Matrix(float first, Args&&... args) {
	matrix.push_back(first);
	using expander = float[];
	(void) expander {
		matrix.push_back(std::forward<Args>(args), void(), 0)...
	};

	dim = sqrt(matrix.size());
	if ((int)dim != dim) {
		// matrix is not square
		dim = 0;
		matrix.clear();
	}

}

Matrix::Matrix() : dim(0) {
	matrix = std::vector<float>();
}

uint32_t Matrix::GetDim() const noexcept {
	return dim;
}

Matrix::~Matrix() {
	dim = 0;
	matrix = std::vector<float>();
}

void Matrix::PushBack(float val) {
	matrix.push_back(val);
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

Matrix operator+(Matrix& m, Matrix& other) {
	Matrix res;
	if (m.dim != other.dim) {
		return res;
	}
	for (uint32_t i = 0; i < m.matrix.size(); i++) {
		res.PushBack(m[i] + other[i]);
	}

	return res;
}

Matrix operator-(Matrix& m, Matrix& other) {
	Matrix res;
	if (m.dim != other.dim) {
		return res;
	}
	for (uint32_t i = 0; i < m.matrix.size(); i++) {
		res.PushBack(m[i] - other[i]);
	}

	return res;
}

Matrix operator*(Matrix& m, Matrix& other) {
	Matrix res;
	
	if (m.dim != other.dim) {
		return res;
	}

	for (int i = 0; i < m.dim; i++) {
		for (int j = 0; j < m.dim; j++) {
			for (int k = 0; k < m.dim; k++) {
				res.PushBack(m[i * m.dim + k] * other[k * other.dim + j]);
			}
		}
	}

	return res;

}