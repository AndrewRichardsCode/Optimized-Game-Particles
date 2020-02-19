//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Math\Matrix.h"
#include "Math\Vect4D.h"
#include "Particle.h"
#include "Settings.h"

class ParticleEmitter : public Align16
{
public:
	ParticleEmitter();
	~ParticleEmitter();
	ParticleEmitter(const ParticleEmitter&) = default;
	ParticleEmitter& operator = (const ParticleEmitter&) = default;

	void SpawnParticle();
	void update();
	void draw(Matrix& camera);

	void addParticleToList(Particle *p);
	void removeParticleFromList(Particle *p);

	void Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc);
	void GoodBye();

private:

	Vect4D	start_position;
	Vect4D	start_velocity;
	Particle* headParticle;
	Particle* tmpParticle;
	Vect4D	vel_variance;
	Vect4D	pos_variance;

	float	max_life;
	float	spawn_frequency;
	float	last_spawn;
	float	last_loop;

	Particle* buffer[NUM_PARTICLES*2];
	float	scale_variance;
	int		max_particles;
	int		last_active_particle;
};

#endif

// End of File
