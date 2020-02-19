//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

// This is a 4 dimensional Vect4D class
#ifndef Vect4D_H
#define Vect4D_H

// includes
#include "Enum.h"
#include <xmmintrin.h>
#include <smmintrin.h>

// Foward Declarations
class Matrix;

// class
class Vect4D : public Align16
{
public:
	friend class Matrix;

	Vect4D();
	Vect4D(const Vect4D&) = default;
	Vect4D& operator = (const Vect4D& tmp);
	Vect4D(float tx, float ty, float tz, float tw = 1.0f);
	~Vect4D();

	void norm(Vect4D &out);
	void set(float tx, float ty, float tz, float tw = 1.0f);

	Vect4D operator + (Vect4D t);
	Vect4D operator - (Vect4D t);
	Vect4D operator * (const float& scale);
	Vect4D& Vect4D::operator *= (const float& t);
	Vect4D& Vect4D::operator -= (const Vect4D& t);
	Vect4D& Vect4D::operator += (const Vect4D& t);

	void Cross(const Vect4D &vin, Vect4D &vout);

	float &operator[](VECT_ENUM e);

private:

	union {
		struct {
			__m128 _m;
		};

		struct {
			float x;
			float y;
			float z;
			float w;
		};
	};
};

#endif  

// End of file

