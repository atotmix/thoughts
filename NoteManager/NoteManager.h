#pragma once
#include <string>
#include "../Libs/json.hpp"

class CNote {
public:
	CNote();
	CNote(nlohmann::json input, std::string id);
	nlohmann::json dump();
	bool write(nlohmann::json data);
	std::string szNoteId;
private:
	nlohmann::json jsonData;
};

template <typename T>
class CSingleton
{
protected:
	CSingleton() { }
	~CSingleton() { }

	CSingleton(const CSingleton&) = delete;
	CSingleton& operator=(const CSingleton&) = delete;

	CSingleton(CSingleton&&) = delete;
	CSingleton& operator=(CSingleton&&) = delete;
public:
	static T& Get()
	{
		static T pInstance{ };
		return pInstance;
	}
};


class CNoteMgr :public CSingleton<CNoteMgr> {
public:
	bool AddNote(CNote note);
	void DelNote(std::string id);
	bool UpdateNote(CNote note);
	CNote GetNote(std::string id);
	std::vector<std::tuple<std::string, std::string, std::string, int>>* GetNotes();
	bool SaveToDisk();
	bool LoadFromDisk();
private:
	bool bNotesFetched = false;
	std::vector<std::tuple<std::string, std::string, std::string, int>> arrNotes;
	nlohmann::json jsonData;
};