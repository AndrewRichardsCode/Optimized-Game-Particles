//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#if WIN32

// -------------------------------------------------------------------------------
//   DO NOT MODIFY this FILE
// -------------------------------------------------------------------------------

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		// Declaration For WndProc

class EventHandler
{
public:
	EventHandler();
	~EventHandler();
	static void ProcessEvents();
private:
	EventHandler(const EventHandler &toCopy);
	EventHandler& operator=(const EventHandler& toCopy);
	
	static EventHandler& Instance();
	void processEvents();
};

#endif

// End of File

