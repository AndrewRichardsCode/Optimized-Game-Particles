//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PARTICLE_H
#define PARTICLE_H

// include
#include "Vect4D.h"

class Particle : public Align16
{
public:
	friend class ParticleEmitter;

	Particle();
	Particle(const Particle&) = default;
	Particle& operator = (const Particle&) = default;
	~Particle();
	void Update(const float& time_elapsed);
	void CopyDataOnly(const Particle &p);

private:
	Particle* next;//8
	Particle* prev;//8

	float	life;
	float	rotation;
	float	rotation_velocity;
	char	pad0;
	char	pad1;
	char	pad2;
	char	pad3;

	Vect4D	position;
	Vect4D	velocity;
	Vect4D	scale;
};


#endif 

// End of File
