#include <windows.h>
#include "omtypes.h"
#include "malodytypes.h"
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	
	/* tests */
	const char replayPath[] = "C:\\Users\\Olive\\AppData\\Local\\osu!\\Replays\\crazybull - typeMARS - Triumph & Regret [Regret] (2023-07-28) OsuMania.osr";
	const char resultPath[] = "test1.out";
	const char debugPath[] = "test.out";
	const char testMrPath[] = "test.mr";
	const unsigned char mldversion[4] = {7,3,4,0};
	
	osu::OsuManiaReplayData dat;
	int result = dat.readFromPath(replayPath);
	int decoderes = dat.decode();
	//MessageBox(NULL, std::to_string(decoderes).data(),"decode result",MB_OK);
	//MessageBox(NULL, std::to_string(dat.mods).data(),"mods",MB_OK);
	std::ofstream fout(resultPath, std::ios::out);
	//MessageBox(NULL, (char*)dat.decodedReplayData,"mods",MB_OK);
	fout<<(char*)dat.decodedReplayData<<std::endl;
	fout.close();
	result = dat.readFrames();
	//MessageBox(NULL, std::to_string(result).data(),"decode result",MB_OK);
	//MessageBox(NULL, std::to_string(dat.createTime).data(),"ids",MB_OK);
	
	malody::MalodyReplayData mdat;
	mdat.transformFrom(dat,mldversion,1);
	MessageBox(NULL, "transform complete","ids",MB_OK); 
	mdat.writeTo(testMrPath);
	MessageBox(NULL, "write complete","ids",MB_OK);
	
	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
