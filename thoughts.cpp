// thoughts.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "NoteManager/NoteManager.h"
#include <stdio.h>
#include <thread>
#include "UI/UI.h"
#include <chrono>



int main()
{


	CWindow window;
	CNoteMgr::Get().LoadFromDisk();
	if (!window.init()) {
		
		std::cout << "imgui init failed";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return 0;
	}
	window.RenderLoop();
 
}

