//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Particle.h"
#include "Matrix.h"

Particle::Particle()
	: next(0),
	prev(0),
	position(0.0f, 0.0f, 0.0f), 
	life(0.0f), 
	velocity(0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 1.0f),
	rotation(0.0f),
	rotation_velocity(0.25f)

{}

Particle::~Particle()
{
	// nothing to do
}

void Particle::CopyDataOnly(const Particle &p)
{
	// copy the data only
	// this way of copying data is more efficient that element by element
	this->position = p.position;
	this->velocity = p.velocity;
	this->scale = p.scale;
	this->rotation = p.rotation;
	this->rotation_velocity = p.rotation_velocity;
	this->life = p.life;
}

void Particle::Update(const float& time_elapsed)
{
	// serious math below - magic secret sauce
	life += time_elapsed;

	Vect4D tmpVect = velocity;
	tmpVect *= time_elapsed;
	position += tmpVect;

	Vect4D axis(1.0f, 0.0f, 0.0f);
	Vect4D v(0.0f, 50.0f, 0.0f);
	position.Cross(axis, v);
	v.norm(v);
	
	tmpVect = v;
	tmpVect *= 0.07f;
	tmpVect *= life;
	position += tmpVect;

	float tempf = time_elapsed;
	tempf *= 4.0f;
	float tempf2 = rotation_velocity;
	tempf2 *= tempf;
	rotation += tempf2;
	
}


// End of file
