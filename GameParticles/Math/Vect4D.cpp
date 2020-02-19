//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <math.h>
#include "Vect4D.h"

Vect4D::Vect4D()
	: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{}

Vect4D& Vect4D::operator = (const Vect4D& tmp) {
	this->x = tmp.x;
	this->y = tmp.y;
	this->z = tmp.z;
	this->w = tmp.w;

	return *this;
}

Vect4D::Vect4D(float tx, float ty, float tz, float tw)
	: x(tx), y(ty), z(tz), w(tw)
{}

Vect4D::~Vect4D()
{
	// nothing to delete
}

void Vect4D::norm(Vect4D& out)
{
	float temp = this->x;
	float temp2 = this->y;
	temp *= this->x;
	temp2 *= this->y;
	temp += temp2;

	temp2 = this->z;
	temp2 *= this->z;
	temp += temp2;

	//float mag = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
	float mag = sqrtf(temp);

	if (0.0f < mag)
	{
		/*out.x = this->x / mag;
		out.y = this->y / mag;
		out.z = this->z / mag;
		out.w = 1.0f;*/
		temp = this->x;
		temp /= mag;
		out.x = temp;

		temp = this->y;
		temp /= mag;
		out.y = temp;

		temp = this->z;
		temp /= mag;
		out.z = temp;

		out.w = 1.0f;
	}
}

Vect4D Vect4D::operator + (Vect4D t)
{
	Vect4D out;

	out.x = this->x + t.x;
	out.y = this->y + t.y;
	out.z = this->z + t.z;

	return out;
}

Vect4D Vect4D::operator - (Vect4D t)
{
	Vect4D out;

	out.x = this->x - t.x;
	out.y = this->y - t.y;
	out.z = this->z - t.z;

	return out;
}

Vect4D Vect4D::operator *(const float& scale)
{
	return Vect4D(this->x * scale, this->y * scale, this->z * scale);
}

Vect4D& Vect4D:: operator *= (const float& t)
{
	this->x *= t;
	this->y *= t;
	this->z *= t;
	

	return *this;
}

Vect4D& Vect4D:: operator -= (const Vect4D& t)
{
	this->x -= t.x;
	this->y -= t.y;
	this->z -= t.z;

	return *this;
}

Vect4D& Vect4D:: operator += (const Vect4D& t)
{
	this->x += t.x;
	this->y += t.y;
	this->z += t.z;

	return *this;
}


float& Vect4D::operator[](VECT_ENUM e)
{
	switch (e)
	{
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	case 3:
		return w;
		break;
	default:
		assert(0);
		return x;
	}
}

void Vect4D::Cross(const Vect4D& vin, Vect4D& vout)
{
	/*vout.x = (y*vin.z - z * vin.y);
	vout.y = (z*vin.x - x * vin.z);
	vout.z = (x*vin.y - y * vin.x);
	vout.w = 1.0f;*/
	float temp = y;
	float temp2 = z;
	temp *= vin.z;
	temp2 *= vin.y;
	temp -= temp2;
	vout.x = temp;
	//vout.x = (y * vin.z - z * vin.y);

	temp = z;
	temp2 = x;
	temp *= vin.x;
	temp2 *= vin.z;
	temp -= temp2;
	vout.y = temp;
	//vout.y = (z * vin.x - x * vin.z);

	temp = x;
	temp2 = y;
	temp *= vin.y;
	temp2 *= vin.x;
	temp -= temp2;
	vout.z = temp;
	//vout.z = (x * vin.y - y * vin.x);

	vout.w = 1.0f;
}

void Vect4D::set(float tx, float ty, float tz, float tw)
{
	this->x = tx;
	this->y = ty;
	this->z = tz;
	this->w = tw;
}

// End of file
