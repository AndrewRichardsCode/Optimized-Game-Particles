//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <conio.h>
#include "WinEventHandler.h"
#include "OpenGLInterface.h"
#include "ParticleEmitter.h"


// Timer needed in Particle Emission
PerformanceTimer GlobalTimer;

// -----------------------------------------------------------
// main:                
// -----------------------------------------------------------
int main()
{
	Trace::out("System Start()\n");

	bool success = false;
	srand(1);

	// initialize timers:------------------------------

	PerformanceTimer UpdateTimer;
	PerformanceTimer DrawTimer;

	// create a window:---------------------------------

	success = OpenGLDevice::InitWindow();
	assert(success);

	// create an emitter:-------------------------------

	ParticleEmitter emitter;

	// Get the inverse Camera Matrix:-------------------
	// initialize the camera matrix
	Matrix CameraMatrix;
	CameraMatrix.setIdentMatrix();

	// setup the translation matrix
	Matrix TransMatrix;
	Vect4D Trans(0.0f, 10.0f, 15.0f);
	TransMatrix.setTransMatrix(Trans);

	// multiply them together
	Matrix tmp;
	tmp = CameraMatrix;
	tmp *= TransMatrix;

	// counter for printing
	int i = 0;
	bool exit = false;
	Matrix cameraMatrix;
	Matrix transMatrix;
	Vect4D trans(0.0f, 10.0f, 15.0f);
	Matrix inverseCameraMatrix;
	float updateTime;
	float drawTime;

	// initialize the camera matrix
	cameraMatrix.setIdentMatrix();
	// setup the translation matrix
	transMatrix.setTransMatrix(trans);
	// multiply them together
	tmp = cameraMatrix * transMatrix;
	// get the inverse matrix
	tmp.Inverse(inverseCameraMatrix);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(reinterpret_cast<float*>(&inverseCameraMatrix));
	glPushMatrix(); // push the camera matrix
	glGetFloatv(GL_MODELVIEW_MATRIX, reinterpret_cast<float*>(&cameraMatrix));
	

	// main update loop... do this forever or until some breaks 
	while (OpenGLDevice::IsRunning() && !exit )
	{
		// break if escape key is hit
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			exit = true;
			break;
		}
		
		// start update timer ---------------------------------------
		UpdateTimer.Tic();

			// start draw... end draw (the draw updates)
			OpenGLDevice::StartDraw();
			// set matrix to Model View
			glMatrixMode(GL_MODELVIEW);
			// update the emitter
			emitter.update();

		// stop update timer: -----------------------------------------
		UpdateTimer.Toc();

		// start draw timer: ----------------------------------------
		DrawTimer.Tic();

			// draw particles
			emitter.draw(cameraMatrix);
			// pop matrix - needs to correspond to previous push
			glPopMatrix();

		// stop draw timer: -----------------------------------------
		DrawTimer.Toc();

		// finish draw update
		OpenGLDevice::EndDraw();

		// Love for Windows - allows the windows to behave to external events
		EventHandler::ProcessEvents();

		// update ouput every 20 times
		i++;
		if (i > 20)
		{
			i = 0;
			updateTime = UpdateTimer.TimeInSeconds();
			drawTime = DrawTimer.TimeInSeconds();
			printf("LoopTime: update:%f ms  draw:%f ms  tot:%f\n", updateTime * 1000.0f, drawTime * 1000.0f, (updateTime + drawTime) *1000.0f);
		}
	}

	// Cleanup
	emitter.GoodBye();

	Trace::out("System End()\n");

	return 0;
}

// ---------------------------------------------------------------------
// WinMain required by windows for all win32 applications.
// This is our Windows entry point.
// ---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	OpenGLDevice::SetHInstance(hInstance);
	//main(__argc, __argv);
	main();
}

// End of file
