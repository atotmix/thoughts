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

	//for (int i = 0; i < 40000; i++) {
	//	CNote bruh;
	//	CNoteMgr::Get().AddNote(bruh);
	//}
	CWindow window;
	CNoteMgr::Get().LoadFromDisk();
	if (!window.init()) {
		
		std::cout << "imgui init failed";
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return 0;
	}
	window.RenderLoop();
 
}


//
//nlohmann::json notes;
//
//std::cout << "Welcome to thoughts\n ";
//
//
//for (int i = 0; i < 4; i++) {
//    std::string name;
//
//    std::cout << "please enter the name of the note: ";
//    std::cin >> name;
//    nlohmann::json data;
//    data["text"] = "info and note data!!! ";
//    CNote test(name);
//    test.write(data);
//    notes[test.dump().first] = test.dump().second;
//}
//for (auto note : notes.items()) {
//    std::cout << note.key() << " : " << note.value() << std::endl;
//}
//std::cout << notes.dump() << std::endl;
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
