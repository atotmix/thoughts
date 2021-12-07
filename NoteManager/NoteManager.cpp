#include <iostream>
#include <stdio.h>
#include <thread> 
#include <random>
#include <sstream>
#include <chrono>
#include <ctime>
#include <fstream>

#include "NoteManager.h"
CNote::CNote() {
	static std::random_device              rd;
	static std::mt19937                    gen(rd());
	static std::uniform_int_distribution<> dis(0, 15);
	static std::uniform_int_distribution<> dis2(8, 11);
    std::stringstream ss;

    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    };
    std::cout << "new note created with id: " << ss.str() << std::endl;
    szNoteId = ss.str();
    
    jsonData["created"] = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now());
    jsonData["modified"] = jsonData["created"];
    jsonData["title"] = "New note.";
    jsonData["content"] = "";
    std::cout << jsonData.dump() << std::endl;
}
CNote::CNote(nlohmann::json input, std::string id) {
    jsonData = input;
    szNoteId = id;
}
bool CNote::write(nlohmann::json data) {
	jsonData = data;
	return true;
}
nlohmann::json CNote::dump() {
	return jsonData;
}
bool CNoteMgr::AddNote(CNote note) {
    if (!jsonData[note.szNoteId].is_null()) {
        std::cout << "HOW";
        return false;
    }
	jsonData[note.szNoteId] = note.dump();
    bNotesFetched = false;
    return true;
}
void CNoteMgr::DelNote(std::string id) {
    jsonData.erase(id);
    bNotesFetched = false;
}
bool CNoteMgr::UpdateNote(CNote note)
{
    jsonData[note.szNoteId] = note.dump();
    bNotesFetched = false;
    return true;
}
CNote CNoteMgr::GetNote(std::string id) {
	return CNote(jsonData[id],id);
}

bool CNoteMgr::LoadFromDisk() {
    std::stringstream buffer;
    std::ifstream in("note.json");
    buffer << in.rdbuf();
    if (buffer.str().empty())
        return false;
    jsonData = nlohmann::json::parse(buffer.str());
    if (jsonData.is_null())
        return false;
    return true;
}
bool CNoteMgr::SaveToDisk() {
    std::ofstream out("note.json");
    out << jsonData.dump();
    out.close();
    return true;
}


std::vector<std::tuple<std::string,std::string, std::string, int>>* CNoteMgr::GetNotes() {
   
    if (bNotesFetched)
        return &arrNotes;
    arrNotes.clear();
    for (auto& note : jsonData.items()) {
        std::time_t tt = note.value()["created"].get<int>();
        
        std::tm ptm;
        localtime_s(&ptm, &tt);
        char buffer[32];
        // Format: Mo, 15.06.2009 20:20:00
        std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", &ptm);
        arrNotes.emplace_back(std::tuple<std::string, std::string, std::string, int>{note.value()["title"].get<std::string>(),
                                                                                note.key(), 
                                                                                buffer, tt });//optimised :^)
        } 
    std::sort(arrNotes.begin(), arrNotes.end(), [](std::tuple<std::string, std::string, std::string, int> a, std::tuple<std::string, std::string, std::string, int> b) {
        return (std::get<3>(a) < std::get<3>(b));
     });
    bNotesFetched = true;
    return &arrNotes;
}