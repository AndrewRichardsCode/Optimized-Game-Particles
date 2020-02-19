//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "DO_NOT_MODIFY\OpenGLInterface.h"
#include "ParticleEmitter.h"
#include "Settings.h"

extern PerformanceTimer GlobalTimer;

static const unsigned char squareColors[] =
{
	// ----------------------------
	//  point is actually a quad   
	//  set color on each vertex   
	// ----------------------------
	//    Vert A = Yellow 
	//    Vert B = Yellow
	//    Vert C = Yellow
	//    Vert D = Yellow
	// ----------------------------

	200,  200,  0,  255,
	200,  200,  0,  255,
	200,  200,  0,  255,
	200,  200,  0,  255,
};

static const float squareVertices[] =
{
	// --------------------
	//   Vert A = (x,y,z)
	//   Vert B = (x,y,z)
	//   Vert C = (x,y,z)
	//   Vert D = (x,y,z)
	// --------------------

	-0.015f,  -0.015f, 0.0f, // Size of Triangle
	 0.015f,  -0.015f, 0.0f, // Size of Triangle
	-0.015f,   0.015f, 0.0f, // Size of Triangle
	 0.015f,   0.015f, 0.0f, // Size of Triangle
};

ParticleEmitter::ParticleEmitter()
	: start_position(0.0f, 0.0f, 0.0f),
	start_velocity(0.0f, 1.0f, 0.0f),
	max_life(MAX_LIFE),
	max_particles(NUM_PARTICLES),
	last_active_particle(-1),
	spawn_frequency(0.0000001f),
	headParticle(0),
	vel_variance(1.0f, 4.0f, 0.4f),
	pos_variance(1.0f, 1.0f, 1.0f),
	scale_variance(2.5f)
{
	GlobalTimer.Toc();

	last_spawn = GlobalTimer.TimeInSeconds();
	last_loop = GlobalTimer.TimeInSeconds();
	*buffer = new Particle[NUM_PARTICLES*2];
}

ParticleEmitter::~ParticleEmitter()
{
	// do nothing
}

void ParticleEmitter::SpawnParticle()
{
	// create another particle if there are ones free
	if (last_active_particle < max_particles -1 )
	{
		Particle* newParticle;
		if (this->headParticle == 0 || last_active_particle == 0) {
			this->headParticle = *buffer;
			newParticle = this->headParticle;
			tmpParticle = this->headParticle;
		}
		else {
			newParticle = tmpParticle+1;
			tmpParticle++;
		}
		// initialize the particle
		newParticle->life = 0.0f;
		newParticle->position = start_position;
		newParticle->velocity = start_velocity;
		newParticle->scale = Vect4D(2.0f, 2.0f, 2.0f, 2.0f);
		newParticle->rotation = 0.0f;
		newParticle->rotation_velocity = 0.25f;

		// apply the variance
		this->Execute(newParticle->position, newParticle->velocity, newParticle->scale);
		// increment count
		last_active_particle++;
		// add to list
		this->addParticleToList(newParticle);
	}
}

void ParticleEmitter::update()
{
	// get current time
	GlobalTimer.Toc();
	float current_time = GlobalTimer.TimeInSeconds();

	// spawn particles
	float time_elapsed = current_time;// -last_spawn;
	time_elapsed -= last_spawn;
		
	while (spawn_frequency < time_elapsed && last_active_particle < max_particles - 1)
	{
		// spawn a particle
		this->SpawnParticle();
		// adjust time
		time_elapsed -= spawn_frequency;
		// last time
		last_spawn = current_time;
	}
	
		// total elapsed
	time_elapsed = current_time;// -last_loop;
	time_elapsed -= last_loop;
	
	
	Particle *p = this->headParticle;
	// walk the particles
	while (p != 0)
	{
		// call every particle and update its position 
		p->Update(time_elapsed);
		p = p->next;
	}
	if (this->headParticle != nullptr && this->headParticle->life > max_life)
	{
		last_active_particle = 0;
	}
	// make sure the counts track (asserts go away in release - relax Christos)
	//assert(bufferCount == (last_active_particle ));//should be +1
	last_loop = current_time;
}

void ParticleEmitter::addParticleToList(Particle *p)
{
	assert(p);
	if (this->headParticle == p)
	{ // first on list
		//this->headParticle = p;
		p->next = 0;
		p->prev = 0;
	}
	else
	{ // add to front of list...
		headParticle->prev = p;
		p->next = headParticle;
		p->prev = 0;
		headParticle = p;
	}
}

void ParticleEmitter::removeParticleFromList(Particle *p)
{
	// make sure we are not screwed with a null pointer
	assert(p);

	if (p->prev == 0 && p->next == 0)//never used
	{ // only one on the list
		this->headParticle = 0;
		
	}
	else if (p->prev == 0 && p->next != 0)//this is used
	{ // first on the list
		p->next->prev = 0;
		this->headParticle = p->next;
	}
	else if (p->prev != 0 && p->next == 0)//this is used
	{ // last on the list 
		p->prev->next = 0;
	}
	else//( p->next != 0  && p->prev !=0 )//never used
	{ // middle of the list
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}
}

void ParticleEmitter::draw(Matrix& cameraMatrix)
{
	Matrix tmp;
	// get the position from this matrix
	Vect4D camPosVect;
	cameraMatrix.get(cameraMatrix.v3, &camPosVect);
	// camera position
	Matrix transCamera;
	transCamera.setTransMatrix(camPosVect);

	glVertexPointer(3, GL_FLOAT, 0, squareVertices);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	Matrix transParticle;
	Matrix rotParticle;
	Matrix scaleMatrix;
	
	// iterate throught the list of particles
	Particle* iterator = this->headParticle;
	Particle* temp = *buffer;
	while(iterator != 0)
	{
		temp->CopyDataOnly(*iterator);
		temp->position *= 0.35f;

		transParticle.setTransMatrix(temp->position);
		rotParticle.setRotZMatrix(temp->rotation);
		scaleMatrix.setScaleMatrix(temp->scale);

		// total transformation of particle
		tmp = scaleMatrix;
		tmp *= transCamera;
		tmp *= transParticle;
		tmp *= rotParticle;
		tmp *= scaleMatrix;

		// set the transformation matrix
		glLoadMatrixf(reinterpret_cast<float*>(&(tmp)));
		// draw the trangle strip
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		iterator = iterator->next;
	}
}

void ParticleEmitter::Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc)
{
	// Add some randomness...

	// --------------------------------------------------------------
	//   Ses it's ugly - I didn't write this so don't bitch at me   |
	//   Sometimes code like this is inside real commerical code    |
	//   ( so now you know how it feels )  |
	//---------------------------------------------------------------

	// x - variance
	float var = static_cast<float>(rand() % 1000) * 0.005f; // Var
	float sign = static_cast<float>(rand() % 2);  // Sign 
	float*t_pos = reinterpret_cast<float*>(&pos);
	float*t_var = &pos_variance[x];
	if (sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// y - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// z - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);

	// x  - add velocity
	t_pos = &vel[x];
	t_var = &vel_variance[x];
	if (sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// y - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// z - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// correct the sign
	var = 1.5f * static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);

	if (sign == 0)
	{
		var *= -1.0f;
	}
	sc = sc * var;
}


void ParticleEmitter::GoodBye()
{
	delete[] *buffer;
}

// End of file
