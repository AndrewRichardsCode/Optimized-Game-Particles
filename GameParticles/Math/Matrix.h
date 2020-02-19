//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MATRIX_H
#define MATRIX_H

// includes
#include "Enum.h"
#include "Vect4D.h"

// forward declare
class Vect4D;

// class
class Matrix : public Align16
{
public:

	Matrix();
	Matrix(const Matrix& t);
	Matrix& operator = (const Matrix& tmp);
	~Matrix();

	void set(Vect4D* row, const Vect4D& t);
	void get(const Vect4D& row, Vect4D* t);

	void setIdentMatrix();
	void setTransMatrix(const Vect4D &t);
	void setScaleMatrix(const Vect4D &s);
	void setRotZMatrix(float Z_Radians);

	float&operator[](const INDEX_ENUM& e);

	Matrix operator*(const Matrix &t) const;

	float Determinant();

	Matrix GetAdjugate();
	Matrix& Matrix::operator/=(const float& t);
	Matrix& Matrix::operator*=(const Matrix& rhs);

	void Matrix::Inverse(Matrix &out);

//private:

	union {
		struct {
			Vect4D	v0;
			Vect4D	v1;
			Vect4D	v2;
			Vect4D	v3;
		};

		struct {
			// ROW 0
			float m0;
			float m1;
			float m2;
			float m3;

			// ROW 1
			float m4;
			float m5;
			float m6;
			float m7;

			// ROW 2
			float m8;
			float m9;
			float m10;
			float m11;

			// ROW 3
			float m12;
			float m13;
			float m14;
			float m15;
		};
	};
};

#endif 

// End of File

