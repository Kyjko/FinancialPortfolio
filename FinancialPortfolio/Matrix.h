#pragma once

#include <stdint.h>
#include <vector>
#include <iostream>

/*
	This matrix implementation only supports square matrices with float values!
	`dim` here refers to column (row) count!
*/

class Matrix {
public:
	template<typename... Args>
	Matrix(float first, Args&&... args);
	Matrix();
	~Matrix();

	friend std::ostream& operator<<(const Matrix& m, std::ostream& os);
	float operator[](uint32_t index) {
		return matrix[index];
	}
	friend Matrix operator*(Matrix& m, Matrix& other);
	friend Matrix operator+(Matrix& m, Matrix& other);
	friend Matrix operator-(Matrix& m, Matrix& other);

	uint32_t GetDim() const noexcept;
	void PushBack(float val);

	uint32_t dim;
	std::vector<float> matrix;
	
};

