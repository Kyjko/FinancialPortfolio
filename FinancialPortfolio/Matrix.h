#pragma once

#include <stdint.h>
#include <vector>
#include <iostream>

/*
	This matrix implementation only supports square matrices with float values!
*/

class Matrix {
public:
	template<typename... Args>
	Matrix(float first, Args&&... args);
	~Matrix();

	friend std::ostream& operator<<(const Matrix& m, std::ostream& os);

private:
	std::vector<float> matrix;
	uint32_t dim;
	
};

